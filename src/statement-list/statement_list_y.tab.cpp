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
#define yyparse         yystatement_listparse
#define yylex           yystatement_listlex
#define yyerror         yystatement_listerror
#define yydebug         yystatement_listdebug
#define yynerrs         yystatement_listnerrs
#define yylval          yystatement_listlval
#define yychar          yystatement_listchar

/* First part of user prologue.  */
#line 1 "parser.y"


// This parser is based on the IEC standard 61131-3 which, among other things,
// includes a BNF grammar for the Instruction List (IL) language. The
// Statement List language (STL) by Siemens complies with the standards 
// defined by the IEC, although some modifications were made for compatibility
// reasons. As a consequence, the general language structure specified by the 
// IEC is similar to the structure of STL, but there are differences between
// their syntax and some language features (In general, Siemens implements more
// language features in STL than described in the standard).

#ifdef STATEMENT_LIST_DEBUG
#define YYDEBUG 1
#endif
#define PARSER statement_list_parser

#include "statement_list_parser.h"
#include "converters/convert_string_value.h"
#include "converters/statement_list_types.h"

#include <util/bitvector_types.h>

#include <iterator>

int yystatement_listlex();
extern char *yystatement_listtext;

#define YYSTYPE unsigned
#define YYSTYPE_IS_TRIVIAL 1

#include "statement_list_y.tab.h"

// Visual Studio
#ifdef _MSC_VER
// Disable warnings for possible loss of data.
#pragma warning(disable:4242)
#pragma warning(disable:4244)
// Disable warning for signed/unsigned mismatch.
#pragma warning(disable:4365)
// Disable warning for switch with default but no case labels.
#pragma warning(disable:4065)
// Disable warning for unreachable code.
#pragma warning(disable:4702)
#endif

/*** Token declaration *******************************************************/
#line 148 "parser.y"

/*** Grammar rules ***********************************************************/

// The follwing abbreviations will be used:
//   Zom: "Zero or more", eqivalent to the '*' quantifier
//   Opt: "Optional", equivalent to the '?' quantifier
//   Oom: "One or more", equivalent to the '+' quantifier

#line 133 "statement_list_y.tab.cpp"

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
typedef yytype_uint8 yy_state_t;

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
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   230

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  95
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  84
/* YYNRULES -- Number of rules.  */
#define YYNRULES  184
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  238

