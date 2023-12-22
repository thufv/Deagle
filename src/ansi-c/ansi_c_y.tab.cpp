/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         yyansi_cparse
#define yylex           yyansi_clex
#define yyerror         yyansi_cerror
#define yydebug         yyansi_cdebug
#define yynerrs         yyansi_cnerrs
#define yylval          yyansi_clval
#define yychar          yyansi_cchar

/* First part of user prologue.  */
#line 1 "parser.y"


/*
 * This parser is based on:
 *
 * c5.y, a ANSI-C grammar written by James A. Roskind.
 * "Portions Copyright (c) 1989, 1990 James A. Roskind".
 * (http://www.idiom.com/free-compilers/,
 * ftp://ftp.infoseek.com/ftp/pub/c++grammar/,
 * ftp://ftp.sra.co.jp/.a/pub/cmd/c++grammar2.0.tar.gz)
 */

#ifdef ANSI_C_DEBUG
#define YYDEBUG 1
#endif
#define PARSER ansi_c_parser

#include "ansi_c_parser.h"

int yyansi_clex();
extern char *yyansi_ctext;

#include "parser_static.inc"

#include "literals/convert_integer_literal.h"

#include "ansi_c_y.tab.h"

#include <util/mathematical_expr.h>

#ifdef _MSC_VER
// possible loss of data
#pragma warning(disable:4242)
// possible loss of data
#pragma warning(disable:4244)
// signed/unsigned mismatch
#pragma warning(disable:4365)
// switch with default but no case labels
#pragma warning(disable:4065)
// unreachable code
#pragma warning(disable:4702)
#endif

// statements have right recursion, deep nesting of statements thus
// requires more stack space
#define YYMAXDEPTH 25600

/*** token declaration **************************************************/
#line 284 "parser.y"

/************************************************************************/
/*** rules **************************************************************/
/************************************************************************/

#line 132 "ansi_c_y.tab.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  149
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   8028

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  227
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  226
/* YYNRULES -- Number of rules.  */
#define YYNRULES  650
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1175

#define YYUNDEFTOK  2
#define YYMAXUTOK   457


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   217,     2,     2,     2,   219,   212,     2,
     203,   204,   213,   214,   205,   215,   207,   218,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   206,   226,
     220,   225,   221,   224,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   208,     2,   209,   222,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   210,   223,   211,   216,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   292,   292,   298,   299,   314,   318,   322,   326,   330,
     335,   336,   337,   338,   344,   345,   346,   348,   349,   350,
     351,   352,   353,   354,   358,   368,   372,   379,   386,   396,
     403,   417,   427,   436,   446,   454,   461,   468,   481,   489,
     500,   505,   507,   513,   514,   518,   520,   526,   527,   531,
     541,   542,   544,   554,   564,   570,   576,   582,   589,   599,
     613,   614,   618,   623,   631,   632,   638,   644,   649,   660,
     665,   670,   675,   680,   685,   690,   695,   701,   710,   711,
     718,   723,   731,   732,   734,   736,   741,   742,   744,   749,
     750,   752,   757,   758,   760,   762,   764,   769,   770,   772,
     777,   778,   783,   784,   789,   790,   795,   796,   801,   802,
     807,   808,   816,   817,   825,   826,   832,   833,   841,   849,
     860,   861,   868,   878,   879,   881,   883,   885,   887,   889,
     891,   893,   895,   897,   899,   904,   905,   910,   915,   916,
     922,   928,   934,   935,   936,   940,   952,   951,   964,   963,
     976,   975,   991,   998,  1002,  1006,  1011,  1014,  1020,  1018,
    1036,  1034,  1050,  1068,  1066,  1083,  1084,  1085,  1086,  1087,
    1091,  1092,  1093,  1094,  1095,  1099,  1100,  1104,  1105,  1109,
    1116,  1117,  1124,  1131,  1132,  1139,  1140,  1144,  1145,  1146,
    1147,  1148,  1149,  1150,  1151,  1152,  1156,  1161,  1169,  1170,
    1174,  1175,  1176,  1180,  1181,  1188,  1192,  1196,  1200,  1207,
    1211,  1215,  1219,  1226,  1230,  1234,  1241,  1242,  1246,  1253,
    1257,  1261,  1268,  1272,  1276,  1283,  1287,  1291,  1298,  1302,
    1306,  1313,  1318,  1326,  1327,  1331,  1335,  1342,  1351,  1352,
    1356,  1360,  1367,  1371,  1375,  1382,  1383,  1387,  1391,  1395,
    1399,  1403,  1407,  1414,  1419,  1427,  1430,  1447,  1448,  1449,
    1450,  1451,  1452,  1453,  1454,  1455,  1456,  1478,  1479,  1480,
    1481,  1482,  1483,  1484,  1485,  1486,  1487,  1488,  1489,  1490,
    1491,  1492,  1493,  1494,  1495,  1496,  1497,  1498,  1499,  1500,
    1501,  1502,  1503,  1504,  1505,  1511,  1518,  1525,  1529,  1530,
    1531,  1535,  1540,  1552,  1549,  1570,  1566,  1590,  1586,  1603,
    1605,  1610,  1612,  1614,  1616,  1618,  1620,  1622,  1624,  1626,
    1628,  1630,  1632,  1634,  1636,  1642,  1645,  1650,  1654,  1655,
    1662,  1664,  1670,  1673,  1677,  1678,  1686,  1689,  1693,  1698,
    1706,  1707,  1708,  1713,  1720,  1732,  1740,  1768,  1776,  1787,
    1790,  1801,  1810,  1822,  1825,  1829,  1842,  1839,  1859,  1855,
    1885,  1886,  1892,  1893,  1897,  1901,  1908,  1912,  1917,  1926,
    1929,  1933,  1938,  1943,  1950,  1961,  1965,  1972,  1973,  1982,
    1987,  1994,  2003,  2008,  2016,  2024,  2031,  2039,  2048,  2056,
    2063,  2071,  2079,  2086,  2094,  2102,  2110,  2117,  2128,  2129,
    2133,  2137,  2142,  2146,  2155,  2159,  2169,  2170,  2176,  2185,
    2193,  2201,  2203,  2211,  2212,  2220,  2226,  2241,  2248,  2255,
    2263,  2270,  2278,  2290,  2291,  2292,  2293,  2294,  2295,  2296,
    2297,  2298,  2299,  2300,  2301,  2302,  2306,  2315,  2323,  2330,
    2339,  2350,  2362,  2369,  2377,  2389,  2396,  2401,  2408,  2423,
    2430,  2437,  2446,  2447,  2451,  2470,  2489,  2488,  2528,  2545,
    2552,  2554,  2556,  2562,  2567,  2589,  2594,  2601,  2605,  2611,
    2622,  2628,  2637,  2646,  2654,  2662,  2667,  2675,  2685,  2687,
    2688,  2689,  2701,  2707,  2714,  2722,  2731,  2743,  2747,  2751,
    2755,  2761,  2771,  2776,  2784,  2788,  2792,  2798,  2808,  2813,
    2821,  2825,  2829,  2837,  2842,  2850,  2854,  2858,  2863,  2871,
    2879,  2881,  2885,  2886,  2890,  2895,  2899,  2900,  2904,  2908,
    2915,  2936,  2941,  2944,  2948,  2953,  2961,  2966,  2967,  2971,
    2978,  2982,  2986,  2990,  2998,  3002,  3006,  3014,  3019,  3029,
    3030,  3031,  3035,  3041,  3047,  3055,  3063,  3071,  3079,  3086,
    3096,  3097,  3098,  3102,  3109,  3119,  3127,  3128,  3132,  3133,
    3138,  3142,  3143,  3148,  3156,  3158,  3168,  3169,  3174,  3180,
    3185,  3193,  3195,  3200,  3210,  3211,  3216,  3217,  3221,  3222,
    3227,  3234,  3249,  3255,  3257,  3267,  3274,  3279,  3280,  3281,
    3285,  3286,  3290,  3296,  3302,  3312,  3322,  3323,  3327,  3333,
    3341,  3348,  3358,  3363,  3371,  3375,  3381,  3387,  3397,  3398,
    3402,  3408,  3417,  3418,  3427,  3428,  3445,  3447,  3458,  3457,
    3480,  3482,  3481,  3510,  3509,  3550,  3557,  3567,  3575,  3582,
    3591,  3600,  3613,  3621,  3631,  3636,  3648,  3659,  3667,  3677,
    3682,  3694,  3705,  3707,  3709,  3711,  3717,  3726,  3728,  3730,
    3731
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "\"auto\"", "\"bool\"", "\"complex\"",
  "\"break\"", "\"case\"", "\"char\"", "\"const\"", "\"continue\"",
  "\"default\"", "\"do\"", "\"double\"", "\"else\"", "\"enum\"",
  "\"extern\"", "\"float\"", "\"for\"", "\"goto\"", "\"if\"", "\"inline\"",
  "\"int\"", "\"long\"", "\"register\"", "\"restrict\"", "\"return\"",
  "\"short\"", "\"signed\"", "\"sizeof\"", "\"static\"", "\"struct\"",
  "\"switch\"", "\"typedef\"", "\"union\"", "\"unsigned\"", "\"void\"",
  "\"volatile\"", "\"wchar_t\"", "\"while\"", "\"->\"", "\"++\"", "\"--\"",
  "\"<<\"", "\">>\"", "\"<=\"", "\">=\"", "\"==\"", "\"!=\"", "\"&&\"",
  "\"||\"", "\"...\"", "\"*=\"", "\"/=\"", "\"%=\"", "\"+=\"", "\"-=\"",
  "\"<<=\"", "\">>=\"", "\"&=\"", "\"^=\"", "\"|=\"", "TOK_IDENTIFIER",
  "TOK_TYPEDEFNAME", "TOK_INTEGER", "TOK_FLOATING", "TOK_CHARACTER",
  "TOK_STRING", "TOK_ASM_STRING", "\"__int8\"", "\"__int16\"",
  "\"__int32\"", "\"__int64\"", "\"__ptr32\"", "\"__ptr64\"", "\"typeof\"",
  "\"__auto_type\"", "\"_Float16\"", "\"_Float32\"", "\"_Float32x\"",
  "\"__float80\"", "\"_Float64\"", "\"_Float64x\"", "\"_Float128\"",
  "\"_Float128x\"", "\"__int128\"", "\"_Decimal32\"", "\"_Decimal64\"",
  "\"_Decimal128\"", "\"__asm__\"", "\"__asm__ (with parentheses)\"",
  "\"__attribute__\"", "\"aligned\"", "\"transparent_union\"",
  "\"packed\"", "\"vector_size\"", "\"mode\"", "\"__gnu_inline__\"",
  "\"weak\"", "\"alias\"", "\"section\"", "\"noreturn\"",
  "\"constructor\"", "\"destructor\"", "\"fallthrough\"", "\"used\"",
  "\"__label__\"", "\"__asm\"", "\"__based\"", "\"_var_arg_typeof\"",
  "\"__builtin_va_arg\"", "\"__builtin_types_compatible_p\"",
  "\"__builtin_convertvector\"", "\"__offsetof\"", "\"__alignof__\"",
  "\"__try\"", "\"__finally\"", "\"__except\"", "\"__leave\"",
  "\"__declspec\"", "\"__forceinline\"", "\"__interface\"", "\"__cdecl\"",
  "\"__stdcall\"", "\"__fastcall\"", "\"__clrcall\"", "\"forall\"",
  "\"exists\"", "\"\\\\forall\"", "\"\\\\exists\"", "\"\\\\lambda\"",
  "\"\\\\let\"", "\"array_of\"", "\"__CPROVER_bitvector\"",
  "\"__CPROVER_floatbv\"", "\"__CPROVER_fixedbv\"", "\"__CPROVER_atomic\"",
  "\"__CPROVER_bool\"", "\"__CPROVER_throw\"", "\"__CPROVER_catch\"",
  "\"__CPROVER_try\"", "\"__CPROVER_finally\"", "\"__CPROVER_ID\"",
  "\"__CPROVER_loop_invariant\"", "\"__CPROVER_decreases\"",
  "\"__CPROVER_requires_contract\"", "\"__CPROVER_requires\"",
  "\"__CPROVER_ensures_contract\"", "\"__CPROVER_ensures\"",
  "\"__CPROVER_assigns\"", "\"__CPROVER_frees\"", "\"==>\"", "\"<==>\"",
  "\"^^\"", "\"TRUE\"", "\"FALSE\"", "\"__real__\"", "\"__imag__\"",
  "\"_Alignas\"", "\"_Atomic\"", "\"_Atomic()\"", "\"_Generic\"",
  "\"_Imaginary\"", "\"_Noreturn\"", "\"_Static_assert\"",
  "\"_Thread_local\"", "\"nullptr\"", "\"constexpr\"", "TOK_SCANNER_ERROR",
  "TOK_SCANNER_EOF", "\"catch\"", "\"char16_t\"", "\"char32_t\"",
  "\"class\"", "\"delete\"", "\"decltype\"", "\"explicit\"", "\"friend\"",
  "\"mutable\"", "\"namespace\"", "\"new\"", "\"noexcept\"",
  "\"operator\"", "\"private\"", "\"protected\"", "\"public\"",
  "\"template\"", "\"this\"", "\"throw\"", "\"typeid\"", "\"typename\"",
  "\"try\"", "\"using\"", "\"virtual\"", "\"::\"", "\".*\"", "\"->*\"",
  "TOK_UNARY_TYPE_PREDICATE", "TOK_BINARY_TYPE_PREDICATE", "\"__uuidof\"",
  "\"__if_exists\"", "\"__if_not_exists\"", "\"__underlying_type\"", "'('",
  "')'", "','", "':'", "'.'", "'['", "']'", "'{'", "'}'", "'&'", "'*'",
  "'+'", "'-'", "'~'", "'!'", "'/'", "'%'", "'<'", "'>'", "'^'", "'|'",
  "'?'", "'='", "';'", "$accept", "grammar", "identifier", "typedef_name",
  "integer", "floating", "character", "string", "constant",
  "primary_expression", "generic_selection", "generic_assoc_list",
  "generic_association", "gcc_builtin_expressions",
  "clang_builtin_expressions", "cw_builtin_expressions", "offsetof",
  "offsetof_member_designator", "quantifier_expression",
  "cprover_contract_loop_invariant",
  "cprover_contract_loop_invariant_list",
  "cprover_contract_loop_invariant_list_opt",
  "ACSL_binding_expression_list", "cprover_contract_decreases_opt",
  "statement_expression", "postfix_expression", "member_name",
  "argument_expression_list", "unary_expression", "cast_expression",
  "multiplicative_expression", "additive_expression", "shift_expression",
  "relational_expression", "equality_expression", "and_expression",
  "exclusive_or_expression", "inclusive_or_expression",
  "logical_and_expression", "logical_xor_expression",
  "logical_or_expression", "logical_implication_expression",
  "logical_equivalence_expression", "ACSL_binding_expression",
  "conditional_expression", "assignment_expression", "comma_expression",
  "constant_expression", "comma_expression_opt", "declaration",
  "static_assert_declaration", "default_declaring_list", "@1", "@2", "$@3",
  "post_declarator_attribute", "post_declarator_attributes",
  "post_declarator_attributes_opt", "declaring_list", "@4", "@5", "$@6",
  "declaration_specifier", "type_specifier", "declaration_qualifier_list",
  "type_qualifier_list", "attribute_type_qualifier_list",
  "declaration_qualifier", "type_qualifier", "alignas_specifier",
  "attribute_or_type_qualifier",
  "attribute_or_type_qualifier_or_storage_class",
  "attribute_type_qualifier_storage_class_list",
  "basic_declaration_specifier", "basic_type_specifier",
  "sue_declaration_specifier", "sue_type_specifier",
  "typedef_declaration_specifier", "typeof_declaration_specifier",
  "atomic_declaration_specifier", "typedef_type_specifier",
  "typeof_specifier", "typeof_type_specifier", "atomic_specifier",
  "atomic_type_specifier", "msc_decl_identifier", "msc_decl_modifier",
  "msc_declspec_seq", "msc_declspec", "msc_declspec_opt", "storage_class",
  "basic_type_name", "elaborated_type_name", "array_of_construct",
  "pragma_packed", "aggregate_name", "$@7", "$@8", "$@9", "aggregate_key",
  "gcc_type_attribute", "gcc_attribute", "gcc_attribute_list",
  "gcc_attribute_specifier", "gcc_type_attribute_opt",
  "gcc_type_attribute_list", "member_declaration_list_opt",
  "member_declaration_list", "member_declaration",
  "member_default_declaring_list", "member_declaring_list",
  "member_declarator", "member_identifier_declarator",
  "bit_field_size_opt", "bit_field_size", "enum_name", "$@10", "$@11",
  "basic_type_name_list", "enum_underlying_type",
  "enum_underlying_type_opt", "braced_enumerator_list_opt", "enum_key",
  "enumerator_list_opt", "enumerator_list", "enumerator_declaration",
  "enumerator_value_opt", "parameter_type_list", "KnR_parameter_list",
  "KnR_parameter", "parameter_list", "parameter_declaration",
  "identifier_or_typedef_name", "type_name", "initializer_opt",
  "initializer", "initializer_list", "initializer_list_opt",
  "designated_initializer", "designator", "statement",
  "declaration_statement", "labeled_statement", "statement_attribute",
  "compound_statement", "compound_scope", "statement_list",
  "expression_statement", "selection_statement",
  "declaration_or_expression_statement", "iteration_statement", "$@12",
  "jump_statement", "gcc_local_label_statement", "gcc_local_label_list",
  "gcc_local_label", "gcc_asm_statement", "msc_asm_statement",
  "msc_seh_statement", "cprover_exception_statement",
  "volatile_or_goto_opt", "gcc_asm_commands", "gcc_asm_assembler_template",
  "gcc_asm_outputs", "gcc_asm_output", "gcc_asm_output_list",
  "gcc_asm_inputs", "gcc_asm_input", "gcc_asm_input_list",
  "gcc_asm_clobbered_registers", "gcc_asm_clobbered_register",
  "gcc_asm_clobbered_registers_list", "gcc_asm_labels",
  "gcc_asm_labels_list", "gcc_asm_label", "translation_unit",
  "external_definition_list", "external_definition", "asm_definition",
  "function_definition", "function_body", "KnR_parameter_header_opt",
  "KnR_parameter_header", "KnR_parameter_declaration",
  "KnR_declaration_qualifier_list", "KnR_basic_declaration_specifier",
  "KnR_typedef_declaration_specifier", "KnR_sue_declaration_specifier",
  "KnR_declaration_specifier", "KnR_parameter_declaring_list",
  "function_head", "declarator", "paren_attribute_declarator",
  "typedef_declarator", "parameter_typedef_declarator",
  "clean_typedef_declarator", "clean_postfix_typedef_declarator",
  "paren_typedef_declarator", "paren_postfix_typedef_declarator",
  "simple_paren_typedef_declarator", "identifier_declarator",
  "unary_identifier_declarator", "postfix_identifier_declarator",
  "paren_identifier_declarator", "abstract_declarator",
  "parameter_abstract_declarator", "cprover_function_contract",
  "unary_expression_list", "conditional_target_group",
  "conditional_target_list", "conditional_target_list_opt_semicol",
  "cprover_contract_assigns", "cprover_contract_assigns_opt",
  "cprover_contract_frees", "cprover_function_contract_sequence",
  "cprover_function_contract_sequence_opt",
  "postfixing_abstract_declarator", "$@13",
  "parameter_postfixing_abstract_declarator", "$@14", "$@15",
  "array_abstract_declarator", "unary_abstract_declarator",
  "parameter_unary_abstract_declarator", "postfix_abstract_declarator",
  "parameter_postfix_abstract_declarator", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,    40,    41,    44,    58,    46,    91,    93,
     123,   125,    38,    42,    43,    45,   126,    33,    47,    37,
      60,    62,    94,   124,    63,    61,    59
};
# endif

