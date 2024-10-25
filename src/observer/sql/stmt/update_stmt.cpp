/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2022/5/22.
//

#include "sql/stmt/update_stmt.h"
#include "common/log/log.h"
#include "common/value.h"
#include "storage/db/db.h"
#include "storage/field/field_meta.h"
#include "storage/table/table.h"
#include "sql/stmt/filter_stmt.h"

UpdateStmt::UpdateStmt(Table *table, const FieldMeta *field, const Value &value, FilterStmt *filter_stmt)
    : table_(table), field_(field), value_(value), filter_stmt_(filter_stmt)
{}

RC UpdateStmt::create(Db *db, const UpdateSqlNode &update_sql, Stmt *&stmt)
{
  const char *table_name = update_sql.relation_name.c_str();
  const char *attr_name  = update_sql.attribute_name.c_str();

  if (nullptr == db || nullptr == table_name || nullptr == attr_name) {
    return RC::INVALID_ARGUMENT;
  }

  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }
  const FieldMeta *field = table->table_meta().field(update_sql.attribute_name.c_str());

  if (nullptr == field) {
    return RC::SCHEMA_FIELD_NOT_EXIST;
  }

  Value val = update_sql.value;
  if (update_sql.value.attr_type() != field->type() &&
      update_sql.value.cast_to(update_sql.value, field->type(), val) != RC::SUCCESS) {
    return RC::VARIABLE_NOT_VALID;
  }

  std::unordered_map<std::string, Table *> table_map;
  table_map.insert(std::pair<std::string, Table *>(std::string(table_name), table));

  RC          rc          = RC::SUCCESS;
  FilterStmt *filter_stmt = nullptr;

  if (update_sql.conditions != nullptr) {
    rc = FilterStmt::create(db, table, &table_map, update_sql.conditions, filter_stmt);
    if (rc != RC::SUCCESS) {
      return rc;
    }
  }

  stmt = new UpdateStmt(table, field, val, filter_stmt);
  return RC::SUCCESS;
}