#define YYUNDEFTOK  2
#define YYMAXUTOK   344


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    93,     2,     2,     2,     2,
      94,     2,     2,     2,    91,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    90,    92,
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
      85,    86,    87,    88,    89
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   159,   159,   160,   161,   162,   167,   176,   184,   190,
     198,   207,   211,   215,   219,   220,   221,   222,   226,   230,
     238,   242,   250,   258,   265,   270,   276,   280,   294,   301,
     307,   314,   315,   316,   317,   321,   322,   323,   327,   334,
     340,   347,   355,   362,   368,   375,   383,   390,   396,   403,
     411,   418,   424,   431,   439,   447,   461,   465,   469,   476,
     482,   489,   490,   491,   495,   496,   497,   501,   508,   514,
     521,   525,   532,   538,   545,   549,   556,   562,   569,   573,
     580,   586,   593,   597,   604,   610,   617,   625,   633,   639,
     646,   657,   659,   666,   668,   676,   680,   685,   694,   702,
     704,   711,   715,   716,   720,   730,   732,   739,   744,   749,
     754,   759,   764,   769,   774,   779,   784,   789,   794,   799,
     804,   809,   814,   819,   824,   829,   834,   839,   844,   849,
     854,   859,   864,   869,   874,   879,   884,   889,   894,   899,
     904,   909,   914,   919,   924,   929,   934,   939,   944,   949,
     954,   959,   964,   969,   974,   979,   984,   989,   994,   999,
    1004,  1009,  1014,  1022,  1023,  1027,  1031,  1038,  1039,  1040,
    1044,  1057,  1065,  1074,  1079,  1085,  1090,  1098,  1106,  1113,
    1120,  1127,  1129,  1135,  1141
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "\"VERSION\"", "\"BEGIN\"",
  "\"FUNCTION_BLOCK\"", "\"END_FUNCTION_BLOCK\"", "\"FUNCTION\"",
  "\"END_FUNCTION\"", "\"VAR_INPUT\"", "\"VAR_IN_OUT\"", "\"VAR_OUTPUT\"",
  "\"VAR\"", "\"VAR_TEMP\"", "\"VAR CONSTANT\"", "\"END_VAR\"",
  "\"NETWORK\"", "\"TITLE\"", "\"TAG\"", "\"END_TAG\"", "\"Int\"",
  "\"DInt\"", "\"Real\"", "\"Bool\"", "\"Void\"", "\"L\"", "\"T\"",
  "\"CALL\"", "\"NOP\"", "\"SET\"", "\"CLR\"", "\"S\"", "\"R\"", "\"NOT\"",
  "\"A\"", "\"AN\"", "\"O\"", "\"ON\"", "\"X\"", "\"XN\"", "\"A(\"",
  "\"AN(\"", "\"O(\"", "\"ON(\"", "\"X(\"", "\"XN(\"", "\")\"", "\"=\"",
  "\"+\"", "\"+I\"", "\"-I\"", "\"*I\"", "\"/I\"", "\"==I\"", "\"<>I\"",
  "\">I\"", "\"<I\"", "\">=I\"", "\"<=I\"", "\"+R\"", "\"-R\"", "\"*R\"",
  "\"/R\"", "\"==R\"", "\"<>R\"", "\">R\"", "\"<R\"", "\">=R\"", "\"<=R\"",
  "\"+D\"", "\"-D\"", "\"*D\"", "\"/D\"", "\"==D\"", "\"<>D\"", "\">D\"",
  "\"<D\"", "\">=D\"", "\"<=D\"", "\":=\"", "\"JU\"", "\"JC\"", "\"JCN\"",
  "TOK_INT_LITERAL", "TOK_BOOL_LITERAL", "TOK_REAL_LITERAL",
  "TOK_IDENTIFIER", "TOK_TITLE_VALUE", "TOK_VERSION_VALUE", "TOK_LABEL",
  "':'", "','", "';'", "'#'", "'('", "$accept", "init", "Var_Decl_Init",
  "Variable_List", "Zom_Separated_Variable_Name", "Variable_Name",
  "Simple_Spec_Init", "Simple_Spec", "Elem_Type_Name", "Numeric_Type_Name",
  "Int_Type_Name", "Sign_Int_Type_Name", "DInt_Type_Name",
  "Sign_DInt_Type_Name", "Real_Type_Name", "Bool_Type_Name",
  "Opt_Assignment", "Derived_FB_Name", "FB_Decl", "Version_Label",
  "Zom_FB_General_Var_Decls", "FB_General_Var_Decl", "FB_IO_Var_Decls",
  "FB_Input_Decls", "Zom_FB_Input_Decl", "FB_Input_Decl",
  "FB_Output_Decls", "Zom_FB_Output_Decl", "FB_Output_Decl",
  "FB_Inout_Decls", "Zom_FB_Inout_Decl", "FB_Inout_Decl",
  "FB_Static_Decls", "Zom_FB_Static_Decl", "FB_Static_Decl", "FB_Body",
  "Func_Decl", "Derived_Func_Name", "Func_Return_Value",
  "Zom_Func_General_Var_Decls", "Func_General_Var_Decl", "IO_Var_Decls",
  "Input_Decls", "Zom_Input_Decl", "Input_Decl", "Inout_Decls",
  "Zom_Inout_Decl", "Inout_Decl", "Output_Decls", "Zom_Output_Decl",
  "Output_Decl", "Temp_Decls", "Zom_Temp_Decl", "Temp_Decl",
  "Constant_Decls", "Zom_Constant_Decl", "Constant_Decl", "Func_Body",
  "Zom_IL_Network", "IL_Network", "Opt_TITLE_VALUE",
  "Opt_Instruction_List", "Instruction_List", "Oom_IL_Instruction",
  "IL_Instruction", "Opt_Label", "IL_Label", "Instruction",
  "IL_Simple_Operation", "Opt_Operand", "IL_Simple_Operator", "IL_Operand",
  "Variable_Access", "Constant", "IL_Invocation", "Call", "Callee_Name",
  "Opt_Param_List", "Oom_Param_Assignment", "Param_Assignment",
  "Opt_Data_Block", "Tag_Decl", "Opt_Tag_List", "Tag_List", YY_NULLPTR
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
      58,    44,    59,    35,    40
};
# endif