#define YYPACT_NINF (-1002)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-619)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    2218, -1002, -1002, -1002, -1002, -1002, -1002, -1002, -1002, -1002,
   -1002, -1002, -1002, -1002, -1002, -1002, -1002, -1002, -1002, -1002,
   -1002, -1002, -1002, -1002, -1002, -1002, -1002, -1002, -1002, -1002,
   -1002, -1002,  -161,   839, -1002, -1002, -1002, -1002, -1002, -1002,
   -1002, -1002, -1002, -1002, -1002, -1002, -1002,  -119,   -23,    23,
      44, -1002,    -3,   -66,    69,   112, -1002, -1002,   242,   148,
   -1002,   170, -1002,   179, -1002,    32,   272,  4160,    32, -1002,
     389, -1002,    18, -1002,   169,   -76,   -68,    68,   108,  2601,
    2601, -1002, -1002,  7363,  7363,  7451,  7451,  7451,  7451,  7451,
    7451,  1372,   320,  1372,   320, -1002, -1002,    18, -1002, -1002,
   -1002,    18, -1002, -1002,    18, -1002,  2218, -1002, -1002, -1002,
   -1002, -1002, -1002, -1002,    -4,  4890,   406,  1870,    -4,    14,
   -1002, -1002, -1002, -1002, -1002, -1002, -1002, -1002,   337,   204,
    5435,     7,    18,  5435,  5435,  5435, -1002,  4890,    18,  5435,
     210,   254,   211,  4160, -1002, -1002, -1002, -1002, -1002, -1002,
   -1002, -1002,    18, -1002,    32, -1002,    18, -1002, -1002,    14,
   -1002,    14,    18, -1002, -1002,    18,    18, -1002,    18, -1002,
   -1002,   209,    18, -1002,  1372,  1372, -1002,    18, -1002, -1002,
     215,    18,    18, -1002,    18,    18,    18,    18,    18,    18,
      18,    18,    18,    18,  1374,    18,  1374,    18, -1002, -1002,
      49, -1002, -1002,   218, -1002,   -10,  3887, -1002, -1002,   229,
    6307,  6416,  6416,    40, -1002, -1002, -1002, -1002,   227,   241,
     251,   262,   273,  6422, -1002, -1002, -1002, -1002, -1002,  6198,
    6198,   278,  4781,  6198,  6198,  6198,  6198,  6198,  6198, -1002,
   -1002, -1002, -1002, -1002, -1002, -1002, -1002, -1002, -1002, -1002,
   -1002, -1002, -1002,    41, -1002,  1310,   145,   117,   420,    17,
     466,   277,   280,   276,   457,   362,    47, -1002,    -5, -1002,
   -1002, -1002,   323,  7707,   318,   892,  1922, -1002,   223,   321,
     340,    -4,   406,  4104, -1002, -1002, -1002, -1002, -1002, -1002,
      14,   326, -1002,   354,  1474,   412, -1002, -1002, -1002, -1002,
   -1002,   357, -1002,    61,   342,   107,   165,   167,   425,   364,
     370,   361,    -4, -1002, -1002, -1002, -1002, -1002, -1002,   839,
     367,   371, -1002, -1002, -1002, -1002,   360, -1002,  1374,  1374,
   -1002,   360, -1002, -1002, -1002, -1002, -1002, -1002, -1002, -1002,
   -1002, -1002, -1002, -1002, -1002, -1002,   266,  2123, -1002, -1002,
   -1002,   384,  3083,  7066,   -28,  6903, -1002,  5653, -1002,   386,
   -1002, -1002,  4044, -1002, -1002,  5544,  4781, -1002,  4781, -1002,
   -1002, -1002, -1002,    18,  5435,    18,  5435,    18,  4781, -1002,
     398,   401,  6577,  6577,  6577, -1002, -1002,  5435,   436,   414,
     417, -1002, -1002, -1002, -1002, -1002, -1002,    40, -1002, -1002,
    4999,    40,  5435,  5435,  5435,  5435,  5435,  5435,  5435,  5435,
    5435,  5435,  5435,  5435,  6198,  6198,  6198,  6198,  6198,  6198,
    6198,  6198,  6198,  6198,  6198,  6198,  6198,  6198,  6198,  6198,
    6198,  6198,  6198,  6198,  6198,  5108, -1002,  5435,   230,  2920,
    7799,  1372,  1372, -1002, -1002, -1002,   375,   892,   421,    -4,
      -4,   427,   393,   892, -1002, -1002,    53, -1002,  5217,   416,
     448, -1002, -1002,   458,   463, -1002, -1002,   465,   468, -1002,
   -1002, -1002, -1002, -1002, -1002, -1002,   440, -1002,   540, -1002,
   -1002, -1002, -1002,   467,   479, -1002, -1002, -1002,  5435, -1002,
     360,    14,   360,   360,  5217, -1002, -1002, -1002,   482,   490,
   -1002, -1002,  7891, -1002,   495, -1002,   476,  5435,   483,   512,
    3513, -1002,  5326,   520,  4223,   525,   527,   530, -1002,    40,
      19, -1002, -1002,   524, -1002, -1002,   532,    30,   577,   564,
   -1002,    68,   108,  2601,  2601,   574,   596, -1002, -1002, -1002,
   -1002, -1002,  3298, -1002, -1002, -1002, -1002, -1002, -1002, -1002,
   -1002, -1002,   839,  7568,   320,  7363,  7451, -1002,  7066, -1002,
    7224,  7363,  7451, -1002,   839,    27,   489, -1002,   442, -1002,
     714,   714,  2438,  2438,   608,   609, -1002, -1002, -1002, -1002,
     607, -1002,  5762,   612,   625,   635,   638,   645,   646,   649,
     650,   662,  6577,  6577,  5435,  5435,  5435,   663, -1002,  5871,
   -1002, -1002, -1002, -1002, -1002,   445, -1002, -1002,   208, -1002,
   -1002, -1002, -1002, -1002, -1002, -1002, -1002, -1002, -1002, -1002,
   -1002, -1002, -1002, -1002,   145,   145,   117,   117,   420,   420,
     420,   420,    17,    17,   466,   277,   280,   276,   457,   362,
   -1002, -1002,  6198,   449, -1002,   614,  1316, -1002, -1002, -1002,
   -1002, -1002, -1002, -1002,    -4, -1002, -1002, -1002, -1002,   402,
   -1002, -1002, -1002,   337,  4672, -1002, -1002, -1002,  5435,  5435,
     -28,   802,   804,   668,  1474,   670,   671,   652,  5435,  5435,
     676, -1002, -1002, -1002, -1002, -1002,   248,   677,    18, -1002,
      40,   679, -1002,   -14, -1002,  3513,   855,   693,   672,    79,
    5435, -1002,   101,  5435,  5435, -1002,    24,   119, -1002, -1002,
     832,   557, -1002,   253, -1002,    14,    14, -1002, -1002, -1002,
    3513, -1002, -1002, -1002, -1002,    18,    18, -1002,    18, -1002,
      18,    40,    40,    18,    18,    18, -1002,   839, -1002,   700,
     701,   702,   704,   706,   708, -1002, -1002, -1002,   489, -1002,
    7066,   -28,   811,  1615,    32, -1002,    18, -1002, -1002, -1002,
   -1002, -1002,    18, -1002,   876,   966,    18, -1002,    18, -1002,
    7066,  6740, -1002, -1002, -1002,   703,   703, -1002,    18,    18,
      18,    40,   703,  5435,  5435, -1002, -1002, -1002,    62,  4672,
   -1002, -1002,  5435, -1002, -1002,  6198,   711,   715,   719,  1316,
   -1002, -1002, -1002, -1002,   720,   724,    40,  5435,   726,   727,
   -1002,   729,   728, -1002,  4345,   477,   481,   721,   737,   740,
   -1002, -1002, -1002, -1002,   275,   222,   237, -1002,   360, -1002,
     731,  7707,   747,   297, -1002,   121,   130,   248, -1002,   750,
     760, -1002,    18,    40,    18,  5435,  3513, -1002,   763,  3728,
   -1002, -1002,   492, -1002,   508,   764,   337,   902,    40, -1002,
     761, -1002,   768, -1002, -1002, -1002, -1002, -1002, -1002, -1002,
   -1002, -1002,    18,    18, -1002, -1002, -1002, -1002,  6416,  5435,
    6416,  5435,  5980,  6089, -1002, -1002,  7066, -1002, -1002,   769,
     770,  1615, -1002, -1002, -1002,   966, -1002, -1002,   489, -1002,
   -1002,   773,   774,   778,    85, -1002,   -38,   137,   777,   510,
   -1002,   779,   781,   785, -1002, -1002, -1002,    -4,    -4, -1002,
   -1002,    72,   786, -1002,    59,  5217,  4454, -1002,    40,  5435,
    5217, -1002, -1002, -1002, -1002, -1002, -1002,   517, -1002, -1002,
   -1002, -1002,   654,  2763,    18, -1002,   485, -1002,   654, -1002,
     787,    18,    40,   759,   789, -1002,   795, -1002,  5435, -1002,
   -1002,  5435,  3513,  3513,   853,   800,   796, -1002, -1002, -1002,
    5435, -1002, -1002, -1002, -1002,   801,   806,   803,   807, -1002,
     -85,   -45,   816, -1002,   790,   808, -1002,   821,    71, -1002,
   -1002, -1002, -1002, -1002, -1002,    40, -1002,    40,  5435, -1002,
   -1002,  5435, -1002,    62,  5435,  4563, -1002, -1002, -1002,    40,
     823, -1002,   822,    88,   824,  5435, -1002, -1002, -1002, -1002,
   -1002,    82, -1002, -1002,   275,  5435, -1002,    18,   829, -1002,
      18,   829, -1002, -1002, -1002,    18, -1002, -1002,  5435, -1002,
   -1002,  3513,   521,   810,  1023, -1002, -1002,   895,   819, -1002,
     556,  6416, -1002,  6416, -1002,  6416,  6416,  6198, -1002, -1002,
     842, -1002, -1002, -1002,   279, -1002, -1002, -1002, -1002,   838,
    5435,    72,    40,   845, -1002,   844,   337,   847,   282,  5435,
   -1002,   825, -1002, -1002,    18, -1002, -1002,    18, -1002, -1002,
   -1002, -1002,   853,  5435,  3513,   851, -1002,   895,   914, -1002,
   -1002,   856,   858, -1002,   816, -1002, -1002, -1002,   337,   563,
   -1002,   850,  5435,    88, -1002, -1002,   860,    40, -1002, -1002,
     344,   275, -1002, -1002, -1002,   895,   859, -1002,  5435, -1002,
     864,  3513, -1002, -1002, -1002,   865, -1002,   337,   604, -1002,
     337, -1002,   868, -1002, -1002,   866,   914,   853,   871,  5435,
   -1002,  5435,   875, -1002, -1002,    40, -1002,   843,   895, -1002,
     606, -1002,   615,  5435, -1002, -1002,   914, -1002,  5435, -1002,
     620,  3513, -1002, -1002, -1002
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
     510,   260,   292,   293,   272,   188,   288,   368,   258,   275,
     262,   267,   274,   261,   189,   273,   289,   259,   309,   257,
     310,   290,   291,   190,     3,     5,   268,   269,   270,   271,
     192,   193,     0,     0,   276,   277,   278,   281,   279,   280,
     282,   283,   284,   285,   286,   287,   264,     0,     0,     0,
       0,   266,     0,     0,     0,     0,   191,   297,     0,     0,
     187,     0,   331,     0,   263,     0,     0,     0,     0,   517,
       0,   585,   332,   515,     0,     0,     0,     0,     0,     0,
       0,   180,   195,   165,   170,   166,   171,   167,   168,   169,
     172,   233,   173,   238,   174,   265,   175,   332,   216,   300,
     298,   332,   177,   299,   332,     2,   511,   512,   516,   514,
     445,   545,   576,   578,   577,   332,     0,     0,   558,   156,
     552,   551,   557,   560,   561,   556,   566,   550,     0,     0,
       0,     0,   332,     0,     0,     0,     4,   332,   332,     0,
       0,     0,     0,     0,   198,   183,   199,   579,   580,     1,
     334,   228,   333,   142,     0,   144,   332,   143,   140,   156,
     141,   156,   332,   179,   186,   332,   332,   185,   332,   213,
     178,   146,   332,   181,   234,   239,   176,   332,   217,   182,
     148,   332,   332,   211,   332,   332,   332,   332,   332,   332,
     332,   332,   332,   332,   236,   332,   241,   332,   209,   255,
     364,   513,   521,     0,   520,   623,     0,   582,   616,   620,
       0,     0,     0,     0,     6,     7,     8,     9,     0,     0,
       0,     0,     0,     0,   445,   445,   445,   445,   445,     0,
       0,     0,   332,     0,     0,     0,     0,     0,     0,    14,
      10,    11,    12,    13,    15,    50,    23,    18,    19,    20,
      21,    22,    17,    64,    78,    82,    86,    89,    92,    97,
     100,   102,   104,   106,   108,   110,   112,   114,   120,   123,
     116,   135,     0,     0,     0,     0,     0,   574,     0,     0,
       0,     0,     0,     0,   555,   562,   569,   559,   478,   155,
     157,     0,   153,     0,   325,     0,   244,   242,   243,   254,
     250,   245,   251,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   583,   586,   519,   184,   581,   335,   150,     0,
     158,   160,   220,   223,   226,   205,   404,   229,   235,   240,
     210,   404,   207,   208,   206,   212,   215,   218,   214,   221,
     224,   227,   230,   219,   222,   225,   303,     0,   398,   399,
     356,   364,   138,   621,     0,     0,   625,     0,   137,     0,
     200,   203,     0,   202,   201,     0,   332,    74,   332,    65,
      66,   467,    68,   332,     0,   332,     0,   332,   332,    76,
       0,     0,     0,     0,     0,    80,    81,     0,     0,     0,
       0,    67,    69,    70,    71,    72,    73,     0,    56,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   231,     0,   400,   402,
     170,   171,   172,   173,   174,   232,     0,     0,     0,   564,
     571,     0,     0,     0,   563,   570,     0,   154,     0,     0,
     314,   312,   311,     0,     0,   317,   318,     0,     0,   321,
     322,   323,   326,   324,   327,   328,     0,   194,     0,   253,
     252,   301,   294,     0,     0,   196,   197,   237,     0,   584,
     404,   156,   404,   404,     0,   147,   149,   256,     0,   307,
     363,   360,   362,   365,     0,   358,     0,     0,     0,     0,
     138,   456,     0,     0,     0,     0,     0,     0,   478,     0,
       0,   445,   474,     0,   445,   442,    14,   332,   139,     0,
     436,     0,     0,     0,     0,   177,     0,   446,   425,   423,
     435,   424,   138,   426,   427,   428,   429,   431,   430,   432,
     433,   434,     0,     0,   180,   170,   172,   527,   617,   524,
       0,   539,   540,   541,     0,     0,   614,   381,     0,   379,
     384,   391,   388,   395,     0,   377,   382,   627,   628,   626,
       0,   204,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    16,     0,
      49,    60,    61,    55,    52,     0,    62,    54,     0,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   124,
      83,    84,    85,    82,    87,    88,    90,    91,    95,    96,
      93,    94,    98,    99,   101,   103,   105,   107,   109,   111,
     113,   115,     0,     0,   136,   623,   632,   636,   401,   589,
     587,   588,   403,   575,   553,   565,   573,   572,   567,     0,
     480,   481,   479,     0,   412,   406,   162,   518,     0,     0,
       0,     0,     0,     0,   325,     0,     0,     0,     0,     0,
       0,   151,   163,   159,   161,   405,   332,     0,   332,   361,
     369,   366,   461,     0,   460,   138,     0,     0,     0,     0,
       0,   462,     0,     0,   138,   444,     0,     0,   465,   471,
       0,     0,   475,     0,   448,   156,   156,   146,   148,   441,
     138,   443,   447,   543,   528,   332,   332,   525,   332,   529,
     332,     0,     0,   332,   332,   332,   542,     0,   526,     0,
       0,     0,     0,     0,     0,   612,   596,   597,   615,   622,
     522,     0,   623,   637,   641,   387,   332,   385,   649,   590,
     591,   394,   332,   392,   623,   637,   332,   389,   332,   396,
     522,     0,   629,   631,   630,    75,     0,    32,   332,   332,
     332,     0,    77,     0,     0,   117,   118,   119,   332,   412,
      79,    53,     0,    51,   122,     0,     0,     0,     0,   633,
     634,   554,   568,   487,     0,   482,     0,     0,    14,     0,
     413,   411,     0,   409,     0,     0,     0,     0,     0,     0,
     330,   329,   247,   246,     0,     0,     0,   145,   404,   342,
       0,     0,     0,   332,   338,     0,     0,   332,   308,     0,
     370,   371,   332,   369,   332,     0,   138,   440,     0,   138,
     459,   458,     0,   463,     0,     0,     0,     0,     0,   464,
       0,   445,     0,   445,   445,   158,   160,   437,   531,   534,
     535,   530,   332,   332,   532,   533,   536,   544,     0,     0,
       0,     0,     0,     0,   613,   619,   523,   380,   621,     0,
       0,   638,   639,   386,   393,   638,   390,   397,   614,   378,
     383,     0,     0,     0,     0,    34,     0,     0,     0,     0,
      25,     0,   411,     0,    63,   121,   644,   642,   643,   635,
     152,   489,   483,   417,     0,     0,     0,   407,     0,     0,
       0,   415,   315,   313,   316,   319,   320,     0,   295,   296,
     164,   343,   349,     0,   332,   339,     0,   341,   349,   340,
       0,   332,   373,   375,     0,   359,     0,   438,     0,   452,
     453,   138,   138,   138,   608,     0,     0,   466,   470,   473,
       0,   476,   477,   537,   538,     0,     0,     0,     0,   607,
      78,     0,   600,   602,   605,     0,   611,     0,   647,   648,
     640,   624,    29,    30,    31,     0,    33,     0,     0,    38,
      39,     0,    24,   332,     0,     0,    58,   646,   645,     0,
       0,   492,   488,   495,   484,     0,   418,   416,   408,   410,
     422,     0,   414,   248,     0,     0,   346,   332,   353,   344,
     332,   353,   302,   345,   347,   332,   357,   372,     0,   374,
     367,   138,     0,     0,   449,   451,   609,    43,     0,   469,
       0,     0,   593,     0,   592,     0,     0,   604,   606,   610,
     623,   650,    37,    35,     0,    28,    26,    27,    59,     0,
       0,     0,     0,     0,   498,   494,   501,   485,     0,     0,
     420,     0,   355,   350,   332,   354,   352,   332,   304,   302,
     376,   439,   608,   138,   138,     0,    41,    44,    47,   468,
     445,     0,     0,   598,   601,   599,   603,    36,     0,     0,
     493,     0,     0,     0,   502,   503,   500,   506,   486,   419,
       0,     0,   348,   351,   306,    43,     0,   450,     0,    42,
       0,   138,   472,   595,   594,     0,   490,     0,     0,   499,
       0,   509,   505,   507,   421,     0,    47,   608,     0,     0,
     454,     0,     0,   496,   504,     0,   249,     0,    43,    40,
       0,    45,     0,     0,   508,   455,    47,    48,     0,   491,
       0,   138,    46,   497,   457
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1002, -1002,     0,   387, -1002, -1002, -1002,  -120, -1002, -1002,
   -1002, -1002,    78, -1002, -1002, -1002, -1002, -1002, -1002,   -12,
   -1002,  -935, -1002,  -938, -1002, -1002,  -371, -1002,  -172,  1069,
     413,   418,   114,   407,   659,   660,   658,   666,   661,   665,
   -1002,   411,  -787,  -355,  -586,   884,   -80,  -197,  -694,    66,
    -530, -1002, -1002, -1002, -1002,   809, -1002,  -101, -1002, -1002,
   -1002, -1002,    12,    28,    16,    22,   -64,   118,  1514, -1002,
    -132,   738, -1002, -1002,  -209, -1002,  -138, -1002, -1002, -1002,
    -137,   -61,  -107,   -55,   -51,  -465,   798, -1002,   757, -1002,
    1055,   -78,   -33, -1002,     4, -1002, -1002, -1002, -1002,   544,
   -1002,   432, -1002,   870,  1019,   -93,   274, -1002,   288, -1002,
   -1002,   161,   164,    81,  -798, -1002, -1002, -1002, -1002, -1002,
     762, -1002,   555,   283, -1002,   175, -1002, -1002, -1002,   377,
   -1002,   351,  -186,    25,  -272,  -409,   341,   343,  -827, -1002,
    -279,   284, -1002, -1002,  -106,   519, -1002,   287, -1002, -1002,
   -1002, -1002, -1002, -1002, -1002,  -206, -1002, -1002, -1002, -1002,
     632,   295, -1002, -1002,    83, -1002, -1002,    39, -1002, -1002,
      26, -1002, -1002, -1002,     1, -1002, -1002,  1053, -1002, -1002,
   -1002,   391,   812,  -543, -1002, -1002, -1002, -1002, -1002, -1002,
   -1002,    -2,  1045, -1002,  -100,   -87, -1002,   -24, -1002,   -96,
      21,    33, -1002,    43,  -403,  -386,   415,   109,   110, -1002,
     289,  -923, -1001, -1002, -1002,   285,   -70, -1002,   291, -1002,
   -1002, -1002,   528,  -550,   533,  -491
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    70,   239,   118,   240,   241,   242,   243,   244,   245,
     246,   909,   910,   247,   248,   249,   250,   904,   251,  1096,
    1097,  1098,  1160,  1131,   252,   253,   809,   605,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   528,   665,   529,   530,
      74,    75,   326,   331,   490,   289,   290,   291,    76,   492,
     493,   828,   531,   532,   533,   534,   283,   163,    81,    82,
     145,   361,   362,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,   301,   302,   303,    95,   346,
      96,    97,    98,    99,  1088,   100,   498,   687,   688,   101,
     474,   475,   476,   150,   273,   152,   832,   833,   834,   835,
     836,  1026,  1029,  1084,  1027,   103,   504,   691,   502,   503,
     350,   844,   104,   839,   840,   841,  1039,   574,   568,   569,
     575,   576,   536,   911,   495,   810,   811,   812,   813,   814,
     537,   538,   539,   540,   541,   203,   542,   543,   544,   961,
     545,   697,   546,   547,   707,  1141,   548,   549,   550,   551,
     456,   804,   805,   922,  1011,  1012,  1014,  1074,  1075,  1077,
    1115,  1116,  1118,  1142,  1143,   105,   106,   107,   108,   109,
     204,   885,   886,   559,   560,   561,   562,   563,   564,   565,
     110,  1028,   120,   121,   122,   123,   124,   125,   126,   452,
     127,   112,   113,   114,   648,   892,   745,   982,   983,   984,
     985,   746,  1047,   747,   748,   749,   451,   354,   208,   566,
     355,   209,   650,   759,   651,   760
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      71,   168,   177,   143,   202,   182,   185,   372,   293,   359,
     855,   315,    77,   677,   351,   727,    79,   285,   165,   174,
     281,   111,    80,   278,   166,   175,   603,   371,    78,   279,
     607,   119,   296,    71,    24,   272,   652,   845,   367,   369,
     370,  1046,   115,   660,   207,   661,   169,   178,   287,   666,
     295,   379,  -618,   305,   306,   307,   794,   308,   320,   496,
     321,   662,   421,   422,   440,    71,    73,    71,    71,   297,
     298,   207,   660,   908,   661,   159,   161,    71,    71,    71,
      71,   397,   398,   399,   128,   685,   296,   709,   147,   148,
     662,  1125,   280,   286,    24,   981,   981,   432,   140,  1019,
     171,   180,    24,    25,   288,    48,    71,   434,   141,    48,
    1015,    24,    25,   194,    58,   196,    71,    71,    77,  -598,
    -598,    48,    79,   297,   298,   995,   390,   111,    80,   154,
      24,    25,  -618,  1079,    78,   441,   442,   156,   147,   217,
     274,  -598,   133,    71,   555,  1030,  1158,   434,  1030,   140,
     155,   315,   388,    48,    71,   217,   830,   304,   157,   141,
     499,  1055,   309,   310,   316,   580,   443,   437,   583,  1046,
      24,    25,    73,   999,    58,   318,   285,    62,  1019,   446,
     129,    62,    58,   454,   757,   763,   767,   769,   279,   278,
    1146,    58,   846,    62,   353,   279,   328,   329,   433,   205,
     348,   181,   889,   186,   206,   189,   190,   191,  1157,   915,
      58,   299,   300,   348,   889,   441,   556,   132,   681,   435,
     683,   684,   444,  1166,  1046,    62,   130,   856,  1171,   710,
    1085,   696,   737,  1085,   857,    65,  -399,   423,   424,   785,
     786,   787,   489,   800,   400,    67,   443,   131,   401,   402,
      58,   280,   286,   738,    68,   347,   663,   389,   280,   455,
     390,   890,   390,   722,   437,   479,   300,  1043,  1016,   501,
     981,   116,   390,   890,  1060,    71,    71,   134,    71,   206,
    1009,   117,    71,    71,   437,    24,   388,   437,   388,   996,
      68,  1080,   997,   998,   158,   439,  1072,   147,   388,   448,
     296,   438,   444,   830,   316,   851,   437,   830,   140,   136,
     693,   116,   437,   708,    48,   140,   482,   491,   141,    71,
     135,   117,   608,     1,   858,   141,   946,   853,    24,    25,
      68,   417,   418,   371,   160,   948,     8,   297,   298,    48,
     142,    10,   437,   727,    13,   859,   348,   947,  1000,   555,
      17,   137,   526,    19,   567,   643,   949,   659,   414,   937,
     279,   177,   185,   415,   416,    58,   279,   570,   649,   649,
     437,   572,   437,   138,   483,   553,   484,   573,   174,   655,
     656,   552,   139,   571,   175,    50,    62,    72,    48,   149,
     682,   584,   863,   585,   864,   153,   919,   601,   586,  1126,
     588,   601,   590,   591,   217,   931,   178,   294,    58,    46,
     905,    62,    63,   437,   312,   711,   847,   793,   713,  -548,
     441,   556,   314,   280,   689,  -549,    65,   437,   352,   280,
     373,   938,   699,   645,   702,   923,    67,   365,   206,    50,
      51,   867,   437,   646,   374,    68,   939,    71,   594,   595,
     596,   443,   647,    71,   375,   168,   177,   205,   313,  -336,
      62,    63,   206,   419,   420,   376,   162,   172,    24,    25,
     755,   761,   165,   174,   829,   177,   377,   185,   166,   175,
     140,   387,   730,   734,   437,    64,   140,   437,  1107,   427,
     141,  1119,   174,    72,   168,   177,   141,    48,   175,   429,
     169,   178,   428,   277,   842,   990,   430,   444,  -337,   990,
     526,   165,   174,   425,   426,   431,  1017,   166,   175,   348,
     178,  1022,   445,   829,   976,   449,   978,   436,   437,   715,
     716,    71,    71,    71,    71,   628,   629,   630,   631,   169,
     178,   555,   526,   803,   450,   872,   873,    24,    58,   437,
     723,   458,    71,  1144,   717,   718,   940,  1020,   459,  1081,
     478,   555,   736,   481,    71,   296,   488,   957,   486,    62,
      71,    71,    71,    71,   487,   796,   649,  -546,   205,   653,
     553,  -547,   799,   206,   801,   494,   552,   349,   815,   816,
     347,   756,   762,   766,   768,   578,   205,   658,   825,   826,
     349,   206,   297,   298,   675,   205,   802,   676,   592,   275,
     206,   593,   441,   556,   865,   866,   477,   437,   599,   276,
     852,   600,   440,   854,  1062,   654,  1063,    58,    68,   485,
     437,   657,   441,   556,   739,   740,   741,   742,   743,   744,
     598,   437,   667,   443,   673,   674,   750,   751,   956,   791,
     792,   668,   967,   285,   437,   795,  1145,   842,   783,   784,
      72,   669,   277,   443,   808,   279,   670,   315,   671,   277,
     817,   672,   371,   861,   862,   678,  -618,   555,   729,   733,
     735,   932,   437,  1044,  1045,   933,   437,   679,    65,   891,
     348,  1025,   686,   441,   442,   526,   962,   437,    67,   444,
    -305,   895,   692,   906,   907,   690,   975,    68,   977,   694,
     980,   980,   963,   437,  1002,  1003,    24,    25,   695,   444,
     526,  1023,  1024,   700,   443,  1092,   437,   924,   703,   649,
     704,   348,   348,   349,   500,   877,   803,    71,  -398,   527,
      72,   705,    72,   380,   381,   382,   383,   384,   441,   556,
     712,   567,    71,    71,    71,   969,  -618,   971,   972,   315,
    1100,   437,  1091,   315,    71,    71,   842,  1136,   437,    72,
      72,    72,   553,  1148,   147,   148,    24,    25,   552,   443,
     444,   601,   437,   570,   602,   140,   147,   572,   602,   808,
     714,   454,   553,   573,  1161,   141,    58,   140,   552,   571,
     719,  1010,   720,   901,   902,   903,   601,   141,  1153,   437,
    1167,  1168,   770,  1172,   771,  1127,   772,   645,   353,  1169,
     437,   774,   206,  1069,  1173,   437,   172,   646,  1082,   775,
     624,   625,   632,   633,   277,   444,   647,   626,   627,   776,
     277,  1090,   777,   348,   640,   641,   526,  1007,  1008,  1021,
     778,   779,  1150,   943,   780,   781,    58,   116,   348,   942,
    1025,   758,   758,   758,   758,   177,   782,   117,   788,   818,
     102,   819,   820,    24,   822,   823,    68,   824,  1042,  1101,
     827,  1102,   174,  1103,  1105,   980,  1111,   837,   175,   843,
    1050,    71,  1174,  1073,   848,    71,   849,   527,   850,   698,
     860,    24,    25,   878,   879,   880,   349,   881,   553,   882,
     178,   883,   316,   789,   552,   916,   316,   752,  1064,   917,
     162,   172,   206,   918,   920,   934,   808,   753,   601,   527,
     921,   371,   -60,   925,   926,  1078,   754,   146,    24,   927,
     172,   935,    71,    71,   936,    72,    71,   728,    71,   170,
     179,  1010,   348,    58,    24,    25,  1114,   941,   944,   162,
     172,   951,   526,   526,  1031,   952,   958,  1031,   964,   371,
     966,   970,   968,   988,   989,     5,   102,   992,   993,    72,
      72,    58,   994,  1001,  1038,  1004,  1005,   146,  1135,   292,
    1109,    14,  1013,  1073,  1132,   601,  1006,   601,  1035,  1120,
    1040,  1041,   743,    23,  1048,   808,  1051,  1049,  1053,   348,
    1052,  1054,  1058,   146,   752,   888,  1057,  1152,    58,   206,
    1114,  1056,   317,   311,   753,  1059,  1070,  1071,    24,   292,
    1076,   292,  1138,   754,    58,  1025,  1093,  1094,  1095,    30,
      31,   526,   116,   758,   758,  1099,   888,  1108,  1112,  1113,
    1121,   602,   117,  1117,  1128,   758,   758,    48,  1130,  1137,
    1133,    68,  1134,  1147,   179,  1140,   179,  1149,  1151,  1165,
    1156,  1162,   348,  1155,    49,  1159,   364,   349,  1163,   764,
     888,  1066,   527,  1170,   206,  1129,   634,   636,   635,   765,
     358,   151,   638,  1124,   526,   275,   637,   639,   754,   457,
     581,   480,    56,   497,   731,   276,   821,   527,    58,  1034,
    1033,   950,  1087,   505,    68,   732,   198,   348,   349,   349,
     199,   945,   900,   200,    59,    60,   954,  1037,   887,    62,
     912,   526,   913,   959,   167,   176,   960,    72,   167,   184,
     167,   188,   167,   167,   167,   193,   146,   195,   317,   197,
     706,   965,  1139,   146,  1110,   348,  1164,    72,    72,   201,
     292,   898,   284,   884,  1104,   558,  1154,  1106,   602,   764,
       0,   526,   987,   797,   206,   319,   602,     0,   798,   765,
       0,   322,   758,   991,   323,   324,   758,   325,   754,     0,
       0,   327,     0,   602,     0,     0,   330,     0,   179,   179,
     332,   333,     0,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,     0,   344,     0,   345,     0,    72,     0,
       0,     0,   535,     0,     0,   102,     0,     0,     0,     0,
     349,     0,   364,   527,     0,     0,    72,     0,     0,     0,
       0,     0,     0,     0,     0,   349,   358,     0,     0,   358,
       0,     0,   102,   102,   102,     0,     0,     0,   587,     0,
     589,   363,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   597,     0,    72,     0,     0,     0,     0,     0,  1061,
       0,     0,     0,     0,   606,     0,     0,   609,   610,   611,
     612,   613,   614,   615,   616,   617,   618,   619,   385,   386,
       0,     0,   391,   392,   393,   394,   395,   396,     0,   179,
       0,     0,     0,   602,     0,   602,     0,     0,     0,     0,
       0,   644,     0,     0,     0,     5,     0,     0,     0,     0,
     172,     0,     0,     0,     0,     0,     0,     0,     0,   349,
       0,    14,   358,     0,     0,     0,     0,     0,     0,   527,
     527,     0,     0,    23,     0,     0,     0,     0,     0,     0,
       0,   292,   403,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   680,     0,     0,     0,     0,     0,   358,     0,
     535,     5,   602,     5,   602,     0,     0,     0,     0,    30,
      31,   358,   602,     0,     0,     0,   349,    14,     0,    14,
       0,     0,     0,   170,   179,     0,     0,    48,   557,    23,
       0,    23,   535,     0,     0,     0,     0,   363,     0,     0,
       0,     0,     0,   179,    49,     0,   392,     0,   527,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   170,   179,     0,    30,    31,    30,    31,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,   349,
       0,     0,   102,   102,     0,    48,     0,     0,     0,     0,
       0,     0,     0,     0,    59,    60,     0,     0,     0,    62,
      49,   527,    49,   620,   621,   622,   623,   623,   623,   623,
     623,   623,   623,   623,   623,   623,   623,   623,   623,   623,
     623,   623,   623,   623,   349,     0,     0,     0,    56,     0,
      56,     0,     0,     0,     0,     0,   146,     0,   527,   645,
       0,     0,     0,     0,   206,     0,     0,     0,     0,   646,
      59,    60,    59,    60,     0,   413,     0,    62,   647,     0,
       0,     0,   349,     0,     0,     0,   151,     0,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   527,     0,
       0,     0,     0,     0,     0,   535,   460,   461,   462,   463,
     464,   465,   466,   467,   468,   469,   470,   471,   472,   473,
       0,   144,     0,     0,     0,   292,   292,     0,   167,   176,
     535,     0,     0,   164,   173,     0,     0,   164,   183,   164,
     187,   164,   164,   164,   192,     0,     0,     0,     0,   724,
     725,   726,     0,   557,     0,   167,   167,   167,     0,     0,
       0,     0,     0,   146,     5,     0,     0,   167,   176,     0,
       0,   144,     0,     0,     0,   146,     0,     0,     0,     0,
      14,   102,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   392,    23,     0,     0,     0,     0,   144,     0,     0,
       0,     0,     0,   623,   623,   623,     0,     0,   790,   146,
       0,     0,     0,   358,     0,     0,   914,    24,    25,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    30,    31,
       0,     0,     0,     0,     0,     0,     0,     0,   358,     0,
       0,     0,     0,     0,     0,   831,    48,   838,   173,     0,
     173,   623,     0,     0,     0,     0,   535,     0,     0,   102,
     360,     0,     0,    49,     0,     0,     0,     0,     0,   358,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   868,   869,     0,   870,     0,   871,
       0,    56,   874,   875,   876,     0,     0,    58,     0,     0,
       0,   146,     0,     0,     0,   146,     0,     0,     0,     0,
       0,     0,     0,    59,    60,   893,     0,     0,    62,     0,
       0,   894,     0,     0,     0,   896,     0,   897,     0,     0,
     144,     0,     0,     0,     0,     0,     0,   144,     0,     0,
       0,     0,     0,     0,     0,   557,     0,     0,     0,   358,
     358,     0,     0,   179,   358,     0,     0,     0,   752,     0,
       0,     0,     0,   206,     0,   557,     0,     0,   753,     0,
       0,     0,   535,   535,     0,     0,     0,   754,     0,     0,
       0,     0,   173,   173,     0,     0,     0,     0,     0,     0,
       0,     0,   831,     0,     0,     0,   831,     0,     0,     0,
       0,   953,     0,   955,   623,     0,     0,   554,     0,     0,
       0,     0,     0,     0,     0,     0,   360,     0,     0,     5,
       0,     0,     0,     0,     0,  1065,     0,     0,  1067,   358,
       0,   973,   974,     0,     0,    14,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    23,     0,   358,
       0,   535,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     5,    24,    25,     0,     0,     0,     0,     0,     0,
       0,   557,     0,    30,    31,     0,     0,    14,   623,     0,
     623,   623,   623,   173,   183,   187,   192,     0,     0,    23,
       0,    48,     0,  1032,   535,     0,     0,     0,     0,     0,
    1036,     0,     0,     0,     0,     0,     0,     0,    49,     0,
       0,     0,     0,     0,    24,    25,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    30,    31,     0,     0,     0,
       0,   535,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,    58,    48,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    59,    60,
      49,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   535,     0,     0,     0,     0,  1083,   164,   173,  1086,
       0,     0,     0,     0,  1089,     0,     0,     0,    56,     0,
       0,     0,     0,     0,    58,     0,     0,   173,     0,   183,
     192,     0,   554,   282,   164,   164,   164,     0,     0,     0,
      59,    60,     0,   117,     0,    62,   164,   173,     0,     0,
       0,     0,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1122,     0,     0,  1123,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   447,   623,     2,     3,     0,
       0,     4,     0,     0,     0,   276,     6,     0,     0,     0,
       9,     0,     0,     0,    68,    11,    12,     0,     0,     0,
      15,    16,     0,     0,     0,     0,     0,     0,    21,    22,
     144,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    25,     0,     0,     0,
       0,     0,    26,    27,    28,    29,     0,   623,     0,     0,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,     0,     0,     0,     0,     0,     0,   623,     0,
       0,     1,     2,     3,     0,     0,     4,     5,     0,     0,
       0,     6,     0,     7,     8,     9,     0,   623,     0,    10,
      11,    12,    13,    14,     0,    15,    16,     0,    17,    18,
       0,    19,    20,    21,    22,    23,    53,    54,    55,     0,
      57,     0,     0,     0,   554,     0,     0,   144,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   144,
      24,    25,     0,     0,   554,     0,     0,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
       0,     0,     0,   144,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    49,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,    51,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,    53,    54,    55,    56,    57,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,    60,    61,     0,
       0,    62,    63,    64,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     554,     0,     0,     0,     0,   144,     0,     0,     0,   144,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    65,     0,     0,     0,     0,     0,     0,    66,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     1,     2,     3,    69,     0,     4,     5,     0,     0,
       0,     6,     0,     7,     8,     9,     0,   173,     0,    10,
      11,    12,    13,    14,     0,    15,    16,     0,    17,    18,
       0,    19,    20,    21,    22,    23,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      24,    25,     0,     0,     0,     0,     0,    26,    27,    28,
      29,    30,    31,    32,     0,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,     0,    48,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    49,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,    51,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,    53,    54,    55,    56,    57,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,    60,    61,     0,
       0,    62,     0,    64,     1,     2,     3,     0,     0,     4,
       5,     0,     0,     0,     6,     0,     7,     8,     9,     0,
       0,     0,    10,    11,    12,    13,    14,     0,    15,    16,
       0,    17,    18,     0,    19,    20,    21,    22,    23,     0,
       0,   764,     0,     0,     0,     0,   206,     0,     0,     0,
       0,   765,     0,     0,     0,     0,     0,     0,     0,     0,
     754,     0,     0,    24,    25,     0,     0,     0,     0,     0,
      26,    27,    28,    29,    30,    31,    32,     0,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,     0,    48,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    49,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,    51,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,    53,    54,    55,    56,    57,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
      60,    61,     0,     0,    62,     0,    64,     2,     3,     0,
       0,     4,     5,     0,     0,     0,     6,     0,     7,     0,
       9,     0,     0,     0,     0,    11,    12,     0,    14,     0,
      15,    16,     0,     0,    18,     0,     0,    20,    21,    22,
      23,     0,     0,     0,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    24,    25,     0,     0,     0,
       0,     0,    26,    27,    28,    29,    30,    31,    32,     0,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,     0,     0,    48,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    49,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,    53,    54,    55,    56,
      57,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,    60,    61,     2,     3,    62,     0,     4,     5,
       0,     0,     0,     6,     0,     7,     0,     9,     0,     0,
       0,     0,    11,    12,     0,    14,     0,    15,    16,     0,
       0,    18,     0,     0,    20,    21,    22,    23,     0,     0,
       0,     0,     0,     0,     0,     0,    65,     0,     0,  1025,
       0,     0,     0,     0,     0,     0,    67,     0,     0,     0,
       0,     0,     0,    25,     0,    68,     0,     0,     0,    26,
      27,    28,    29,    30,    31,    32,     0,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,     0,
       0,    48,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    49,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,    53,    54,    55,    56,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    59,    60,
      61,     0,     0,    62,     0,     0,     1,     2,     3,   506,
     507,     4,     5,   508,   509,   510,     6,     0,     7,     8,
       9,   511,   512,   513,    10,    11,    12,    13,    14,   514,
      15,    16,   210,    17,    18,   515,    19,    20,    21,    22,
      23,     0,   516,   645,   211,   212,     0,     0,   206,     0,
       0,     0,   213,   646,     0,     0,     0,     0,     0,     0,
       0,     0,   647,     0,     0,    24,    25,   214,   215,   216,
     217,   517,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,   518,    48,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   519,
     520,    49,   218,   219,   220,   221,   222,   223,   521,     0,
       0,   522,    50,    51,     0,     0,     0,     0,     0,   224,
     225,   226,   227,   228,     0,    52,    53,    54,    55,    56,
      57,   523,     0,   524,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   229,
     230,    59,    60,    61,   231,     0,    62,    63,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   232,     0,     0,     0,
       0,     0,     0,  -445,   525,   233,   234,   235,   236,   237,
     238,     1,     2,     3,   506,   507,     4,     5,   508,   509,
     510,     6,     0,     7,     8,     9,   511,   512,   513,    10,
      11,    12,    13,    14,   514,    15,    16,   210,    17,    18,
     515,    19,    20,    21,    22,    23,     0,   516,     0,   211,
     212,     0,     0,     0,     0,     0,     0,   213,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      24,    25,   214,   215,   216,   217,     0,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,   518,    48,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   519,   520,    49,   218,   219,   220,
     221,   222,   223,   521,     0,     0,   522,    50,    51,     0,
       0,     0,     0,     0,   224,   225,   226,   227,   228,     0,
      52,    53,    54,    55,    56,    57,   523,     0,   524,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   229,   230,    59,    60,    61,   231,
       0,    62,    63,    64,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   232,     0,     0,     0,     0,     0,     0,  -445,   721,
     233,   234,   235,   236,   237,   238,     1,     2,     3,   506,
     507,     4,     5,   508,   509,   510,     6,     0,     7,     8,
       9,   511,   512,   513,    10,    11,    12,    13,    14,   514,
      15,    16,   210,    17,    18,   515,    19,    20,    21,    22,
      23,     0,   516,     0,   211,   212,     0,     0,     0,     0,
       0,     0,   213,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    24,    25,   214,   215,   216,
     217,     0,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,   518,    48,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   519,
     520,    49,   218,   219,   220,   221,   222,   223,   521,     0,
       0,   522,    50,    51,     0,     0,     0,     0,     0,   224,
     225,   226,   227,   228,     0,    52,    53,    54,    55,    56,
      57,   523,     0,   524,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   229,
     230,    59,    60,    61,   231,     0,    62,    63,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   232,     0,     0,     0,
       0,     0,     0,  -445,     0,   233,   234,   235,   236,   237,
     238,     1,     2,     3,     0,     0,     4,     5,     0,     0,
       0,     6,     0,     7,     8,     9,     0,     0,     0,    10,
      11,    12,    13,    14,     0,    15,    16,   210,    17,    18,
       0,    19,    20,    21,    22,    23,     0,     0,     0,   211,
     212,     0,     0,     0,     0,     0,     0,   213,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      24,    25,   214,   215,   216,   217,     0,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,     0,    48,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    49,   218,   219,   220,
     221,   222,   223,     0,     0,     0,     0,    50,    51,     0,
       0,     0,     0,     0,   224,   225,   226,   227,   228,     0,
      52,    53,    54,    55,    56,    57,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   229,   230,    59,    60,    61,   231,
       1,    62,    63,    64,     0,     0,     5,     0,     0,     0,
       0,     0,     0,     8,     0,     0,     0,     0,    10,     0,
       0,    13,    14,     0,     0,     0,   210,    17,     0,     0,
      19,     0,     0,     0,    23,     0,     0,     0,   211,   212,
       0,   232,     0,     0,     0,     0,   213,     0,     0,     0,
     233,   234,   235,   236,   237,   238,     0,     0,     0,    24,
       0,   214,   215,   216,   217,     0,     0,     0,     0,     0,
      30,    31,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    46,     0,    48,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    49,   218,   219,   220,   221,
     222,   223,     0,     0,     0,     0,    50,    51,     0,     0,
       0,     0,     0,   224,   225,   226,   227,   228,     0,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   229,   230,    59,    60,     1,   231,     0,
      62,     0,    64,     5,     0,     0,     0,     0,     0,     0,
       8,     0,     0,     0,     0,    10,     0,     0,    13,    14,
       0,     0,     0,   210,    17,     0,     0,    19,     0,     0,
       0,    23,     0,     0,     0,   211,   212,     0,     0,     0,
     232,     0,     0,   213,     0,     0,   356,     0,     0,   233,
     357,   235,   236,   237,   238,     0,    24,     0,   214,   215,
     216,   217,     0,     5,     0,     0,     0,    30,    31,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    14,
       0,     0,     0,    46,     0,    48,     0,     0,     0,     0,
       0,    23,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    49,   218,   219,   220,   221,   222,   223,     0,
       0,     0,     0,    50,    51,     0,    24,    25,     0,     5,
     224,   225,   226,   227,   228,     0,     0,    30,    31,     0,
      56,     0,     0,     0,     0,    14,    58,     0,     0,     0,
       0,     0,     0,     0,     0,    48,     0,    23,     0,     0,
     229,   230,    59,    60,     0,   231,     0,    62,     0,    64,
       0,     0,    49,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    24,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    30,    31,     0,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,    58,   232,     0,     0,
       0,    48,   210,   579,     0,     0,   233,   234,   235,   236,
     237,   238,    59,    60,   211,   212,     0,    62,    49,     0,
       0,     0,   213,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    24,     0,   214,   215,   216,
     217,     0,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,   453,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   276,    59,    60,
       0,     0,     0,    62,     0,     0,    68,     0,     0,     0,
       0,     0,   218,   219,   220,   221,   222,   223,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   224,
     225,   226,   227,   228,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,    58,     0,     0,     0,     0,
       0,     0,     0,    67,   210,     0,     0,     0,     0,   229,
     230,     0,    68,     0,   231,     0,   211,   212,     0,     0,
       0,     0,     0,     0,   213,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    24,     0,   214,
     215,   216,   217,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   232,     0,     0,     0,
       0,     0,     0,     0,     0,   233,   234,   235,   236,   237,
     238,     0,     0,     0,     0,     0,     0,     0,     0,   701,
       0,     0,     0,     0,   218,   219,   220,   221,   222,   223,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   224,   225,   226,   227,   228,     0,     0,     0,     0,
       0,     0,     0,   210,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   211,   212,     0,     0,     0,
       0,   229,   230,   213,     0,     0,   231,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    24,    25,   214,   215,
     216,   217,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   232,     0,
       0,     0,   928,   929,     0,   664,     0,   233,   234,   235,
     236,   237,   238,   218,   219,   220,   221,   222,   223,     0,
     930,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     224,   225,   226,   227,   228,     0,     0,     0,     0,     0,
       0,     0,   210,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   211,   212,     0,     0,     0,     0,
     229,   230,   213,     0,     0,   231,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    24,    25,   214,   215,   216,
     217,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   232,     0,     0,
       0,   806,   807,     0,   664,  1018,   233,   234,   235,   236,
     237,   238,   218,   219,   220,   221,   222,   223,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   224,
     225,   226,   227,   228,     0,     0,     0,     0,     0,     0,
       0,   210,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   211,   212,     0,     0,     0,     0,   229,
     230,   213,     0,     0,   231,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    24,    25,   214,   215,   216,   217,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   232,     0,     0,     0,
     806,   807,     0,   664,  1068,   233,   234,   235,   236,   237,
     238,   218,   219,   220,   221,   222,   223,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   224,   225,
     226,   227,   228,     0,     0,     0,     0,     0,     0,     0,
     210,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   211,   212,     0,     0,     0,     0,   229,   230,
     213,     0,     0,   231,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    24,     0,   214,   215,   216,   217,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,     0,     0,   232,     0,     0,     0,   806,
     807,     0,   664,     0,   233,   234,   235,   236,   237,   238,
     218,   219,   220,   221,   222,   223,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   224,   225,   226,
     227,   228,     0,     0,     0,     0,     0,     0,     0,   210,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   211,   212,     0,     0,     0,     0,   229,   230,   213,
       0,     0,   231,     0,    62,     0,     0,     0,     0,     0,
       0,     0,    24,     0,   214,   215,   216,   217,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,     0,     0,   232,     0,     0,     0,     0,     0,
       0,  -445,     0,   233,   234,   235,   236,   237,   238,   218,
     219,   220,   221,   222,   223,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,   225,   226,   227,
     228,     0,     0,     0,     0,     0,     0,     0,   210,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     211,   212,     0,     0,     0,     0,   229,   230,   213,     0,
       0,   231,     0,    62,     0,     0,     0,     0,     0,     0,
       0,    24,     0,   214,   215,   216,   217,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   232,     0,     0,     0,     0,     0,     0,
       0,     0,   233,   234,   235,   236,   237,   238,   218,   219,
     220,   221,   222,   223,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   224,   225,   226,   227,   228,
       0,     0,     0,     0,     0,     0,     0,   210,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   211,
     212,     0,     0,     0,     0,   229,   230,   213,     0,     0,
     231,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      24,     0,   214,   215,   216,   217,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   232,   604,     0,     0,     0,     0,     0,     0,
       0,   233,   234,   235,   236,   237,   238,   218,   219,   220,
     221,   222,   223,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   224,   225,   226,   227,   228,     0,
       0,     0,     0,     0,     0,     0,   210,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   211,   212,
       0,     0,     0,     0,   229,   230,   213,     0,     0,   231,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    24,
       0,   214,   215,   216,   217,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   232,     0,     0,   642,     0,     0,     0,     0,     0,
     233,   234,   235,   236,   237,   238,   218,   219,   220,   221,
     222,   223,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   224,   225,   226,   227,   228,     0,     0,
       0,     0,     0,     0,     0,   210,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   211,   212,     0,
       0,     0,     0,   229,   230,   213,     0,     0,   231,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    24,    25,
     214,   215,   216,   217,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     232,     0,     0,     0,     0,     0,     0,   664,     0,   233,
     234,   235,   236,   237,   238,   218,   219,   220,   221,   222,
     223,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   224,   225,   226,   227,   228,     0,     0,     0,
       0,     0,     0,     0,   210,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   211,   212,     0,     0,
       0,     0,   229,   230,   213,     0,     0,   231,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    24,     0,   214,
     215,   216,   217,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   232,
       0,     0,     0,     0,     0,     0,     0,     0,   233,   234,
     235,   236,   237,   238,   218,   219,   220,   221,   222,   223,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   224,   225,   226,   227,   228,     0,     0,     0,     0,
       0,     0,     0,   210,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,   211,   212,     0,     0,     0,
       0,   229,   230,   213,     0,     0,   231,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    24,     0,   214,   215,
     216,   217,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   232,     0,
       0,     0,     0,     0,     0,     0,     0,   233,   234,   235,
     236,   237,   238,   218,   219,   220,   221,   222,   223,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     224,   225,   226,   227,   228,     0,     0,     0,     0,     0,
       0,     0,   210,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   211,   212,     0,     0,     0,     0,
     229,   230,   213,     0,     0,   231,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    24,     0,   214,   215,   216,
     217,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   232,     0,     0,
       0,     0,     0,     0,     0,     0,   233,   582,   235,   236,
     237,   238,   218,   219,   220,   221,   222,   223,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   224,
     225,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   210,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,   211,   212,     0,     0,     0,     0,   229,
     230,   213,     0,     0,   231,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    24,     0,   214,   215,   216,   217,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   232,     0,     0,     0,
       0,     0,   577,     0,     0,   233,   234,   235,   236,   237,
     238,   218,   219,   220,   221,   222,   223,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   224,   225,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     210,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   211,   212,     0,     0,     0,     0,   229,   230,
     213,     0,     0,   231,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    24,     0,   214,   215,   216,   217,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   232,     0,     0,     0,     0,
       0,   773,     0,     0,   233,   234,   235,   236,   237,   238,
     218,   219,   220,   221,   222,   223,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   224,   225,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   210,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   211,   212,     0,     0,     0,     0,   229,   230,   213,
       0,     0,   231,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    24,     0,   214,   215,   216,   217,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   232,     0,     0,     0,     0,     0,
       0,   789,     0,   233,   234,   235,   236,   237,   238,   218,
     219,   220,   221,   222,   223,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,   225,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   210,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     211,   212,     0,     0,     0,     0,   229,   230,   213,     0,
       0,   231,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    24,     0,   214,   215,   216,   217,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   232,   979,     0,     0,     0,     0,     0,
       0,     0,   233,   234,   235,   236,   237,   238,   218,   219,
     220,   221,   222,   223,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   224,   225,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   210,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   211,
     212,     0,     0,     0,     0,   229,   230,   213,     0,     0,
     231,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      24,     0,   214,   215,   216,   217,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   232,   986,     0,     0,     0,     0,     0,     0,
       0,   233,   234,   235,   236,   237,   238,   218,   219,   220,
     221,   222,   223,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   224,   225,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   210,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   211,   212,
       0,     0,     0,     0,   229,   230,   213,     0,     0,   231,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    24,
       0,   214,   215,   216,   217,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   232,     0,     0,     0,     0,     0,     0,     0,     0,
     233,   234,   235,   236,   237,   238,   218,   219,   220,   221,
     222,   223,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   224,   225,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   210,     0,     0,     0,    58,
       0,   210,     0,     0,     0,     0,     0,   211,   212,     0,
       0,     0,     0,   211,   212,   213,     0,     0,   231,     0,
       0,   213,     0,     0,     0,     0,     0,     0,    24,     0,
     214,   215,   216,   217,    24,     0,   214,   215,   216,   217,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     366,     0,     0,     0,     0,     0,     0,     0,     0,   233,
     234,   235,   236,   237,   238,   218,   219,   220,   221,   222,
     223,   218,   219,   220,   221,   222,   223,     0,     0,     0,
       0,     0,   224,   225,     0,     0,     0,     0,   224,   225,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   231,     0,     0,
       1,     2,     3,   231,     0,     4,     5,     0,     0,     0,
       6,     0,     7,     8,     9,     0,     0,     0,    10,    11,
      12,    13,    14,     0,    15,    16,     0,    17,    18,     0,
      19,    20,    21,    22,    23,     0,     0,     0,     0,   368,
       0,     0,     0,     0,     0,   378,     0,     0,   233,   234,
     235,   236,   237,   238,   233,   234,   235,   236,   237,   238,
      25,     0,     0,     0,     0,     0,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,     0,    48,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    49,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,    51,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
      53,    54,    55,    56,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,    60,    61,     0,     0,
      62,    63,    64,     1,     2,     3,     0,     0,     4,     5,
       0,     0,     0,     6,     0,     7,     8,     9,     0,     0,
       0,    10,    11,    12,    13,    14,     0,    15,    16,     0,
      17,    18,     0,    19,    20,    21,    22,    23,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   899,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    25,     0,     0,     0,     0,     0,    26,
      27,    28,    29,    30,    31,    32,     0,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
       0,    48,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    49,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,    53,    54,    55,    56,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    59,    60,
      61,     0,     0,    62,     0,    64,     1,     2,     3,     0,
       0,     4,     5,     0,     0,     0,     6,     0,     7,     8,
       9,     0,     0,     0,    10,    11,    12,    13,    14,     0,
      15,    16,     0,    17,    18,     0,    19,    20,    21,    22,
      23,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    25,     0,     0,     0,
       0,     0,    26,    27,    28,    29,    30,    31,    32,     0,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,     0,    48,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    49,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,    53,    54,    55,    56,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,    60,    61,     0,     0,    62,     0,    64,     1,
       2,     3,     0,     0,     4,     5,     0,     0,     0,     6,
       0,     7,     8,     9,     0,     0,     0,    10,    11,    12,
      13,    14,     0,    15,    16,     0,    17,    18,     0,    19,
      20,    21,    22,    23,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    25,
       0,     0,     0,     0,     0,    26,    27,    28,    29,    30,
      31,    32,     0,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    49,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,    53,
      54,    55,    56,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    59,    60,    61,     1,     2,     3,
       0,    64,     4,     5,     0,     0,     0,     6,     0,     7,
       8,     9,     0,     0,     0,    10,    11,    12,    13,    14,
       0,    15,    16,     0,    17,    18,     0,    19,    20,    21,
      22,    23,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    25,     0,     0,
       0,     0,     0,    26,    27,    28,    29,    30,    31,     0,
       0,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    49,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,    51,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    53,    54,    55,
      56,    57,     0,     0,     0,     0,     1,     2,     3,     0,
       0,     4,     5,     0,     0,     0,     6,     0,     0,     8,
       9,     0,    59,    60,    10,    11,    12,    13,    14,    64,
      15,    16,     0,    17,     0,     0,    19,     0,    21,    22,
      23,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    26,    27,    28,    29,    30,    31,     0,     0,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,     0,     1,     0,     0,     0,     0,     0,
       5,     0,     0,     0,     0,     0,     0,     8,     0,     0,
       0,    49,    10,     0,     0,    13,    14,     0,     0,     0,
       0,    17,    50,    51,    19,     0,     0,     0,    23,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,    56,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,    60,     0,    30,    31,     0,     0,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      46,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    49,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,    51,     2,     3,     0,     0,     4,     5,     0,     0,
       0,     6,     0,     7,     0,     9,     0,    56,     0,     0,
      11,    12,     0,    14,     0,    15,    16,     0,     0,    18,
       0,     0,    20,    21,    22,    23,     0,     0,     0,    59,
      60,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    25,     0,     0,     0,     0,     0,    26,    27,    28,
      29,    30,    31,    32,     0,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,     0,     0,    48,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    49,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,    53,    54,    55,    56,    57,     0,     0,     0,     0,
       0,     2,     3,     0,     0,     4,     5,     0,     0,     0,
       6,     0,     7,     0,     9,     0,    59,    60,    61,    11,
      12,    62,    14,     0,    15,    16,     0,     0,    18,     0,
       0,    20,    21,    22,    23,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      25,     0,     0,     0,     0,     0,    26,    27,    28,    29,
      30,    31,    32,     0,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,     0,     0,     0,     0,
       0,     0,     0,     2,     3,     0,     0,     4,     5,     0,
       0,     0,     6,     0,     0,    49,     9,     0,     0,     0,
       0,    11,    12,     0,    14,     0,    15,    16,     0,     0,
       0,     0,     0,     0,    21,    22,    23,     0,     0,    52,
      53,    54,    55,    56,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,    60,    61,    26,    27,
      28,    29,    30,    31,     0,     0,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,     0,     0,
       0,     0,     0,     0,     0,     2,     3,     0,     0,     4,
       0,     0,     0,     0,     6,     0,     0,    49,     9,     0,
       0,     0,     0,    11,    12,     0,     0,     0,    15,    16,
       0,     0,     0,     0,     0,     0,    21,    22,     0,     0,
       0,     0,    53,    54,    55,    56,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    59,    60,     0,
      26,    27,    28,    29,     0,     0,     0,     0,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    53,    54,    55,     0,    57
};

