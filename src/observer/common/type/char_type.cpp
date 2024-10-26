/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "common/lang/comparator.h"
#include "common/log/log.h"
#include "common/type/attr_type.h"
#include "common/type/char_type.h"
#include "common/type/date_type.h"
#include "common/value.h"

int CharType::compare(const Value &left, const Value &right) const
{
  ASSERT(left.attr_type() == AttrType::CHARS, "invalid type");

  if (left.is_null(left)) {
    return -1;
  }

  if (right.is_null(right)) {
    return 1;
  }

  switch (right.attr_type()) {
    case AttrType::CHARS: {
      return common::compare_string(
          (void *)left.value_.pointer_value_, left.length_, (void *)right.value_.pointer_value_, right.length_);
    } break;
    case AttrType::DATES: {
      Value date_val;
      left.cast_to(left, AttrType::DATES, date_val);
      return date_val.compare(right);
    } break;
    default: {
      return INT32_MAX;
    } break;
  }
  return INT32_MAX;
}

RC CharType::set_value_from_str(Value &val, const string &data) const
{
  val.set_string(data.c_str());
  return RC::SUCCESS;
}

bool CharType::is_null(const Value &val) const
{
  return val.length_ == 0 || val.value_.pointer_value_ == nullptr || val.value_.pointer_value_[0] == '\0';
}

RC CharType::cast_to(const Value &val, AttrType type, Value &result) const
{
  switch (type) {
    case AttrType::DATES: {
      result.attr_type_ = AttrType::DATES;
      int y, m, d;
      if (sscanf(val.value_.pointer_value_, "%d-%d-%d", &y, &m, &d) != 3) {
        return RC::INVALID_ARGUMENT;
      }
      bool check = DateType::check_data(y, m, d);
      if (!check) {
        return RC::INVALID_ARGUMENT;
      }
      result.set_date(y, m, d);
    } break;
    case AttrType::CHARS: {
      result.set_string(val.data(), val.length());
      return RC::SUCCESS;
    };
    default: {
      return RC::UNIMPLEMENTED;
    }
  }
  return RC::SUCCESS;
}

int CharType::cast_cost(AttrType type)
{
  if (type == AttrType::CHARS) {
    return 0;
  }
  if (type == AttrType::DATES) {
    return 1;
  }
  return INT32_MAX;
}

RC CharType::to_string(const Value &val, string &result) const
{
  stringstream ss;
  ss << val.value_.pointer_value_;
  result = ss.str();
  return RC::SUCCESS;
}