#define YYPACT_NINF (-121)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-96)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
    -121,    15,  -121,   -65,   -63,   -62,  -121,  -121,  -121,  -121,
      27,  -121,   -58,  -121,    48,    16,   -62,   -54,  -121,    43,
    -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,
    -121,  -121,  -121,  -121,  -121,    48,   -50,    35,  -121,  -121,
      27,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,
    -121,  -121,  -121,  -121,  -121,  -121,    44,  -121,  -121,  -121,
      57,   -14,    -9,    -8,    -7,    -6,     1,  -121,    47,    58,
    -121,  -121,    12,   -16,  -121,     0,  -121,    12,     5,  -121,
      12,     6,  -121,    12,     7,  -121,  -121,     8,  -121,    12,
       9,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,
    -121,  -121,    68,    46,   -43,  -121,    48,    11,  -121,  -121,
    -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,    57,     2,
       3,     4,  -121,    17,  -121,  -121,  -121,  -121,  -121,   -62,
    -121,  -121,    13,  -121,  -121,    14,  -121,  -121,    18,  -121,
      -4,  -121,  -121,  -121,  -121,  -121,  -121,  -121,    -3,  -121,
     100,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,
    -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,
    -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,
    -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,
    -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,
    -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,
      19,  -121,   -31,  -121,   -63,  -121,   -62,  -121,  -121,  -121,
    -121,  -121,  -121,    21,  -121,   -62,    20,  -121,   -62,  -121,
      24,   -32,  -121,   -31,  -121,   -62,  -121,  -121
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,     0,     1,     0,     0,   182,     2,     3,     4,    26,
       0,    56,     0,    10,     0,     0,   181,     0,    30,     0,
      19,    21,    22,    23,   184,    11,    12,    13,    14,    18,
      15,    20,    16,    17,   180,     0,     0,     0,    57,    58,
       0,   183,    28,    89,    40,    48,    44,    52,    81,    85,
      29,    31,    35,    36,    37,    32,     0,    33,    34,    60,
      54,     0,     0,     0,     0,     0,     0,    27,     0,     0,
      88,    38,    25,     0,     9,     0,    46,    25,     0,    42,
      25,     0,    50,    25,     0,    79,    82,     0,    83,    25,
       0,    89,    69,    73,    77,    59,    61,    64,    66,    65,
      62,    63,     0,     0,     0,    41,     0,     7,    39,    49,
      47,    45,    43,    53,    51,    80,    86,    84,    87,     0,
       0,     0,    55,    92,   167,   168,   169,    24,     6,     0,
      67,    70,     0,    71,    74,     0,    75,    78,     0,    91,
     100,     8,    68,    72,    76,   101,    90,    93,   100,    97,
       0,    99,    96,   107,   108,   171,   109,   155,   156,   157,
     158,   159,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   160,   161,   162,
       0,   102,   106,   103,     0,    98,     0,   166,   104,   105,
     164,   163,   172,   179,   165,     0,   174,   178,     0,   170,
       0,     0,   176,     0,   173,     0,   177,   175
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -121,  -121,   -37,  -121,  -121,    -5,   -25,    75,  -121,  -121,
    -121,  -121,  -121,  -121,  -121,  -121,   -46,  -121,  -121,    55,
    -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,
    -121,  -121,  -121,  -121,  -121,  -121,  -121,  -118,  -121,  -121,
    -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,
    -121,    39,  -121,  -121,    40,  -121,  -121,  -121,    22,  -121,
    -121,  -121,  -121,  -121,   -39,  -121,  -121,  -121,  -121,  -121,
    -121,  -117,  -121,    79,  -121,  -121,  -121,  -121,  -121,  -120,
    -121,  -121,  -121,  -121
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    72,    73,   107,    74,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,   105,    10,     6,    18,
      37,    50,    51,    52,    61,    75,    53,    63,    81,    54,
      62,    78,    55,    64,    84,    56,     7,    12,    40,    68,
      95,    96,    97,   119,   132,    98,   120,   135,    99,   121,
     138,    57,    65,    87,    58,    66,    90,   102,    60,    70,
     140,   146,   147,   148,   149,   150,   151,   210,   211,   218,
     212,   219,   220,   221,   213,   214,   223,   229,   231,   232,
     226,     8,    15,    16
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      14,    71,   -94,   -95,   -94,   -95,    76,    79,    82,    85,
      41,    35,   -94,   -95,   234,     2,    88,   130,   133,   136,
       3,     9,     4,    11,    13,    77,    80,    83,    86,    89,
      17,   109,    19,     5,   111,    34,    36,   113,    42,    43,
     124,   125,   126,   116,    44,    45,    46,    47,    48,    49,
      67,    91,   124,   125,   126,    13,    92,    93,    94,   235,
      48,    49,   216,    20,    21,    22,    23,    38,    20,    21,
      22,    23,    13,    69,   106,   103,   122,    13,    13,    13,
      13,   128,   131,   134,   137,   145,   145,    13,    13,    13,
      13,   104,   108,   123,    39,    59,   222,   110,   112,   114,
     115,   117,   129,   233,   139,   142,   143,   100,   101,   152,
     144,   215,   225,   118,   228,   237,   236,     0,     0,     0,
       0,     0,     0,     0,   141,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,     0,
     207,   208,   209,   127,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   217,     0,     0,
       0,   224,     0,     0,     0,     0,     0,     0,     0,     0,
     227,     0,     0,   230,     0,     0,     0,     0,   217,     0,
     230
};

