/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_YACC_SQL_HPP_INCLUDED
# define YY_YY_YACC_SQL_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    SEMICOLON = 258,               /* SEMICOLON  */
    CREATE = 259,                  /* CREATE  */
    DROP = 260,                    /* DROP  */
    TABLE = 261,                   /* TABLE  */
    TABLES = 262,                  /* TABLES  */
    INDEX = 263,                   /* INDEX  */
    CALC = 264,                    /* CALC  */
    SELECT = 265,                  /* SELECT  */
    SUM = 266,                     /* SUM  */
    MAX = 267,                     /* MAX  */
    MIN = 268,                     /* MIN  */
    COUNT = 269,                   /* COUNT  */
    AVG = 270,                     /* AVG  */
    IS_T = 271,                    /* IS_T  */
    NOT = 272,                     /* NOT  */
    NULL_T = 273,                  /* NULL_T  */
    LIKE_T = 274,                  /* LIKE_T  */
    INNER_T = 275,                 /* INNER_T  */
    JOIN_T = 276,                  /* JOIN_T  */
    IN_T = 277,                    /* IN_T  */
    EXIST_T = 278,                 /* EXIST_T  */
    UNIQUE_T = 279,                /* UNIQUE_T  */
    ORDER_T = 280,                 /* ORDER_T  */
    BY_T = 281,                    /* BY_T  */
    ASC_T = 282,                   /* ASC_T  */
    DESC = 283,                    /* DESC  */
    SHOW = 284,                    /* SHOW  */
    SYNC = 285,                    /* SYNC  */
    INSERT = 286,                  /* INSERT  */
    DELETE = 287,                  /* DELETE  */
    UPDATE = 288,                  /* UPDATE  */
    LBRACE = 289,                  /* LBRACE  */
    RBRACE = 290,                  /* RBRACE  */
    COMMA = 291,                   /* COMMA  */
    TRX_BEGIN = 292,               /* TRX_BEGIN  */
    TRX_COMMIT = 293,              /* TRX_COMMIT  */
    TRX_ROLLBACK = 294,            /* TRX_ROLLBACK  */
    DATE_T = 295,                  /* DATE_T  */
    INT_T = 296,                   /* INT_T  */
    STRING_T = 297,                /* STRING_T  */
    FLOAT_T = 298,                 /* FLOAT_T  */
    HELP = 299,                    /* HELP  */
    EXIT = 300,                    /* EXIT  */
    DOT = 301,                     /* DOT  */
    INTO = 302,                    /* INTO  */
    VALUES = 303,                  /* VALUES  */
    FROM = 304,                    /* FROM  */
    WHERE = 305,                   /* WHERE  */
    AND = 306,                     /* AND  */
    SET = 307,                     /* SET  */
    ON = 308,                      /* ON  */
    LOAD = 309,                    /* LOAD  */
    DATA = 310,                    /* DATA  */
    INFILE = 311,                  /* INFILE  */
    EXPLAIN = 312,                 /* EXPLAIN  */
    EQ = 313,                      /* EQ  */
    LT = 314,                      /* LT  */
    GT = 315,                      /* GT  */
    LE = 316,                      /* LE  */
    GE = 317,                      /* GE  */
    NE = 318,                      /* NE  */
    NUMBER = 319,                  /* NUMBER  */
    FLOAT = 320,                   /* FLOAT  */
    ID = 321,                      /* ID  */
    DATE_STR = 322,                /* DATE_STR  */
    SSS = 323,                     /* SSS  */
    UMINUS = 324                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 128 "yacc_sql.y"

  ParsedSqlNode *                   sql_node;
  ConditionSqlNode *                condition;

  JoinSqlNode *                     join;
  std::vector<JoinSqlNode> *        join_list;

  std::vector<update_value> *       update_list;

  std::vector<OrderBySqlNode> *     order_by_list;
  OrderBySqlNode *                  order_by_node;

  Value *                           value;
  enum CompOp                       comp;
  RelAttrSqlNode *                  rel_attr;
  std::vector<AttrInfoSqlNode> *    attr_infos;
  AttrInfoSqlNode *                 attr_info;
  Expression *                      expression;
  std::vector<Expression *> *       expression_list;
  std::vector<Value> *              value_list;
  std::vector<ConditionSqlNode> *   condition_list;
  std::vector<RelAttrSqlNode> *     rel_attr_list;
  std::vector<std::string> *        relation_list;
  char *                            string;
  int                               number;
  float                             floats;

#line 161 "yacc_sql.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif




int yyparse (const char * sql_string, ParsedSqlResult * sql_result, void * scanner);


#endif /* !YY_YY_YACC_SQL_HPP_INCLUDED  */