static const yytype_int16 yycheck[] =
{
       0,    79,    80,    67,   110,    83,    84,   213,   128,   206,
     704,   143,     0,   478,   200,   558,     0,   117,    79,    80,
     116,     0,     0,   116,    79,    80,   397,   213,     0,   116,
     401,    33,    25,    33,    62,   115,   439,    51,   210,   211,
     212,   964,   203,    19,   114,    21,    79,    80,   118,   458,
     130,   223,    62,   133,   134,   135,   642,   137,   159,   331,
     161,    37,    45,    46,   273,    65,     0,    67,    68,    62,
      63,   141,    19,    11,    21,    77,    78,    77,    78,    79,
      80,    40,    41,    42,   203,   494,    25,    68,    67,    68,
      37,  1092,   116,   117,    62,   882,   883,    50,    65,   926,
      79,    80,    62,    63,    90,    91,   106,   152,    65,    91,
      51,    62,    63,    91,   142,    93,   116,   117,   106,   204,
     205,    91,   106,    62,    63,    40,   232,   106,   106,   205,
      62,    63,   142,    51,   106,   273,   273,   205,   117,    67,
     115,   226,   208,   143,   353,   943,  1147,   152,   946,   116,
     226,   283,   232,    91,   154,    67,   686,   132,   226,   116,
     346,   206,   137,   138,   143,   362,   273,   205,   365,  1092,
      62,    63,   106,   211,   142,   154,   276,   163,  1005,   275,
     203,   163,   142,   283,   570,   571,   572,   573,   275,   282,
    1125,   142,   206,   163,   204,   282,   174,   175,   151,   203,
     200,    83,   752,    85,   208,    87,    88,    89,  1146,   795,
     142,   204,   205,   213,   764,   353,   353,   220,   490,   224,
     492,   493,   273,  1158,  1147,   163,   203,   203,  1166,   210,
    1028,   510,   205,  1031,   210,   203,   206,   220,   221,   594,
     595,   596,   312,   646,   203,   213,   353,   203,   207,   208,
     142,   275,   276,   226,   222,   206,   203,   232,   282,   283,
     366,   752,   368,   542,   205,   204,   205,   961,   209,   347,
    1057,   203,   378,   764,   203,   275,   276,   208,   278,   208,
     208,   213,   282,   283,   205,    62,   366,   205,   368,   204,
     222,   209,   207,   208,   226,   273,   208,   276,   378,   278,
      25,   273,   353,   833,   283,   226,   205,   837,   275,    67,
     507,   203,   205,   519,    91,   282,   209,   319,   275,   319,
     208,   213,   402,     3,   205,   282,   205,   226,    62,    63,
     222,   214,   215,   519,   226,   205,    16,    62,    63,    91,
      68,    21,   205,   886,    24,   226,   346,   226,   211,   558,
      30,   203,   352,    33,   354,   435,   226,   453,   213,   824,
     447,   439,   440,   218,   219,   142,   453,   355,   438,   439,
     205,   355,   205,   203,   209,   353,   209,   355,   439,   449,
     450,   353,   203,   355,   439,   119,   163,     0,    91,     0,
     491,   366,   139,   368,   141,   226,   799,   397,   373,  1093,
     375,   401,   377,   378,    67,   814,   439,   203,   142,    89,
     781,   163,   164,   205,   204,   521,   695,   209,   524,   210,
     558,   558,   211,   447,   502,   210,   203,   205,   210,   453,
     203,   209,   512,   203,   514,   806,   213,   208,   208,   119,
     120,   720,   205,   213,   203,   222,   209,   447,   382,   383,
     384,   558,   222,   453,   203,   533,   534,   203,   204,   211,
     163,   164,   208,    43,    44,   203,    79,    80,    62,    63,
     570,   571,   533,   534,   226,   553,   203,   555,   533,   534,
     447,   203,   560,   561,   205,   165,   453,   205,   209,   212,
     447,   209,   553,   106,   572,   573,   453,    91,   553,   223,
     533,   534,   222,   116,   690,   891,    49,   558,   211,   895,
     510,   572,   573,    47,    48,   153,   925,   572,   573,   519,
     553,   930,   204,   226,   879,   204,   881,   204,   205,   531,
     532,   531,   532,   533,   534,   421,   422,   423,   424,   572,
     573,   750,   542,   663,   204,   731,   732,    62,   142,   205,
     552,   225,   552,   209,   533,   534,   828,   928,   204,  1024,
     203,   770,   564,   221,   564,    25,   205,   846,   204,   163,
     570,   571,   572,   573,   204,   645,   646,   210,   203,   204,
     558,   210,   646,   208,   654,   225,   558,   200,   668,   669,
     206,   570,   571,   572,   573,   209,   203,   204,   678,   679,
     213,   208,    62,    63,    64,   203,   204,    67,   210,   203,
     208,   210,   750,   750,   715,   716,   204,   205,   204,   213,
     700,   204,   831,   703,   995,   204,   997,   142,   222,   204,
     205,   204,   770,   770,   145,   146,   147,   148,   149,   150,
     204,   205,   226,   750,   204,   205,   204,   205,   845,   204,
     205,   203,   858,   753,   205,   206,  1121,   843,   592,   593,
     273,   203,   275,   770,   664,   752,   203,   799,   203,   282,
     670,   203,   858,   116,   117,   208,    62,   886,   560,   561,
     562,   204,   205,   962,   963,   204,   205,   208,   203,   753,
     690,   206,   210,   831,   831,   695,   204,   205,   213,   750,
     210,   765,   226,   783,   784,   210,   878,   222,   880,   226,
     882,   883,   204,   205,   204,   205,    62,    63,   206,   770,
     720,   204,   205,   203,   831,   204,   205,   807,   203,   799,
     203,   731,   732,   346,   347,   737,   856,   737,   206,   352,
     353,   211,   355,   224,   225,   226,   227,   228,   886,   886,
     226,   751,   752,   753,   754,   861,   142,   863,   864,   891,
     204,   205,  1041,   895,   764,   765,   952,   204,   205,   382,
     383,   384,   750,  1128,   753,   754,    62,    63,   750,   886,
     831,   781,   205,   771,   397,   752,   765,   771,   401,   789,
     226,   891,   770,   771,  1149,   752,   142,   764,   770,   771,
     226,   921,   206,   778,   779,   780,   806,   764,   204,   205,
     204,   205,   204,  1168,   205,  1094,   209,   203,   204,   204,
     205,   209,   208,  1009,   204,   205,   439,   213,  1025,   204,
     417,   418,   425,   426,   447,   886,   222,   419,   420,   204,
     453,  1038,   204,   843,   433,   434,   846,   917,   918,   929,
     205,   205,  1131,   831,   205,   205,   142,   203,   858,   831,
     206,   570,   571,   572,   573,   943,   204,   213,   205,    67,
       0,    67,   204,    62,   204,   204,   222,   225,   958,  1051,
     204,  1053,   943,  1055,  1056,  1057,  1072,   210,   943,   210,
     970,   891,  1171,  1013,    39,   895,   203,   510,   226,   512,
      68,    62,    63,   203,   203,   203,   519,   203,   886,   203,
     943,   203,   891,   210,   886,   204,   895,   203,   998,   204,
     533,   534,   208,   204,   204,   204,   926,   213,   928,   542,
     206,  1117,   206,   206,   205,  1015,   222,    67,    62,   211,
     553,   204,   942,   943,   204,   558,   946,   560,   948,    79,
      80,  1071,   952,   142,    62,    63,  1076,   226,   211,   572,
     573,   211,   962,   963,   943,   205,   203,   946,   204,  1155,
      68,   203,   211,   204,   204,     9,   106,   204,   204,   592,
     593,   142,   204,   206,   225,   206,   205,   117,  1108,   119,
    1070,    25,   206,  1113,  1100,   995,   211,   997,   211,  1079,
     211,   206,   149,    37,   204,  1005,   205,   211,   205,  1009,
     204,   204,   204,   143,   203,   204,   226,  1137,   142,   208,
    1140,   205,   152,   139,   213,   204,   203,   205,    62,   159,
     206,   161,  1112,   222,   142,   206,   226,    14,   143,    73,
      74,  1041,   203,   752,   753,   226,   204,   209,   203,   205,
     225,   664,   213,   206,   203,   764,   765,    91,   144,   209,
     204,   222,   204,   204,   194,   205,   196,   203,   203,   226,
     204,  1151,  1072,   205,   108,   204,   206,   690,   203,   203,
     204,  1003,   695,  1163,   208,  1097,   427,   429,   428,   213,
     206,    72,   431,  1089,  1094,   203,   430,   432,   222,   290,
     362,   303,   136,   346,   560,   213,   674,   720,   142,   948,
     946,   837,  1031,   351,   222,   560,    97,  1117,   731,   732,
     101,   833,   771,   104,   158,   159,   843,   952,   751,   163,
     789,  1131,   789,   849,    79,    80,   849,   750,    83,    84,
      85,    86,    87,    88,    89,    90,   276,    92,   278,    94,
     518,   856,  1113,   283,  1071,  1155,  1155,   770,   771,   106,
     290,   770,   117,   748,  1055,   353,  1140,  1057,   781,   203,
      -1,  1171,   883,   645,   208,   156,   789,    -1,   645,   213,
      -1,   162,   891,   898,   165,   166,   895,   168,   222,    -1,
      -1,   172,    -1,   806,    -1,    -1,   177,    -1,   328,   329,
     181,   182,    -1,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,    -1,   195,    -1,   197,    -1,   831,    -1,
      -1,    -1,   352,    -1,    -1,   355,    -1,    -1,    -1,    -1,
     843,    -1,   362,   846,    -1,    -1,   849,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   858,   362,    -1,    -1,   365,
      -1,    -1,   382,   383,   384,    -1,    -1,    -1,   374,    -1,
     376,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   387,    -1,   886,    -1,    -1,    -1,    -1,    -1,   988,
      -1,    -1,    -1,    -1,   400,    -1,    -1,   403,   404,   405,
     406,   407,   408,   409,   410,   411,   412,   413,   229,   230,
      -1,    -1,   233,   234,   235,   236,   237,   238,    -1,   439,
      -1,    -1,    -1,   926,    -1,   928,    -1,    -1,    -1,    -1,
      -1,   437,    -1,    -1,    -1,     9,    -1,    -1,    -1,    -1,
     943,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   952,
      -1,    25,   458,    -1,    -1,    -1,    -1,    -1,    -1,   962,
     963,    -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   491,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,   488,    -1,    -1,    -1,    -1,    -1,   494,    -1,
     510,     9,   995,     9,   997,    -1,    -1,    -1,    -1,    73,
      74,   507,  1005,    -1,    -1,    -1,  1009,    25,    -1,    25,
      -1,    -1,    -1,   533,   534,    -1,    -1,    91,   353,    37,
      -1,    37,   542,    -1,    -1,    -1,    -1,   362,    -1,    -1,
      -1,    -1,    -1,   553,   108,    -1,   357,    -1,  1041,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   572,   573,    -1,    73,    74,    73,    74,    -1,
      -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,    -1,  1072,
      -1,    -1,   592,   593,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,   159,    -1,    -1,    -1,   163,
     108,  1094,   108,   414,   415,   416,   417,   418,   419,   420,
     421,   422,   423,   424,   425,   426,   427,   428,   429,   430,
     431,   432,   433,   434,  1117,    -1,    -1,    -1,   136,    -1,
     136,    -1,    -1,    -1,    -1,    -1,   646,    -1,  1131,   203,
      -1,    -1,    -1,    -1,   208,    -1,    -1,    -1,    -1,   213,
     158,   159,   158,   159,    -1,   225,    -1,   163,   222,    -1,
      -1,    -1,  1155,    -1,    -1,    -1,   527,    -1,   664,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1171,    -1,
      -1,    -1,    -1,    -1,    -1,   695,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
      -1,    67,    -1,    -1,    -1,   715,   716,    -1,   533,   534,
     720,    -1,    -1,    79,    80,    -1,    -1,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    -1,    -1,    -1,   554,
     555,   556,    -1,   558,    -1,   560,   561,   562,    -1,    -1,
      -1,    -1,    -1,   753,     9,    -1,    -1,   572,   573,    -1,
      -1,   117,    -1,    -1,    -1,   765,    -1,    -1,    -1,    -1,
      25,   771,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   582,    37,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,   594,   595,   596,    -1,    -1,   599,   799,
      -1,    -1,    -1,   789,    -1,    -1,   792,    62,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   814,    -1,
      -1,    -1,    -1,    -1,    -1,   686,    91,   688,   194,    -1,
     196,   642,    -1,    -1,    -1,    -1,   846,    -1,    -1,   849,
     206,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,   845,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   725,   726,    -1,   728,    -1,   730,
      -1,   136,   733,   734,   735,    -1,    -1,   142,    -1,    -1,
      -1,   891,    -1,    -1,    -1,   895,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,   159,   756,    -1,    -1,   163,    -1,
      -1,   762,    -1,    -1,    -1,   766,    -1,   768,    -1,    -1,
     276,    -1,    -1,    -1,    -1,    -1,    -1,   283,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   750,    -1,    -1,    -1,   925,
     926,    -1,    -1,   943,   930,    -1,    -1,    -1,   203,    -1,
      -1,    -1,    -1,   208,    -1,   770,    -1,    -1,   213,    -1,
      -1,    -1,   962,   963,    -1,    -1,    -1,   222,    -1,    -1,
      -1,    -1,   328,   329,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   833,    -1,    -1,    -1,   837,    -1,    -1,    -1,
      -1,   842,    -1,   844,   795,    -1,    -1,   353,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   362,    -1,    -1,     9,
      -1,    -1,    -1,    -1,    -1,  1001,    -1,    -1,  1004,  1005,
      -1,   872,   873,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,  1025,
      -1,  1041,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1038,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    62,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   886,    -1,    73,    74,    -1,    -1,    25,   879,    -1,
     881,   882,   883,   439,   440,   441,   442,    -1,    -1,    37,
      -1,    91,    -1,   944,  1094,    -1,    -1,    -1,    -1,    -1,
     951,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    62,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    73,    74,    -1,    -1,    -1,
      -1,  1131,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,
      -1,    -1,   142,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,
     108,    -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1171,    -1,    -1,    -1,    -1,  1027,   533,   534,  1030,
      -1,    -1,    -1,    -1,  1035,    -1,    -1,    -1,   136,    -1,
      -1,    -1,    -1,    -1,   142,    -1,    -1,   553,    -1,   555,
     556,    -1,   558,   203,   560,   561,   562,    -1,    -1,    -1,
     158,   159,    -1,   213,    -1,   163,   572,   573,    -1,    -1,
      -1,    -1,   222,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1084,    -1,    -1,  1087,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   203,  1057,     4,     5,    -1,
      -1,     8,    -1,    -1,    -1,   213,    13,    -1,    -1,    -1,
      17,    -1,    -1,    -1,   222,    22,    23,    -1,    -1,    -1,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    36,
     646,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    69,    70,    71,    72,    -1,  1128,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,  1149,    -1,
      -1,     3,     4,     5,    -1,    -1,     8,     9,    -1,    -1,
      -1,    13,    -1,    15,    16,    17,    -1,  1168,    -1,    21,
      22,    23,    24,    25,    -1,    27,    28,    -1,    30,    31,
      -1,    33,    34,    35,    36,    37,   133,   134,   135,    -1,
     137,    -1,    -1,    -1,   750,    -1,    -1,   753,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   765,
      62,    63,    -1,    -1,   770,    -1,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    -1,    -1,   799,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,    -1,
      -1,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     886,    -1,    -1,    -1,    -1,   891,    -1,    -1,    -1,   895,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   203,    -1,    -1,    -1,    -1,    -1,    -1,   210,    -1,
      -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     222,     3,     4,     5,   226,    -1,     8,     9,    -1,    -1,
      -1,    13,    -1,    15,    16,    17,    -1,   943,    -1,    21,
      22,    23,    24,    25,    -1,    27,    28,    -1,    30,    31,
      -1,    33,    34,    35,    36,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,
      72,    73,    74,    75,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,    -1,
      -1,   163,    -1,   165,     3,     4,     5,    -1,    -1,     8,
       9,    -1,    -1,    -1,    13,    -1,    15,    16,    17,    -1,
      -1,    -1,    21,    22,    23,    24,    25,    -1,    27,    28,
      -1,    30,    31,    -1,    33,    34,    35,    36,    37,    -1,
      -1,   203,    -1,    -1,    -1,    -1,   208,    -1,    -1,    -1,
      -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     222,    -1,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,
      69,    70,    71,    72,    73,    74,    75,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
     159,   160,    -1,    -1,   163,    -1,   165,     4,     5,    -1,
      -1,     8,     9,    -1,    -1,    -1,    13,    -1,    15,    -1,
      17,    -1,    -1,    -1,    -1,    22,    23,    -1,    25,    -1,
      27,    28,    -1,    -1,    31,    -1,    -1,    34,    35,    36,
      37,    -1,    -1,    -1,   203,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   222,    -1,    62,    63,    -1,    -1,    -1,
      -1,    -1,    69,    70,    71,    72,    73,    74,    75,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,   160,     4,     5,   163,    -1,     8,     9,
      -1,    -1,    -1,    13,    -1,    15,    -1,    17,    -1,    -1,
      -1,    -1,    22,    23,    -1,    25,    -1,    27,    28,    -1,
      -1,    31,    -1,    -1,    34,    35,    36,    37,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   203,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    -1,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,   222,    -1,    -1,    -1,    69,
      70,    71,    72,    73,    74,    75,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,
     160,    -1,    -1,   163,    -1,    -1,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    -1,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    -1,    39,   203,    41,    42,    -1,    -1,   208,    -1,
      -1,    -1,    49,   213,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   222,    -1,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,    -1,
      -1,   118,   119,   120,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,   130,    -1,   132,   133,   134,   135,   136,
     137,   138,    -1,   140,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
     157,   158,   159,   160,   161,    -1,   163,   164,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   203,    -1,    -1,    -1,
      -1,    -1,    -1,   210,   211,   212,   213,   214,   215,   216,
     217,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    -1,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    -1,    39,    -1,    41,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,    -1,    -1,   118,   119,   120,    -1,
      -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,    -1,
     132,   133,   134,   135,   136,   137,   138,    -1,   140,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,   157,   158,   159,   160,   161,
      -1,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   203,    -1,    -1,    -1,    -1,    -1,    -1,   210,   211,
     212,   213,   214,   215,   216,   217,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    -1,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    -1,    39,    -1,    41,    42,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,    -1,
      -1,   118,   119,   120,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,   130,    -1,   132,   133,   134,   135,   136,
     137,   138,    -1,   140,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
     157,   158,   159,   160,   161,    -1,   163,   164,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   203,    -1,    -1,    -1,
      -1,    -1,    -1,   210,    -1,   212,   213,   214,   215,   216,
     217,     3,     4,     5,    -1,    -1,     8,     9,    -1,    -1,
      -1,    13,    -1,    15,    16,    17,    -1,    -1,    -1,    21,
      22,    23,    24,    25,    -1,    27,    28,    29,    30,    31,
      -1,    33,    34,    35,    36,    37,    -1,    -1,    -1,    41,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,   109,   110,   111,
     112,   113,   114,    -1,    -1,    -1,    -1,   119,   120,    -1,
      -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,    -1,
     132,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,   157,   158,   159,   160,   161,
       3,   163,   164,   165,    -1,    -1,     9,    -1,    -1,    -1,
      -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    24,    25,    -1,    -1,    -1,    29,    30,    -1,    -1,
      33,    -1,    -1,    -1,    37,    -1,    -1,    -1,    41,    42,
      -1,   203,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,
     212,   213,   214,   215,   216,   217,    -1,    -1,    -1,    62,
      -1,    64,    65,    66,    67,    -1,    -1,    -1,    -1,    -1,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,   109,   110,   111,   112,
     113,   114,    -1,    -1,    -1,    -1,   119,   120,    -1,    -1,
      -1,    -1,    -1,   126,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   156,   157,   158,   159,     3,   161,    -1,
     163,    -1,   165,     9,    -1,    -1,    -1,    -1,    -1,    -1,
      16,    -1,    -1,    -1,    -1,    21,    -1,    -1,    24,    25,
      -1,    -1,    -1,    29,    30,    -1,    -1,    33,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,
     203,    -1,    -1,    49,    -1,    -1,   209,    -1,    -1,   212,
     213,   214,   215,   216,   217,    -1,    62,    -1,    64,    65,
      66,    67,    -1,     9,    -1,    -1,    -1,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    89,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,   109,   110,   111,   112,   113,   114,    -1,
      -1,    -1,    -1,   119,   120,    -1,    62,    63,    -1,     9,
     126,   127,   128,   129,   130,    -1,    -1,    73,    74,    -1,
     136,    -1,    -1,    -1,    -1,    25,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    37,    -1,    -1,
     156,   157,   158,   159,    -1,   161,    -1,   163,    -1,   165,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    73,    74,    -1,    -1,    -1,    -1,    -1,
     136,    -1,    -1,    -1,    -1,    -1,   142,   203,    -1,    -1,
      -1,    91,    29,   209,    -1,    -1,   212,   213,   214,   215,
     216,   217,   158,   159,    41,    42,    -1,   163,   108,    -1,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    -1,    64,    65,    66,
      67,    -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,   203,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   213,   158,   159,
      -1,    -1,    -1,   163,    -1,    -1,   222,    -1,    -1,    -1,
      -1,    -1,   109,   110,   111,   112,   113,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   203,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   213,    29,    -1,    -1,    -1,    -1,   156,
     157,    -1,   222,    -1,   161,    -1,    41,    42,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    64,
      65,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   203,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   212,   213,   214,   215,   216,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    -1,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,
      -1,   156,   157,    49,    -1,    -1,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,    -1,
      -1,    -1,   207,   208,    -1,   210,    -1,   212,   213,   214,
     215,   216,   217,   109,   110,   111,   112,   113,   114,    -1,
     225,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,    -1,
     156,   157,    49,    -1,    -1,   161,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,    -1,    -1,
      -1,   207,   208,    -1,   210,   211,   212,   213,   214,   215,
     216,   217,   109,   110,   111,   112,   113,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    29,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    41,    42,    -1,    -1,    -1,    -1,   156,
     157,    49,    -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   203,    -1,    -1,    -1,
     207,   208,    -1,   210,   211,   212,   213,   214,   215,   216,
     217,   109,   110,   111,   112,   113,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      29,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    41,    42,    -1,    -1,    -1,    -1,   156,   157,
      49,    -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    -1,    64,    65,    66,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,   203,    -1,    -1,    -1,   207,
     208,    -1,   210,    -1,   212,   213,   214,   215,   216,   217,
     109,   110,   111,   112,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,
     129,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,
      -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    41,    42,    -1,    -1,    -1,    -1,   156,   157,    49,
      -1,    -1,   161,    -1,   163,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    -1,    64,    65,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,   203,    -1,    -1,    -1,    -1,    -1,
      -1,   210,    -1,   212,   213,   214,   215,   216,   217,   109,
     110,   111,   112,   113,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
     130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      41,    42,    -1,    -1,    -1,    -1,   156,   157,    49,    -1,
      -1,   161,    -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    -1,    64,    65,    66,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   203,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   212,   213,   214,   215,   216,   217,   109,   110,
     111,   112,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,
      -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,
      42,    -1,    -1,    -1,    -1,   156,   157,    49,    -1,    -1,
     161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    -1,    64,    65,    66,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   212,   213,   214,   215,   216,   217,   109,   110,   111,
     112,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    42,
      -1,    -1,    -1,    -1,   156,   157,    49,    -1,    -1,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      -1,    64,    65,    66,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   203,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,
     212,   213,   214,   215,   216,   217,   109,   110,   111,   112,
     113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,   127,   128,   129,   130,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    42,    -1,
      -1,    -1,    -1,   156,   157,    49,    -1,    -1,   161,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    65,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     203,    -1,    -1,    -1,    -1,    -1,    -1,   210,    -1,   212,
     213,   214,   215,   216,   217,   109,   110,   111,   112,   113,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,   127,   128,   129,   130,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    42,    -1,    -1,
      -1,    -1,   156,   157,    49,    -1,    -1,   161,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    64,
      65,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   212,   213,
     214,   215,   216,   217,   109,   110,   111,   112,   113,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    29,    -1,    -1,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,
      -1,   156,   157,    49,    -1,    -1,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    64,    65,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   212,   213,   214,
     215,   216,   217,   109,   110,   111,   112,   113,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    -1,    -1,    -1,   142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,    -1,
     156,   157,    49,    -1,    -1,   161,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    -1,    64,    65,    66,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   212,   213,   214,   215,
     216,   217,   109,   110,   111,   112,   113,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    29,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    41,    42,    -1,    -1,    -1,    -1,   156,
     157,    49,    -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    -1,    64,    65,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   203,    -1,    -1,    -1,
      -1,    -1,   209,    -1,    -1,   212,   213,   214,   215,   216,
     217,   109,   110,   111,   112,   113,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      29,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    41,    42,    -1,    -1,    -1,    -1,   156,   157,
      49,    -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    -1,    64,    65,    66,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   203,    -1,    -1,    -1,    -1,
      -1,   209,    -1,    -1,   212,   213,   214,   215,   216,   217,
     109,   110,   111,   112,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,
      -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    41,    42,    -1,    -1,    -1,    -1,   156,   157,    49,
      -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    -1,    64,    65,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   203,    -1,    -1,    -1,    -1,    -1,
      -1,   210,    -1,   212,   213,   214,   215,   216,   217,   109,
     110,   111,   112,   113,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      41,    42,    -1,    -1,    -1,    -1,   156,   157,    49,    -1,
      -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    -1,    64,    65,    66,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   203,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   212,   213,   214,   215,   216,   217,   109,   110,
     111,   112,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,
      -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,
      42,    -1,    -1,    -1,    -1,   156,   157,    49,    -1,    -1,
     161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    -1,    64,    65,    66,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   212,   213,   214,   215,   216,   217,   109,   110,   111,
     112,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    42,
      -1,    -1,    -1,    -1,   156,   157,    49,    -1,    -1,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      -1,    64,    65,    66,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   203,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     212,   213,   214,   215,   216,   217,   109,   110,   111,   112,
     113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,   127,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,   142,
      -1,    29,    -1,    -1,    -1,    -1,    -1,    41,    42,    -1,
      -1,    -1,    -1,    41,    42,    49,    -1,    -1,   161,    -1,
      -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,
      64,    65,    66,    67,    62,    -1,    64,    65,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     203,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   109,   110,   111,   112,   113,
     114,   109,   110,   111,   112,   113,   114,    -1,    -1,    -1,
      -1,    -1,   126,   127,    -1,    -1,    -1,    -1,   126,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,
       3,     4,     5,   161,    -1,     8,     9,    -1,    -1,    -1,
      13,    -1,    15,    16,    17,    -1,    -1,    -1,    21,    22,
      23,    24,    25,    -1,    27,    28,    -1,    30,    31,    -1,
      33,    34,    35,    36,    37,    -1,    -1,    -1,    -1,   203,
      -1,    -1,    -1,    -1,    -1,   203,    -1,    -1,   212,   213,
     214,   215,   216,   217,   212,   213,   214,   215,   216,   217,
      63,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,   159,   160,    -1,    -1,
     163,   164,   165,     3,     4,     5,    -1,    -1,     8,     9,
      -1,    -1,    -1,    13,    -1,    15,    16,    17,    -1,    -1,
      -1,    21,    22,    23,    24,    25,    -1,    27,    28,    -1,
      30,    31,    -1,    33,    34,    35,    36,    37,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    69,
      70,    71,    72,    73,    74,    75,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   132,   133,   134,   135,   136,   137,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,
     160,    -1,    -1,   163,    -1,   165,     3,     4,     5,    -1,
      -1,     8,     9,    -1,    -1,    -1,    13,    -1,    15,    16,
      17,    -1,    -1,    -1,    21,    22,    23,    24,    25,    -1,
      27,    28,    -1,    30,    31,    -1,    33,    34,    35,    36,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    69,    70,    71,    72,    73,    74,    75,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,   160,    -1,    -1,   163,    -1,   165,     3,
       4,     5,    -1,    -1,     8,     9,    -1,    -1,    -1,    13,
      -1,    15,    16,    17,    -1,    -1,    -1,    21,    22,    23,
      24,    25,    -1,    27,    28,    -1,    30,    31,    -1,    33,
      34,    35,    36,    37,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,    73,
      74,    75,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,   159,   160,     3,     4,     5,
      -1,   165,     8,     9,    -1,    -1,    -1,    13,    -1,    15,
      16,    17,    -1,    -1,    -1,    21,    22,    23,    24,    25,
      -1,    27,    28,    -1,    30,    31,    -1,    33,    34,    35,
      36,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    69,    70,    71,    72,    73,    74,    -1,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,
      -1,     8,     9,    -1,    -1,    -1,    13,    -1,    -1,    16,
      17,    -1,   158,   159,    21,    22,    23,    24,    25,   165,
      27,    28,    -1,    30,    -1,    -1,    33,    -1,    35,    36,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    70,    71,    72,    73,    74,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,     3,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,
      -1,   108,    21,    -1,    -1,    24,    25,    -1,    -1,    -1,
      -1,    30,   119,   120,    33,    -1,    -1,    -1,    37,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,   136,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,    -1,    73,    74,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,     4,     5,    -1,    -1,     8,     9,    -1,    -1,
      -1,    13,    -1,    15,    -1,    17,    -1,   136,    -1,    -1,
      22,    23,    -1,    25,    -1,    27,    28,    -1,    -1,    31,
      -1,    -1,    34,    35,    36,    37,    -1,    -1,    -1,   158,
     159,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,
      72,    73,    74,    75,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,   133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,
      -1,     4,     5,    -1,    -1,     8,     9,    -1,    -1,    -1,
      13,    -1,    15,    -1,    17,    -1,   158,   159,   160,    22,
      23,   163,    25,    -1,    27,    28,    -1,    -1,    31,    -1,
      -1,    34,    35,    36,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,
      73,    74,    75,    -1,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     4,     5,    -1,    -1,     8,     9,    -1,
      -1,    -1,    13,    -1,    -1,   108,    17,    -1,    -1,    -1,
      -1,    22,    23,    -1,    25,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    36,    37,    -1,    -1,   132,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,   159,   160,    69,    70,
      71,    72,    73,    74,    -1,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     4,     5,    -1,    -1,     8,
      -1,    -1,    -1,    -1,    13,    -1,    -1,   108,    17,    -1,
      -1,    -1,    -1,    22,    23,    -1,    -1,    -1,    27,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    36,    -1,    -1,
      -1,    -1,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,    -1,
      69,    70,    71,    72,    -1,    -1,    -1,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,   137
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,     3,     4,     5,     8,     9,    13,    15,    16,    17,
      21,    22,    23,    24,    25,    27,    28,    30,    31,    33,
      34,    35,    36,    37,    62,    63,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,   108,
     119,   120,   132,   133,   134,   135,   136,   137,   142,   158,
     159,   160,   163,   164,   165,   203,   210,   213,   222,   226,
     228,   229,   230,   276,   277,   278,   285,   289,   290,   291,
     292,   295,   296,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   315,   317,   318,   319,   320,
     322,   326,   330,   342,   349,   402,   403,   404,   405,   406,
     417,   427,   428,   429,   430,   203,   203,   213,   230,   418,
     419,   420,   421,   422,   423,   424,   425,   427,   203,   203,
     203,   203,   220,   208,   208,   208,    67,   203,   203,   203,
     428,   430,    68,   293,   295,   297,   330,   427,   427,     0,
     330,   331,   332,   226,   205,   226,   205,   226,   226,   418,
     226,   418,   230,   294,   295,   308,   310,   317,   318,   319,
     330,   427,   230,   295,   308,   310,   317,   318,   319,   330,
     427,   294,   318,   295,   317,   318,   294,   295,   317,   294,
     294,   294,   295,   317,   292,   317,   292,   317,   331,   331,
     331,   404,   371,   372,   407,   203,   208,   443,   445,   448,
      29,    41,    42,    49,    64,    65,    66,    67,   109,   110,
     111,   112,   113,   114,   126,   127,   128,   129,   130,   156,
     157,   161,   203,   212,   213,   214,   215,   216,   217,   229,
     231,   232,   233,   234,   235,   236,   237,   240,   241,   242,
     243,   245,   251,   252,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   331,   360,   203,   213,   230,   332,   422,
     424,   426,   203,   293,   419,   421,   424,   443,    90,   282,
     283,   284,   330,   234,   203,   273,    25,    62,    63,   204,
     205,   312,   313,   314,   360,   273,   273,   273,   273,   360,
     360,   272,   204,   204,   211,   297,   427,   330,   427,   331,
     284,   284,   331,   331,   331,   331,   279,   331,   292,   292,
     331,   280,   331,   331,   331,   331,   331,   331,   331,   331,
     331,   331,   331,   331,   331,   331,   316,   206,   229,   230,
     347,   359,   210,   204,   444,   447,   209,   213,   272,   274,
     295,   298,   299,   317,   330,   208,   203,   255,   203,   255,
     255,   359,   382,   203,   203,   203,   203,   203,   203,   255,
     372,   372,   372,   372,   372,   256,   256,   203,   273,   360,
     371,   256,   256,   256,   256,   256,   256,    40,    41,    42,
     203,   207,   208,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,   225,   213,   218,   219,   214,   215,    43,
      44,    45,    46,   220,   221,    47,    48,   212,   222,   223,
      49,   153,    50,   151,   152,   224,   204,   205,   290,   292,
     301,   303,   307,   309,   311,   204,   426,   203,   427,   204,
     204,   443,   426,   203,   421,   424,   387,   282,   225,   204,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   327,   328,   329,   204,   203,   204,
     313,   221,   209,   209,   209,   204,   204,   204,   205,   443,
     281,   418,   286,   287,   225,   361,   361,   315,   323,   359,
     230,   318,   345,   346,   343,   347,     6,     7,    10,    11,
      12,    18,    19,    20,    26,    32,    39,    68,    90,   106,
     107,   115,   118,   138,   140,   211,   229,   230,   273,   275,
     276,   289,   290,   291,   292,   330,   359,   367,   368,   369,
     370,   371,   373,   374,   375,   377,   379,   380,   383,   384,
     385,   386,   290,   292,   295,   301,   307,   317,   409,   410,
     411,   412,   413,   414,   415,   416,   446,   229,   355,   356,
     289,   290,   291,   292,   354,   357,   358,   209,   209,   209,
     274,   298,   213,   274,   360,   360,   360,   272,   360,   272,
     360,   360,   210,   210,   276,   276,   276,   272,   204,   204,
     204,   229,   230,   253,   204,   254,   272,   253,   273,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     256,   256,   256,   256,   257,   257,   258,   258,   259,   259,
     259,   259,   260,   260,   261,   262,   263,   264,   265,   266,
     268,   268,   206,   273,   272,   203,   213,   222,   431,   443,
     449,   451,   431,   204,   204,   443,   443,   204,   204,   426,
      19,    21,    37,   203,   210,   274,   362,   226,   203,   203,
     203,   203,   203,   204,   205,    64,    67,   312,   208,   208,
     272,   361,   284,   361,   361,   362,   210,   324,   325,   318,
     210,   344,   226,   274,   226,   206,   367,   378,   230,   273,
     203,   226,   273,   203,   203,   211,   387,   381,   382,    68,
     210,   371,   226,   371,   226,   418,   418,   427,   427,   226,
     206,   211,   367,   418,   317,   317,   317,   410,   230,   294,
     318,   326,   349,   294,   318,   294,   418,   205,   226,   145,
     146,   147,   148,   149,   150,   433,   438,   440,   441,   442,
     204,   205,   203,   213,   222,   421,   427,   432,   445,   450,
     452,   421,   427,   432,   203,   213,   427,   432,   427,   432,
     204,   205,   209,   209,   209,   204,   204,   204,   205,   205,
     205,   205,   204,   276,   276,   270,   270,   270,   205,   210,
     256,   204,   205,   209,   271,   206,   443,   449,   451,   293,
     431,   443,   204,   234,   388,   389,   207,   208,   229,   253,
     362,   363,   364,   365,   366,   273,   273,   229,    67,    67,
     204,   328,   204,   204,   225,   273,   273,   204,   288,   226,
     277,   331,   333,   334,   335,   336,   337,   210,   331,   350,
     351,   352,   359,   210,   348,    51,   206,   367,    39,   203,
     226,   226,   273,   226,   273,   275,   203,   210,   205,   226,
      68,   116,   117,   139,   141,   284,   284,   367,   331,   331,
     331,   331,   359,   359,   331,   331,   331,   418,   203,   203,
     203,   203,   203,   203,   433,   408,   409,   356,   204,   450,
     452,   293,   432,   331,   331,   293,   331,   331,   408,    51,
     358,   360,   360,   360,   244,   253,   273,   273,    11,   238,
     239,   360,   363,   364,   272,   271,   204,   204,   204,   431,
     204,   206,   390,   253,   273,   206,   205,   211,   207,   208,
     225,   362,   204,   204,   204,   204,   204,   312,   209,   209,
     361,   226,   290,   292,   211,   335,   205,   226,   205,   226,
     333,   211,   205,   331,   350,   331,   274,   367,   203,   368,
     374,   376,   204,   204,   204,   388,    68,   382,   211,   371,
     203,   371,   371,   331,   331,   255,   270,   255,   270,   204,
     255,   269,   434,   435,   436,   437,   204,   437,   204,   204,
     432,   442,   204,   204,   204,    40,   204,   207,   208,   211,
     211,   206,   204,   205,   206,   205,   211,   443,   443,   208,
     234,   391,   392,   206,   393,    51,   209,   362,   211,   365,
     253,   273,   362,   204,   205,   206,   338,   341,   418,   339,
     341,   427,   331,   339,   338,   211,   331,   352,   225,   353,
     211,   206,   273,   275,   367,   367,   438,   439,   204,   211,
     273,   205,   204,   205,   204,   206,   205,   226,   204,   204,
     203,   445,   253,   253,   273,   272,   239,   272,   211,   359,
     203,   205,   208,   234,   394,   395,   206,   396,   273,    51,
     209,   312,   274,   331,   340,   341,   331,   340,   321,   331,
     274,   367,   204,   226,    14,   143,   246,   247,   248,   226,
     204,   255,   255,   255,   434,   255,   435,   209,   209,   273,
     391,   359,   203,   205,   234,   397,   398,   206,   399,   209,
     273,   225,   331,   331,   321,   439,   275,   367,   203,   246,
     144,   250,   371,   204,   204,   234,   204,   209,   273,   394,
     205,   382,   400,   401,   209,   312,   248,   204,   270,   203,
     367,   203,   234,   204,   397,   205,   204,   250,   439,   204,
     249,   270,   273,   203,   401,   226,   248,   204,   205,   204,
     273,   250,   270,   204,   367
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   227,   228,   229,   229,   230,   231,   232,   233,   234,
     235,   235,   235,   235,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   237,   238,   238,   239,   239,   240,
     240,   241,   242,   243,   244,   244,   244,   244,   245,   245,
     246,   247,   247,   248,   248,   249,   249,   250,   250,   251,
     252,   252,   252,   252,   252,   252,   252,   252,   252,   252,
     253,   253,   254,   254,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   255,   255,   255,   256,   256,
     256,   256,   257,   257,   257,   257,   258,   258,   258,   259,
     259,   259,   260,   260,   260,   260,   260,   261,   261,   261,
     262,   262,   263,   263,   264,   264,   265,   265,   266,   266,
     267,   267,   268,   268,   269,   269,   270,   270,   270,   270,
     271,   271,   271,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   273,   273,   274,   275,   275,
     276,   276,   276,   276,   276,   277,   279,   278,   280,   278,
     281,   278,   282,   282,   283,   283,   284,   284,   286,   285,
     287,   285,   285,   288,   285,   289,   289,   289,   289,   289,
     290,   290,   290,   290,   290,   291,   291,   291,   291,   291,
     292,   292,   292,   293,   293,   294,   294,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   296,   296,   297,   297,
     298,   298,   298,   299,   299,   300,   300,   300,   300,   301,
     301,   301,   301,   302,   302,   302,   303,   303,   303,   304,
     304,   304,   305,   305,   305,   306,   306,   306,   307,   307,
     307,   308,   308,   309,   309,   309,   309,   310,   311,   311,
     311,   311,   312,   312,   312,   313,   313,   313,   313,   313,
     313,   314,   314,   315,   315,   316,   316,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   319,   319,
     319,   320,   321,   323,   322,   324,   322,   325,   322,   326,
     326,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   328,   328,   328,   329,   329,
     330,   330,   331,   331,   332,   332,   333,   333,   334,   334,
     335,   335,   335,   335,   336,   336,   337,   337,   338,   338,
     338,   339,   339,   340,   340,   341,   343,   342,   344,   342,
     345,   345,   346,   346,   347,   347,   348,   348,   349,   350,
     350,   351,   351,   351,   352,   353,   353,   354,   354,   355,
     355,   356,   357,   357,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   359,   359,
     360,   360,   360,   360,   361,   361,   362,   362,   362,   363,
     363,   364,   364,   365,   365,   365,   365,   366,   366,   366,
     366,   366,   366,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   368,   369,   369,   369,
     369,   370,   371,   371,   371,   372,   373,   373,   374,   375,
     375,   375,   376,   376,   377,   377,   378,   377,   379,   379,
     379,   379,   379,   379,   380,   381,   381,   382,   383,   383,
     384,   384,   385,   385,   385,   386,   386,   386,   387,   387,
     387,   387,   388,   388,   388,   388,   388,   389,   390,   390,
     391,   391,   392,   392,   393,   393,   394,   394,   395,   395,
     396,   396,   397,   398,   398,   399,   399,   400,   400,   401,
     402,   402,   403,   403,   404,   404,   404,   404,   405,   405,
     406,   407,   408,   408,   409,   409,   410,   411,   411,   411,
     412,   412,   412,   412,   413,   413,   413,   414,   414,   415,
     415,   415,   416,   416,   416,   417,   417,   417,   417,   417,
     418,   418,   418,   419,   419,   419,   420,   420,   421,   421,
     421,   422,   422,   422,   423,   423,   424,   424,   424,   424,
     424,   425,   425,   425,   426,   426,   427,   427,   428,   428,
     428,   428,   429,   429,   429,   430,   430,   431,   431,   431,
     432,   432,   433,   433,   433,   433,   433,   433,   434,   434,
     435,   435,   436,   436,   437,   437,   438,   438,   439,   439,
     440,   440,   441,   441,   442,   442,   443,   443,   444,   443,
     445,   446,   445,   447,   445,   448,   448,   448,   448,   448,
     448,   448,   449,   449,   449,   449,   449,   450,   450,   450,
     450,   450,   451,   451,   451,   451,   451,   452,   452,   452,
     452
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     6,     1,     3,     3,     3,     6,
       6,     6,     4,     6,     1,     3,     4,     3,     6,     6,
       4,     1,     2,     0,     1,     1,     3,     0,     4,     3,
       1,     4,     3,     4,     3,     3,     2,     2,     6,     7,
       1,     1,     1,     3,     1,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     4,     2,     4,     1,     4,
       2,     2,     1,     3,     3,     3,     1,     3,     3,     1,
       3,     3,     1,     3,     3,     3,     3,     1,     3,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     4,     4,     4,
       1,     5,     4,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     3,     1,     0,     1,
       2,     2,     2,     2,     2,     6,     0,     4,     0,     4,
       0,     5,     5,     1,     2,     1,     0,     1,     0,     5,
       0,     5,     5,     0,     7,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     2,     2,
       1,     2,     2,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     1,     4,     4,     1,     1,
       1,     1,     1,     1,     2,     3,     3,     3,     3,     2,
       3,     2,     3,     2,     3,     3,     1,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     3,
       3,     4,     4,     1,     2,     3,     2,     4,     1,     2,
       3,     2,     1,     1,     1,     1,     4,     4,     6,    10,
       1,     1,     2,     4,     3,     0,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     7,     7,     1,     1,     1,
       1,     4,     0,     0,     9,     0,    10,     0,     6,     1,
       1,     1,     1,     4,     1,     4,     4,     1,     1,     4,
       4,     1,     1,     1,     1,     0,     1,     1,     1,     3,
       6,     1,     0,     1,     1,     2,     0,     1,     1,     2,
       2,     2,     1,     2,     3,     3,     3,     3,     3,     0,
       2,     3,     2,     0,     1,     2,     0,     8,     0,     7,
       1,     2,     1,     1,     0,     2,     0,     3,     1,     0,
       1,     1,     3,     2,     3,     0,     2,     1,     3,     1,
       3,     1,     1,     3,     1,     2,     3,     2,     1,     2,
       3,     1,     2,     3,     2,     1,     2,     3,     1,     1,
       2,     3,     2,     3,     0,     2,     1,     3,     4,     1,
       3,     1,     0,     1,     3,     2,     3,     2,     3,     5,
       4,     6,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     4,     6,
       3,     2,     3,     4,     4,     0,     1,     2,     2,     5,
       7,     5,     1,     1,     8,    10,     0,    12,     3,     3,
       2,     2,     2,     3,     3,     1,     3,     1,     6,     5,
       4,     2,     7,     4,     1,     2,     4,     4,     0,     2,
       2,     2,     1,     2,     3,     4,     5,     1,     2,     1,
       4,     7,     1,     3,     2,     1,     4,     7,     1,     3,
       2,     1,     1,     1,     3,     2,     1,     1,     3,     1,
       0,     1,     1,     2,     1,     1,     1,     1,     5,     3,
       2,     1,     0,     1,     1,     2,     2,     1,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     4,     4,     1,
       1,     1,     2,     2,     3,     1,     3,     3,     2,     2,
       1,     1,     1,     4,     5,     2,     1,     1,     1,     2,
       1,     1,     2,     3,     3,     4,     1,     4,     5,     2,
       3,     3,     4,     4,     1,     3,     1,     1,     1,     2,
       2,     3,     2,     3,     4,     1,     3,     1,     1,     1,
       1,     1,     4,     4,     6,     6,     1,     1,     1,     3,
       1,     3,     1,     3,     2,     1,     4,     3,     0,     1,
       4,     3,     1,     2,     0,     1,     1,     3,     0,     5,
       1,     0,     4,     0,     6,     2,     3,     3,     3,     4,
       4,     4,     1,     2,     2,     3,     1,     1,     2,     2,
       3,     1,     3,     3,     3,     4,     4,     3,     3,     1,
       4
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 4:
#line 300 "parser.y"
        {
          // construct an identifier from a string that would otherwise not be a
          // valid identifier in C
          yyval=yyvsp[-1];
          parser_stack(yyval).id(ID_symbol);
          irep_idt value=parser_stack(yyvsp[0]).get(ID_value);
          parser_stack(yyval).set(ID_C_base_name, value);
          parser_stack(yyval).set(ID_identifier, value);
          parser_stack(yyval).set(
            ID_C_id_class, static_cast<int>(ansi_c_id_classt::ANSI_C_SYMBOL));
        }
#line 3929 "ansi_c_y.tab.cpp"
    break;

  case 16:
#line 347 "parser.y"
        { yyval = yyvsp[-1]; }
#line 3935 "ansi_c_y.tab.cpp"
    break;

  case 24:
#line 359 "parser.y"
        {
          yyval=yyvsp[-5];
          set(yyval, ID_generic_selection);
          mto(yyval, yyvsp[-3]);
          parser_stack(yyval).add(ID_generic_associations).get_sub().swap((irept::subt&)parser_stack(yyvsp[-1]).operands());
        }
#line 3946 "ansi_c_y.tab.cpp"
    break;

  case 25:
#line 369 "parser.y"
        {
          init(yyval); mto(yyval, yyvsp[0]);
        }
#line 3954 "ansi_c_y.tab.cpp"
    break;

  case 26:
#line 373 "parser.y"
        {
          yyval=yyvsp[-2]; mto(yyval, yyvsp[0]);
        }
#line 3962 "ansi_c_y.tab.cpp"
    break;

  case 27:
#line 380 "parser.y"
        {
          yyval=yyvsp[-1];
          parser_stack(yyval).id(ID_generic_association);
          parser_stack(yyval).set(ID_type_arg, parser_stack(yyvsp[-2]));
          parser_stack(yyval).set(ID_value, parser_stack(yyvsp[0]));
        }
#line 3973 "ansi_c_y.tab.cpp"
    break;

  case 28:
#line 387 "parser.y"
        {
          yyval=yyvsp[-1];
          parser_stack(yyval).id(ID_generic_association);
          parser_stack(yyval).set(ID_type_arg, irept(ID_default));
          parser_stack(yyval).set(ID_value, parser_stack(yyvsp[0]));
        }
#line 3984 "ansi_c_y.tab.cpp"
    break;

  case 29:
#line 397 "parser.y"
        {
          yyval=yyvsp[-5];
          parser_stack(yyval).id(ID_gcc_builtin_va_arg);
          mto(yyval, yyvsp[-3]);
          parser_stack(yyval).type().swap(parser_stack(yyvsp[-1]));
        }
#line 3995 "ansi_c_y.tab.cpp"
    break;

  case 30:
#line 405 "parser.y"
        {
          yyval=yyvsp[-5];
          parser_stack(yyval).id(ID_gcc_builtin_types_compatible_p);
          auto &type_arg=static_cast<type_with_subtypest &>(parser_stack(yyval).add(ID_type_arg));
          auto &subtypes=type_arg.subtypes();
          subtypes.resize(2);
          subtypes[0].swap(parser_stack(yyvsp[-3]));
          subtypes[1].swap(parser_stack(yyvsp[-1]));
        }
#line 4009 "ansi_c_y.tab.cpp"
    break;

  case 31:
#line 418 "parser.y"
        {
          yyval=yyvsp[-5];
          parser_stack(yyval).id(ID_clang_builtin_convertvector);
          mto(yyval, yyvsp[-3]);
          parser_stack(yyval).type().swap(parser_stack(yyvsp[-1]));
        }
#line 4020 "ansi_c_y.tab.cpp"
    break;

  case 32:
#line 428 "parser.y"
        {
          yyval=yyvsp[-3];
          parser_stack(yyval).id(ID_cw_va_arg_typeof);
          parser_stack(yyval).add(ID_type_arg).swap(parser_stack(yyvsp[-1]));
        }
#line 4030 "ansi_c_y.tab.cpp"
    break;

  case 33:
#line 437 "parser.y"
        {
          yyval=yyvsp[-5];
          parser_stack(yyval).id(ID_builtin_offsetof);
          parser_stack(yyval).add(ID_type_arg).swap(parser_stack(yyvsp[-3]));
          parser_stack(yyval).add(ID_designator).swap(parser_stack(yyvsp[-1]));
        }
#line 4041 "ansi_c_y.tab.cpp"
    break;

  case 34:
#line 447 "parser.y"
        {
          init(yyval);
          exprt op{ID_member};
          op.add_source_location()=parser_stack(yyvsp[0]).source_location();
          op.set(ID_component_name, parser_stack(yyvsp[0]).get(ID_C_base_name));
          parser_stack(yyval).add_to_operands(std::move(op));
        }
#line 4053 "ansi_c_y.tab.cpp"
    break;

  case 35:
#line 455 "parser.y"
        {
          yyval=yyvsp[-2];
          set(yyvsp[-1], ID_member);
          parser_stack(yyvsp[-1]).set(ID_component_name, parser_stack(yyvsp[0]).get(ID_C_base_name));
          mto(yyval, yyvsp[-1]);
        }
#line 4064 "ansi_c_y.tab.cpp"
    break;

  case 36:
#line 462 "parser.y"
        {
          yyval=yyvsp[-3];
          set(yyvsp[-2], ID_index);
          mto(yyvsp[-2], yyvsp[-1]);
          mto(yyval, yyvsp[-2]);
        }
#line 4075 "ansi_c_y.tab.cpp"
    break;

  case 37:
#line 469 "parser.y"
        {
          yyval=yyvsp[-2];
          set(yyvsp[-1], ID_index);
          parser_stack(yyvsp[-1]).add_to_operands(convert_integer_literal("0"));
          mto(yyval, yyvsp[-1]);
          set(yyvsp[-1], ID_member);
          parser_stack(yyvsp[-1]).set(ID_component_name, parser_stack(yyvsp[0]).get(ID_C_base_name));
          mto(yyval, yyvsp[-1]);
        }
#line 4089 "ansi_c_y.tab.cpp"
    break;

  case 38:
#line 482 "parser.y"
        {
          yyval=yyvsp[-5];
          set(yyval, ID_forall);
          parser_stack(yyval).add_to_operands(tuple_exprt( { std::move(parser_stack(yyvsp[-2])) } ));
          mto(yyval, yyvsp[-1]);
          PARSER.pop_scope();
        }
#line 4101 "ansi_c_y.tab.cpp"
    break;

  case 39:
#line 490 "parser.y"
        {
          yyval=yyvsp[-5];
          set(yyval, ID_exists);
          parser_stack(yyval).add_to_operands(tuple_exprt( { std::move(parser_stack(yyvsp[-2])) } ));
          mto(yyval, yyvsp[-1]);
          PARSER.pop_scope();
        }
#line 4113 "ansi_c_y.tab.cpp"
    break;

  case 40:
#line 501 "parser.y"
        { yyval=yyvsp[-1]; }
#line 4119 "ansi_c_y.tab.cpp"
    break;

  case 41:
#line 506 "parser.y"
        { init(yyval); mto(yyval, yyvsp[0]); }
#line 4125 "ansi_c_y.tab.cpp"
    break;

  case 42:
#line 508 "parser.y"
        { yyval=yyvsp[-1]; mto(yyval, yyvsp[0]); }
#line 4131 "ansi_c_y.tab.cpp"
    break;

  case 43:
#line 513 "parser.y"
        { init(yyval); parser_stack(yyval).make_nil(); }
#line 4137 "ansi_c_y.tab.cpp"
    break;

  case 45:
#line 519 "parser.y"
        { init(yyval); mto(yyval, yyvsp[0]); }
#line 4143 "ansi_c_y.tab.cpp"
    break;

  case 46:
#line 521 "parser.y"
        { yyval=yyvsp[-2]; mto(yyval, yyvsp[0]); }
#line 4149 "ansi_c_y.tab.cpp"
    break;

  case 47:
#line 526 "parser.y"
        { init(yyval); parser_stack(yyval).make_nil(); }
#line 4155 "ansi_c_y.tab.cpp"
    break;

  case 48:
#line 528 "parser.y"
        { yyval=yyvsp[-1]; }
#line 4161 "ansi_c_y.tab.cpp"
    break;

  case 49:
#line 532 "parser.y"
        { 
          yyval=yyvsp[-2];
          set(yyval, ID_side_effect);
          parser_stack(yyval).set(ID_statement, ID_statement_expression);
          mto(yyval, yyvsp[-1]);
        }
#line 4172 "ansi_c_y.tab.cpp"
    break;

  case 51:
#line 543 "parser.y"
        { binary(yyval, yyvsp[-3], yyvsp[-2], ID_index, yyvsp[-1]); }
#line 4178 "ansi_c_y.tab.cpp"
    break;

  case 52:
#line 545 "parser.y"
        { yyval=yyvsp[-1];
          set(yyval, ID_side_effect);
          auto &side_effect = to_side_effect_expr(parser_stack(yyval));
          side_effect.set_statement(ID_function_call);
          side_effect.operands().resize(2);
          to_binary_expr(side_effect).op0().swap(parser_stack(yyvsp[-2]));
          to_binary_expr(side_effect).op1().clear();
          to_binary_expr(side_effect).op1().id(ID_arguments);
        }
#line 4192 "ansi_c_y.tab.cpp"
    break;

  case 53:
#line 555 "parser.y"
        { yyval=yyvsp[-2];
          set(yyval, ID_side_effect);
          auto &side_effect = to_side_effect_expr(parser_stack(yyval));
          side_effect.set_statement(ID_function_call);
          side_effect.operands().resize(2);
          to_binary_expr(side_effect).op0().swap(parser_stack(yyvsp[-3]));
          to_binary_expr(side_effect).op1().swap(parser_stack(yyvsp[-1]));
          to_binary_expr(side_effect).op1().id(ID_arguments);
        }
#line 4206 "ansi_c_y.tab.cpp"
    break;

  case 54:
#line 565 "parser.y"
        { yyval=yyvsp[-1];
          set(yyval, ID_member);
          mto(yyval, yyvsp[-2]);
          parser_stack(yyval).set(ID_component_name, parser_stack(yyvsp[0]).get(ID_C_base_name));
        }
#line 4216 "ansi_c_y.tab.cpp"
    break;

  case 55:
#line 571 "parser.y"
        { yyval=yyvsp[-1];
          set(yyval, ID_ptrmember);
          mto(yyval, yyvsp[-2]);
          parser_stack(yyval).set(ID_component_name, parser_stack(yyvsp[0]).get(ID_C_base_name));
        }
#line 4226 "ansi_c_y.tab.cpp"
    break;

  case 56:
#line 577 "parser.y"
        { yyval=yyvsp[0];
          set(yyval, ID_side_effect);
          parser_stack(yyval).set(ID_statement, ID_postincrement);
          mto(yyval, yyvsp[-1]);
        }
#line 4236 "ansi_c_y.tab.cpp"
    break;

  case 57:
#line 583 "parser.y"
        { yyval=yyvsp[0];
          set(yyval, ID_side_effect);
          parser_stack(yyval).set(ID_statement, ID_postdecrement);
          mto(yyval, yyvsp[-1]);
        }
#line 4246 "ansi_c_y.tab.cpp"
    break;

  case 58:
#line 590 "parser.y"
        {
          exprt tmp(ID_initializer_list);
          tmp.add_source_location()=parser_stack(yyvsp[-2]).source_location();
          tmp.operands().swap(parser_stack(yyvsp[-1]).operands());
          yyval=yyvsp[-5];
          set(yyval, ID_typecast);
          parser_stack(yyval).add_to_operands(std::move(tmp));
          parser_stack(yyval).type().swap(parser_stack(yyvsp[-4]));
        }
#line 4260 "ansi_c_y.tab.cpp"
    break;

  case 59:
#line 600 "parser.y"
        {
          // same as above
          exprt tmp(ID_initializer_list);
          tmp.add_source_location()=parser_stack(yyvsp[-3]).source_location();
          tmp.operands().swap(parser_stack(yyvsp[-2]).operands());
          yyval=yyvsp[-6];
          set(yyval, ID_typecast);
          parser_stack(yyval).add_to_operands(std::move(tmp));
          parser_stack(yyval).type().swap(parser_stack(yyvsp[-5]));
        }
#line 4275 "ansi_c_y.tab.cpp"
    break;

  case 62:
#line 619 "parser.y"
        {
          init(yyval, ID_expression_list);
          mto(yyval, yyvsp[0]);
        }
#line 4284 "ansi_c_y.tab.cpp"
    break;

  case 63:
#line 624 "parser.y"
        {
          yyval=yyvsp[-2];
          mto(yyval, yyvsp[0]);
        }
#line 4293 "ansi_c_y.tab.cpp"
    break;

  case 65:
#line 633 "parser.y"
        { yyval=yyvsp[-1];
          set(yyval, ID_side_effect);
          parser_stack(yyval).set(ID_statement, ID_preincrement);
          mto(yyval, yyvsp[0]);
        }
#line 4303 "ansi_c_y.tab.cpp"
    break;

  case 66:
#line 639 "parser.y"
        { yyval=yyvsp[-1];
          set(yyval, ID_side_effect);
          parser_stack(yyval).set(ID_statement, ID_predecrement);
          mto(yyval, yyvsp[0]);
        }
#line 4313 "ansi_c_y.tab.cpp"
    break;

  case 67:
#line 645 "parser.y"
        { yyval=yyvsp[-1];
          set(yyval, ID_address_of);
          mto(yyval, yyvsp[0]);
        }
#line 4322 "ansi_c_y.tab.cpp"
    break;

  case 68:
#line 650 "parser.y"
        { // this takes the address of a label (a gcc extension)
          yyval=yyvsp[-1];
          irep_idt identifier=PARSER.lookup_label(parser_stack(yyvsp[0]).get(ID_C_base_name));
          set(yyval, ID_address_of);
          parser_stack(yyval).operands().resize(1);
          auto &op = to_unary_expr(parser_stack(yyval)).op();
          op=parser_stack(yyvsp[0]);
          op.id(ID_label);
          op.set(ID_identifier, identifier);
        }
#line 4337 "ansi_c_y.tab.cpp"
    break;

  case 69:
#line 661 "parser.y"
        { yyval=yyvsp[-1];
          set(yyval, ID_dereference);
          mto(yyval, yyvsp[0]);
        }
#line 4346 "ansi_c_y.tab.cpp"
    break;

  case 70:
#line 666 "parser.y"
        { yyval=yyvsp[-1];
          set(yyval, ID_unary_plus);
          mto(yyval, yyvsp[0]);
        }
#line 4355 "ansi_c_y.tab.cpp"
    break;

  case 71:
#line 671 "parser.y"
        { yyval=yyvsp[-1];
          set(yyval, ID_unary_minus);
          mto(yyval, yyvsp[0]);
        }
#line 4364 "ansi_c_y.tab.cpp"
    break;

  case 72:
#line 676 "parser.y"
        { yyval=yyvsp[-1];
          set(yyval, ID_bitnot);
          mto(yyval, yyvsp[0]);
        }
#line 4373 "ansi_c_y.tab.cpp"
    break;

  case 73:
#line 681 "parser.y"
        { yyval=yyvsp[-1];
          set(yyval, ID_not);
          mto(yyval, yyvsp[0]);
        }
#line 4382 "ansi_c_y.tab.cpp"
    break;

  case 74:
#line 686 "parser.y"
        { yyval=yyvsp[-1];
          set(yyval, ID_sizeof);
          mto(yyval, yyvsp[0]);
        }
#line 4391 "ansi_c_y.tab.cpp"
    break;

  case 75:
#line 691 "parser.y"
        { yyval=yyvsp[-3];
          set(yyval, ID_sizeof);
          parser_stack(yyval).add(ID_type_arg).swap(parser_stack(yyvsp[-1]));
        }
#line 4400 "ansi_c_y.tab.cpp"
    break;

  case 76:
#line 696 "parser.y"
        { // note no parentheses for expressions, just like sizeof
          yyval=yyvsp[-1];
          set(yyval, ID_alignof);
          mto(yyval, yyvsp[0]);
        }
#line 4410 "ansi_c_y.tab.cpp"
    break;

  case 77:
#line 702 "parser.y"
        {
          yyval=yyvsp[-3];
          parser_stack(yyval).id(ID_alignof);
          parser_stack(yyval).add(ID_type_arg).swap(parser_stack(yyvsp[-1]));
        }
#line 4420 "ansi_c_y.tab.cpp"
    break;

  case 79:
#line 712 "parser.y"
        {
          yyval=yyvsp[-3];
          set(yyval, ID_typecast);
          mto(yyval, yyvsp[0]);
          parser_stack(yyval).type().swap(parser_stack(yyvsp[-2]));
        }
#line 4431 "ansi_c_y.tab.cpp"
    break;

  case 80:
#line 719 "parser.y"
        { yyval=yyvsp[-1];
          set(yyval, ID_complex_real);
          mto(yyval, yyvsp[0]);
        }
#line 4440 "ansi_c_y.tab.cpp"
    break;

  case 81:
#line 724 "parser.y"
        { yyval=yyvsp[-1];
          set(yyval, ID_complex_imag);
          mto(yyval, yyvsp[0]);
        }
#line 4449 "ansi_c_y.tab.cpp"
    break;

  case 83:
#line 733 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_mult, yyvsp[0]); }
#line 4455 "ansi_c_y.tab.cpp"
    break;

  case 84:
#line 735 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_div, yyvsp[0]); }
#line 4461 "ansi_c_y.tab.cpp"
    break;

  case 85:
#line 737 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_mod, yyvsp[0]); }
#line 4467 "ansi_c_y.tab.cpp"
    break;

  case 87:
#line 743 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_plus, yyvsp[0]); }
#line 4473 "ansi_c_y.tab.cpp"
    break;

  case 88:
#line 745 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_minus, yyvsp[0]); }
#line 4479 "ansi_c_y.tab.cpp"
    break;

  case 90:
#line 751 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_shl, yyvsp[0]); }
#line 4485 "ansi_c_y.tab.cpp"
    break;

  case 91:
#line 753 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_shr, yyvsp[0]); }
#line 4491 "ansi_c_y.tab.cpp"
    break;

  case 93:
#line 759 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_lt, yyvsp[0]); }
#line 4497 "ansi_c_y.tab.cpp"
    break;

  case 94:
#line 761 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_gt, yyvsp[0]); }
#line 4503 "ansi_c_y.tab.cpp"
    break;

  case 95:
#line 763 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_le, yyvsp[0]); }
#line 4509 "ansi_c_y.tab.cpp"
    break;

  case 96:
#line 765 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_ge, yyvsp[0]); }
#line 4515 "ansi_c_y.tab.cpp"
    break;

  case 98:
#line 771 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_equal, yyvsp[0]); }
#line 4521 "ansi_c_y.tab.cpp"
    break;

  case 99:
#line 773 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_notequal, yyvsp[0]); }
#line 4527 "ansi_c_y.tab.cpp"
    break;

  case 101:
#line 779 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_bitand, yyvsp[0]); }
#line 4533 "ansi_c_y.tab.cpp"
    break;

  case 103:
#line 785 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_bitxor, yyvsp[0]); }
#line 4539 "ansi_c_y.tab.cpp"
    break;

  case 105:
#line 791 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_bitor, yyvsp[0]); }
#line 4545 "ansi_c_y.tab.cpp"
    break;

  case 107:
#line 797 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_and, yyvsp[0]); }
#line 4551 "ansi_c_y.tab.cpp"
    break;

  case 109:
#line 803 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_xor, yyvsp[0]); }
#line 4557 "ansi_c_y.tab.cpp"
    break;

  case 111:
#line 809 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_or, yyvsp[0]); }
#line 4563 "ansi_c_y.tab.cpp"
    break;

  case 113:
#line 818 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_implies, yyvsp[0]); }
#line 4569 "ansi_c_y.tab.cpp"
    break;

  case 115:
#line 827 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_equal, yyvsp[0]); }
#line 4575 "ansi_c_y.tab.cpp"
    break;

  case 117:
#line 834 "parser.y"
        {
          yyval=yyvsp[-3];
          set(yyval, ID_forall);
          parser_stack(yyval).add_to_operands(tuple_exprt( { std::move(parser_stack(yyvsp[-1])) } ));
          mto(yyval, yyvsp[0]);
          PARSER.pop_scope();
        }
#line 4587 "ansi_c_y.tab.cpp"
    break;

  case 118:
#line 842 "parser.y"
        {
          yyval=yyvsp[-3];
          set(yyval, ID_exists);
          parser_stack(yyval).add_to_operands(tuple_exprt( { std::move(parser_stack(yyvsp[-1])) } ));
          mto(yyval, yyvsp[0]);
          PARSER.pop_scope();
        }
#line 4599 "ansi_c_y.tab.cpp"
    break;

  case 119:
#line 850 "parser.y"
        {
          yyval=yyvsp[-3];
          set(yyval, ID_lambda);
          parser_stack(yyval).add_to_operands(tuple_exprt( { std::move(parser_stack(yyvsp[-1])) } ));
          mto(yyval, yyvsp[0]);
          PARSER.pop_scope();
        }
#line 4611 "ansi_c_y.tab.cpp"
    break;

  case 121:
#line 862 "parser.y"
        { yyval=yyvsp[-3];
          parser_stack(yyval).id(ID_if);
          mto(yyval, yyvsp[-4]);
          mto(yyval, yyvsp[-2]);
          mto(yyval, yyvsp[0]);
        }
#line 4622 "ansi_c_y.tab.cpp"
    break;

  case 122:
#line 869 "parser.y"
        { yyval=yyvsp[-2];
          parser_stack(yyval).id(ID_side_effect);
          parser_stack(yyval).set(ID_statement, ID_gcc_conditional_expression);
          mto(yyval, yyvsp[-3]);
          mto(yyval, yyvsp[0]);
        }
#line 4633 "ansi_c_y.tab.cpp"
    break;

  case 124:
#line 880 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_side_effect, yyvsp[0]); parser_stack(yyval).set(ID_statement, ID_assign); }
#line 4639 "ansi_c_y.tab.cpp"
    break;

  case 125:
#line 882 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_side_effect, yyvsp[0]); parser_stack(yyval).set(ID_statement, ID_assign_mult); }
#line 4645 "ansi_c_y.tab.cpp"
    break;

  case 126:
#line 884 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_side_effect, yyvsp[0]); parser_stack(yyval).set(ID_statement, ID_assign_div); }
#line 4651 "ansi_c_y.tab.cpp"
    break;

  case 127:
#line 886 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_side_effect, yyvsp[0]); parser_stack(yyval).set(ID_statement, ID_assign_mod); }
#line 4657 "ansi_c_y.tab.cpp"
    break;

  case 128:
#line 888 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_side_effect, yyvsp[0]); parser_stack(yyval).set(ID_statement, ID_assign_plus); }
#line 4663 "ansi_c_y.tab.cpp"
    break;

  case 129:
#line 890 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_side_effect, yyvsp[0]); parser_stack(yyval).set(ID_statement, ID_assign_minus); }
#line 4669 "ansi_c_y.tab.cpp"
    break;

  case 130:
#line 892 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_side_effect, yyvsp[0]); parser_stack(yyval).set(ID_statement, ID_assign_shl); }
#line 4675 "ansi_c_y.tab.cpp"
    break;

  case 131:
#line 894 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_side_effect, yyvsp[0]); parser_stack(yyval).set(ID_statement, ID_assign_shr); }
#line 4681 "ansi_c_y.tab.cpp"
    break;

  case 132:
#line 896 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_side_effect, yyvsp[0]); parser_stack(yyval).set(ID_statement, ID_assign_bitand); }
#line 4687 "ansi_c_y.tab.cpp"
    break;

  case 133:
#line 898 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_side_effect, yyvsp[0]); parser_stack(yyval).set(ID_statement, ID_assign_bitxor); }
#line 4693 "ansi_c_y.tab.cpp"
    break;

  case 134:
#line 900 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_side_effect, yyvsp[0]); parser_stack(yyval).set(ID_statement, ID_assign_bitor); }
#line 4699 "ansi_c_y.tab.cpp"
    break;

  case 136:
#line 906 "parser.y"
        { binary(yyval, yyvsp[-2], yyvsp[-1], ID_comma, yyvsp[0]); }
#line 4705 "ansi_c_y.tab.cpp"
    break;

  case 138:
#line 915 "parser.y"
        { init(yyval); parser_stack(yyval).make_nil(); }
#line 4711 "ansi_c_y.tab.cpp"
    break;

  case 140:
#line 923 "parser.y"
        {
          // type only, no declarator!
          init(yyval, ID_declaration);
          parser_stack(yyval).type().swap(parser_stack(yyvsp[-1]));
        }
#line 4721 "ansi_c_y.tab.cpp"
    break;

  case 141:
#line 929 "parser.y"
        {
          // type only, no identifier!
          init(yyval, ID_declaration);
          parser_stack(yyval).type().swap(parser_stack(yyvsp[-1]));
        }
#line 4731 "ansi_c_y.tab.cpp"
    break;

  case 145:
#line 941 "parser.y"
        {
          yyval=yyvsp[-5];
          set(yyval, ID_declaration);
          to_ansi_c_declaration(parser_stack(yyval)).set_is_static_assert(true);
          mto(yyval, yyvsp[-3]);
          mto(yyval, yyvsp[-1]);
        }
#line 4743 "ansi_c_y.tab.cpp"
    break;

  case 146:
#line 952 "parser.y"
          {
            init(yyval, ID_declaration);
            parser_stack(yyval).type().swap(parser_stack(yyvsp[-1]));
            PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[0]));
          }