static const yytype_int16 yycheck[] =
{
       5,    15,     6,     6,     8,     8,    15,    15,    15,    15,
      35,    16,    16,    16,    46,     0,    15,    15,    15,    15,
       5,    86,     7,    86,    86,    62,    63,    64,    65,    66,
       3,    77,    90,    18,    80,    19,    90,    83,    88,     4,
      83,    84,    85,    89,     9,    10,    11,    12,    13,    14,
       6,     4,    83,    84,    85,    86,     9,    10,    11,    91,
      13,    14,    93,    20,    21,    22,    23,    24,    20,    21,
      22,    23,    86,    16,    90,    17,     8,    86,    86,    86,
      86,   106,   119,   120,   121,    89,    89,    86,    86,    86,
      86,    79,    92,    47,    19,    40,   214,    92,    92,    92,
      92,    92,    91,    79,    87,    92,    92,    68,    68,   148,
      92,    92,    91,    91,    94,   235,   233,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    -1,
      80,    81,    82,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   212,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     225,    -1,    -1,   228,    -1,    -1,    -1,    -1,   233,    -1,
     235
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    96,     0,     5,     7,    18,   113,   131,   176,    86,
     112,    86,   132,    86,   100,   177,   178,     3,   114,    90,
      20,    21,    22,    23,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,    19,   100,    90,   115,    24,   102,
     133,   101,    88,     4,     9,    10,    11,    12,    13,    14,
     116,   117,   118,   121,   124,   127,   130,   146,   149,   114,
     153,   119,   125,   122,   128,   147,   150,     6,   134,    16,
     154,    15,    97,    98,   100,   120,    15,    97,   126,    15,
      97,   123,    15,    97,   129,    15,    97,   148,    15,    97,
     151,     4,     9,    10,    11,   135,   136,   137,   140,   143,
     146,   149,   152,    17,    79,   111,    90,    99,    92,   111,
      92,   111,    92,   111,    92,    92,   111,    92,   153,   138,
     141,   144,     8,    47,    83,    84,    85,   168,   101,    91,
      15,    97,   139,    15,    97,   142,    15,    97,   145,    87,
     155,   100,    92,    92,    92,    89,   156,   157,   158,   159,
     160,   161,   159,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    80,    81,    82,
     162,   163,   165,   169,   170,    92,    93,   100,   164,   166,
     167,   168,   132,   171,   100,    91,   175,   100,    94,   172,
     100,   173,   174,    79,    46,    91,   166,   174
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    95,    96,    96,    96,    96,    97,    98,    99,    99,
     100,   101,   102,   103,   104,   104,   104,   104,   105,   106,
     107,   108,   109,   110,   111,   111,   112,   113,   114,   115,
     115,   116,   116,   116,   116,   117,   117,   117,   118,   119,
     119,   120,   121,   122,   122,   123,   124,   125,   125,   126,
     127,   128,   128,   129,   130,   131,   132,   133,   133,   134,
     134,   135,   135,   135,   136,   136,   136,   137,   138,   138,
     139,   140,   141,   141,   142,   143,   144,   144,   145,   146,
     147,   147,   148,   149,   150,   150,   151,   152,   153,   153,
     154,   155,   155,   156,   156,   157,   158,   158,   159,   160,
     160,   161,   162,   162,   163,   164,   164,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   166,   166,   167,   167,   168,   168,   168,
     169,   170,   171,   172,   172,   173,   173,   174,   175,   175,
     176,   177,   177,   178,   178
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     2,     2,     0,     3,     2,     3,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     0,     1,     6,     3,     2,
       0,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       0,     2,     3,     3,     0,     2,     3,     3,     0,     2,
       3,     3,     0,     2,     2,     8,     1,     1,     1,     2,
       0,     1,     1,     1,     1,     1,     1,     3,     3,     0,
       1,     3,     3,     0,     1,     3,     3,     0,     1,     3,
       3,     0,     1,     3,     3,     0,     2,     2,     2,     0,
       5,     1,     0,     1,     0,     1,     2,     1,     3,     1,
       0,     1,     1,     1,     2,     1,     0,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       4,     1,     1,     3,     0,     3,     1,     3,     2,     0,
       3,     1,     0,     3,     2
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
  case 6:
#line 168 "parser.y"
    {
      yyval = yyvsp[-2];
      for(auto &sym : parser_stack(yyval).operands())
        sym = symbol_exprt(sym.get(ID_identifier), parser_stack(yyvsp[0]).type());
    }
#line 1677 "statement_list_y.tab.cpp"
    break;

  case 7:
#line 177 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-1])));
    }
#line 1686 "statement_list_y.tab.cpp"
    break;

  case 8:
#line 185 "parser.y"
    {
      yyval = yyvsp[-2];
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[0])));
    }
#line 1695 "statement_list_y.tab.cpp"
    break;

  case 9:
#line 190 "parser.y"
    {
      newstack(yyval);
      parser_stack(yyval).id(ID_statement_list_var_entry);
      
    }
#line 1705 "statement_list_y.tab.cpp"
    break;

  case 10:
#line 199 "parser.y"
    {
      newstack(yyval);
      parser_stack(yyval) = 
      symbol_exprt::typeless(parser_stack(yyvsp[0]).get(ID_value));
    }
#line 1715 "statement_list_y.tab.cpp"
    break;

  case 19:
#line 231 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).type() = get_int_type();
    }
#line 1724 "statement_list_y.tab.cpp"
    break;

  case 21:
#line 243 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).type() = get_dint_type();
    }
#line 1733 "statement_list_y.tab.cpp"
    break;

  case 22:
#line 251 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).type() = get_real_type();
    }
#line 1742 "statement_list_y.tab.cpp"
    break;

  case 23:
#line 259 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).type() = get_bool_type();
    }
#line 1751 "statement_list_y.tab.cpp"
    break;

  case 24:
#line 266 "parser.y"
    {
      yyval = yyvsp[0];
    }
#line 1759 "statement_list_y.tab.cpp"
    break;

  case 25:
#line 270 "parser.y"
    {
      newstack(yyval);
    }
#line 1767 "statement_list_y.tab.cpp"
    break;

  case 27:
#line 282 "parser.y"
    {
      newstack(yyval);
      parser_stack(yyval).id(ID_statement_list_function_block);
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-4])), 
        std::move(parser_stack(yyvsp[-3])));
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-2])),
        std::move(parser_stack(yyvsp[-1])));
      PARSER.add_function_block(parser_stack(yyval));
    }
#line 1781 "statement_list_y.tab.cpp"
    break;

  case 28:
#line 295 "parser.y"
    {
      yyval = yyvsp[0];
    }
#line 1789 "statement_list_y.tab.cpp"
    break;

  case 29:
