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

#include "sql/stmt/filter_stmt.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/value.h"
#include "sql/stmt/delete_stmt.h"
#include "storage/db/db.h"
#include "storage/table/table.h"

FilterStmt::~FilterStmt()
{
  for (FilterUnit *unit : filter_units_) {
    delete unit;
  }
  filter_units_.clear();
}

RC FilterStmt::create(Db *db, Table *default_table,
    std::unordered_map<std::string, Table *> *tables, const ConditionSqlNode *conditions,
    int condition_num, FilterStmt *&stmt)
{
  RC rc = RC::SUCCESS;
  stmt  = nullptr;

  FilterStmt *tmp_stmt = new FilterStmt();

  // 循环，遍历所有where里面的condition
  for (int i = 0; i < condition_num; i++) {
    // 在这里得知表和值的类型，可以进行转化，
    // select、delete都需要在这里进行筛选，所以在这里进行修改可以一劳两逸。
    /// TODO: update暂时不知道需要怎么做。
    // update 已经完成
    if (default_table != nullptr && conditions[i].left_is_attr == 0 &&
        conditions[i].right_is_attr == 1 && conditions[i].left_value.attr_type() == CHARS &&
        default_table->table_meta()
                .field(conditions[i].right_attr.attribute_name.c_str())
                ->type() == DATES) {
      date_t v = str_to_date(conditions[i].left_value.data());

      if (check_date(v)) {
        Value *value = const_cast<Value *>(&conditions[i].left_value);
        value->set_date(v);
      } else {
        rc = RC::VARIABLE_NOT_VALID;
        delete tmp_stmt;
        return rc;
      }
    }

    if (default_table != nullptr && conditions[i].right_is_attr == 0 &&
        conditions[i].left_is_attr == 1 && conditions[i].right_value.attr_type() == CHARS &&
        default_table->table_meta().field(conditions[i].left_attr.attribute_name.c_str())->type() ==
            DATES) {
      date_t v = str_to_date(conditions[i].right_value.data());

      if (check_date(v)) {
        Value *value = const_cast<Value *>(&conditions[i].right_value);
        value->set_date(v);
      } else {
        rc = RC::VARIABLE_NOT_VALID;
        delete tmp_stmt;
        return rc;
      }
    }

    FilterUnit *filter_unit = nullptr;

    // 对第i个condition进行处理。...
    rc = create_filter_unit(db, default_table, tables, conditions[i], filter_unit);
    if (rc != RC::SUCCESS) {
      delete tmp_stmt;
      LOG_WARN("failed to create filter unit. condition index=%d", i);
      return rc;
    }
    tmp_stmt->filter_units_.push_back(filter_unit);
  }

  stmt = tmp_stmt;
  return rc;
}

RC get_table_and_field(Db *db, Table *default_table,
    std::unordered_map<std::string, Table *> *tables, const RelAttrSqlNode &attr, Table *&table,
    const FieldMeta *&field)
{
  // 空白
  if (common::is_blank(attr.relation_name.c_str())) {
    table = default_table;
  } else if (nullptr != tables) {
    auto iter = tables->find(attr.relation_name);
    if (iter != tables->end()) {
      table = iter->second;
    }
  } else {
    table = db->find_table(attr.relation_name.c_str());
  }

  if (nullptr == table) {
    LOG_WARN("No such table: attr.relation_name: %s", attr.relation_name.c_str());
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  field = table->table_meta().field(attr.attribute_name.c_str());
  if (nullptr == field) {
    LOG_WARN("no such field in table: table %s, field %s", table->name(), attr.attribute_name.c_str());
    table = nullptr;
    return RC::SCHEMA_FIELD_NOT_EXIST;
  }

  return RC::SUCCESS;
}

RC FilterStmt::create_filter_unit(Db *db, Table *default_table,
    std::unordered_map<std::string, Table *> *tables, const ConditionSqlNode &condition,
    FilterUnit *&filter_unit)
{
  RC rc = RC::SUCCESS;

  CompOp comp = condition.comp;
  if (comp < EQUAL_TO || comp > IS_NOT) {
    LOG_WARN("invalid compare operator : %d", comp);
    return RC::INVALID_ARGUMENT;
  }

  filter_unit = new FilterUnit;

  // 在inner join里面下面两个if应该都会进入if。

  // 是属性名称
  if (condition.left_is_attr) {
    Table           *table = nullptr;
    const FieldMeta *field = nullptr;
    rc = get_table_and_field(db, default_table, tables, condition.left_attr, table, field);
    if (rc != RC::SUCCESS) {
      LOG_WARN("cannot find attr");
      return rc;
    }
    FilterObj filter_obj;
    filter_obj.init_attr(Field(table, field));
    filter_unit->set_left(filter_obj);
  } else {
    FilterObj filter_obj;
    filter_obj.init_value(condition.left_value);
    filter_unit->set_left(filter_obj);
  }

  if (condition.right_is_attr) {
    Table           *table = nullptr;
    const FieldMeta *field = nullptr;
    rc = get_table_and_field(db, default_table, tables, condition.right_attr, table, field);
    if (rc != RC::SUCCESS) {
      LOG_WARN("cannot find attr");
      return rc;
    }
    FilterObj filter_obj;
    filter_obj.init_attr(Field(table, field));
    filter_unit->set_right(filter_obj);
  } else {
    FilterObj filter_obj;
    filter_obj.init_value(condition.right_value);
    filter_unit->set_right(filter_obj);
  }

  filter_unit->set_comp(comp);

  // 检查两个类型是否能够比较
  return rc;
}
