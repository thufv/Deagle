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

#ifndef YY_YYANSI_C_ANSI_C_Y_TAB_HPP_INCLUDED
# define YY_YYANSI_C_ANSI_C_Y_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yyansi_cdebug;
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
    TOK_AUTO = 258,                /* "auto"  */
    TOK_BOOL = 259,                /* "bool"  */
    TOK_COMPLEX = 260,             /* "complex"  */
    TOK_BREAK = 261,               /* "break"  */
    TOK_CASE = 262,                /* "case"  */
    TOK_CHAR = 263,                /* "char"  */
    TOK_CONST = 264,               /* "const"  */
    TOK_CONTINUE = 265,            /* "continue"  */
    TOK_DEFAULT = 266,             /* "default"  */
    TOK_DO = 267,                  /* "do"  */
    TOK_DOUBLE = 268,              /* "double"  */
    TOK_ELSE = 269,                /* "else"  */
    TOK_ENUM = 270,                /* "enum"  */
    TOK_EXTERN = 271,              /* "extern"  */
    TOK_FLOAT = 272,               /* "float"  */
    TOK_FOR = 273,                 /* "for"  */
    TOK_GOTO = 274,                /* "goto"  */
    TOK_IF = 275,                  /* "if"  */
    TOK_INLINE = 276,              /* "inline"  */
    TOK_INT = 277,                 /* "int"  */
    TOK_LONG = 278,                /* "long"  */
    TOK_REGISTER = 279,            /* "register"  */
    TOK_RESTRICT = 280,            /* "restrict"  */
    TOK_RETURN = 281,              /* "return"  */
    TOK_SHORT = 282,               /* "short"  */
    TOK_SIGNED = 283,              /* "signed"  */
    TOK_SIZEOF = 284,              /* "sizeof"  */
    TOK_STATIC = 285,              /* "static"  */
    TOK_STRUCT = 286,              /* "struct"  */
    TOK_SWITCH = 287,              /* "switch"  */
    TOK_TYPEDEF = 288,             /* "typedef"  */
    TOK_UNION = 289,               /* "union"  */
    TOK_UNSIGNED = 290,            /* "unsigned"  */
    TOK_VOID = 291,                /* "void"  */
    TOK_VOLATILE = 292,            /* "volatile"  */
    TOK_WCHAR_T = 293,             /* "wchar_t"  */
    TOK_WHILE = 294,               /* "while"  */
    TOK_ARROW = 295,               /* "->"  */
    TOK_INCR = 296,                /* "++"  */
    TOK_DECR = 297,                /* "--"  */
    TOK_SHIFTLEFT = 298,           /* "<<"  */
    TOK_SHIFTRIGHT = 299,          /* ">>"  */
    TOK_LE = 300,                  /* "<="  */
    TOK_GE = 301,                  /* ">="  */
    TOK_EQ = 302,                  /* "=="  */
    TOK_NE = 303,                  /* "!="  */
    TOK_ANDAND = 304,              /* "&&"  */
    TOK_OROR = 305,                /* "||"  */
    TOK_ELLIPSIS = 306,            /* "..."  */
    TOK_MULTASSIGN = 307,          /* "*="  */
    TOK_DIVASSIGN = 308,           /* "/="  */
    TOK_MODASSIGN = 309,           /* "%="  */
    TOK_PLUSASSIGN = 310,          /* "+="  */
    TOK_MINUSASSIGN = 311,         /* "-="  */
    TOK_SHLASSIGN = 312,           /* "<<="  */
    TOK_SHRASSIGN = 313,           /* ">>="  */
    TOK_ANDASSIGN = 314,           /* "&="  */
    TOK_XORASSIGN = 315,           /* "^="  */
    TOK_ORASSIGN = 316,            /* "|="  */
    TOK_IDENTIFIER = 317,          /* TOK_IDENTIFIER  */
    TOK_TYPEDEFNAME = 318,         /* TOK_TYPEDEFNAME  */
    TOK_INTEGER = 319,             /* TOK_INTEGER  */
    TOK_FLOATING = 320,            /* TOK_FLOATING  */
    TOK_CHARACTER = 321,           /* TOK_CHARACTER  */
    TOK_STRING = 322,              /* TOK_STRING  */
    TOK_ASM_STRING = 323,          /* TOK_ASM_STRING  */
    TOK_INT8 = 324,                /* "__int8"  */
    TOK_INT16 = 325,               /* "__int16"  */
    TOK_INT32 = 326,               /* "__int32"  */
    TOK_INT64 = 327,               /* "__int64"  */
    TOK_PTR32 = 328,               /* "__ptr32"  */
    TOK_PTR64 = 329,               /* "__ptr64"  */
    TOK_TYPEOF = 330,              /* "typeof"  */
    TOK_GCC_AUTO_TYPE = 331,       /* "__auto_type"  */
    TOK_GCC_FLOAT128 = 332,        /* "__float128"  */
    TOK_GCC_INT128 = 333,          /* "__int128"  */
    TOK_GCC_DECIMAL32 = 334,       /* "_Decimal32"  */
    TOK_GCC_DECIMAL64 = 335,       /* "_Decimal64"  */
    TOK_GCC_DECIMAL128 = 336,      /* "_Decimal128"  */
    TOK_GCC_ASM = 337,             /* "__asm__"  */
    TOK_GCC_ASM_PAREN = 338,       /* "__asm__ (with parentheses)"  */
    TOK_GCC_ATTRIBUTE = 339,       /* "__attribute__"  */
    TOK_GCC_ATTRIBUTE_ALIGNED = 340, /* "aligned"  */
    TOK_GCC_ATTRIBUTE_TRANSPARENT_UNION = 341, /* "transparent_union"  */
    TOK_GCC_ATTRIBUTE_PACKED = 342, /* "packed"  */
    TOK_GCC_ATTRIBUTE_VECTOR_SIZE = 343, /* "vector_size"  */
    TOK_GCC_ATTRIBUTE_MODE = 344,  /* "mode"  */
    TOK_GCC_ATTRIBUTE_GNU_INLINE = 345, /* "__gnu_inline__"  */
    TOK_GCC_ATTRIBUTE_END = 346,   /* ")"  */
    TOK_GCC_LABEL = 347,           /* "__label__"  */
    TOK_MSC_ASM = 348,             /* "__asm"  */
    TOK_MSC_BASED = 349,           /* "__based"  */
    TOK_CW_VAR_ARG_TYPEOF = 350,   /* "_var_arg_typeof"  */
    TOK_BUILTIN_VA_ARG = 351,      /* "__builtin_va_arg"  */
    TOK_GCC_BUILTIN_TYPES_COMPATIBLE_P = 352, /* "__builtin_types_compatible_p"  */
    TOK_OFFSETOF = 353,            /* "__offsetof"  */
    TOK_ALIGNOF = 354,             /* "__alignof__"  */
    TOK_MSC_TRY = 355,             /* "__try"  */
    TOK_MSC_FINALLY = 356,         /* "__finally"  */
    TOK_MSC_EXCEPT = 357,          /* "__except"  */
    TOK_MSC_LEAVE = 358,           /* "__leave"  */
    TOK_MSC_DECLSPEC = 359,        /* "__declspec"  */
    TOK_INTERFACE = 360,           /* "__interface"  */
    TOK_CDECL = 361,               /* "__cdecl"  */
    TOK_STDCALL = 362,             /* "__stdcall"  */
    TOK_FASTCALL = 363,            /* "__fastcall"  */
    TOK_CLRCALL = 364,             /* "__clrcall"  */
    TOK_FORALL = 365,              /* "forall"  */
    TOK_EXISTS = 366,              /* "exists"  */
    TOK_ACSL_FORALL = 367,         /* "\\forall"  */
    TOK_ACSL_EXISTS = 368,         /* "\\exists"  */
    TOK_ARRAY_OF = 369,            /* "array_of"  */
    TOK_CPROVER_BITVECTOR = 370,   /* "__CPROVER_bitvector"  */
    TOK_CPROVER_FLOATBV = 371,     /* "__CPROVER_floatbv"  */
    TOK_CPROVER_FIXEDBV = 372,     /* "__CPROVER_fixedbv"  */
    TOK_CPROVER_ATOMIC = 373,      /* "__CPROVER_atomic"  */
    TOK_CPROVER_BOOL = 374,        /* "__CPROVER_bool"  */
    TOK_CPROVER_THROW = 375,       /* "__CPROVER_throw"  */
    TOK_CPROVER_CATCH = 376,       /* "__CPROVER_catch"  */
    TOK_CPROVER_TRY = 377,         /* "__CPROVER_try"  */
    TOK_CPROVER_FINALLY = 378,     /* "__CPROVER_finally"  */
    TOK_IMPLIES = 379,             /* "==>"  */
    TOK_EQUIVALENT = 380,          /* "<==>"  */
    TOK_TRUE = 381,                /* "TRUE"  */
    TOK_FALSE = 382,               /* "FALSE"  */
    TOK_REAL = 383,                /* "__real__"  */
    TOK_IMAG = 384,                /* "__imag__"  */
    TOK_ALIGNAS = 385,             /* "_Alignas"  */
    TOK_ATOMIC_TYPE_QUALIFIER = 386, /* "_Atomic"  */
    TOK_ATOMIC_TYPE_SPECIFIER = 387, /* "_Atomic()"  */
    TOK_GENERIC = 388,             /* "_Generic"  */
    TOK_IMAGINARY = 389,           /* "_Imaginary"  */
    TOK_NORETURN = 390,            /* "_Noreturn"  */
    TOK_STATIC_ASSERT = 391,       /* "_Static_assert"  */
    TOK_THREAD_LOCAL = 392,        /* "_Thread_local"  */
    TOK_NULLPTR = 393,             /* "nullptr"  */
    TOK_CONSTEXPR = 394,           /* "constexpr"  */
    TOK_SCANNER_ERROR = 395,       /* TOK_SCANNER_ERROR  */
    TOK_SCANNER_EOF = 396,         /* TOK_SCANNER_EOF  */
    TOK_CATCH = 397,               /* "catch"  */
    TOK_CLASS = 398,               /* "class"  */
    TOK_DELETE = 399,              /* "delete"  */
    TOK_DECLTYPE = 400,            /* "decltype"  */
    TOK_EXPLICIT = 401,            /* "explicit"  */
    TOK_FRIEND = 402,              /* "friend"  */
    TOK_MUTABLE = 403,             /* "mutable"  */
    TOK_NAMESPACE = 404,           /* "namespace"  */
    TOK_NEW = 405,                 /* "new"  */
    TOK_OPERATOR = 406,            /* "operator"  */
    TOK_PRIVATE = 407,             /* "private"  */
    TOK_PROTECTED = 408,           /* "protected"  */
    TOK_PUBLIC = 409,              /* "public"  */
    TOK_TEMPLATE = 410,            /* "template"  */
    TOK_THIS = 411,                /* "this"  */
    TOK_THROW = 412,               /* "throw"  */
    TOK_TYPEID = 413,              /* "typeid"  */
    TOK_TYPENAME = 414,            /* "typename"  */
    TOK_TRY = 415,                 /* "try"  */
    TOK_USING = 416,               /* "using"  */
    TOK_VIRTUAL = 417,             /* "virtual"  */
    TOK_SCOPE = 418,               /* "::"  */
    TOK_DOTPM = 419,               /* ".*"  */
    TOK_ARROWPM = 420,             /* "->*"  */
    TOK_MSC_UNARY_TYPE_PREDICATE = 421, /* TOK_MSC_UNARY_TYPE_PREDICATE  */
    TOK_MSC_BINARY_TYPE_PREDICATE = 422, /* TOK_MSC_BINARY_TYPE_PREDICATE  */
    TOK_MSC_UUIDOF = 423,          /* "__uuidof"  */
    TOK_MSC_IF_EXISTS = 424,       /* "__if_exists"  */
    TOK_MSC_IF_NOT_EXISTS = 425,   /* "__if_not_exists"  */
    TOK_MSC_UNDERLYING_TYPE = 426  /* "__underlying_type"  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define TOK_AUTO 258
#define TOK_BOOL 259
#define TOK_COMPLEX 260
#define TOK_BREAK 261
#define TOK_CASE 262
#define TOK_CHAR 263
#define TOK_CONST 264
#define TOK_CONTINUE 265
#define TOK_DEFAULT 266
#define TOK_DO 267
#define TOK_DOUBLE 268
#define TOK_ELSE 269
#define TOK_ENUM 270
#define TOK_EXTERN 271
#define TOK_FLOAT 272
#define TOK_FOR 273
#define TOK_GOTO 274
#define TOK_IF 275
#define TOK_INLINE 276
#define TOK_INT 277
#define TOK_LONG 278
#define TOK_REGISTER 279
#define TOK_RESTRICT 280
#define TOK_RETURN 281
#define TOK_SHORT 282
#define TOK_SIGNED 283
#define TOK_SIZEOF 284
#define TOK_STATIC 285
#define TOK_STRUCT 286
#define TOK_SWITCH 287
#define TOK_TYPEDEF 288
#define TOK_UNION 289
#define TOK_UNSIGNED 290
#define TOK_VOID 291
#define TOK_VOLATILE 292
#define TOK_WCHAR_T 293
#define TOK_WHILE 294
#define TOK_ARROW 295
#define TOK_INCR 296
#define TOK_DECR 297
#define TOK_SHIFTLEFT 298
#define TOK_SHIFTRIGHT 299
#define TOK_LE 300
#define TOK_GE 301
#define TOK_EQ 302
#define TOK_NE 303
#define TOK_ANDAND 304
#define TOK_OROR 305
#define TOK_ELLIPSIS 306
#define TOK_MULTASSIGN 307
#define TOK_DIVASSIGN 308
#define TOK_MODASSIGN 309
#define TOK_PLUSASSIGN 310
#define TOK_MINUSASSIGN 311
#define TOK_SHLASSIGN 312
#define TOK_SHRASSIGN 313
#define TOK_ANDASSIGN 314
#define TOK_XORASSIGN 315
#define TOK_ORASSIGN 316
#define TOK_IDENTIFIER 317
#define TOK_TYPEDEFNAME 318
#define TOK_INTEGER 319
#define TOK_FLOATING 320
#define TOK_CHARACTER 321
#define TOK_STRING 322
#define TOK_ASM_STRING 323
#define TOK_INT8 324
#define TOK_INT16 325
#define TOK_INT32 326
#define TOK_INT64 327
#define TOK_PTR32 328
#define TOK_PTR64 329
#define TOK_TYPEOF 330
#define TOK_GCC_AUTO_TYPE 331
#define TOK_GCC_FLOAT128 332
#define TOK_GCC_INT128 333
#define TOK_GCC_DECIMAL32 334
#define TOK_GCC_DECIMAL64 335
#define TOK_GCC_DECIMAL128 336
#define TOK_GCC_ASM 337
#define TOK_GCC_ASM_PAREN 338
#define TOK_GCC_ATTRIBUTE 339
#define TOK_GCC_ATTRIBUTE_ALIGNED 340
#define TOK_GCC_ATTRIBUTE_TRANSPARENT_UNION 341
#define TOK_GCC_ATTRIBUTE_PACKED 342
#define TOK_GCC_ATTRIBUTE_VECTOR_SIZE 343
#define TOK_GCC_ATTRIBUTE_MODE 344
#define TOK_GCC_ATTRIBUTE_GNU_INLINE 345
#define TOK_GCC_ATTRIBUTE_END 346
#define TOK_GCC_LABEL 347
#define TOK_MSC_ASM 348
#define TOK_MSC_BASED 349
#define TOK_CW_VAR_ARG_TYPEOF 350
#define TOK_BUILTIN_VA_ARG 351
#define TOK_GCC_BUILTIN_TYPES_COMPATIBLE_P 352
#define TOK_OFFSETOF 353
#define TOK_ALIGNOF 354
#define TOK_MSC_TRY 355
#define TOK_MSC_FINALLY 356
#define TOK_MSC_EXCEPT 357
#define TOK_MSC_LEAVE 358
#define TOK_MSC_DECLSPEC 359
#define TOK_INTERFACE 360
#define TOK_CDECL 361
#define TOK_STDCALL 362
#define TOK_FASTCALL 363
#define TOK_CLRCALL 364
#define TOK_FORALL 365
#define TOK_EXISTS 366
#define TOK_ACSL_FORALL 367
#define TOK_ACSL_EXISTS 368
#define TOK_ARRAY_OF 369
#define TOK_CPROVER_BITVECTOR 370
#define TOK_CPROVER_FLOATBV 371
#define TOK_CPROVER_FIXEDBV 372
#define TOK_CPROVER_ATOMIC 373
#define TOK_CPROVER_BOOL 374
#define TOK_CPROVER_THROW 375
#define TOK_CPROVER_CATCH 376
#define TOK_CPROVER_TRY 377
#define TOK_CPROVER_FINALLY 378
#define TOK_IMPLIES 379
#define TOK_EQUIVALENT 380
#define TOK_TRUE 381
#define TOK_FALSE 382
#define TOK_REAL 383
#define TOK_IMAG 384
#define TOK_ALIGNAS 385
#define TOK_ATOMIC_TYPE_QUALIFIER 386
#define TOK_ATOMIC_TYPE_SPECIFIER 387
#define TOK_GENERIC 388
#define TOK_IMAGINARY 389
#define TOK_NORETURN 390
#define TOK_STATIC_ASSERT 391
#define TOK_THREAD_LOCAL 392
#define TOK_NULLPTR 393
#define TOK_CONSTEXPR 394
#define TOK_SCANNER_ERROR 395
#define TOK_SCANNER_EOF 396
#define TOK_CATCH 397
#define TOK_CLASS 398
#define TOK_DELETE 399
#define TOK_DECLTYPE 400
#define TOK_EXPLICIT 401
#define TOK_FRIEND 402
#define TOK_MUTABLE 403
#define TOK_NAMESPACE 404
#define TOK_NEW 405
#define TOK_OPERATOR 406
#define TOK_PRIVATE 407
#define TOK_PROTECTED 408
#define TOK_PUBLIC 409
#define TOK_TEMPLATE 410
#define TOK_THIS 411
#define TOK_THROW 412
#define TOK_TYPEID 413
#define TOK_TYPENAME 414
#define TOK_TRY 415
#define TOK_USING 416
#define TOK_VIRTUAL 417
#define TOK_SCOPE 418
#define TOK_DOTPM 419
#define TOK_ARROWPM 420
#define TOK_MSC_UNARY_TYPE_PREDICATE 421
#define TOK_MSC_BINARY_TYPE_PREDICATE 422
#define TOK_MSC_UUIDOF 423
#define TOK_MSC_IF_EXISTS 424
#define TOK_MSC_IF_NOT_EXISTS 425
#define TOK_MSC_UNDERLYING_TYPE 426

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yyansi_clval;


int yyansi_cparse (void);


#endif /* !YY_YYANSI_C_ANSI_C_Y_TAB_HPP_INCLUDED  */
