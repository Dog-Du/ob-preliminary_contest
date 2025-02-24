/* Copyright (c) 2023 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2023/08/16.
//

#pragma once

#include <memory>

#include "common/rc.h"
#include "common/type/attr_type.h"
#include "sql/stmt/create_table_stmt.h"
#include "sql/stmt/groupby_stmt.h"
#include "sql/stmt/orderby_stmt.h"
#include "sql/stmt/update_stmt.h"

class Stmt;
class CalcStmt;
class SelectStmt;
class FilterStmt;
class InsertStmt;
class DeleteStmt;
class ExplainStmt;
class LogicalOperator;

class LogicalPlanGenerator
{
public:
  LogicalPlanGenerator()          = default;
  virtual ~LogicalPlanGenerator() = default;

  RC create(Stmt *stmt, std::shared_ptr<LogicalOperator> &logical_operator);

private:
  RC create_plan(CalcStmt *calc_stmt, std::shared_ptr<LogicalOperator> &logical_operator);
  RC create_plan(SelectStmt *select_stmt, std::shared_ptr<LogicalOperator> &logical_operator);
  RC create_plan(FilterStmt *filter_stmt, std::shared_ptr<LogicalOperator> &logical_operator);
  RC create_plan(InsertStmt *insert_stmt, std::shared_ptr<LogicalOperator> &logical_operator);
  RC create_plan(DeleteStmt *delete_stmt, std::shared_ptr<LogicalOperator> &logical_operator);
  RC create_plan(ExplainStmt *explain_stmt, std::shared_ptr<LogicalOperator> &logical_operator);
  RC create_plan(UpdateStmt *update_stmt, std::shared_ptr<LogicalOperator> &logical_operator);
  RC create_plan(GroupByStmt *groupby_stmt, std::shared_ptr<LogicalOperator> &logical_operator);
  RC create_plan(OrderByStmt *orderby_stmt, std::shared_ptr<LogicalOperator> &logical_operator);
  RC create_plan(CreateTableStmt *create_table_stmt, std::shared_ptr<LogicalOperator> &logical_operator);
  // RC create_group_by_plan(SelectStmt *select_stmt, std::shared_ptr<LogicalOperator> &logical_operator);
  RC join_table_in_tree(std::shared_ptr<LogicalOperator> &oper, Table *table, FilterStmt *filter_stmt,
      SelectStmt *view_select_stmt, const std::string &alias);

  int implicit_cast_cost(AttrType from, AttrType to);
};