#line 302 "parser.y"
    {
      yyval = yyvsp[-1];
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[0])));
    }
#line 1798 "statement_list_y.tab.cpp"
    break;

  case 30:
#line 307 "parser.y"
    {
      newstack(yyval);
      parser_stack(yyval).id(ID_statement_list_var_decls);
    }
#line 1807 "statement_list_y.tab.cpp"
    break;

  case 38:
#line 328 "parser.y"
    {
      yyval = yyvsp[-1];
    }
#line 1815 "statement_list_y.tab.cpp"
    break;

  case 39:
#line 335 "parser.y"
    {
      yyval = yyvsp[-2];
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-1])));
    }
#line 1824 "statement_list_y.tab.cpp"
    break;

  case 40:
#line 340 "parser.y"
    {
      newstack(yyval);
      parser_stack(yyval).id(ID_statement_list_var_input);
    }
#line 1833 "statement_list_y.tab.cpp"
    break;

  case 41:
#line 348 "parser.y"
    {
      yyval = yyvsp[-1];
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[0])));
    }
#line 1842 "statement_list_y.tab.cpp"
    break;

  case 42:
#line 356 "parser.y"
    {
      yyval = yyvsp[-1];
    }
#line 1850 "statement_list_y.tab.cpp"
    break;

  case 43:
#line 363 "parser.y"
    {
      yyval = yyvsp[-2];
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-1])));
    }
#line 1859 "statement_list_y.tab.cpp"
    break;

  case 44:
#line 368 "parser.y"
    {
      newstack(yyval);
      parser_stack(yyval).id(ID_statement_list_var_output);
    }
#line 1868 "statement_list_y.tab.cpp"
    break;

  case 45:
#line 376 "parser.y"
    {
      yyval = yyvsp[-1];
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[0])));
    }
#line 1877 "statement_list_y.tab.cpp"
    break;

  case 46:
#line 384 "parser.y"
    {
      yyval = yyvsp[-1];
    }
#line 1885 "statement_list_y.tab.cpp"
    break;

  case 47:
#line 391 "parser.y"
    {
      yyval = yyvsp[-2];
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-1])));
    }
#line 1894 "statement_list_y.tab.cpp"
    break;

  case 48:
#line 396 "parser.y"
    {
      newstack(yyval);
      parser_stack(yyval).id(ID_statement_list_var_inout);
    }
#line 1903 "statement_list_y.tab.cpp"
    break;

  case 49:
#line 404 "parser.y"
    {
      yyval = yyvsp[-1];
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[0])));
    }
#line 1912 "statement_list_y.tab.cpp"
    break;

  case 50:
#line 412 "parser.y"
    {
      yyval = yyvsp[-1];
    }
#line 1920 "statement_list_y.tab.cpp"
    break;

  case 51:
#line 419 "parser.y"
    {
      yyval = yyvsp[-2];
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-1])));
    }
#line 1929 "statement_list_y.tab.cpp"
    break;

  case 52:
#line 424 "parser.y"
    {
      newstack(yyval);
      parser_stack(yyval).id(ID_statement_list_var_static);
    }
#line 1938 "statement_list_y.tab.cpp"
    break;

  case 53:
#line 432 "parser.y"
    {
      yyval = yyvsp[-1];
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[0])));
    }
#line 1947 "statement_list_y.tab.cpp"
    break;

  case 54:
#line 440 "parser.y"
    {
      yyval = yyvsp[0];
    }
#line 1955 "statement_list_y.tab.cpp"
    break;

  case 55:
#line 449 "parser.y"
    {
      newstack(yyval);
      parser_stack(yyval).id(ID_statement_list_function);
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-6])),
      std::move(parser_stack(yyvsp[-4])), std::move(parser_stack(yyvsp[-3])));
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-2])), 
        std::move(parser_stack(yyvsp[-1])));
      PARSER.add_function(parser_stack(yyval));
    }
#line 1969 "statement_list_y.tab.cpp"
    break;

  case 57:
#line 466 "parser.y"
    {
      parser_stack(yyval).set(ID_statement_list_type, ID_statement_list_return);
    }
#line 1977 "statement_list_y.tab.cpp"
    break;

  case 58:
#line 470 "parser.y"
    {
      parser_stack(yyval).set(ID_statement_list_type, ID_statement_list_return);
    }
#line 1985 "statement_list_y.tab.cpp"
    break;

  case 59:
#line 477 "parser.y"
    {
      yyval = yyvsp[-1];
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[0])));
    }
#line 1994 "statement_list_y.tab.cpp"
    break;

  case 60:
#line 482 "parser.y"
    {
      newstack(yyval);
      parser_stack(yyval).id(ID_statement_list_var_decls);
    }
#line 2003 "statement_list_y.tab.cpp"
    break;

  case 67:
#line 502 "parser.y"
    {
      yyval = yyvsp[-1];
    }
#line 2011 "statement_list_y.tab.cpp"
    break;

  case 68:
#line 509 "parser.y"
    {
      yyval = yyvsp[-2];
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-1])));
    }
#line 2020 "statement_list_y.tab.cpp"
    break;

  case 69:
#line 514 "parser.y"
    {
      newstack(yyval);
      parser_stack(yyval).id(ID_statement_list_var_input);
    }
