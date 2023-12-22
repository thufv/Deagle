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

#ifndef YY_YYANSI_C_ANSI_C_Y_TAB_H_INCLUDED
# define YY_YYANSI_C_ANSI_C_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yyansi_cdebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOK_AUTO = 258,
    TOK_BOOL = 259,
    TOK_COMPLEX = 260,
    TOK_BREAK = 261,
    TOK_CASE = 262,
    TOK_CHAR = 263,
    TOK_CONST = 264,
    TOK_CONTINUE = 265,
    TOK_DEFAULT = 266,
    TOK_DO = 267,
    TOK_DOUBLE = 268,
    TOK_ELSE = 269,
    TOK_ENUM = 270,
    TOK_EXTERN = 271,
    TOK_FLOAT = 272,
    TOK_FOR = 273,
    TOK_GOTO = 274,
    TOK_IF = 275,
    TOK_INLINE = 276,
    TOK_INT = 277,
    TOK_LONG = 278,
    TOK_REGISTER = 279,
    TOK_RESTRICT = 280,
    TOK_RETURN = 281,
    TOK_SHORT = 282,
    TOK_SIGNED = 283,
    TOK_SIZEOF = 284,
    TOK_STATIC = 285,
    TOK_STRUCT = 286,
    TOK_SWITCH = 287,
    TOK_TYPEDEF = 288,
    TOK_UNION = 289,
    TOK_UNSIGNED = 290,
    TOK_VOID = 291,
    TOK_VOLATILE = 292,
    TOK_WCHAR_T = 293,
    TOK_WHILE = 294,
    TOK_ARROW = 295,
    TOK_INCR = 296,
    TOK_DECR = 297,
    TOK_SHIFTLEFT = 298,
    TOK_SHIFTRIGHT = 299,
    TOK_LE = 300,
    TOK_GE = 301,
    TOK_EQ = 302,
    TOK_NE = 303,
    TOK_ANDAND = 304,
    TOK_OROR = 305,
    TOK_ELLIPSIS = 306,
    TOK_MULTASSIGN = 307,
    TOK_DIVASSIGN = 308,
    TOK_MODASSIGN = 309,
    TOK_PLUSASSIGN = 310,
    TOK_MINUSASSIGN = 311,
    TOK_SHLASSIGN = 312,
    TOK_SHRASSIGN = 313,
    TOK_ANDASSIGN = 314,
    TOK_XORASSIGN = 315,
    TOK_ORASSIGN = 316,
    TOK_IDENTIFIER = 317,
    TOK_TYPEDEFNAME = 318,
    TOK_INTEGER = 319,
    TOK_FLOATING = 320,
    TOK_CHARACTER = 321,
    TOK_STRING = 322,
    TOK_ASM_STRING = 323,
    TOK_INT8 = 324,
    TOK_INT16 = 325,
    TOK_INT32 = 326,
    TOK_INT64 = 327,
    TOK_PTR32 = 328,
    TOK_PTR64 = 329,
    TOK_TYPEOF = 330,
    TOK_GCC_AUTO_TYPE = 331,
    TOK_GCC_FLOAT16 = 332,
    TOK_GCC_FLOAT32 = 333,
    TOK_GCC_FLOAT32X = 334,
    TOK_GCC_FLOAT80 = 335,
    TOK_GCC_FLOAT64 = 336,
    TOK_GCC_FLOAT64X = 337,
    TOK_GCC_FLOAT128 = 338,
    TOK_GCC_FLOAT128X = 339,
    TOK_GCC_INT128 = 340,
    TOK_GCC_DECIMAL32 = 341,
    TOK_GCC_DECIMAL64 = 342,
    TOK_GCC_DECIMAL128 = 343,
    TOK_GCC_ASM = 344,
    TOK_GCC_ASM_PAREN = 345,
    TOK_GCC_ATTRIBUTE = 346,
    TOK_GCC_ATTRIBUTE_ALIGNED = 347,
    TOK_GCC_ATTRIBUTE_TRANSPARENT_UNION = 348,
    TOK_GCC_ATTRIBUTE_PACKED = 349,
    TOK_GCC_ATTRIBUTE_VECTOR_SIZE = 350,
    TOK_GCC_ATTRIBUTE_MODE = 351,
    TOK_GCC_ATTRIBUTE_GNU_INLINE = 352,
    TOK_GCC_ATTRIBUTE_WEAK = 353,
    TOK_GCC_ATTRIBUTE_ALIAS = 354,
    TOK_GCC_ATTRIBUTE_SECTION = 355,
    TOK_GCC_ATTRIBUTE_NORETURN = 356,
    TOK_GCC_ATTRIBUTE_CONSTRUCTOR = 357,
    TOK_GCC_ATTRIBUTE_DESTRUCTOR = 358,
    TOK_GCC_ATTRIBUTE_FALLTHROUGH = 359,
    TOK_GCC_ATTRIBUTE_USED = 360,
    TOK_GCC_LABEL = 361,
    TOK_MSC_ASM = 362,
    TOK_MSC_BASED = 363,
    TOK_CW_VAR_ARG_TYPEOF = 364,
    TOK_BUILTIN_VA_ARG = 365,
    TOK_GCC_BUILTIN_TYPES_COMPATIBLE_P = 366,
    TOK_CLANG_BUILTIN_CONVERTVECTOR = 367,
    TOK_OFFSETOF = 368,
    TOK_ALIGNOF = 369,
    TOK_MSC_TRY = 370,
    TOK_MSC_FINALLY = 371,
    TOK_MSC_EXCEPT = 372,
    TOK_MSC_LEAVE = 373,
    TOK_MSC_DECLSPEC = 374,
    TOK_MSC_FORCEINLINE = 375,
    TOK_INTERFACE = 376,
    TOK_CDECL = 377,
    TOK_STDCALL = 378,
    TOK_FASTCALL = 379,
    TOK_CLRCALL = 380,
    TOK_FORALL = 381,
    TOK_EXISTS = 382,
    TOK_ACSL_FORALL = 383,
    TOK_ACSL_EXISTS = 384,
    TOK_ACSL_LAMBDA = 385,
    TOK_ACSL_LET = 386,
    TOK_ARRAY_OF = 387,
    TOK_CPROVER_BITVECTOR = 388,
    TOK_CPROVER_FLOATBV = 389,
    TOK_CPROVER_FIXEDBV = 390,
    TOK_CPROVER_ATOMIC = 391,
    TOK_CPROVER_BOOL = 392,
    TOK_CPROVER_THROW = 393,
    TOK_CPROVER_CATCH = 394,
    TOK_CPROVER_TRY = 395,
    TOK_CPROVER_FINALLY = 396,
    TOK_CPROVER_ID = 397,
    TOK_CPROVER_LOOP_INVARIANT = 398,
    TOK_CPROVER_DECREASES = 399,
    TOK_CPROVER_REQUIRES_CONTRACT = 400,
    TOK_CPROVER_REQUIRES = 401,
    TOK_CPROVER_ENSURES_CONTRACT = 402,
    TOK_CPROVER_ENSURES = 403,
    TOK_CPROVER_ASSIGNS = 404,
    TOK_CPROVER_FREES = 405,
    TOK_IMPLIES = 406,
    TOK_EQUIVALENT = 407,
    TOK_XORXOR = 408,
    TOK_TRUE = 409,
    TOK_FALSE = 410,
    TOK_REAL = 411,
    TOK_IMAG = 412,
    TOK_ALIGNAS = 413,
    TOK_ATOMIC_TYPE_QUALIFIER = 414,
    TOK_ATOMIC_TYPE_SPECIFIER = 415,
    TOK_GENERIC = 416,
    TOK_IMAGINARY = 417,
    TOK_NORETURN = 418,
    TOK_STATIC_ASSERT = 419,
    TOK_THREAD_LOCAL = 420,
    TOK_NULLPTR = 421,
    TOK_CONSTEXPR = 422,
    TOK_SCANNER_ERROR = 423,
    TOK_SCANNER_EOF = 424,
    TOK_CATCH = 425,
    TOK_CHAR16_T = 426,
    TOK_CHAR32_T = 427,
    TOK_CLASS = 428,
    TOK_DELETE = 429,
    TOK_DECLTYPE = 430,
    TOK_EXPLICIT = 431,
    TOK_FRIEND = 432,
    TOK_MUTABLE = 433,
    TOK_NAMESPACE = 434,
    TOK_NEW = 435,
    TOK_NOEXCEPT = 436,
    TOK_OPERATOR = 437,
    TOK_PRIVATE = 438,
    TOK_PROTECTED = 439,
    TOK_PUBLIC = 440,
    TOK_TEMPLATE = 441,
    TOK_THIS = 442,
    TOK_THROW = 443,
    TOK_TYPEID = 444,
    TOK_TYPENAME = 445,
    TOK_TRY = 446,
    TOK_USING = 447,
    TOK_VIRTUAL = 448,
    TOK_SCOPE = 449,
    TOK_DOTPM = 450,
    TOK_ARROWPM = 451,
    TOK_UNARY_TYPE_PREDICATE = 452,
    TOK_BINARY_TYPE_PREDICATE = 453,
    TOK_MSC_UUIDOF = 454,
    TOK_MSC_IF_EXISTS = 455,
    TOK_MSC_IF_NOT_EXISTS = 456,
    TOK_UNDERLYING_TYPE = 457
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yyansi_clval;

int yyansi_cparse (void);

#endif /* !YY_YYANSI_C_ANSI_C_Y_TAB_H_INCLUDED  */
