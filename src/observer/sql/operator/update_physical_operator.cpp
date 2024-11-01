/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL
v2. You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by WangYunlai on 2022/6/27.
//

#include "sql/operator/update_physical_operator.h"
#include "common/log/log.h"
#include "common/value.h"
#include "storage/record/record.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"
#include <cstddef>

RC UpdatePhysicalOperator::open(Trx *trx)
{
  if (children_.empty()) {
    return RC::SUCCESS;
  }

  if (!field_->nullable() && value_.is_null(value_)) {
    LOG_WARN("update a attr not nullable.");
    return RC::VARIABLE_NOT_VALID;
  }

  auto &child = children_[0];

  RC rc = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }

  trx_ = trx;
  value_.resize(field_->len());

  std::vector<Record> new_records;
  std::vector<Record> old_records;
  while (OB_SUCC(rc = child->next())) {
    Tuple *tuple = child->current_tuple();
    if (nullptr == tuple) {
      LOG_WARN("failed to get current record: %s", strrc(rc));
      return rc;
    }

    RowTuple *row_tuple  = static_cast<RowTuple *>(tuple);
    Record    new_record = row_tuple->record();
    Record    old_record(new_record);
    memcpy(new_record.data() + field_->offset(), value_.data(), field_->len());
    old_records.emplace_back(old_record);
    new_records.emplace_back(new_record);
  }
  child->close();

  for (int i = 0; i < old_records.size(); ++i) {
    rc = table_->update_record(old_records[i].rid(), old_records[i], new_records[i]);
    if (rc != RC::SUCCESS) {
      LOG_WARN("update_record failed. maybe duplicate key.");
      // 回滚。
      RC rc2 = RC::SUCCESS;
      for (int j = i - 1; j >= 0; --j) {
        rc2 = table_->update_record(new_records[j].rid(), new_records[j], old_records[j]);
        if (rc2 != RC::SUCCESS) {
          LOG_WARN("rollback failed while update_record.");
          break;
        }
      }
      break;
    }
  }
  return RC::SUCCESS;
}

RC UpdatePhysicalOperator::next() { return RC::RECORD_EOF; }

RC UpdatePhysicalOperator::close() { return RC::SUCCESS; }