#line 2029 "statement_list_y.tab.cpp"
    break;

  case 71:
#line 526 "parser.y"
    {
      yyval = yyvsp[-1];
    }
#line 2037 "statement_list_y.tab.cpp"
    break;

  case 72:
#line 533 "parser.y"
    {
      yyval = yyvsp[-2];
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-1])));
    }
#line 2046 "statement_list_y.tab.cpp"
    break;

  case 73:
#line 538 "parser.y"
    {
      newstack(yyval);
      parser_stack(yyval).id(ID_statement_list_var_inout);
    }
#line 2055 "statement_list_y.tab.cpp"
    break;

  case 75:
#line 550 "parser.y"
    {
      yyval = yyvsp[-1];
    }
#line 2063 "statement_list_y.tab.cpp"
    break;

  case 76:
#line 557 "parser.y"
    {
      yyval = yyvsp[-2];
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-1])));
    }
#line 2072 "statement_list_y.tab.cpp"
    break;

  case 77:
#line 562 "parser.y"
    {
      newstack(yyval);
      parser_stack(yyval).id(ID_statement_list_var_output);
    }
#line 2081 "statement_list_y.tab.cpp"
    break;

  case 79:
#line 574 "parser.y"
    {
      yyval = yyvsp[-1];
    }
#line 2089 "statement_list_y.tab.cpp"
    break;

  case 80:
#line 581 "parser.y"
    {
      yyval = yyvsp[-2];
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-1])));
    }
#line 2098 "statement_list_y.tab.cpp"
    break;

  case 81:
#line 586 "parser.y"
    {
      newstack(yyval);
      parser_stack(yyval).id(ID_statement_list_var_temp);
    }
#line 2107 "statement_list_y.tab.cpp"
    break;

  case 83:
#line 598 "parser.y"
    {
      yyval = yyvsp[-1];
    }
#line 2115 "statement_list_y.tab.cpp"
    break;

  case 84:
#line 605 "parser.y"
    {
      yyval = yyvsp[-2];
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-1])));
    }
#line 2124 "statement_list_y.tab.cpp"
    break;

  case 85:
#line 610 "parser.y"
    {
      newstack(yyval);
      parser_stack(yyval).id(ID_statement_list_var_constant);
    }
#line 2133 "statement_list_y.tab.cpp"
    break;

  case 86:
#line 618 "parser.y"
    {
      yyval = yyvsp[-1];
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[0])));
    }
#line 2142 "statement_list_y.tab.cpp"
    break;

  case 87:
#line 626 "parser.y"
    {
      yyval = yyvsp[0];
    }
#line 2150 "statement_list_y.tab.cpp"
    break;

  case 88:
#line 634 "parser.y"
    {
      yyval = yyvsp[-1];
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[0])));
    }
#line 2159 "statement_list_y.tab.cpp"
    break;

  case 89:
#line 639 "parser.y"
    {
      newstack(yyval);
      parser_stack(yyval).id(ID_statement_list_networks);
    }
#line 2168 "statement_list_y.tab.cpp"
    break;

  case 90:
#line 647 "parser.y"
    {
      newstack(yyval);
      parser_stack(yyval).id(ID_statement_list_network);
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-1])), 
        std::move(parser_stack(yyvsp[0])));
    }
#line 2179 "statement_list_y.tab.cpp"
    break;

  case 92:
#line 659 "parser.y"
    {
      newstack(yyval);
      parser_stack(yyval) = convert_title("");
    }
#line 2188 "statement_list_y.tab.cpp"
    break;

  case 94:
#line 668 "parser.y"
    {
      newstack(yyval);
      parser_stack(yyval).id(ID_statement_list_instructions);
    }
#line 2197 "statement_list_y.tab.cpp"
    break;

  case 96:
#line 681 "parser.y"
    {
      yyval = yyvsp[-1];
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[0])));
    }
#line 2206 "statement_list_y.tab.cpp"
    break;

  case 97:
#line 686 "parser.y"
    {
      newstack(yyval);
      parser_stack(yyval).id(ID_statement_list_instructions);
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[0])));
    }
#line 2216 "statement_list_y.tab.cpp"
    break;

  case 98:
#line 695 "parser.y"
    {
      yyval = yyvsp[-1];
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-2])));
    }
#line 2225 "statement_list_y.tab.cpp"
    break;

  case 100:
#line 704 "parser.y"
    {
      newstack(yyval);
       // ID of expression is nil to indicate that there is no label
    }
#line 2234 "statement_list_y.tab.cpp"
    break;

  case 104:
#line 721 "parser.y"
    {
      newstack(yyval);
      parser_stack(yyval).id(ID_statement_list_instruction);
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-1])), 
        std::move(parser_stack(yyvsp[0])));
    }
#line 2245 "statement_list_y.tab.cpp"
    break;

  case 106:
#line 732 "parser.y"
    {
      newstack(yyval);
      // ID of expression is nil to indicate that there is no operand
    }
#line 2254 "statement_list_y.tab.cpp"
    break;

  case 107:
#line 740 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_load);
    }
#line 2263 "statement_list_y.tab.cpp"
    break;

  case 108:
#line 745 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_transfer);
    }