#line 4753 "ansi_c_y.tab.cpp"
    break;

  case 147:
#line 958 "parser.y"
        {
          // patch on the initializer
          yyval=yyvsp[-1];
          to_ansi_c_declaration(parser_stack(yyval)).add_initializer(parser_stack(yyvsp[0]));
        }
#line 4763 "ansi_c_y.tab.cpp"
    break;

  case 148:
#line 964 "parser.y"
          {
            init(yyval, ID_declaration);
            parser_stack(yyval).type().swap(parser_stack(yyvsp[-1]));
            PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[0]));
          }
#line 4773 "ansi_c_y.tab.cpp"
    break;

  case 149:
#line 970 "parser.y"
        {
          // patch on the initializer
          yyval=yyvsp[-1];
          to_ansi_c_declaration(parser_stack(yyval)).add_initializer(parser_stack(yyvsp[0]));
        }
#line 4783 "ansi_c_y.tab.cpp"
    break;

  case 150:
#line 976 "parser.y"
          {
            // just add the declarator
            PARSER.add_declarator(parser_stack(yyvsp[-2]), parser_stack(yyvsp[0]));
            // Needs to be done before initializer, as we want to see that identifier
            // already there!
          }
#line 4794 "ansi_c_y.tab.cpp"
    break;

  case 151:
#line 983 "parser.y"
        {
          // patch on the initializer
          yyval=yyvsp[-4];
          to_ansi_c_declaration(parser_stack(yyval)).add_initializer(parser_stack(yyvsp[0]));
        }
#line 4804 "ansi_c_y.tab.cpp"
    break;

  case 152:
#line 992 "parser.y"
        {
          yyval=yyvsp[-4];
          parser_stack(yyval).id(ID_asm);
          parser_stack(yyval).set(ID_flavor, ID_gcc);
          parser_stack(yyval).operands().swap(parser_stack(yyvsp[-1]).operands());
        }
#line 4815 "ansi_c_y.tab.cpp"
    break;

  case 154:
#line 1003 "parser.y"
        {
          yyval=merge(yyvsp[-1], yyvsp[0]);
        }
#line 4823 "ansi_c_y.tab.cpp"
    break;

  case 156:
#line 1011 "parser.y"
        {
          init(yyval);
        }
#line 4831 "ansi_c_y.tab.cpp"
    break;

  case 158:
#line 1020 "parser.y"
          {
            yyvsp[-1]=merge(yyvsp[0], yyvsp[-1]); // type attribute
            
            // the symbol has to be visible during initialization
            init(yyval, ID_declaration);
            parser_stack(yyval).type().swap(parser_stack(yyvsp[-2]));
            PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[-1]));
          }
#line 4844 "ansi_c_y.tab.cpp"
    break;

  case 159:
#line 1029 "parser.y"
        {
          // add the initializer
          yyval=yyvsp[-1];
          to_ansi_c_declaration(parser_stack(yyval)).add_initializer(parser_stack(yyvsp[0]));
        }
#line 4854 "ansi_c_y.tab.cpp"
    break;

  case 160:
#line 1036 "parser.y"
          {
            yyvsp[-1]=merge(yyvsp[0], yyvsp[-1]);
            
            // the symbol has to be visible during initialization
            init(yyval, ID_declaration);
            parser_stack(yyval).type().swap(parser_stack(yyvsp[-2]));
            PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[-1]));
          }
#line 4867 "ansi_c_y.tab.cpp"
    break;

  case 161:
#line 1045 "parser.y"
        {
          // add the initializer
          yyval=yyvsp[-1];
          to_ansi_c_declaration(parser_stack(yyval)).add_initializer(parser_stack(yyvsp[0]));
        }
#line 4877 "ansi_c_y.tab.cpp"
    break;

  case 162:
#line 1052 "parser.y"
        {
          // handled as typeof(initializer)
          parser_stack(yyvsp[-4]).id(ID_typeof);
          parser_stack(yyvsp[-4]).copy_to_operands(parser_stack(yyvsp[0]));

          yyvsp[-3]=merge(yyvsp[-2], yyvsp[-3]);

          // the symbol has to be visible during initialization
          init(yyval, ID_declaration);
          parser_stack(yyval).type().swap(parser_stack(yyvsp[-4]));
          PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[-3]));
          // add the initializer
          to_ansi_c_declaration(parser_stack(yyval)).add_initializer(parser_stack(yyvsp[0]));
        }
#line 4896 "ansi_c_y.tab.cpp"
    break;

  case 163:
#line 1068 "parser.y"
          {
            // type attribute goes into declarator
            yyvsp[0]=merge(yyvsp[0], yyvsp[-2]);
            yyvsp[-1]=merge(yyvsp[0], yyvsp[-1]);
            PARSER.add_declarator(parser_stack(yyvsp[-4]), parser_stack(yyvsp[-1]));
          }
#line 4907 "ansi_c_y.tab.cpp"
    break;

  case 164:
#line 1075 "parser.y"
        {
          // add in the initializer
          yyval=yyvsp[-6];
          to_ansi_c_declaration(parser_stack(yyval)).add_initializer(parser_stack(yyvsp[0]));
        }
#line 4917 "ansi_c_y.tab.cpp"
    break;

  case 176:
#line 1101 "parser.y"
        {
          yyval=merge(yyvsp[-1], yyvsp[0]);
        }
#line 4925 "ansi_c_y.tab.cpp"
    break;

  case 178:
#line 1106 "parser.y"
        {
          yyval=merge(yyvsp[-1], yyvsp[0]);
        }
#line 4933 "ansi_c_y.tab.cpp"
    break;

  case 179:
#line 1110 "parser.y"
        {
          yyval=merge(yyvsp[-1], yyvsp[0]);
        }
#line 4941 "ansi_c_y.tab.cpp"
    break;

  case 181:
#line 1118 "parser.y"
        {
          yyval=merge(yyvsp[-1], yyvsp[0]);
        }
#line 4949 "ansi_c_y.tab.cpp"
    break;

  case 182:
#line 1125 "parser.y"
        {
          yyval=merge(yyvsp[-1], yyvsp[0]);
        }
#line 4957 "ansi_c_y.tab.cpp"
    break;

  case 184:
#line 1133 "parser.y"
        {
          yyval=merge(yyvsp[-1], yyvsp[0]);
        }
#line 4965 "ansi_c_y.tab.cpp"
    break;

  case 187:
#line 1144 "parser.y"
                                    { yyval=yyvsp[0]; set(yyval, ID_atomic); }
#line 4971 "ansi_c_y.tab.cpp"
    break;

  case 188:
#line 1145 "parser.y"
                                    { yyval=yyvsp[0]; set(yyval, ID_const); }
#line 4977 "ansi_c_y.tab.cpp"
    break;

  case 189:
#line 1146 "parser.y"
                                    { yyval=yyvsp[0]; set(yyval, ID_restrict); }
#line 4983 "ansi_c_y.tab.cpp"
    break;

  case 190:
#line 1147 "parser.y"
                                    { yyval=yyvsp[0]; set(yyval, ID_volatile); }
#line 4989 "ansi_c_y.tab.cpp"
    break;

  case 191:
#line 1148 "parser.y"
                                    { yyval=yyvsp[0]; set(yyval, ID_cprover_atomic); }
#line 4995 "ansi_c_y.tab.cpp"
    break;

  case 192:
#line 1149 "parser.y"
                                    { yyval=yyvsp[0]; set(yyval, ID_ptr32); }
#line 5001 "ansi_c_y.tab.cpp"
    break;

  case 193:
#line 1150 "parser.y"
                                    { yyval=yyvsp[0]; set(yyval, ID_ptr64); }
#line 5007 "ansi_c_y.tab.cpp"
    break;

  case 194:
#line 1151 "parser.y"
                                                 { yyval=yyvsp[-3]; set(yyval, ID_msc_based); mto(yyval, yyvsp[-1]); }
#line 5013 "ansi_c_y.tab.cpp"
    break;

  case 196:
#line 1157 "parser.y"
        { yyval = yyvsp[-3];
          parser_stack(yyval).id(ID_aligned);
          parser_stack(yyval).set(ID_size, parser_stack(yyvsp[-1]));
        }
#line 5022 "ansi_c_y.tab.cpp"
    break;

  case 197:
#line 1162 "parser.y"
        { yyval = yyvsp[-3];
          parser_stack(yyval).id(ID_aligned);
          parser_stack(yyvsp[-1]).set(ID_type_arg, parser_stack(yyvsp[-1]));
        }
#line 5031 "ansi_c_y.tab.cpp"
    break;

  case 204:
#line 1182 "parser.y"
        {
          yyval=merge(yyvsp[-1], yyvsp[0]);
        }
#line 5039 "ansi_c_y.tab.cpp"
    break;

  case 205:
#line 1189 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 5047 "ansi_c_y.tab.cpp"
    break;

  case 206:
#line 1193 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 5055 "ansi_c_y.tab.cpp"
    break;

  case 207:
#line 1197 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 5063 "ansi_c_y.tab.cpp"
    break;

  case 208:
#line 1201 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 5071 "ansi_c_y.tab.cpp"
    break;

  case 209:
#line 1208 "parser.y"
        {
          yyval=merge(yyvsp[-1], yyvsp[0]); // type attribute
        }
#line 5079 "ansi_c_y.tab.cpp"
    break;

  case 210:
#line 1212 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 5087 "ansi_c_y.tab.cpp"
    break;

  case 211:
#line 1216 "parser.y"
        {
          yyval=merge(yyvsp[-1], yyvsp[0]);
        }
#line 5095 "ansi_c_y.tab.cpp"
    break;

  case 212:
#line 1220 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 5103 "ansi_c_y.tab.cpp"
    break;

  case 213:
#line 1227 "parser.y"
        {
          yyval=merge(yyvsp[-1], yyvsp[0]);
        }
#line 5111 "ansi_c_y.tab.cpp"
    break;

  case 214:
#line 1231 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 5119 "ansi_c_y.tab.cpp"
    break;

  case 215:
#line 1235 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 5127 "ansi_c_y.tab.cpp"
    break;

  case 217:
#line 1243 "parser.y"
        {
          yyval=merge(yyvsp[-1], yyvsp[0]);
        }
#line 5135 "ansi_c_y.tab.cpp"
    break;

  case 218:
#line 1247 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 5143 "ansi_c_y.tab.cpp"
    break;

  case 219:
#line 1254 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 5151 "ansi_c_y.tab.cpp"
    break;

  case 220:
#line 1258 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 5159 "ansi_c_y.tab.cpp"
    break;

  case 221:
#line 1262 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 5167 "ansi_c_y.tab.cpp"
    break;

  case 222:
#line 1269 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 5175 "ansi_c_y.tab.cpp"
    break;

  case 223:
#line 1273 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 5183 "ansi_c_y.tab.cpp"
    break;

  case 224:
#line 1277 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 5191 "ansi_c_y.tab.cpp"
    break;

  case 225:
#line 1284 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 5199 "ansi_c_y.tab.cpp"
    break;

  case 226:
#line 1288 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 5207 "ansi_c_y.tab.cpp"
    break;

  case 227:
#line 1292 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 5215 "ansi_c_y.tab.cpp"
    break;

  case 228:
#line 1299 "parser.y"
        {
          yyval=merge(yyvsp[-1], yyvsp[0]);
        }
#line 5223 "ansi_c_y.tab.cpp"
    break;

  case 229:
#line 1303 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 5231 "ansi_c_y.tab.cpp"
    break;

  case 230:
#line 1307 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 5239 "ansi_c_y.tab.cpp"
    break;

  case 231:
#line 1314 "parser.y"
        { yyval = yyvsp[-3];
          parser_stack(yyval).id(ID_typeof);
          mto(yyval, yyvsp[-1]);
        }
#line 5248 "ansi_c_y.tab.cpp"
    break;

  case 232:
#line 1319 "parser.y"
        { yyval = yyvsp[-3];
          parser_stack(yyval).id(ID_typeof);
          parser_stack(yyval).set(ID_type_arg, parser_stack(yyvsp[-1]));
        }
#line 5257 "ansi_c_y.tab.cpp"
    break;

  case 234:
#line 1328 "parser.y"
        {
          yyval=merge(yyvsp[-1], yyvsp[0]);
        }
#line 5265 "ansi_c_y.tab.cpp"
    break;

  case 235:
#line 1332 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 5273 "ansi_c_y.tab.cpp"
    break;

  case 236:
#line 1336 "parser.y"
        {
          yyval=merge(yyvsp[-1], yyvsp[0]);
        }
#line 5281 "ansi_c_y.tab.cpp"
    break;

  case 237:
#line 1343 "parser.y"
        {
          yyval=yyvsp[-3];
          parser_stack(yyval).id(ID_atomic_type_specifier);
          stack_type(yyval).add_subtype()=stack_type(yyvsp[-1]);
        }
#line 5291 "ansi_c_y.tab.cpp"
    break;

  case 239:
#line 1353 "parser.y"
        {
          yyval=merge(yyvsp[-1], yyvsp[0]);
        }
#line 5299 "ansi_c_y.tab.cpp"
    break;

  case 240:
#line 1357 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 5307 "ansi_c_y.tab.cpp"
    break;

  case 241:
#line 1361 "parser.y"
        {
          yyval=merge(yyvsp[-1], yyvsp[0]);
        }
#line 5315 "ansi_c_y.tab.cpp"
    break;

  case 242:
#line 1368 "parser.y"
        {
          parser_stack(yyval).id(parser_stack(yyval).get(ID_identifier));
        }
#line 5323 "ansi_c_y.tab.cpp"
    break;

  case 243:
#line 1372 "parser.y"
        {
          parser_stack(yyval).id(parser_stack(yyval).get(ID_identifier));
        }
#line 5331 "ansi_c_y.tab.cpp"
    break;

  case 244:
#line 1376 "parser.y"
        {
          parser_stack(yyval).id(ID_restrict);
        }
#line 5339 "ansi_c_y.tab.cpp"
    break;

  case 246:
#line 1384 "parser.y"
        {
          yyval=yyvsp[-3]; mto(yyval, yyvsp[-1]);
        }
#line 5347 "ansi_c_y.tab.cpp"
    break;

  case 247:
#line 1388 "parser.y"
        {
          yyval=yyvsp[-3]; mto(yyval, yyvsp[-1]);
        }
#line 5355 "ansi_c_y.tab.cpp"
    break;

  case 248:
#line 1392 "parser.y"
        {
          yyval=yyvsp[-5]; mto(yyval, yyvsp[-3]); mto(yyval, yyvsp[-1]);
        }
#line 5363 "ansi_c_y.tab.cpp"
    break;

  case 249:
#line 1396 "parser.y"
        {
          yyval=yyvsp[-9]; mto(yyval, yyvsp[-7]); mto(yyval, yyvsp[-5]); mto(yyval, yyvsp[-3]); mto(yyval, yyvsp[-1]);
        }
#line 5371 "ansi_c_y.tab.cpp"
    break;

  case 250:
#line 1399 "parser.y"
              { init(yyval, ID_nil); }
#line 5377 "ansi_c_y.tab.cpp"
    break;

  case 251:
#line 1404 "parser.y"
        {
          init(yyval); mto(yyval, yyvsp[0]);
        }
#line 5385 "ansi_c_y.tab.cpp"
    break;

  case 252:
#line 1408 "parser.y"
        {
          yyval=yyvsp[-1]; mto(yyval, yyvsp[0]);
        }
#line 5393 "ansi_c_y.tab.cpp"
    break;

  case 253:
#line 1415 "parser.y"
        {
          yyval=yyvsp[-3]; set(yyval, ID_msc_declspec);
          parser_stack(yyval).operands().swap(parser_stack(yyvsp[-1]).operands());
        }
#line 5402 "ansi_c_y.tab.cpp"
    break;

  case 254:
#line 1420 "parser.y"
        {
          yyval=yyvsp[-2]; set(yyval, ID_msc_declspec);
        }
#line 5410 "ansi_c_y.tab.cpp"
    break;

  case 255:
#line 1427 "parser.y"
        {
          init(yyval, ID_nil);
        }
#line 5418 "ansi_c_y.tab.cpp"
    break;

  case 256:
#line 1431 "parser.y"
        {
          if(parser_stack(yyvsp[-1]).is_not_nil())
          {
            yyval = yyvsp[-1];
            exprt::operandst &operands = parser_stack(yyvsp[-1]).operands();
            operands.insert(
              operands.end(),
              parser_stack(yyvsp[0]).operands().begin(),
              parser_stack(yyvsp[0]).operands().end());
          }
          else
            yyval = yyvsp[0];
        }
#line 5436 "ansi_c_y.tab.cpp"
    break;

  case 257:
#line 1447 "parser.y"
                           { yyval=yyvsp[0]; set(yyval, ID_typedef); }
#line 5442 "ansi_c_y.tab.cpp"
    break;

  case 258:
#line 1448 "parser.y"
                           { yyval=yyvsp[0]; set(yyval, ID_extern); }
#line 5448 "ansi_c_y.tab.cpp"
    break;

  case 259:
#line 1449 "parser.y"
                           { yyval=yyvsp[0]; set(yyval, ID_static); }
#line 5454 "ansi_c_y.tab.cpp"
    break;

  case 260:
#line 1450 "parser.y"
                           { yyval=yyvsp[0]; set(yyval, ID_auto); }
#line 5460 "ansi_c_y.tab.cpp"
    break;

  case 261:
#line 1451 "parser.y"
                           { yyval=yyvsp[0]; set(yyval, ID_register); }
#line 5466 "ansi_c_y.tab.cpp"
    break;

  case 262:
#line 1452 "parser.y"
                           { yyval=yyvsp[0]; set(yyval, ID_inline); }
#line 5472 "ansi_c_y.tab.cpp"
    break;

  case 263:
#line 1453 "parser.y"
                           { yyval=yyvsp[0]; set(yyval, ID_thread_local); }
#line 5478 "ansi_c_y.tab.cpp"
    break;

  case 264:
#line 1454 "parser.y"
                           { yyval=yyvsp[0]; set(yyval, ID_asm); }
#line 5484 "ansi_c_y.tab.cpp"
    break;

  case 265:
#line 1455 "parser.y"
                           { yyval=yyvsp[0]; }
#line 5490 "ansi_c_y.tab.cpp"
    break;

  case 266:
#line 1457 "parser.y"
        {
          // equivalent to always_inline, and seemingly also has the semantics
          // of extern inline in that multiple definitions can be provided in
          // the same translation unit
          init(yyval);
          set(yyval, ID_static);
          set(yyvsp[0], ID_inline);
          #if 0
          // enable once always_inline support is reinstantiated
          yyvsp[0]=merge(yyvsp[0], yyval);

          init(yyval);
          set(yyval, ID_always_inline);
          yyval=merge(yyvsp[0], yyval);
          #else
          yyval=merge(yyvsp[0], yyval);
          #endif
        }
#line 5513 "ansi_c_y.tab.cpp"
    break;

  case 267:
#line 1478 "parser.y"
                       { yyval=yyvsp[0]; set(yyval, ID_int); }
#line 5519 "ansi_c_y.tab.cpp"
    break;

  case 268:
#line 1479 "parser.y"
                       { yyval=yyvsp[0]; set(yyval, ID_int8); }
#line 5525 "ansi_c_y.tab.cpp"
    break;

  case 269:
#line 1480 "parser.y"
                       { yyval=yyvsp[0]; set(yyval, ID_int16); }
#line 5531 "ansi_c_y.tab.cpp"
    break;

  case 270:
#line 1481 "parser.y"
                       { yyval=yyvsp[0]; set(yyval, ID_int32); }
#line 5537 "ansi_c_y.tab.cpp"
    break;

  case 271:
#line 1482 "parser.y"
                       { yyval=yyvsp[0]; set(yyval, ID_int64); }
#line 5543 "ansi_c_y.tab.cpp"
    break;

  case 272:
#line 1483 "parser.y"
                       { yyval=yyvsp[0]; set(yyval, ID_char); }
#line 5549 "ansi_c_y.tab.cpp"
    break;

  case 273:
#line 1484 "parser.y"
                       { yyval=yyvsp[0]; set(yyval, ID_short); }
#line 5555 "ansi_c_y.tab.cpp"
    break;

  case 274:
#line 1485 "parser.y"
                       { yyval=yyvsp[0]; set(yyval, ID_long); }
#line 5561 "ansi_c_y.tab.cpp"
    break;

  case 275:
#line 1486 "parser.y"
                       { yyval=yyvsp[0]; set(yyval, ID_float); }
#line 5567 "ansi_c_y.tab.cpp"
    break;

  case 276:
#line 1487 "parser.y"
                            { yyval=yyvsp[0]; set(yyval, ID_gcc_float16); }
#line 5573 "ansi_c_y.tab.cpp"
    break;

  case 277:
#line 1488 "parser.y"
                            { yyval=yyvsp[0]; set(yyval, ID_gcc_float32); }
#line 5579 "ansi_c_y.tab.cpp"
    break;

  case 278:
#line 1489 "parser.y"
                            { yyval=yyvsp[0]; set(yyval, ID_gcc_float32x); }
#line 5585 "ansi_c_y.tab.cpp"
    break;

  case 279:
#line 1490 "parser.y"
                            { yyval=yyvsp[0]; set(yyval, ID_gcc_float64); }
#line 5591 "ansi_c_y.tab.cpp"
    break;

  case 280:
#line 1491 "parser.y"
                            { yyval=yyvsp[0]; set(yyval, ID_gcc_float64x); }
#line 5597 "ansi_c_y.tab.cpp"
    break;

  case 281:
#line 1492 "parser.y"
                            { yyval=yyvsp[0]; set(yyval, ID_gcc_float80); }
#line 5603 "ansi_c_y.tab.cpp"
    break;

  case 282:
#line 1493 "parser.y"
                            { yyval=yyvsp[0]; set(yyval, ID_gcc_float128); }
#line 5609 "ansi_c_y.tab.cpp"
    break;

  case 283:
#line 1494 "parser.y"
                            { yyval=yyvsp[0]; set(yyval, ID_gcc_float128x); }
#line 5615 "ansi_c_y.tab.cpp"
    break;

  case 284:
#line 1495 "parser.y"
                            { yyval=yyvsp[0]; set(yyval, ID_gcc_int128); }
#line 5621 "ansi_c_y.tab.cpp"
    break;

  case 285:
#line 1496 "parser.y"
                            { yyval=yyvsp[0]; set(yyval, ID_gcc_decimal32); }
#line 5627 "ansi_c_y.tab.cpp"
    break;

  case 286:
#line 1497 "parser.y"
                            { yyval=yyvsp[0]; set(yyval, ID_gcc_decimal64); }
#line 5633 "ansi_c_y.tab.cpp"
    break;

  case 287:
#line 1498 "parser.y"
                             { yyval=yyvsp[0]; set(yyval, ID_gcc_decimal128); }
#line 5639 "ansi_c_y.tab.cpp"
    break;

  case 288:
#line 1499 "parser.y"
                       { yyval=yyvsp[0]; set(yyval, ID_double); }
#line 5645 "ansi_c_y.tab.cpp"
    break;

  case 289:
#line 1500 "parser.y"
                       { yyval=yyvsp[0]; set(yyval, ID_signed); }
#line 5651 "ansi_c_y.tab.cpp"
    break;

  case 290:
#line 1501 "parser.y"
                       { yyval=yyvsp[0]; set(yyval, ID_unsigned); }
#line 5657 "ansi_c_y.tab.cpp"
    break;

  case 291:
#line 1502 "parser.y"
                       { yyval=yyvsp[0]; set(yyval, ID_void); }
#line 5663 "ansi_c_y.tab.cpp"
    break;

  case 292:
#line 1503 "parser.y"
                       { yyval=yyvsp[0]; set(yyval, ID_c_bool); }
#line 5669 "ansi_c_y.tab.cpp"
    break;

  case 293:
#line 1504 "parser.y"
                       { yyval=yyvsp[0]; set(yyval, ID_complex); }
#line 5675 "ansi_c_y.tab.cpp"
    break;

  case 294:
#line 1506 "parser.y"
        {
          yyval=yyvsp[-3];
          set(yyval, ID_custom_bv);
          parser_stack(yyval).add(ID_size).swap(parser_stack(yyvsp[-1]));
        }
#line 5685 "ansi_c_y.tab.cpp"
    break;

  case 295:
#line 1512 "parser.y"
        {
          yyval=yyvsp[-6];
          set(yyval, ID_custom_floatbv);
          parser_stack(yyval).add(ID_size).swap(parser_stack(yyvsp[-4]));
          parser_stack(yyval).add(ID_f).swap(parser_stack(yyvsp[-1]));
        }
#line 5696 "ansi_c_y.tab.cpp"
    break;

  case 296:
#line 1519 "parser.y"
        {
          yyval=yyvsp[-6];
          set(yyval, ID_custom_fixedbv);
          parser_stack(yyval).add(ID_size).swap(parser_stack(yyvsp[-4]));
          parser_stack(yyval).add(ID_f).swap(parser_stack(yyvsp[-1]));
        }
#line 5707 "ansi_c_y.tab.cpp"
    break;

  case 297:
#line 1525 "parser.y"
                           { yyval=yyvsp[0]; set(yyval, ID_proper_bool); }
#line 5713 "ansi_c_y.tab.cpp"
    break;

  case 301:
#line 1536 "parser.y"
        { yyval=yyvsp[-3]; stack_type(yyval).add_subtype().swap(parser_stack(yyvsp[-2])); }
#line 5719 "ansi_c_y.tab.cpp"
    break;

  case 302:
#line 1540 "parser.y"
        {
          init(yyval);
          if(!PARSER.pragma_pack.empty() &&
             PARSER.pragma_pack.back().is_one())
            set(yyval, ID_packed);
        }
#line 5730 "ansi_c_y.tab.cpp"
    break;

  case 303:
#line 1552 "parser.y"
          {
            // an anon struct/union with body
          }
#line 5738 "ansi_c_y.tab.cpp"
    break;

  case 304:
#line 1558 "parser.y"
        {
          // save the members
          parser_stack(yyvsp[-8]).add(ID_components).get_sub().swap(
            (irept::subt &)parser_stack(yyvsp[-3]).operands());

          // throw in the gcc attributes
          yyval=merge(yyvsp[-8], merge(yyvsp[-7], merge(yyvsp[-1], yyvsp[0])));
        }
#line 5751 "ansi_c_y.tab.cpp"
    break;

  case 305:
#line 1570 "parser.y"
          {
            // A struct/union with tag and body.
            PARSER.add_tag_with_body(parser_stack(yyvsp[0]));
            parser_stack(yyvsp[-3]).set(ID_tag, parser_stack(yyvsp[0]));
          }
#line 5761 "ansi_c_y.tab.cpp"
    break;

  case 306:
#line 1578 "parser.y"
        {
          // save the members
          parser_stack(yyvsp[-9]).add(ID_components).get_sub().swap(
            (irept::subt &)parser_stack(yyvsp[-3]).operands());

          // throw in the gcc attributes
          yyval=merge(yyvsp[-9], merge(yyvsp[-8], merge(yyvsp[-1], yyvsp[0])));
        }
#line 5774 "ansi_c_y.tab.cpp"
    break;

  case 307:
#line 1590 "parser.y"
          {
            // a struct/union with tag but without body
            parser_stack(yyvsp[-3]).set(ID_tag, parser_stack(yyvsp[0]));
          }
#line 5783 "ansi_c_y.tab.cpp"
    break;

  case 308:
#line 1595 "parser.y"
        {
          parser_stack(yyvsp[-5]).set(ID_components, ID_nil);
          // type attributes
          yyval=merge(yyvsp[-5], merge(yyvsp[-4], yyvsp[0]));
        }
#line 5793 "ansi_c_y.tab.cpp"
    break;

  case 309:
#line 1604 "parser.y"
        { yyval=yyvsp[0]; set(yyval, ID_struct); }
#line 5799 "ansi_c_y.tab.cpp"
    break;

  case 310:
#line 1606 "parser.y"
        { yyval=yyvsp[0]; set(yyval, ID_union); }
#line 5805 "ansi_c_y.tab.cpp"
    break;

  case 311:
#line 1611 "parser.y"
        { yyval=yyvsp[0]; set(yyval, ID_packed); }
#line 5811 "ansi_c_y.tab.cpp"
    break;

  case 312:
#line 1613 "parser.y"
        { yyval=yyvsp[0]; set(yyval, ID_transparent_union); }
#line 5817 "ansi_c_y.tab.cpp"
    break;

  case 313:
#line 1615 "parser.y"
        { yyval=yyvsp[-3]; set(yyval, ID_frontend_vector); parser_stack(yyval).add(ID_size)=parser_stack(yyvsp[-1]); }
#line 5823 "ansi_c_y.tab.cpp"
    break;

  case 314:
#line 1617 "parser.y"
        { yyval=yyvsp[0]; set(yyval, ID_aligned); }
#line 5829 "ansi_c_y.tab.cpp"
    break;

  case 315:
#line 1619 "parser.y"
        { yyval=yyvsp[-3]; set(yyval, ID_aligned); parser_stack(yyval).set(ID_size, parser_stack(yyvsp[-1])); }
#line 5835 "ansi_c_y.tab.cpp"
    break;

  case 316:
#line 1621 "parser.y"
        { yyval=yyvsp[-3]; set(yyval, ID_gcc_attribute_mode); parser_stack(yyval).set(ID_size, parser_stack(yyvsp[-1]).get(ID_identifier)); }
#line 5841 "ansi_c_y.tab.cpp"
    break;

  case 317:
#line 1623 "parser.y"
        { yyval=yyvsp[0]; set(yyval, ID_static); }
#line 5847 "ansi_c_y.tab.cpp"
    break;

  case 318:
#line 1625 "parser.y"
        { yyval=yyvsp[0]; set(yyval, ID_weak); }
#line 5853 "ansi_c_y.tab.cpp"
    break;

  case 319:
#line 1627 "parser.y"
        { yyval=yyvsp[-3]; set(yyval, ID_alias); mto(yyval, yyvsp[-1]); }
#line 5859 "ansi_c_y.tab.cpp"
    break;

  case 320:
#line 1629 "parser.y"
        { yyval=yyvsp[-3]; set(yyval, ID_section); mto(yyval, yyvsp[-1]); }
#line 5865 "ansi_c_y.tab.cpp"
    break;

  case 321:
#line 1631 "parser.y"
        { yyval=yyvsp[0]; set(yyval, ID_noreturn); }
#line 5871 "ansi_c_y.tab.cpp"
    break;

  case 322:
#line 1633 "parser.y"
        { yyval=yyvsp[0]; set(yyval, ID_constructor); }
#line 5877 "ansi_c_y.tab.cpp"
    break;

  case 323:
#line 1635 "parser.y"
        { yyval=yyvsp[0]; set(yyval, ID_destructor); }
#line 5883 "ansi_c_y.tab.cpp"
    break;

  case 324:
#line 1637 "parser.y"
        { yyval=yyvsp[0]; set(yyval, ID_used); }
#line 5889 "ansi_c_y.tab.cpp"
    break;

  case 325:
#line 1642 "parser.y"
        {
          init(yyval);
        }
#line 5897 "ansi_c_y.tab.cpp"
    break;

  case 326:
#line 1646 "parser.y"
        {
          // attribute ignored
          init(yyval);
        }
#line 5906 "ansi_c_y.tab.cpp"
    break;

  case 329:
#line 1656 "parser.y"
        {
          yyval=merge(yyvsp[-2], yyvsp[0]);
        }
#line 5914 "ansi_c_y.tab.cpp"
    break;

  case 330:
#line 1663 "parser.y"
        { yyval=yyvsp[-2]; }
#line 5920 "ansi_c_y.tab.cpp"
    break;

  case 331:
#line 1665 "parser.y"
        { yyval=yyvsp[0]; set(yyval, ID_noreturn); }
#line 5926 "ansi_c_y.tab.cpp"
    break;

  case 332:
#line 1670 "parser.y"
        {
          init(yyval);
        }
#line 5934 "ansi_c_y.tab.cpp"
    break;

  case 335:
#line 1679 "parser.y"
        {
          yyval=merge(yyvsp[-1], yyvsp[0]);
        }
#line 5942 "ansi_c_y.tab.cpp"
    break;

  case 336:
#line 1686 "parser.y"
        {
          init(yyval, ID_declaration_list);
        }
#line 5950 "ansi_c_y.tab.cpp"
    break;

  case 338:
#line 1694 "parser.y"
        {
          init(yyval, ID_declaration_list);
          mto(yyval, yyvsp[0]);
        }
#line 5959 "ansi_c_y.tab.cpp"
    break;

  case 339:
#line 1699 "parser.y"
        {
          yyval=yyvsp[-1];
          mto(yyval, yyvsp[0]);
        }
#line 5968 "ansi_c_y.tab.cpp"
    break;

  case 342:
#line 1709 "parser.y"
        {
          yyval=yyvsp[0]; // the ';' becomes the location of the declaration
          parser_stack(yyval).id(ID_declaration);
        }
#line 5977 "ansi_c_y.tab.cpp"
    break;

  case 344:
#line 1723 "parser.y"
        {
          yyvsp[-1]=merge(yyvsp[-1], yyvsp[-2]);

          init(yyval, ID_declaration);
          to_ansi_c_declaration(parser_stack(yyval)).set_is_member(true);
          parser_stack(yyval).add_source_location()=parser_stack(yyvsp[-1]).source_location();
          parser_stack(yyval).type().swap(parser_stack(yyvsp[-1]));
          PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[0]));
        }
#line 5991 "ansi_c_y.tab.cpp"
    break;

  case 345:
#line 1733 "parser.y"
        {
          yyval=yyvsp[-2];
          PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[0]));
        }
#line 6000 "ansi_c_y.tab.cpp"
    break;

  case 346:
#line 1743 "parser.y"
        {
          if(!PARSER.pragma_pack.empty() &&
             !PARSER.pragma_pack.back().is_zero())
          {
            // communicate #pragma pack(n) alignment constraints by
            // by both setting packing AND alignment; see padding.cpp
            // for more details
            init(yyval);
            set(yyval, ID_packed);
            yyvsp[-1]=merge(yyvsp[-1], yyval);

            init(yyval);
            set(yyval, ID_aligned);
            parser_stack(yyval).set(ID_size, PARSER.pragma_pack.back());
            yyvsp[-1]=merge(yyvsp[-1], yyval);
          }

          yyvsp[-1]=merge(yyvsp[-1], yyvsp[-2]);

          init(yyval, ID_declaration);
          to_ansi_c_declaration(parser_stack(yyval)).set_is_member(true);
          parser_stack(yyval).add_source_location()=parser_stack(yyvsp[-1]).source_location();
          parser_stack(yyval).type().swap(parser_stack(yyvsp[-1]));
          PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[0]));
        }
#line 6030 "ansi_c_y.tab.cpp"
    break;

  case 347:
#line 1769 "parser.y"
        {
          yyval=yyvsp[-2];
          PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[0]));
        }
#line 6039 "ansi_c_y.tab.cpp"
    break;

  case 348:
#line 1777 "parser.y"
        {
          yyval=yyvsp[-2];

          if(parser_stack(yyvsp[-1]).is_not_nil())
            make_subtype(yyval, yyvsp[-1]);

          if(parser_stack(yyvsp[0]).is_not_nil()) // type attribute
            yyval=merge(yyvsp[0], yyval);
        }
#line 6053 "ansi_c_y.tab.cpp"
    break;

  case 349:
#line 1787 "parser.y"
        {
          init(yyval, ID_abstract);
        }
#line 6061 "ansi_c_y.tab.cpp"
    break;

  case 350:
#line 1791 "parser.y"
        {
          yyval=yyvsp[-1];
          stack_type(yyval).add_subtype()=typet(ID_abstract);

          if(parser_stack(yyvsp[0]).is_not_nil()) // type attribute
            yyval=merge(yyvsp[0], yyval);
        }
#line 6073 "ansi_c_y.tab.cpp"
    break;

  case 351:
#line 1802 "parser.y"
        {
          yyval=yyvsp[-2];
          if(parser_stack(yyvsp[-1]).is_not_nil())
            make_subtype(yyval, yyvsp[-1]);
          
          if(parser_stack(yyvsp[0]).is_not_nil()) // type attribute
            yyval=merge(yyvsp[0], yyval);
        }
#line 6086 "ansi_c_y.tab.cpp"
    break;

  case 352:
#line 1811 "parser.y"
        {
          yyval=yyvsp[-1];
          stack_type(yyval).add_subtype()=typet(ID_abstract);

          if(parser_stack(yyvsp[0]).is_not_nil()) // type attribute
            yyval=merge(yyvsp[0], yyval);
        }
#line 6098 "ansi_c_y.tab.cpp"
    break;

  case 353:
#line 1822 "parser.y"
        {
          init(yyval, ID_nil);
        }
#line 6106 "ansi_c_y.tab.cpp"
    break;

  case 355:
#line 1830 "parser.y"
        {
          yyval=yyvsp[-1];
          set(yyval, ID_c_bit_field);
          stack_type(yyval).set(ID_size, parser_stack(yyvsp[0]));
          stack_type(yyval).add_subtype().id(ID_abstract);
        }
#line 6117 "ansi_c_y.tab.cpp"
    break;

  case 356:
#line 1842 "parser.y"
        {
          // an anon enum
          if(parser_stack(yyvsp[0]).is_not_nil())
          {
            parser_stack(yyvsp[-2]).set(ID_enum_underlying_type, parser_stack(yyvsp[0]));
          }
        }
#line 6129 "ansi_c_y.tab.cpp"
    break;

  case 357:
#line 1851 "parser.y"
        {
          parser_stack(yyvsp[-7]).operands().swap(parser_stack(yyvsp[-2]).operands());
          yyval=merge(yyvsp[-7], merge(yyvsp[-6], yyvsp[0])); // throw in the gcc attributes
        }
#line 6138 "ansi_c_y.tab.cpp"
    break;

  case 358:
#line 1859 "parser.y"
        {
          // an enum with tag
          parser_stack(yyvsp[-3]).set(ID_tag, parser_stack(yyvsp[-1]));

          if(parser_stack(yyvsp[0]).is_not_nil())
          {
            parser_stack(yyvsp[-3]).set(ID_enum_underlying_type, parser_stack(yyvsp[0]));
          }
        }
#line 6152 "ansi_c_y.tab.cpp"
    break;

  case 359:
#line 1870 "parser.y"
        {
          if(parser_stack(yyvsp[-1]).is_not_nil())
          {
            parser_stack(yyvsp[-6]).operands().swap(parser_stack(yyvsp[-1]).operands());
          }
          else
          {
            parser_stack(yyvsp[-6]).id(ID_c_enum_tag);
          }

          yyval=merge(yyvsp[-6], merge(yyvsp[-5], yyvsp[0])); // throw in the gcc attributes
        }
#line 6169 "ansi_c_y.tab.cpp"
    break;

  case 361:
#line 1887 "parser.y"
  {
    yyval = merge(yyvsp[-1], yyvsp[0]);
  }
#line 6177 "ansi_c_y.tab.cpp"
    break;

  case 364:
#line 1897 "parser.y"
        {
          init(yyval);
          parser_stack(yyval).make_nil();
        }
#line 6186 "ansi_c_y.tab.cpp"
    break;

  case 365:
#line 1902 "parser.y"
        {
          yyval=yyvsp[0];
        }
#line 6194 "ansi_c_y.tab.cpp"
    break;

  case 366:
#line 1908 "parser.y"
        {
          init(yyval);
          parser_stack(yyval).make_nil();
        }
#line 6203 "ansi_c_y.tab.cpp"
    break;

  case 367:
#line 1913 "parser.y"
        {
          yyval=yyvsp[-1];
        }
#line 6211 "ansi_c_y.tab.cpp"
    break;

  case 368:
#line 1918 "parser.y"
        {
          yyval=yyvsp[0];
          set(yyval, ID_c_enum);
        }
#line 6220 "ansi_c_y.tab.cpp"
    break;

  case 369:
#line 1926 "parser.y"
        {
          init(yyval, ID_declaration_list);
        }
#line 6228 "ansi_c_y.tab.cpp"
    break;

  case 371:
#line 1934 "parser.y"
        {
          init(yyval, ID_declaration_list);
          mto(yyval, yyvsp[0]);
        }
#line 6237 "ansi_c_y.tab.cpp"
    break;

  case 372:
#line 1939 "parser.y"
        {
          yyval=yyvsp[-2];
          mto(yyval, yyvsp[0]);
        }
#line 6246 "ansi_c_y.tab.cpp"
    break;

  case 373:
#line 1944 "parser.y"
        {
          yyval=yyvsp[-1];
        }
#line 6254 "ansi_c_y.tab.cpp"
    break;

  case 374:
#line 1951 "parser.y"
        {
          init(yyval, ID_declaration);
          to_ansi_c_declaration(parser_stack(yyval)).set_is_enum_constant(true);
          PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[-2]));
          to_ansi_c_declaration(parser_stack(yyval)).add_initializer(parser_stack(yyvsp[0]));
        }
#line 6265 "ansi_c_y.tab.cpp"
    break;

  case 375:
#line 1961 "parser.y"
        {
          init(yyval);
          parser_stack(yyval).make_nil();
        }
#line 6274 "ansi_c_y.tab.cpp"
    break;

  case 376:
#line 1966 "parser.y"
        {
          yyval=yyvsp[0];
        }
#line 6282 "ansi_c_y.tab.cpp"
    break;

  case 378:
#line 1974 "parser.y"
        {
          typet tmp(ID_ellipsis);
          yyval=yyvsp[-2];
          to_type_with_subtypes(stack_type(yyval)).move_to_subtypes(tmp);
        }
#line 6292 "ansi_c_y.tab.cpp"
    break;

  case 379:
#line 1983 "parser.y"
        {
          init(yyval, ID_parameters);
          mts(yyval, yyvsp[0]);
        }
#line 6301 "ansi_c_y.tab.cpp"
    break;

  case 380:
#line 1988 "parser.y"
        {
          yyval=yyvsp[-2];
          mts(yyval, yyvsp[0]);
        }
#line 6310 "ansi_c_y.tab.cpp"
    break;

  case 381:
#line 1995 "parser.y"
        {
          init(yyval, ID_declaration);
          parser_stack(yyval).type()=typet(ID_KnR);
          PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[0]));
        }
#line 6320 "ansi_c_y.tab.cpp"
    break;

  case 382:
#line 2004 "parser.y"
        {
          init(yyval, ID_parameters);
          mts(yyval, yyvsp[0]);
        }
#line 6329 "ansi_c_y.tab.cpp"
    break;

  case 383:
#line 2009 "parser.y"
        {
          yyval=yyvsp[-2];
          mts(yyval, yyvsp[0]);
        }
#line 6338 "ansi_c_y.tab.cpp"
    break;

  case 384:
#line 2017 "parser.y"
        {
          init(yyval, ID_declaration);
          to_ansi_c_declaration(parser_stack(yyval)).set_is_parameter(true);
          to_ansi_c_declaration(parser_stack(yyval)).type().swap(parser_stack(yyvsp[0]));
          exprt declarator=exprt(ID_abstract);
          PARSER.add_declarator(parser_stack(yyval), declarator);
        }
#line 6350 "ansi_c_y.tab.cpp"
    break;

  case 385:
#line 2025 "parser.y"
        {
          init(yyval, ID_declaration);
          to_ansi_c_declaration(parser_stack(yyval)).set_is_parameter(true);
          to_ansi_c_declaration(parser_stack(yyval)).type().swap(parser_stack(yyvsp[-1]));
          PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[0]));
        }
#line 6361 "ansi_c_y.tab.cpp"
    break;

  case 386:
#line 2032 "parser.y"
        {
          yyvsp[-1]=merge(yyvsp[0], yyvsp[-1]); // type attribute to go into declarator
          init(yyval, ID_declaration);
          to_ansi_c_declaration(parser_stack(yyval)).set_is_parameter(true);
          to_ansi_c_declaration(parser_stack(yyval)).type().swap(parser_stack(yyvsp[-2]));
          PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[-1]));
        }
#line 6373 "ansi_c_y.tab.cpp"
    break;

  case 387:
#line 2040 "parser.y"
        {
          // the second tree is really the declarator -- not part
          // of the type!
          init(yyval, ID_declaration);
          to_ansi_c_declaration(parser_stack(yyval)).set_is_parameter(true);
          to_ansi_c_declaration(parser_stack(yyval)).type().swap(parser_stack(yyvsp[-1]));
          PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[0]));
        }
#line 6386 "ansi_c_y.tab.cpp"
    break;

  case 388:
#line 2049 "parser.y"
        {
          init(yyval, ID_declaration);
          to_ansi_c_declaration(parser_stack(yyval)).set_is_parameter(true);
          to_ansi_c_declaration(parser_stack(yyval)).type().swap(parser_stack(yyvsp[0]));
          exprt declarator=exprt(ID_abstract);
          PARSER.add_declarator(parser_stack(yyval), declarator);
        }
#line 6398 "ansi_c_y.tab.cpp"
    break;

  case 389:
#line 2057 "parser.y"
        {
          init(yyval, ID_declaration);
          to_ansi_c_declaration(parser_stack(yyval)).set_is_parameter(true);
          to_ansi_c_declaration(parser_stack(yyval)).type().swap(parser_stack(yyvsp[-1]));
          PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[0]));
        }
#line 6409 "ansi_c_y.tab.cpp"
    break;

  case 390:
#line 2064 "parser.y"
        {
          yyvsp[-1]=merge(yyvsp[0], yyvsp[-1]); // type attribute to go into declarator
          init(yyval, ID_declaration);
          to_ansi_c_declaration(parser_stack(yyval)).set_is_parameter(true);
          to_ansi_c_declaration(parser_stack(yyval)).type().swap(parser_stack(yyvsp[-2]));
          PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[-1]));
        }
#line 6421 "ansi_c_y.tab.cpp"
    break;

  case 391:
#line 2072 "parser.y"
        {
          init(yyval, ID_declaration);
          to_ansi_c_declaration(parser_stack(yyval)).set_is_parameter(true);
          to_ansi_c_declaration(parser_stack(yyval)).type().swap(parser_stack(yyvsp[0]));
          exprt declarator=exprt(ID_abstract);
          PARSER.add_declarator(parser_stack(yyval), declarator);
        }
#line 6433 "ansi_c_y.tab.cpp"
    break;

  case 392:
#line 2080 "parser.y"
        {
          init(yyval, ID_declaration);
          to_ansi_c_declaration(parser_stack(yyval)).set_is_parameter(true);
          to_ansi_c_declaration(parser_stack(yyval)).type().swap(parser_stack(yyvsp[-1]));
          PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[0]));
        }
#line 6444 "ansi_c_y.tab.cpp"
    break;

  case 393:
#line 2087 "parser.y"
        {
          yyvsp[-1]=merge(yyvsp[0], yyvsp[-1]); // type attribute to go into declarator
          init(yyval, ID_declaration);
          to_ansi_c_declaration(parser_stack(yyval)).set_is_parameter(true);
          to_ansi_c_declaration(parser_stack(yyval)).type().swap(parser_stack(yyvsp[-2]));
          PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[-1]));
        }
#line 6456 "ansi_c_y.tab.cpp"
    break;

  case 394:
#line 2095 "parser.y"
        {
          // the second tree is really the declarator -- not part of the type!
          init(yyval, ID_declaration);
          to_ansi_c_declaration(parser_stack(yyval)).set_is_parameter(true);
          to_ansi_c_declaration(parser_stack(yyval)).type().swap(parser_stack(yyvsp[-1]));
          PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[0]));
        }
#line 6468 "ansi_c_y.tab.cpp"
    break;

  case 395:
#line 2103 "parser.y"
        {
          init(yyval, ID_declaration);
          to_ansi_c_declaration(parser_stack(yyval)).set_is_parameter(true);
          to_ansi_c_declaration(parser_stack(yyval)).type().swap(parser_stack(yyvsp[0]));
          exprt declarator=exprt(ID_abstract);
          PARSER.add_declarator(parser_stack(yyval), declarator);
        }
#line 6480 "ansi_c_y.tab.cpp"
    break;

  case 396:
#line 2111 "parser.y"
        {
          init(yyval, ID_declaration);
          to_ansi_c_declaration(parser_stack(yyval)).set_is_parameter(true);
          to_ansi_c_declaration(parser_stack(yyval)).type().swap(parser_stack(yyvsp[-1]));
          PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[0]));
        }
#line 6491 "ansi_c_y.tab.cpp"
    break;

  case 397:
#line 2118 "parser.y"
        {
          yyvsp[-1]=merge(yyvsp[0], yyvsp[-1]); // type attribute to go into declarator
          init(yyval, ID_declaration);
          to_ansi_c_declaration(parser_stack(yyval)).set_is_parameter(true);
          to_ansi_c_declaration(parser_stack(yyval)).type().swap(parser_stack(yyvsp[-2]));
          PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[-1]));
        }
#line 6503 "ansi_c_y.tab.cpp"
    break;

  case 400:
#line 2134 "parser.y"
        {
          yyval=merge(yyvsp[0], yyvsp[-1]);
        }
#line 6511 "ansi_c_y.tab.cpp"
    break;

  case 401:
#line 2138 "parser.y"
        {
          yyval=merge(yyvsp[-1], yyvsp[-2]);
          make_subtype(yyval, yyvsp[0]);
        }
#line 6520 "ansi_c_y.tab.cpp"
    break;

  case 402:
#line 2143 "parser.y"
        {
          yyval=merge(yyvsp[0], yyvsp[-1]);
        }
#line 6528 "ansi_c_y.tab.cpp"
    break;

  case 403:
#line 2147 "parser.y"
        {
          yyval=merge(yyvsp[-1], yyvsp[-2]);
          make_subtype(yyval, yyvsp[0]);
        }
#line 6537 "ansi_c_y.tab.cpp"
    break;

  case 404:
#line 2155 "parser.y"
        {
          init(yyval);
          parser_stack(yyval).make_nil();
        }
#line 6546 "ansi_c_y.tab.cpp"
    break;

  case 405:
#line 2160 "parser.y"
        { yyval = yyvsp[0]; }
#line 6552 "ansi_c_y.tab.cpp"
    break;

  case 407:
#line 2171 "parser.y"
        {
          yyval=yyvsp[-2];
          set(yyval, ID_initializer_list);
          parser_stack(yyval).operands().swap(parser_stack(yyvsp[-1]).operands());
        }
#line 6562 "ansi_c_y.tab.cpp"
    break;

  case 408:
#line 2177 "parser.y"
        {
          yyval=yyvsp[-3];
          set(yyval, ID_initializer_list);
          parser_stack(yyval).operands().swap(parser_stack(yyvsp[-2]).operands());
        }
#line 6572 "ansi_c_y.tab.cpp"
    break;

  case 409:
#line 2186 "parser.y"
        {
          yyval=yyvsp[0];
          exprt tmp;
          tmp.swap(parser_stack(yyval));
          parser_stack(yyval).clear();
          parser_stack(yyval).add_to_operands(std::move(tmp));
        }
#line 6584 "ansi_c_y.tab.cpp"
    break;

  case 410:
#line 2194 "parser.y"
        {
          yyval=yyvsp[-2];
          mto(yyval, yyvsp[0]);
        }
#line 6593 "ansi_c_y.tab.cpp"
    break;

  case 412:
#line 2203 "parser.y"
        {
          init(yyval);
          set(yyval, ID_initializer_list);
          parser_stack(yyval).operands().clear();
        }
#line 6603 "ansi_c_y.tab.cpp"
    break;

  case 414:
#line 2213 "parser.y"
        {
          yyval=yyvsp[-1];
          parser_stack(yyval).id(ID_designated_initializer);
          parser_stack(yyval).add(ID_designator).swap(parser_stack(yyvsp[-2]));
          mto(yyval, yyvsp[0]);
        }
#line 6614 "ansi_c_y.tab.cpp"
    break;

  case 415:
#line 2221 "parser.y"
        {
          init(yyval, ID_designated_initializer);
          parser_stack(yyval).add(ID_designator).swap(parser_stack(yyvsp[-1]));
          mto(yyval, yyvsp[0]);
        }
#line 6624 "ansi_c_y.tab.cpp"
    break;

  case 416:
#line 2227 "parser.y"
        {
          // yet another GCC speciality
          yyval=yyvsp[-1];
          parser_stack(yyval).id(ID_designated_initializer);
          exprt designator;
          exprt member(ID_member);
          member.set(ID_component_name, parser_stack(yyvsp[-2]).get(ID_C_base_name));
          designator.add_to_operands(std::move(member));
          parser_stack(yyval).add(ID_designator).swap(designator);
          mto(yyval, yyvsp[0]);
        }
#line 6640 "ansi_c_y.tab.cpp"
    break;

  case 417:
#line 2242 "parser.y"
        {
          init(yyval);
          parser_stack(yyvsp[-1]).id(ID_member);
          parser_stack(yyvsp[-1]).set(ID_component_name, parser_stack(yyvsp[0]).get(ID_C_base_name));
          mto(yyval, yyvsp[-1]);
        }
#line 6651 "ansi_c_y.tab.cpp"
    break;

  case 418:
#line 2249 "parser.y"
        {
          init(yyval);
          parser_stack(yyvsp[-2]).id(ID_index);
          mto(yyvsp[-2], yyvsp[-1]);
          mto(yyval, yyvsp[-2]);
        }
#line 6662 "ansi_c_y.tab.cpp"
    break;

  case 419:
#line 2256 "parser.y"
        {
          // TODO
          init(yyval);
          parser_stack(yyvsp[-4]).id(ID_index);
          mto(yyvsp[-4], yyvsp[-3]);
          mto(yyval, yyvsp[-4]);
        }
#line 6674 "ansi_c_y.tab.cpp"
    break;

  case 420:
#line 2264 "parser.y"
        {
          yyval=yyvsp[-3];
          parser_stack(yyvsp[-2]).id(ID_index);
          mto(yyvsp[-2], yyvsp[-1]);
          mto(yyval, yyvsp[-2]);
        }
#line 6685 "ansi_c_y.tab.cpp"
    break;

  case 421:
#line 2271 "parser.y"
        {
          // TODO
          yyval=yyvsp[-5];
          parser_stack(yyvsp[-4]).id(ID_index);
          mto(yyvsp[-4], yyvsp[-3]);
          mto(yyval, yyvsp[-4]);
        }
#line 6697 "ansi_c_y.tab.cpp"
    break;

  case 422:
#line 2279 "parser.y"
        {
          yyval=yyvsp[-2];
          parser_stack(yyvsp[-1]).id(ID_member);
          parser_stack(yyvsp[-1]).set(ID_component_name, parser_stack(yyvsp[0]).get(ID_C_base_name));
          mto(yyval, yyvsp[-1]);
        }
#line 6708 "ansi_c_y.tab.cpp"
    break;

  case 436:
#line 2307 "parser.y"
        {
          init(yyval);
          statement(yyval, ID_decl);
          mto(yyval, yyvsp[0]);
        }
#line 6718 "ansi_c_y.tab.cpp"
    break;

  case 437:
#line 2316 "parser.y"
        {
          yyval=yyvsp[-1];
          statement(yyval, ID_label);
          irep_idt identifier=PARSER.lookup_label(parser_stack(yyvsp[-2]).get(ID_C_base_name));
          parser_stack(yyval).set(ID_label, identifier);
          mto(yyval, yyvsp[0]);
        }
#line 6730 "ansi_c_y.tab.cpp"
    break;

  case 438:
#line 2324 "parser.y"
        {
          yyval=yyvsp[-3];
          statement(yyval, ID_switch_case);
          mto(yyval, yyvsp[-2]);
          mto(yyval, yyvsp[0]);
        }
#line 6741 "ansi_c_y.tab.cpp"
    break;

  case 439:
#line 2331 "parser.y"
        {
          // this is a GCC extension
          yyval=yyvsp[-5];
          statement(yyval, ID_gcc_switch_case_range);
          mto(yyval, yyvsp[-4]);
          mto(yyval, yyvsp[-2]);
          mto(yyval, yyvsp[0]);
        }
#line 6754 "ansi_c_y.tab.cpp"
    break;

  case 440:
#line 2340 "parser.y"
        {
          yyval=yyvsp[-2];
          statement(yyval, ID_switch_case);
          parser_stack(yyval).operands().push_back(nil_exprt());
          mto(yyval, yyvsp[0]);
          parser_stack(yyval).set(ID_default, true);
        }
#line 6766 "ansi_c_y.tab.cpp"
    break;

  case 441:
#line 2351 "parser.y"
        {
          // Really should only be TOK_GCC_ATTRIBUTE_FALLTHROUGH or a label
          // attribute. Only semicolons permitted after the attribute:
          // https://gcc.gnu.org/onlinedocs/gcc/Label-Attributes.html
          // We ignore all such attributes.
          yyval=yyvsp[-1];
          statement(yyval, ID_skip);
        }
#line 6779 "ansi_c_y.tab.cpp"
    break;

  case 442:
#line 2363 "parser.y"
        {
          yyval=yyvsp[-1];
          statement(yyval, ID_block);
          parser_stack(yyval).set(ID_C_end_location, parser_stack(yyvsp[0]).source_location());
          PARSER.pop_scope();
        }
#line 6790 "ansi_c_y.tab.cpp"
    break;

  case 443:
#line 2370 "parser.y"
        {
          yyval=yyvsp[-2];
          statement(yyval, ID_block);
          parser_stack(yyval).set(ID_C_end_location, parser_stack(yyvsp[0]).source_location());
          parser_stack(yyval).operands().swap(parser_stack(yyvsp[-1]).operands());
          PARSER.pop_scope();
        }
#line 6802 "ansi_c_y.tab.cpp"
    break;

  case 444:
#line 2378 "parser.y"
        {
          yyval=yyvsp[-2];
          statement(yyval, ID_asm);
          parser_stack(yyval).set(ID_C_end_location, parser_stack(yyvsp[0]).source_location());
          mto(yyval, yyvsp[-1]);
          PARSER.pop_scope();
        }
#line 6814 "ansi_c_y.tab.cpp"
    break;

  case 445:
#line 2389 "parser.y"
        {
          unsigned prefix=++PARSER.current_scope().compound_counter;
          PARSER.new_scope(std::to_string(prefix)+"::");
        }
#line 6823 "ansi_c_y.tab.cpp"
    break;

  case 446:
#line 2397 "parser.y"
        {
          init(yyval);
          mto(yyval, yyvsp[0]);
        }
#line 6832 "ansi_c_y.tab.cpp"
    break;

  case 447:
#line 2402 "parser.y"
        {
          mto(yyval, yyvsp[0]);
        }
#line 6840 "ansi_c_y.tab.cpp"
    break;

  case 448:
#line 2409 "parser.y"
        {
          yyval=yyvsp[0];

          if(parser_stack(yyvsp[-1]).is_nil())
            statement(yyval, ID_skip);
          else
          {
            statement(yyval, ID_expression);
            mto(yyval, yyvsp[-1]);
          }
        }
#line 6856 "ansi_c_y.tab.cpp"
    break;

  case 449:
#line 2424 "parser.y"
        {
          yyval=yyvsp[-4];
          statement(yyval, ID_ifthenelse);
          parser_stack(yyval).add_to_operands(
            std::move(parser_stack(yyvsp[-2])), std::move(parser_stack(yyvsp[0])), nil_exprt());
        }
#line 6867 "ansi_c_y.tab.cpp"
    break;

  case 450:
#line 2431 "parser.y"
        {
          yyval=yyvsp[-6];
          statement(yyval, ID_ifthenelse);
          parser_stack(yyval).add_to_operands(
            std::move(parser_stack(yyvsp[-4])), std::move(parser_stack(yyvsp[-2])), std::move(parser_stack(yyvsp[0])));
        }
#line 6878 "ansi_c_y.tab.cpp"
    break;

  case 451:
#line 2438 "parser.y"
        {
          yyval=yyvsp[-4];
          statement(yyval, ID_switch);
          parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-2])), std::move(parser_stack(yyvsp[0])));
        }
#line 6888 "ansi_c_y.tab.cpp"
    break;

  case 454:
#line 2456 "parser.y"
        {
          yyval=yyvsp[-7];
          statement(yyval, ID_while);
          parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-5])), std::move(parser_stack(yyvsp[0])));

          if(!parser_stack(yyvsp[-3]).operands().empty())
            static_cast<exprt &>(parser_stack(yyval).add(ID_C_spec_assigns)).operands().swap(parser_stack(yyvsp[-3]).operands());

          if(!parser_stack(yyvsp[-2]).operands().empty())
            static_cast<exprt &>(parser_stack(yyval).add(ID_C_spec_loop_invariant)).operands().swap(parser_stack(yyvsp[-2]).operands());

          if(!parser_stack(yyvsp[-1]).operands().empty())
            static_cast<exprt &>(parser_stack(yyval).add(ID_C_spec_decreases)).operands().swap(parser_stack(yyvsp[-1]).operands());
        }
#line 6907 "ansi_c_y.tab.cpp"
    break;

  case 455:
#line 2474 "parser.y"
        {
          yyval=yyvsp[-9];
          statement(yyval, ID_dowhile);
          parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-5])), std::move(parser_stack(yyvsp[-8])));

          if(!parser_stack(yyvsp[-3]).operands().empty())
            static_cast<exprt &>(parser_stack(yyval).add(ID_C_spec_assigns)).operands().swap(parser_stack(yyvsp[-3]).operands());

          if(!parser_stack(yyvsp[-2]).operands().empty())
            static_cast<exprt &>(parser_stack(yyval).add(ID_C_spec_loop_invariant)).operands().swap(parser_stack(yyvsp[-2]).operands());

          if(!parser_stack(yyvsp[-1]).operands().empty())
            static_cast<exprt &>(parser_stack(yyval).add(ID_C_spec_decreases)).operands().swap(parser_stack(yyvsp[-1]).operands());
        }
#line 6926 "ansi_c_y.tab.cpp"
    break;

  case 456:
#line 2489 "parser.y"
          {
            // In C99 and upwards, for(;;) has a scope
            if(PARSER.for_has_scope)
            {
              unsigned prefix=++PARSER.current_scope().compound_counter;
              PARSER.new_scope(std::to_string(prefix)+"::");
            }
          }
#line 6939 "ansi_c_y.tab.cpp"
    break;

  case 457:
#line 2504 "parser.y"
        {
          yyval=yyvsp[-11];
          statement(yyval, ID_for);
          parser_stack(yyval).operands().reserve(4);
          mto(yyval, yyvsp[-8]);
          mto(yyval, yyvsp[-7]);
          mto(yyval, yyvsp[-5]);
          mto(yyval, yyvsp[0]);

          if(!parser_stack(yyvsp[-3]).operands().empty())
            static_cast<exprt &>(parser_stack(yyval).add(ID_C_spec_assigns)).operands().swap(parser_stack(yyvsp[-3]).operands());

          if(!parser_stack(yyvsp[-2]).operands().empty())
            static_cast<exprt &>(parser_stack(yyval).add(ID_C_spec_loop_invariant)).operands().swap(parser_stack(yyvsp[-2]).operands());

          if(!parser_stack(yyvsp[-1]).operands().empty())
            static_cast<exprt &>(parser_stack(yyval).add(ID_C_spec_decreases)).operands().swap(parser_stack(yyvsp[-1]).operands());

          if(PARSER.for_has_scope)
            PARSER.pop_scope(); // remove the C99 for-scope
        }
#line 6965 "ansi_c_y.tab.cpp"
    break;

  case 458:
#line 2529 "parser.y"
        {
          yyval=yyvsp[-2];
          if(parser_stack(yyvsp[-1]).id()==ID_symbol)
          {
            statement(yyval, ID_goto);
            irep_idt identifier=PARSER.lookup_label(parser_stack(yyvsp[-1]).get(ID_C_base_name));
            parser_stack(yyval).set(ID_destination, identifier);
          }
          else
          {
            // this is a gcc extension.
            // the original grammar uses identifier_or_typedef_name
            statement(yyval, ID_gcc_computed_goto);
            mto(yyval, yyvsp[-1]);
          }
        }
#line 6986 "ansi_c_y.tab.cpp"
    break;

  case 459:
#line 2546 "parser.y"
        {
          yyval=yyvsp[-2];
          statement(yyval, ID_goto);
          irep_idt identifier=PARSER.lookup_label(parser_stack(yyvsp[-1]).get(ID_C_base_name));
          parser_stack(yyval).set(ID_destination, identifier);
        }
#line 6997 "ansi_c_y.tab.cpp"
    break;

  case 460:
#line 2553 "parser.y"
        { yyval=yyvsp[-1]; statement(yyval, ID_continue); }
#line 7003 "ansi_c_y.tab.cpp"
    break;

  case 461:
#line 2555 "parser.y"
        { yyval=yyvsp[-1]; statement(yyval, ID_break); }
#line 7009 "ansi_c_y.tab.cpp"
    break;

  case 462:
#line 2557 "parser.y"
        {
          yyval=yyvsp[-1];
          statement(yyval, ID_return);
          parser_stack(yyval).operands().push_back(nil_exprt());
        }
#line 7019 "ansi_c_y.tab.cpp"
    break;

  case 463:
#line 2563 "parser.y"
        { yyval=yyvsp[-2]; statement(yyval, ID_return); mto(yyval, yyvsp[-1]); }
#line 7025 "ansi_c_y.tab.cpp"
    break;

  case 464:
#line 2568 "parser.y"
        { 
          yyval=yyvsp[-2];
          statement(yyval, ID_gcc_local_label);
          
          // put these into the scope
          forall_operands(it, parser_stack(yyvsp[-1]))
          {
            // labels have a separate name space
            irep_idt base_name=it->get(ID_identifier);
            irep_idt id="label-"+id2string(base_name);
            ansi_c_parsert::identifiert &i=PARSER.current_scope().name_map[id];
            i.id_class=ansi_c_id_classt::ANSI_C_LOCAL_LABEL;
            i.prefixed_name=PARSER.current_scope().prefix+id2string(id);
            i.base_name=base_name;
          }

          parser_stack(yyval).add(ID_label).get_sub().swap((irept::subt&)parser_stack(yyvsp[-1]).operands());
        }
#line 7048 "ansi_c_y.tab.cpp"
    break;

  case 465:
#line 2590 "parser.y"
        {
          init(yyval);
          mto(yyval, yyvsp[0]);
        }
#line 7057 "ansi_c_y.tab.cpp"
    break;

  case 466:
#line 2595 "parser.y"
        {
          yyval=yyvsp[-2];
          mto(yyval, yyvsp[0]);
        }
#line 7066 "ansi_c_y.tab.cpp"
    break;

  case 468:
#line 2606 "parser.y"
        { yyval=yyvsp[-5];
          statement(yyval, ID_asm);
          parser_stack(yyval).set(ID_flavor, ID_gcc);
          parser_stack(yyval).operands().swap(parser_stack(yyvsp[-2]).operands());
        }
#line 7076 "ansi_c_y.tab.cpp"
    break;

  case 469:
#line 2612 "parser.y"
        {
          yyval=yyvsp[-4];
          statement(yyval, ID_asm);
          parser_stack(yyval).set(ID_flavor, ID_gcc);
          parser_stack(yyval).operands().resize(5);
          to_multi_ary_expr(parser_stack(yyval)).op0()=parser_stack(yyvsp[-1]);
        }
#line 7088 "ansi_c_y.tab.cpp"
    break;

  case 470:
#line 2623 "parser.y"
        { yyval=yyvsp[-3];
          statement(yyval, ID_asm);
          parser_stack(yyval).set(ID_flavor, ID_msc);
          mto(yyval, yyvsp[-1]);
        }
#line 7098 "ansi_c_y.tab.cpp"
    break;

  case 471:
#line 2629 "parser.y"
        { yyval=yyvsp[-1];
          statement(yyval, ID_asm);
          parser_stack(yyval).set(ID_flavor, ID_msc);
          mto(yyval, yyvsp[0]);
        }
#line 7108 "ansi_c_y.tab.cpp"
    break;

  case 472:
#line 2639 "parser.y"
        {
          yyval=yyvsp[-6];
          statement(yyval, ID_msc_try_except);
          mto(yyval, yyvsp[-5]);
          mto(yyval, yyvsp[-2]);
          mto(yyval, yyvsp[0]);
        }
#line 7120 "ansi_c_y.tab.cpp"
    break;

  case 473:
#line 2648 "parser.y"
        {
          yyval=yyvsp[-3];
          statement(yyval, ID_msc_try_finally);
          mto(yyval, yyvsp[-2]);
          mto(yyval, yyvsp[0]);
        }
#line 7131 "ansi_c_y.tab.cpp"
    break;

  case 474:
#line 2655 "parser.y"
        {
          yyval=yyvsp[0];
          statement(yyval, ID_msc_leave);
        }
#line 7140 "ansi_c_y.tab.cpp"
    break;

  case 475:
#line 2663 "parser.y"
        {
          yyval=yyvsp[-1];
          statement(yyval, ID_CPROVER_throw);
        }
#line 7149 "ansi_c_y.tab.cpp"
    break;

  case 476:
#line 2669 "parser.y"
        {
          yyval=yyvsp[-3];
          statement(yyval, ID_CPROVER_try_catch);
          mto(yyval, yyvsp[-2]);
          mto(yyval, yyvsp[0]);
        }
#line 7160 "ansi_c_y.tab.cpp"
    break;

  case 477:
#line 2677 "parser.y"
        {
          yyval=yyvsp[-3];
          statement(yyval, ID_CPROVER_try_finally);
          mto(yyval, yyvsp[-2]);
          mto(yyval, yyvsp[0]);
        }
#line 7171 "ansi_c_y.tab.cpp"
    break;

  case 482:
#line 2702 "parser.y"
        {
          init(yyval);
          parser_stack(yyval).operands().resize(5);
          parser_stack(yyval).operands()[0]=parser_stack(yyvsp[0]);
        }
#line 7181 "ansi_c_y.tab.cpp"
    break;

  case 483:
#line 2708 "parser.y"
        {
          init(yyval);
          parser_stack(yyval).operands().resize(5);
          parser_stack(yyval).operands()[0]=parser_stack(yyvsp[-1]);
          parser_stack(yyval).operands()[1]=parser_stack(yyvsp[0]);
        }
#line 7192 "ansi_c_y.tab.cpp"
    break;

  case 484:
#line 2715 "parser.y"
        {
          init(yyval);
          parser_stack(yyval).operands().resize(5);
          parser_stack(yyval).operands()[0]=parser_stack(yyvsp[-2]);
          parser_stack(yyval).operands()[1]=parser_stack(yyvsp[-1]);
          parser_stack(yyval).operands()[2]=parser_stack(yyvsp[0]);
        }
#line 7204 "ansi_c_y.tab.cpp"
    break;

  case 485:
#line 2723 "parser.y"
        {
          init(yyval);
          parser_stack(yyval).operands().resize(5);
          parser_stack(yyval).operands()[0]=parser_stack(yyvsp[-3]);
          parser_stack(yyval).operands()[1]=parser_stack(yyvsp[-2]);
          parser_stack(yyval).operands()[2]=parser_stack(yyvsp[-1]);
          parser_stack(yyval).operands()[3]=parser_stack(yyvsp[0]);
        }
#line 7217 "ansi_c_y.tab.cpp"
    break;

  case 486:
#line 2732 "parser.y"
        {
          init(yyval);
          parser_stack(yyval).operands().resize(5);
          parser_stack(yyval).operands()[0]=parser_stack(yyvsp[-4]);
          parser_stack(yyval).operands()[1]=parser_stack(yyvsp[-3]);
          parser_stack(yyval).operands()[2]=parser_stack(yyvsp[-2]);
          parser_stack(yyval).operands()[3]=parser_stack(yyvsp[-1]);
          parser_stack(yyval).operands()[4]=parser_stack(yyvsp[0]);
        }
#line 7231 "ansi_c_y.tab.cpp"
    break;

  case 488:
#line 2748 "parser.y"
        {
          yyval=yyvsp[0];
        }
#line 7239 "ansi_c_y.tab.cpp"
    break;

  case 490:
#line 2756 "parser.y"
        {
          yyval=yyvsp[-2];
          parser_stack(yyval).id(ID_gcc_asm_output);
          parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-3])), std::move(parser_stack(yyvsp[-1])));
        }
#line 7249 "ansi_c_y.tab.cpp"
    break;

  case 491:
#line 2763 "parser.y"
        {
          yyval=yyvsp[-2];
          parser_stack(yyval).id(ID_gcc_asm_output);
          parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-3])), std::move(parser_stack(yyvsp[-1])));
        }
#line 7259 "ansi_c_y.tab.cpp"
    break;

  case 492:
#line 2772 "parser.y"
        {
          init(yyval, irep_idt());
          mto(yyval, yyvsp[0]);
        }
#line 7268 "ansi_c_y.tab.cpp"
    break;

  case 493:
#line 2777 "parser.y"
        {
          yyval=yyvsp[-2];
          mto(yyval, yyvsp[0]);
        }
#line 7277 "ansi_c_y.tab.cpp"
    break;

  case 494:
#line 2785 "parser.y"
        {
          yyval=yyvsp[0];
        }
#line 7285 "ansi_c_y.tab.cpp"
    break;

  case 496:
#line 2793 "parser.y"
        {
          yyval=yyvsp[-2];
          parser_stack(yyval).id(ID_gcc_asm_input);
          parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-3])), std::move(parser_stack(yyvsp[-1])));
        }
#line 7295 "ansi_c_y.tab.cpp"
    break;

  case 497:
#line 2800 "parser.y"
        {
          yyval=yyvsp[-2];
          parser_stack(yyval).id(ID_gcc_asm_input);
          parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-3])), std::move(parser_stack(yyvsp[-1])));
        }
#line 7305 "ansi_c_y.tab.cpp"
    break;

  case 498:
#line 2809 "parser.y"
        {
          init(yyval, irep_idt());
          mto(yyval, yyvsp[0]);
        }
#line 7314 "ansi_c_y.tab.cpp"
    break;

  case 499:
#line 2814 "parser.y"
        {
          yyval=yyvsp[-2];
          mto(yyval, yyvsp[0]);
        }
#line 7323 "ansi_c_y.tab.cpp"
    break;

  case 500:
#line 2822 "parser.y"
        {
          yyval=yyvsp[0];
        }
#line 7331 "ansi_c_y.tab.cpp"
    break;

  case 502:
#line 2830 "parser.y"
        {
          init(yyval, ID_gcc_asm_clobbered_register);
          mto(yyval, yyvsp[0]);
        }
#line 7340 "ansi_c_y.tab.cpp"
    break;

  case 503:
#line 2838 "parser.y"
        {
          init(yyval, irep_idt());
          mto(yyval, yyvsp[0]);
        }
#line 7349 "ansi_c_y.tab.cpp"
    break;

  case 504:
#line 2843 "parser.y"
        {
          yyval=yyvsp[-2];
          mto(yyval, yyvsp[0]);
        }
#line 7358 "ansi_c_y.tab.cpp"
    break;

  case 505:
#line 2851 "parser.y"
        {
          yyval=yyvsp[0];
        }
#line 7366 "ansi_c_y.tab.cpp"
    break;

  case 507:
#line 2859 "parser.y"
        {
          init(yyval);
          mto(yyval, yyvsp[0]);
        }
#line 7375 "ansi_c_y.tab.cpp"
    break;

  case 508:
#line 2864 "parser.y"
        {
          yyval=yyvsp[-2];
          mto(yyval, yyvsp[0]);
        }
#line 7384 "ansi_c_y.tab.cpp"
    break;

  case 509:
#line 2872 "parser.y"
        {
          yyval=yyvsp[0];
          irep_idt identifier=PARSER.lookup_label(parser_stack(yyval).get(ID_C_base_name));
          parser_stack(yyval).id(ID_label);
          parser_stack(yyval).set(ID_identifier, identifier);
        }
#line 7395 "ansi_c_y.tab.cpp"
    break;

  case 514:
#line 2891 "parser.y"
        {
          // put into global list of items
          PARSER.copy_item(to_ansi_c_declaration(parser_stack(yyvsp[0])));
        }
#line 7404 "ansi_c_y.tab.cpp"
    break;

  case 515:
#line 2896 "parser.y"
        {
          PARSER.copy_item(to_ansi_c_declaration(parser_stack(yyvsp[0])));
        }
#line 7412 "ansi_c_y.tab.cpp"
    break;

  case 518:
#line 2905 "parser.y"
        {
          // Not obvious what to do with this.
        }
#line 7420 "ansi_c_y.tab.cpp"
    break;

  case 519:
#line 2909 "parser.y"
        {
          // Not obvious what to do with this.
        }
#line 7428 "ansi_c_y.tab.cpp"
    break;

  case 520:
#line 2917 "parser.y"
        {
          // The head is a declaration with one declarator,
          // and the body becomes the 'value'.
          yyval=yyvsp[-1];
          ansi_c_declarationt &ansi_c_declaration=
            to_ansi_c_declaration(parser_stack(yyval));
            
          assert(ansi_c_declaration.declarators().size()==1);
          ansi_c_declaration.add_initializer(parser_stack(yyvsp[0]));
          
          // Kill the scope that 'function_head' creates.
          PARSER.pop_scope();
          
          // We are no longer in any function.
          PARSER.set_function(irep_idt());
        }
#line 7449 "ansi_c_y.tab.cpp"
    break;

  case 522:
#line 2941 "parser.y"
        {
          init(yyval);
        }
#line 7457 "ansi_c_y.tab.cpp"
    break;

  case 524:
#line 2949 "parser.y"
        {
          init(yyval, ID_decl_block);
          mto(yyval, yyvsp[0]);
        }
#line 7466 "ansi_c_y.tab.cpp"
    break;

  case 525:
#line 2954 "parser.y"
        {
          yyval=yyvsp[-1];
          mto(yyval, yyvsp[0]);
        }
#line 7475 "ansi_c_y.tab.cpp"
    break;

  case 528:
#line 2968 "parser.y"
        {
          yyval=merge(yyvsp[0], yyvsp[-1]);
        }
#line 7483 "ansi_c_y.tab.cpp"
    break;

  case 529:
#line 2972 "parser.y"
        {
          yyval=merge(yyvsp[0], yyvsp[-1]);
        }
#line 7491 "ansi_c_y.tab.cpp"
    break;

  case 530:
#line 2979 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 7499 "ansi_c_y.tab.cpp"
    break;

  case 531:
#line 2983 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 7507 "ansi_c_y.tab.cpp"
    break;

  case 532:
#line 2987 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 7515 "ansi_c_y.tab.cpp"
    break;

  case 533:
#line 2991 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 7523 "ansi_c_y.tab.cpp"
    break;

  case 534:
#line 2999 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 7531 "ansi_c_y.tab.cpp"
    break;

  case 535:
#line 3003 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 7539 "ansi_c_y.tab.cpp"
    break;

  case 536:
#line 3007 "parser.y"
        {
          yyval=merge(yyvsp[-2], merge(yyvsp[-1], yyvsp[0]));
        }
#line 7547 "ansi_c_y.tab.cpp"
    break;

  case 537:
#line 3015 "parser.y"
        {
          parser_stack(yyvsp[-2]).set(ID_tag, parser_stack(yyvsp[-1]));
          yyval=merge(yyvsp[-3], merge(yyvsp[-2], yyvsp[0]));
        }
#line 7556 "ansi_c_y.tab.cpp"
    break;

  case 538:
#line 3020 "parser.y"
        {
          parser_stack(yyvsp[-2]).id(ID_c_enum_tag);
          parser_stack(yyvsp[-2]).set(ID_tag, parser_stack(yyvsp[-1]));
          yyval=merge(yyvsp[-3], merge(yyvsp[-2], yyvsp[0]));
        }
#line 7566 "ansi_c_y.tab.cpp"
    break;

  case 542:
#line 3036 "parser.y"
        {
          init(yyval, ID_declaration);
          parser_stack(yyval).type().swap(parser_stack(yyvsp[-1]));
          PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[0]));
        }
#line 7576 "ansi_c_y.tab.cpp"
    break;

  case 543:
#line 3042 "parser.y"
        {
          init(yyval, ID_declaration);
          parser_stack(yyval).type().swap(parser_stack(yyvsp[-1]));
          PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[0]));
        }
#line 7586 "ansi_c_y.tab.cpp"
    break;

  case 544:
#line 3048 "parser.y"
        {
          yyval=yyvsp[-2];
          PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[0]));
        }
#line 7595 "ansi_c_y.tab.cpp"
    break;

  case 545:
#line 3056 "parser.y"
        {
          init(yyval, ID_declaration);
          irept return_type(ID_int);
          parser_stack(yyval).type().swap(return_type);
          PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[0]));
          create_function_scope(yyval);
        }
#line 7607 "ansi_c_y.tab.cpp"
    break;

  case 546:
#line 3064 "parser.y"
        {
          init(yyval, ID_declaration);
          parser_stack(yyval).type().swap(parser_stack(yyvsp[-2]));
          yyvsp[-1]=merge(yyvsp[0], yyvsp[-1]);
          PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[-1]));
          create_function_scope(yyval);
        }
#line 7619 "ansi_c_y.tab.cpp"
    break;

  case 547:
#line 3072 "parser.y"
        {
          init(yyval, ID_declaration);
          parser_stack(yyval).type().swap(parser_stack(yyvsp[-2]));
          yyvsp[-1]=merge(yyvsp[0], yyvsp[-1]);
          PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[-1]));
          create_function_scope(yyval);
        }
#line 7631 "ansi_c_y.tab.cpp"
    break;

  case 548:
#line 3080 "parser.y"
        {
          init(yyval, ID_declaration);
          parser_stack(yyval).type().swap(parser_stack(yyvsp[-1]));
          PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[0]));
          create_function_scope(yyval);
        }
#line 7642 "ansi_c_y.tab.cpp"
    break;

  case 549:
#line 3087 "parser.y"
        {
          init(yyval, ID_declaration);
          parser_stack(yyval).type().swap(parser_stack(yyvsp[-1]));
          PARSER.add_declarator(parser_stack(yyval), parser_stack(yyvsp[0]));
          create_function_scope(yyval);
        }
#line 7653 "ansi_c_y.tab.cpp"
    break;

  case 553:
#line 3103 "parser.y"
        {
          stack_type(yyvsp[-3])=typet(ID_abstract);
          yyvsp[-2]=merge(yyvsp[-2], yyvsp[-3]); // dest=$2
          make_subtype(yyvsp[-1], yyvsp[-2]); // dest=$3
          yyval=yyvsp[-1];
        }
#line 7664 "ansi_c_y.tab.cpp"
    break;

  case 554:
#line 3110 "parser.y"
        {
          stack_type(yyvsp[-4])=typet(ID_abstract);
          yyvsp[-3]=merge(yyvsp[-3], yyvsp[-4]); // dest=$2
          make_subtype(yyvsp[-2], yyvsp[-3]); // dest=$3
          /* note: this is (a pointer to) a function ($5) */
          /* or an array ($5) with name ($3) */
          yyval=yyvsp[-2];
          make_subtype(yyval, yyvsp[0]);
        }
#line 7678 "ansi_c_y.tab.cpp"
    break;

  case 555:
#line 3120 "parser.y"
        {
          yyval=yyvsp[0];
          do_pointer(yyvsp[-1], yyvsp[0]);
        }
#line 7687 "ansi_c_y.tab.cpp"
    break;

  case 559:
#line 3134 "parser.y"
        {
          yyval=yyvsp[-1];
          make_subtype(yyval, yyvsp[0]);
        }
#line 7696 "ansi_c_y.tab.cpp"
    break;

  case 562:
#line 3144 "parser.y"
        {
          yyval=yyvsp[0];
          do_pointer(yyvsp[-1], yyvsp[0]);
        }
#line 7705 "ansi_c_y.tab.cpp"
    break;

  case 563:
#line 3149 "parser.y"
        {
          yyval=merge(yyvsp[-1], yyvsp[0]);
          do_pointer(yyvsp[-2], yyvsp[-1]);
        }
#line 7714 "ansi_c_y.tab.cpp"
    break;

  case 564:
#line 3157 "parser.y"
        { yyval = yyvsp[-1]; }
#line 7720 "ansi_c_y.tab.cpp"
    break;

  case 565:
#line 3159 "parser.y"
        {
          /* note: this is a pointer ($2) to a function ($4) */
          /* or an array ($4)! */
          yyval=yyvsp[-2];
          make_subtype(yyval, yyvsp[0]);
        }
#line 7731 "ansi_c_y.tab.cpp"
    break;

  case 567:
#line 3170 "parser.y"
        {
          yyval=yyvsp[-1];
          do_pointer(yyvsp[-3], yyvsp[-1]);
        }
#line 7740 "ansi_c_y.tab.cpp"
    break;

  case 568:
#line 3175 "parser.y"
        {
          // not sure where the type qualifiers belong
          yyval=merge(yyvsp[-3], yyvsp[-1]);
          do_pointer(yyvsp[-4], yyvsp[-3]);
        }
#line 7750 "ansi_c_y.tab.cpp"
    break;

  case 569:
#line 3181 "parser.y"
        {
          yyval=yyvsp[0];
          do_pointer(yyvsp[-1], yyvsp[0]);
        }
#line 7759 "ansi_c_y.tab.cpp"
    break;

  case 570:
#line 3186 "parser.y"
        {
          yyval=merge(yyvsp[-1], yyvsp[0]);
          do_pointer(yyvsp[-2], yyvsp[-1]);
        }
#line 7768 "ansi_c_y.tab.cpp"
    break;

  case 571:
#line 3194 "parser.y"
        { yyval = yyvsp[-1]; }
#line 7774 "ansi_c_y.tab.cpp"
    break;

  case 572:
#line 3196 "parser.y"
        {        /* note: this is a function ($3) with a typedef name ($2) */
          yyval=yyvsp[-2];
          make_subtype(yyval, yyvsp[-1]);
        }
#line 7783 "ansi_c_y.tab.cpp"
    break;

  case 573:
#line 3201 "parser.y"
        {
          /* note: this is a pointer ($2) to a function ($4) */
          /* or an array ($4)! */
          yyval=yyvsp[-2];
          make_subtype(yyval, yyvsp[0]);
        }
#line 7794 "ansi_c_y.tab.cpp"
    break;

  case 575:
#line 3212 "parser.y"
        { yyval=yyvsp[-1]; }
#line 7800 "ansi_c_y.tab.cpp"
    break;

  case 579:
#line 3223 "parser.y"
        {
          yyval=yyvsp[0];
          do_pointer(yyvsp[-1], yyvsp[0]);
        }
#line 7809 "ansi_c_y.tab.cpp"
    break;

  case 580:
#line 3228 "parser.y"
        {
          // This is an Apple extension to C/C++/Objective C.
          // http://en.wikipedia.org/wiki/Blocks_(C_language_extension)
          yyval=yyvsp[0];
          do_pointer(yyvsp[-1], yyvsp[0]);
        }
#line 7820 "ansi_c_y.tab.cpp"
    break;

  case 581:
#line 3235 "parser.y"
        {
          // the type_qualifier_list is for the pointer,
          // and not the identifier_declarator
          // The below is deliberately not using pointer_type();
          // the width is added during conversion.
          stack_type(yyvsp[-2]).id(ID_frontend_pointer);
          stack_type(yyvsp[-2]).add_subtype()=typet(ID_abstract);
          yyvsp[-1]=merge(yyvsp[-1], yyvsp[-2]); // dest=$2
          make_subtype(yyvsp[0], yyvsp[-1]); // dest=$3
          yyval=yyvsp[0];
        }
#line 7836 "ansi_c_y.tab.cpp"
    break;

  case 582:
#line 3250 "parser.y"
        {
          /* note: this is a function or array ($2) with name ($1) */
          yyval=yyvsp[-1];
          make_subtype(yyval, yyvsp[0]);
        }
#line 7846 "ansi_c_y.tab.cpp"
    break;

  case 583:
#line 3256 "parser.y"
        { yyval = yyvsp[-1]; }
#line 7852 "ansi_c_y.tab.cpp"
    break;

  case 584:
#line 3258 "parser.y"
        {
          /* note: this is a pointer ($2) to a function ($4) */
          /* or an array ($4)! */
          yyval=yyvsp[-2];
          make_subtype(yyval, yyvsp[0]);
        }
#line 7863 "ansi_c_y.tab.cpp"
    break;

  case 585:
#line 3268 "parser.y"
        {
          // We remember the last declarator for the benefit
          // of function argument scoping.
          PARSER.current_scope().last_declarator=
            parser_stack(yyvsp[0]).get(ID_identifier);
        }
#line 7874 "ansi_c_y.tab.cpp"
    break;

  case 586:
#line 3275 "parser.y"
        { yyval=yyvsp[-1]; }
#line 7880 "ansi_c_y.tab.cpp"
    break;

  case 592:
#line 3291 "parser.y"
        {
          yyval=yyvsp[-3];
          set(yyval, ID_C_spec_ensures);
          mto(yyval, yyvsp[-1]);
        }
#line 7890 "ansi_c_y.tab.cpp"
    break;

  case 593:
#line 3297 "parser.y"
        {
          yyval=yyvsp[-3];
          set(yyval, ID_C_spec_requires);
          mto(yyval, yyvsp[-1]);
        }
#line 7900 "ansi_c_y.tab.cpp"
    break;

  case 594:
#line 3303 "parser.y"
        {
          yyval=yyvsp[-5];
          set(yyval, ID_C_spec_ensures_contract);
          exprt tmp(ID_function_pointer_obeys_contract);
          tmp.add_to_operands(std::move(parser_stack(yyvsp[-3])));
          tmp.add_to_operands(std::move(parser_stack(yyvsp[-1])));
          tmp.add_source_location()=parser_stack(yyval).source_location();
          parser_stack(yyval).add_to_operands(std::move(tmp));
        }
#line 7914 "ansi_c_y.tab.cpp"
    break;

  case 595:
#line 3313 "parser.y"
        {
          yyval=yyvsp[-5];
          set(yyval, ID_C_spec_requires_contract);
          exprt tmp(ID_function_pointer_obeys_contract);
          tmp.add_to_operands(std::move(parser_stack(yyvsp[-3])));
          tmp.add_to_operands(std::move(parser_stack(yyvsp[-1])));
          tmp.add_source_location()=parser_stack(yyval).source_location();
          parser_stack(yyval).add_to_operands(std::move(tmp));
        }
#line 7928 "ansi_c_y.tab.cpp"
    break;

  case 598:
#line 3328 "parser.y"
        {
          init(yyval, ID_expression_list);
          parser_stack(yyval).add_source_location()=parser_stack(yyvsp[0]).source_location();
          mto(yyval, yyvsp[0]);
        }
#line 7938 "ansi_c_y.tab.cpp"
    break;

  case 599:
#line 3334 "parser.y"
        {
          yyval=yyvsp[-2];
          mto(yyval, yyvsp[0]);
        }
#line 7947 "ansi_c_y.tab.cpp"
    break;

  case 600:
#line 3342 "parser.y"
        {
          init(yyval, ID_conditional_target_group);
          parser_stack(yyval).add_source_location()=parser_stack(yyvsp[0]).source_location();
          parser_stack(yyval).add_to_operands(true_exprt{});
          mto(yyval, yyvsp[0]);
        }
#line 7958 "ansi_c_y.tab.cpp"
    break;

  case 601:
#line 3349 "parser.y"
        { 
          yyval=yyvsp[-1];
          set(yyval, ID_conditional_target_group);
          mto(yyval, yyvsp[-2]);
          mto(yyval, yyvsp[0]);
        }
#line 7969 "ansi_c_y.tab.cpp"
    break;

  case 602:
#line 3359 "parser.y"
        {
          init(yyval, ID_target_list);
          mto(yyval, yyvsp[0]);
        }
#line 7978 "ansi_c_y.tab.cpp"
    break;

  case 603:
#line 3364 "parser.y"
        {
          yyval=yyvsp[-2];
          mto(yyval, yyvsp[0]);
        }
#line 7987 "ansi_c_y.tab.cpp"
    break;

  case 604:
#line 3372 "parser.y"
        {
          yyval = yyvsp[-1];
        }
#line 7995 "ansi_c_y.tab.cpp"
    break;

  case 605:
#line 3376 "parser.y"
        {
          yyval = yyvsp[0];          
        }
#line 8003 "ansi_c_y.tab.cpp"
    break;

  case 606:
#line 3382 "parser.y"
        {
          yyval=yyvsp[-3];
          set(yyval, ID_C_spec_assigns);
          mto(yyval, yyvsp[-1]);
        }
#line 8013 "ansi_c_y.tab.cpp"
    break;

  case 607:
#line 3388 "parser.y"
        {
          yyval=yyvsp[-2];
          set(yyval, ID_C_spec_assigns);
          parser_stack(yyval).add_to_operands(exprt(ID_target_list));
        }
#line 8023 "ansi_c_y.tab.cpp"
    break;

  case 608:
#line 3397 "parser.y"
        { init(yyval); parser_stack(yyval).make_nil(); }
#line 8029 "ansi_c_y.tab.cpp"
    break;

  case 610:
#line 3403 "parser.y"
        {
          yyval=yyvsp[-3];
          set(yyval, ID_C_spec_frees);
          mto(yyval, yyvsp[-1]);
        }
#line 8039 "ansi_c_y.tab.cpp"
    break;

  case 611:
#line 3409 "parser.y"
        {
          yyval=yyvsp[-2];
          set(yyval, ID_C_spec_frees);
          parser_stack(yyval).add_to_operands(exprt(ID_target_list));
        }
#line 8049 "ansi_c_y.tab.cpp"
    break;

  case 613:
#line 3419 "parser.y"
        {
          yyval=yyvsp[-1];
          merge(yyval, yyvsp[0]);
        }
#line 8058 "ansi_c_y.tab.cpp"
    break;

  case 614:
#line 3427 "parser.y"
          { init(yyval); }
#line 8064 "ansi_c_y.tab.cpp"
    break;

  case 615:
#line 3429 "parser.y"
        {
          // Function contracts should either be attached to a
          // top-level function declaration or top-level function
          // definition.  Any embedded function pointer scopes should
          // be disallowed.
          int contract_in_global_scope = (PARSER.scopes.size() == 1);
          int contract_in_top_level_function_scope = (PARSER.scopes.size() == 2);
          if(!contract_in_global_scope && !contract_in_top_level_function_scope)
          {
            yyansi_cerror("Function contracts allowed only at top-level declarations.");
            YYABORT;
          }
        }
#line 8082 "ansi_c_y.tab.cpp"
    break;

  case 617:
#line 3450 "parser.y"
        {
          yyval=yyvsp[-2];
          set(yyval, ID_code);
          stack_type(yyval).add_subtype()=typet(ID_abstract);
          stack_type(yyval).add(ID_parameters);
          stack_type(yyval).set(ID_C_KnR, true);
        }
#line 8094 "ansi_c_y.tab.cpp"
    break;

  case 618:
#line 3458 "parser.y"
          {
            // Use last declarator (i.e., function name) to name
            // the scope.
            PARSER.new_scope(
              id2string(PARSER.current_scope().last_declarator)+"::");
          }
#line 8105 "ansi_c_y.tab.cpp"
    break;

  case 619:
#line 3467 "parser.y"
        {
          yyval=yyvsp[-4];
          set(yyval, ID_code);
          stack_type(yyval).add_subtype()=typet(ID_abstract);
          stack_type(yyval).add(ID_parameters).get_sub().
            swap((irept::subt &)(to_type_with_subtypes(stack_type(yyvsp[-2])).subtypes()));
          PARSER.pop_scope();
          adjust_KnR_parameters(parser_stack(yyval).add(ID_parameters), parser_stack(yyvsp[0]));
          parser_stack(yyval).set(ID_C_KnR, true);
        }
#line 8120 "ansi_c_y.tab.cpp"
    break;

  case 621:
#line 3482 "parser.y"
          {
            // Set function name (last declarator) in source location
            // before parsing function contracts.  Only do this if we
            // are at a global scope.
            if (PARSER.current_scope().prefix.empty()) {
              PARSER.set_function(PARSER.current_scope().last_declarator);
            }
            // Use last declarator (i.e., function name) to name
            // the scope.
            PARSER.new_scope(
              id2string(PARSER.current_scope().last_declarator)+"::");
          }
#line 8137 "ansi_c_y.tab.cpp"
    break;

  case 622:
#line 3495 "parser.y"
        {
          set(yyvsp[-3], ID_code);
          stack_type(yyvsp[-3]).add(ID_parameters);
          stack_type(yyvsp[-3]).add_subtype()=typet(ID_abstract);
          PARSER.pop_scope();

          // Clear function name in source location after parsing if
          // at global scope.
          if (PARSER.current_scope().prefix.empty()) {
            PARSER.set_function(irep_idt());
          }

          yyval = merge(yyvsp[0], yyvsp[-3]);
        }
#line 8156 "ansi_c_y.tab.cpp"
    break;

  case 623:
#line 3510 "parser.y"
          {
            // Set function name (last declarator) in source location
            // before parsing function contracts.  Only do this if we
            // are at a global scope.
            if (PARSER.current_scope().prefix.empty()) {
              PARSER.set_function(PARSER.current_scope().last_declarator);
            }
            // Use last declarator (i.e., function name) to name
            // the scope.
            PARSER.new_scope(
              id2string(PARSER.current_scope().last_declarator)+"::");
          }
#line 8173 "ansi_c_y.tab.cpp"
    break;

  case 624:
#line 3526 "parser.y"
        {
          set(yyvsp[-5], ID_code);
          stack_type(yyvsp[-5]).add_subtype()=typet(ID_abstract);
          stack_type(yyvsp[-5]).add(ID_parameters).get_sub().
            swap((irept::subt &)(to_type_with_subtypes(stack_type(yyvsp[-3])).subtypes()));
          PARSER.pop_scope();

          // Clear function name in source location after parsing if
          // at global scope.
          if (PARSER.current_scope().prefix.empty()) {
            PARSER.set_function(irep_idt());
          }

          if(parser_stack(yyvsp[-1]).is_not_nil())
          {
            adjust_KnR_parameters(parser_stack(yyval).add(ID_parameters), parser_stack(yyvsp[-1]));
            parser_stack(yyval).set(ID_C_KnR, true);
          }

          yyval = merge(yyvsp[0], yyvsp[-5]);
        }
#line 8199 "ansi_c_y.tab.cpp"
    break;

  case 625:
#line 3551 "parser.y"
        {
          yyval=yyvsp[-1];
          set(yyval, ID_array);
          stack_type(yyval).add_subtype()=typet(ID_abstract);
          stack_type(yyval).add(ID_size).make_nil();
        }
#line 8210 "ansi_c_y.tab.cpp"
    break;

  case 626:
#line 3558 "parser.y"
        {
          // this is C99: e.g., restrict, const, etc
          // The type qualifier belongs to the array, not the
          // contents of the array, nor the size.
          set(yyvsp[-2], ID_array);
          stack_type(yyvsp[-2]).add_subtype()=typet(ID_abstract);
          stack_type(yyvsp[-2]).add(ID_size).make_nil();
          yyval=merge(yyvsp[-1], yyvsp[-2]);
        }
#line 8224 "ansi_c_y.tab.cpp"
    break;

  case 627:
#line 3568 "parser.y"
        {
          // these should be allowed in prototypes only
          yyval=yyvsp[-2];
          set(yyval, ID_array);
          stack_type(yyval).add_subtype()=typet(ID_abstract);
          stack_type(yyval).add(ID_size).make_nil();
        }
#line 8236 "ansi_c_y.tab.cpp"
    break;

  case 628:
#line 3576 "parser.y"
        {
          yyval=yyvsp[-2];
          set(yyval, ID_array);
          stack_type(yyval).add(ID_size).swap(parser_stack(yyvsp[-1]));
          stack_type(yyval).add_subtype()=typet(ID_abstract);
        }
#line 8247 "ansi_c_y.tab.cpp"
    break;

  case 629:
#line 3583 "parser.y"
        {
          // The type qualifier belongs to the array, not the
          // contents of the array, nor the size.
          set(yyvsp[-3], ID_array);
          stack_type(yyvsp[-3]).add(ID_size).swap(parser_stack(yyvsp[-1]));
          stack_type(yyvsp[-3]).add_subtype()=typet(ID_abstract);
          yyval=merge(yyvsp[-2], yyvsp[-3]); // dest=$2
        }
#line 8260 "ansi_c_y.tab.cpp"
    break;

  case 630:
#line 3592 "parser.y"
        {
          // we need to push this down
          yyval=yyvsp[-3];
          set(yyvsp[-2], ID_array);
          stack_type(yyvsp[-2]).add(ID_size).swap(parser_stack(yyvsp[-1]));
          stack_type(yyvsp[-2]).add_subtype()=typet(ID_abstract);
          make_subtype(yyvsp[-3], yyvsp[-2]);
        }
#line 8273 "ansi_c_y.tab.cpp"
    break;

  case 631:
#line 3601 "parser.y"
        {
          // these should be allowed in prototypes only
          // we need to push this down
          yyval=yyvsp[-3];
          set(yyvsp[-2], ID_array);
          stack_type(yyvsp[-2]).add(ID_size).make_nil();
          stack_type(yyvsp[-2]).add_subtype()=typet(ID_abstract);
          make_subtype(yyvsp[-3], yyvsp[-2]);
        }
#line 8287 "ansi_c_y.tab.cpp"
    break;

  case 632:
#line 3614 "parser.y"
        {
          yyval=yyvsp[0];
          // The below is deliberately not using pointer_type();
          // the width is added during conversion.
          stack_type(yyval).id(ID_frontend_pointer);
          stack_type(yyval).add_subtype()=typet(ID_abstract);
        }
#line 8299 "ansi_c_y.tab.cpp"
    break;

  case 633:
#line 3622 "parser.y"
        {
          // The type_qualifier_list belongs to the pointer,
          // not to the (missing) abstract declarator.
          // The below is deliberately not using pointer_type();
          // the width is added during conversion.
          stack_type(yyvsp[-1]).id(ID_frontend_pointer);
          stack_type(yyvsp[-1]).add_subtype()=typet(ID_abstract);
          yyval=merge(yyvsp[0], yyvsp[-1]);
        }
#line 8313 "ansi_c_y.tab.cpp"
    break;

  case 634:
#line 3632 "parser.y"
        {
          yyval=yyvsp[0];
          do_pointer(yyvsp[-1], yyvsp[0]);
        }
#line 8322 "ansi_c_y.tab.cpp"
    break;

  case 635:
#line 3637 "parser.y"
        {
          // The type_qualifier_list belongs to the pointer,
          // not to the abstract declarator.
          // The below is deliberately not using pointer_type();
          // the width is added during conversion.
          stack_type(yyvsp[-2]).id(ID_frontend_pointer);
          stack_type(yyvsp[-2]).add_subtype()=typet(ID_abstract);
          yyvsp[-1]=merge(yyvsp[-1], yyvsp[-2]); // dest=$2
          make_subtype(yyvsp[0], yyvsp[-1]); // dest=$3
          yyval=yyvsp[0];
        }
#line 8338 "ansi_c_y.tab.cpp"
    break;

  case 636:
#line 3649 "parser.y"
        {
          // This is an Apple extension to C/C++/Objective C.
          // http://en.wikipedia.org/wiki/Blocks_(C_language_extension)
          yyval=yyvsp[0];
          set(yyval, ID_block_pointer);
          stack_type(yyval).add_subtype()=typet(ID_abstract);
        }
#line 8350 "ansi_c_y.tab.cpp"
    break;

  case 637:
#line 3660 "parser.y"
        {
          yyval=yyvsp[0];
          // The below is deliberately not using pointer_type();
          // the width is added during conversion.
          stack_type(yyval).id(ID_frontend_pointer);
          stack_type(yyval).add_subtype()=typet(ID_abstract);
        }
#line 8362 "ansi_c_y.tab.cpp"
    break;

  case 638:
#line 3668 "parser.y"
        {
          // The type_qualifier_list belongs to the pointer,
          // not to the (missing) abstract declarator.
          // The below is deliberately not using pointer_type();
          // the width is added during conversion.
          stack_type(yyvsp[-1]).id(ID_frontend_pointer);
          stack_type(yyvsp[-1]).add_subtype()=typet(ID_abstract);
          yyval=merge(yyvsp[0], yyvsp[-1]);
        }
#line 8376 "ansi_c_y.tab.cpp"
    break;

  case 639:
#line 3678 "parser.y"
        {
          yyval=yyvsp[0];
          do_pointer(yyvsp[-1], yyvsp[0]);
        }
#line 8385 "ansi_c_y.tab.cpp"
    break;

  case 640:
#line 3683 "parser.y"
        {
          // The type_qualifier_list belongs to the pointer,
          // not to the (missing) abstract declarator.
          // The below is deliberately not using pointer_type();
          // the width is added during conversion.
          stack_type(yyvsp[-2]).id(ID_frontend_pointer);
          stack_type(yyvsp[-2]).add_subtype()=typet(ID_abstract);
          yyvsp[-1]=merge(yyvsp[-1], yyvsp[-2]); // dest=$2
          make_subtype(yyvsp[0], yyvsp[-1]); // dest=$3
          yyval=yyvsp[0];
        }
#line 8401 "ansi_c_y.tab.cpp"
    break;

  case 641:
#line 3695 "parser.y"
        {
          // This is an Apple extension to C/C++/Objective C.
          // http://en.wikipedia.org/wiki/Blocks_(C_language_extension)
          yyval=yyvsp[0];
          set(yyval, ID_block_pointer);
          stack_type(yyval).add_subtype()=typet(ID_abstract);
        }
#line 8413 "ansi_c_y.tab.cpp"
    break;

  case 642:
#line 3706 "parser.y"
        { yyval = yyvsp[-1]; }
#line 8419 "ansi_c_y.tab.cpp"
    break;

  case 643:
#line 3708 "parser.y"
        { yyval = yyvsp[-1]; }
#line 8425 "ansi_c_y.tab.cpp"
    break;

  case 644:
#line 3710 "parser.y"
        { yyval = yyvsp[-1]; }
#line 8431 "ansi_c_y.tab.cpp"
    break;

  case 645:
#line 3712 "parser.y"
        {
          /* note: this is a pointer ($2) to a function or array ($4) */
          yyval=yyvsp[-2];
          make_subtype(yyval, yyvsp[0]);
        }
#line 8441 "ansi_c_y.tab.cpp"
    break;

  case 646:
#line 3718 "parser.y"
        {
          /* note: this is a pointer ($2) to a function or array ($4) */
          yyval=yyvsp[-2];
          make_subtype(yyval, yyvsp[0]);
        }
#line 8451 "ansi_c_y.tab.cpp"
    break;

  case 647:
#line 3727 "parser.y"
        { yyval = yyvsp[-1]; }
#line 8457 "ansi_c_y.tab.cpp"
    break;

  case 648:
#line 3729 "parser.y"
        { yyval = yyvsp[-1]; }
#line 8463 "ansi_c_y.tab.cpp"
    break;

  case 650:
#line 3732 "parser.y"
        {
          /* note: this is a pointer ($2) to a function ($4) */
          yyval=yyvsp[-2];
          make_subtype(yyval, yyvsp[0]);
        }
#line 8473 "ansi_c_y.tab.cpp"
    break;


#line 8477 "ansi_c_y.tab.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 3739 "parser.y"

