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
// Created by Wangyunlai on 2023/4/25.
//

#include "sql/stmt/create_index_stmt.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "common/type/attr_type.h"
#include "storage/db/db.h"
#include "storage/field/field_meta.h"
#include "storage/table/table.h"
#include "sql/expr/expression.h"

using namespace std;
using namespace common;

bool str_equal(const string &l, const string &r)
{
  if (l.size() != r.size()) {
    return false;
  }

  for (size_t i = 0; i < l.size(); ++i) {
    if (tolower(l[i]) != tolower(r[i])) {
      return false;
    }
  }
  return true;
}

RC CreateIndexStmt::create(Db *db, const CreateIndexSqlNode &create_index, Stmt *&stmt)
{
  stmt = nullptr;

  const char *table_name = create_index.relation_name.c_str();
  if (is_blank(table_name) || is_blank(create_index.index_name.c_str())) {
    LOG_WARN("invalid argument. db=%p, table_name=%p, index name=%s",
        db, table_name, create_index.index_name.c_str());
    return RC::INVALID_ARGUMENT;
  }

  // check whether the table exists
  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  Index *index = table->find_index(create_index.index_name.c_str());
  if (nullptr != index) {
    LOG_WARN("index with name(%s) already exists. table name=%s", create_index.index_name.c_str(), table_name);
    return RC::SCHEMA_INDEX_NAME_REPEAT;
  }

  std::vector<const FieldMeta *> fields_meta;

  for (auto &attr_name : create_index.attr_names) {
    const FieldMeta *field_meta = table->table_meta().field(attr_name.c_str());
    if (nullptr == field_meta) {
      LOG_WARN("no such field in table. db=%s, table=%s, field name=%s",
             db->name(), table_name, attr_name.c_str());
      return RC::SCHEMA_FIELD_NOT_EXIST;
    }
    fields_meta.push_back(field_meta);
  }

  if (create_index.is_vector_index) {
    for (auto field : fields_meta) {
      if (field->type() != AttrType::VECTORS) {
        return RC::INVALID_ARGUMENT;
      }
    }

    if (str_equal(create_index.algorithm_type, "ivfflat") == false) {
      return RC::INVALID_ARGUMENT;
    }
  }

  stmt = new CreateIndexStmt(table, fields_meta, create_index.index_name, create_index.unique);
  return RC::SUCCESS;
}