#line 2272 "statement_list_y.tab.cpp"
    break;

  case 109:
#line 750 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_nop);
    }
#line 2281 "statement_list_y.tab.cpp"
    break;

  case 110:
#line 755 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_const_add);
    }
#line 2290 "statement_list_y.tab.cpp"
    break;

  case 111:
#line 760 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_int_add);
    }
#line 2299 "statement_list_y.tab.cpp"
    break;

  case 112:
#line 765 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_int_sub);
    }
#line 2308 "statement_list_y.tab.cpp"
    break;

  case 113:
#line 770 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_int_mul);
    }
#line 2317 "statement_list_y.tab.cpp"
    break;

  case 114:
#line 775 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_int_div);
    }
#line 2326 "statement_list_y.tab.cpp"
    break;

  case 115:
#line 780 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_int_eq);
    }
#line 2335 "statement_list_y.tab.cpp"
    break;

  case 116:
#line 785 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_int_neq);
    }
#line 2344 "statement_list_y.tab.cpp"
    break;

  case 117:
#line 790 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_int_gt);
    }
#line 2353 "statement_list_y.tab.cpp"
    break;

  case 118:
#line 795 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_int_lt);
    }
#line 2362 "statement_list_y.tab.cpp"
    break;

  case 119:
#line 800 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_int_gte);
    }
#line 2371 "statement_list_y.tab.cpp"
    break;

  case 120:
#line 805 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_int_lte);
    }
#line 2380 "statement_list_y.tab.cpp"
    break;

  case 121:
#line 810 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_real_add);
    }
#line 2389 "statement_list_y.tab.cpp"
    break;

  case 122:
#line 815 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_real_sub);
    }
#line 2398 "statement_list_y.tab.cpp"
    break;

  case 123:
#line 820 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_real_mul);
    }
#line 2407 "statement_list_y.tab.cpp"
    break;

  case 124:
#line 825 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_real_div);
    }
#line 2416 "statement_list_y.tab.cpp"
    break;

  case 125:
#line 830 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_real_eq);
    }
#line 2425 "statement_list_y.tab.cpp"
    break;

  case 126:
#line 835 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_real_neq);
    }
#line 2434 "statement_list_y.tab.cpp"
    break;

  case 127:
#line 840 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_real_gt);
    }
#line 2443 "statement_list_y.tab.cpp"
    break;

  case 128:
#line 845 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_real_lt);
    }
#line 2452 "statement_list_y.tab.cpp"
    break;

  case 129:
#line 850 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_real_gte);
    }
#line 2461 "statement_list_y.tab.cpp"
    break;

  case 130:
#line 855 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_real_lte);
    }
#line 2470 "statement_list_y.tab.cpp"
    break;

  case 131:
#line 860 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_dint_add);
    }
#line 2479 "statement_list_y.tab.cpp"
    break;

  case 132:
#line 865 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_dint_sub);
    }
#line 2488 "statement_list_y.tab.cpp"
    break;

  case 133:
#line 870 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_dint_mul);
    }
#line 2497 "statement_list_y.tab.cpp"
    break;

  case 134:
#line 875 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_dint_div);
    }
#line 2506 "statement_list_y.tab.cpp"
    break;

  case 135:
#line 880 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_dint_eq);
    }
#line 2515 "statement_list_y.tab.cpp"
    break;

  case 136:
#line 885 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_dint_neq);
    }
#line 2524 "statement_list_y.tab.cpp"
    break;

  case 137:
#line 890 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_dint_gt);
    }
#line 2533 "statement_list_y.tab.cpp"
    break;

  case 138:
#line 895 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_dint_lt);
    }
#line 2542 "statement_list_y.tab.cpp"
    break;

  case 139:
#line 900 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_dint_gte);
    }
#line 2551 "statement_list_y.tab.cpp"
    break;

  case 140:
#line 905 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_accu_dint_lte);
    }
#line 2560 "statement_list_y.tab.cpp"
    break;

  case 141:
#line 910 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_and);
    }
#line 2569 "statement_list_y.tab.cpp"
    break;

  case 142:
#line 915 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_and_not);
    }
#line 2578 "statement_list_y.tab.cpp"
    break;

  case 143:
#line 920 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_or);
    }
#line 2587 "statement_list_y.tab.cpp"
    break;

  case 144:
#line 925 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_or_not);
    }
#line 2596 "statement_list_y.tab.cpp"
    break;

  case 145:
#line 930 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_xor);
    }
#line 2605 "statement_list_y.tab.cpp"
    break;

  case 146:
#line 935 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_xor_not);
    }
#line 2614 "statement_list_y.tab.cpp"
    break;

  case 147:
#line 940 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_and_nested);
    }
#line 2623 "statement_list_y.tab.cpp"
    break;

  case 148:
#line 945 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_and_not_nested);
    }
#line 2632 "statement_list_y.tab.cpp"
    break;

  case 149:
#line 950 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_or_nested);
    }
#line 2641 "statement_list_y.tab.cpp"
    break;

  case 150:
#line 955 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_or_not_nested);
    }
#line 2650 "statement_list_y.tab.cpp"
    break;

  case 151:
#line 960 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_xor_nested);
    }
#line 2659 "statement_list_y.tab.cpp"
    break;

  case 152:
