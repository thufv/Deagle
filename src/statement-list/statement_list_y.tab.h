/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YYSTATEMENT_LIST_STATEMENT_LIST_Y_TAB_H_INCLUDED
# define YY_YYSTATEMENT_LIST_STATEMENT_LIST_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yystatement_listdebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOK_VERSION = 258,
    TOK_BEGIN = 259,
    TOK_FUNCTION_BLOCK = 260,
    TOK_END_FUNCTION_BLOCK = 261,
    TOK_FUNCTION = 262,
    TOK_END_FUNCTION = 263,
    TOK_VAR_INPUT = 264,
    TOK_VAR_INOUT = 265,
    TOK_VAR_OUTPUT = 266,
    TOK_VAR_STATIC = 267,
    TOK_VAR_TEMP = 268,
    TOK_VAR_CONSTANT = 269,
    TOK_END_VAR = 270,
    TOK_NETWORK = 271,
    TOK_TITLE = 272,
    TOK_TAG = 273,
    TOK_END_TAG = 274,
    TOK_INT = 275,
    TOK_DINT = 276,
    TOK_REAL = 277,
    TOK_BOOL = 278,
    TOK_VOID = 279,
    TOK_LOAD = 280,
    TOK_TRANSFER = 281,
    TOK_CALL = 282,
    TOK_NOP = 283,
    TOK_SET_RLO = 284,
    TOK_CLR_RLO = 285,
    TOK_SET = 286,
    TOK_RESET = 287,
    TOK_NOT = 288,
    TOK_AND = 289,
    TOK_AND_NOT = 290,
    TOK_OR = 291,
    TOK_OR_NOT = 292,
    TOK_XOR = 293,
    TOK_XOR_NOT = 294,
    TOK_AND_NESTED = 295,
    TOK_AND_NOT_NESTED = 296,
    TOK_OR_NESTED = 297,
    TOK_OR_NOT_NESTED = 298,
    TOK_XOR_NESTED = 299,
    TOK_XOR_NOT_NESTED = 300,
    TOK_NESTING_CLOSED = 301,
    TOK_ASSIGN = 302,
    TOK_CONST_ADD = 303,
    TOK_ACCU_INT_ADD = 304,
    TOK_ACCU_INT_SUB = 305,
    TOK_ACCU_INT_MUL = 306,
    TOK_ACCU_INT_DIV = 307,
    TOK_ACCU_INT_EQ = 308,
    TOK_ACCU_INT_NEQ = 309,
    TOK_ACCU_INT_GT = 310,
    TOK_ACCU_INT_LT = 311,
    TOK_ACCU_INT_GTE = 312,
    TOK_ACCU_INT_LTE = 313,
    TOK_ACCU_REAL_ADD = 314,
    TOK_ACCU_REAL_SUB = 315,
    TOK_ACCU_REAL_MUL = 316,
    TOK_ACCU_REAL_DIV = 317,
    TOK_ACCU_REAL_EQ = 318,
    TOK_ACCU_REAL_NEQ = 319,
    TOK_ACCU_REAL_GT = 320,
    TOK_ACCU_REAL_LT = 321,
    TOK_ACCU_REAL_GTE = 322,
    TOK_ACCU_REAL_LTE = 323,
    TOK_ACCU_DINT_ADD = 324,
    TOK_ACCU_DINT_SUB = 325,
    TOK_ACCU_DINT_MUL = 326,
    TOK_ACCU_DINT_DIV = 327,
    TOK_ACCU_DINT_EQ = 328,
    TOK_ACCU_DINT_NEQ = 329,
    TOK_ACCU_DINT_GT = 330,
    TOK_ACCU_DINT_LT = 331,
    TOK_ACCU_DINT_GTE = 332,
    TOK_ACCU_DINT_LTE = 333,
    TOK_ASSIGNMENT = 334,
    TOK_JUMP_UNCONDITIONAL = 335,
    TOK_JUMP_CONDITIONAL = 336,
    TOK_JUMP_CONDITIONAL_NOT = 337,
    TOK_INT_LITERAL = 338,
    TOK_BOOL_LITERAL = 339,
    TOK_REAL_LITERAL = 340,
    TOK_IDENTIFIER = 341,
    TOK_TITLE_VALUE = 342,
    TOK_VERSION_VALUE = 343,
    TOK_LABEL = 344
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yystatement_listlval;

int yystatement_listparse (void);

#endif /* !YY_YYSTATEMENT_LIST_STATEMENT_LIST_Y_TAB_H_INCLUDED  */