#line 965 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_xor_not_nested);
    }
#line 2668 "statement_list_y.tab.cpp"
    break;

  case 153:
#line 970 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_nesting_closed);
    }
#line 2677 "statement_list_y.tab.cpp"
    break;

  case 154:
#line 975 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_assign);
    }
#line 2686 "statement_list_y.tab.cpp"
    break;

  case 155:
#line 980 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_set_rlo);
    }
#line 2695 "statement_list_y.tab.cpp"
    break;

  case 156:
#line 985 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_clr_rlo);
    }
#line 2704 "statement_list_y.tab.cpp"
    break;

  case 157:
#line 990 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_set);
    }
#line 2713 "statement_list_y.tab.cpp"
    break;

  case 158:
#line 995 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_reset);
    }
#line 2722 "statement_list_y.tab.cpp"
    break;

  case 159:
#line 1000 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_not);
    }
#line 2731 "statement_list_y.tab.cpp"
    break;

  case 160:
#line 1005 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_jump_unconditional);
    }
#line 2740 "statement_list_y.tab.cpp"
    break;

  case 161:
#line 1010 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_jump_conditional);
    }
#line 2749 "statement_list_y.tab.cpp"
    break;

  case 162:
#line 1015 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_jump_conditional_not);
    }
#line 2758 "statement_list_y.tab.cpp"
    break;

  case 165:
#line 1028 "parser.y"
    {
      yyval = yyvsp[0];
    }
#line 2766 "statement_list_y.tab.cpp"
    break;

  case 166:
#line 1032 "parser.y"
    {
      yyval = yyvsp[0];
    }
#line 2774 "statement_list_y.tab.cpp"
    break;

  case 170:
#line 1045 "parser.y"
    {
      newstack(yyval);
      parser_stack(yyval).id(ID_statement_list_instruction);
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-3])), 
        std::move(parser_stack(yyvsp[-2])), std::move(parser_stack(yyvsp[-1])));    
      std::move(parser_stack(yyvsp[0]).operands().begin(), 
        parser_stack(yyvsp[0]).operands().end(), 
        std::back_inserter(parser_stack(yyval).operands()));
    }
#line 2788 "statement_list_y.tab.cpp"
    break;

  case 171:
#line 1058 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).id(ID_statement_list_call);
    }
#line 2797 "statement_list_y.tab.cpp"
    break;

  case 172:
#line 1066 "parser.y"
    {
      newstack(yyval);
      parser_stack(yyval) = 
        symbol_exprt::typeless(parser_stack(yyvsp[0]).get(ID_value));
    }
#line 2807 "statement_list_y.tab.cpp"
    break;

  case 173:
#line 1075 "parser.y"
    {
      yyval = yyvsp[-1];
    }
#line 2815 "statement_list_y.tab.cpp"
    break;

  case 174:
#line 1079 "parser.y"
    {
      newstack(yyval);
    }
#line 2823 "statement_list_y.tab.cpp"
    break;

  case 175:
#line 1086 "parser.y"
    {
      yyval = yyvsp[-2];
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[0])));
    }
#line 2832 "statement_list_y.tab.cpp"
    break;

  case 176:
#line 1091 "parser.y"
    {
      newstack(yyval);
      parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[0])));
    }
#line 2841 "statement_list_y.tab.cpp"
    break;

  case 177:
#line 1099 "parser.y"
    {
      newstack(yyval);
      parser_stack(yyval) = code_frontend_assignt(std::move(parser_stack(yyvsp[-2])),
        std::move(parser_stack(yyvsp[0])));
    }
#line 2851 "statement_list_y.tab.cpp"
    break;

  case 178:
#line 1107 "parser.y"
    {
      yyval = yyvsp[0];
      parser_stack(yyval).set(
        ID_statement_list_type, ID_statement_list_data_block);
    }
#line 2861 "statement_list_y.tab.cpp"
    break;

  case 179:
#line 1113 "parser.y"
    {
      newstack(yyval);
    }
#line 2869 "statement_list_y.tab.cpp"
    break;

  case 180:
#line 1121 "parser.y"
    {
      PARSER.add_tag_list(parser_stack(yyvsp[-1]));
    }
#line 2877 "statement_list_y.tab.cpp"
    break;

  case 182:
#line 1129 "parser.y"
    {
      newstack(yyval);
    }
#line 2885 "statement_list_y.tab.cpp"
    break;

  case 183:
#line 1136 "parser.y"
    {
      yyval = yyvsp[-2];
      symbol_exprt sym{parser_stack(yyvsp[-1]).get(ID_identifier), parser_stack(yyvsp[0]).type()};
      parser_stack(yyval).add_to_operands(std::move(sym));
    }
#line 2895 "statement_list_y.tab.cpp"
    break;

  case 184:
#line 1142 "parser.y"
    {
      newstack(yyval);
      symbol_exprt sym{parser_stack(yyvsp[-1]).get(ID_identifier), parser_stack(yyvsp[0]).type()};
      parser_stack(yyval).add_to_operands(std::move(sym));
    }
#line 2905 "statement_list_y.tab.cpp"
    break;


#line 2909 "statement_list_y.tab.cpp"

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
#line 1148 "parser.y"

