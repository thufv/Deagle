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
#define yyparse         yyjsilparse
#define yylex           yyjsillex
#define yyerror         yyjsilerror
#define yydebug         yyjsildebug
#define yynerrs         yyjsilnerrs
#define yylval          yyjsillval
#define yychar          yyjsilchar

/* First part of user prologue.  */
#line 1 "parser.y"


// #define YYDEBUG 1
#define PARSER jsil_parser

#include "jsil_parser.h"

int yyjsillex();
extern char *yyjsiltext;

#define YYSTYPE unsigned
#define YYSTYPE_IS_TRIVIAL 1

#include <goto-programs/goto_instruction_code.h>

#include <util/string_constant.h>

#include "jsil_y.tab.h"

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

/*** token declaration **************************************************/

#line 112 "jsil_y.tab.cpp"

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
#ifndef YY_YYJSIL_JSIL_Y_TAB_H_INCLUDED
# define YY_YYJSIL_JSIL_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yyjsildebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOK_SCANNER_ERROR = 258,
    TOK_NEWLINE = 259,
    TOK_PROCEDURE = 260,
    TOK_RETURNS = 261,
    TOK_TO = 262,
    TOK_THROWS = 263,
    TOK_EVAL = 264,
    TOK_LABEL = 265,
    TOK_GOTO = 266,
    TOK_SKIP = 267,
    TOK_WITH = 268,
    TOK_NEW = 269,
    TOK_HAS_FIELD = 270,
    TOK_DELETE = 271,
    TOK_PROTO_FIELD = 272,
    TOK_PROTO_OBJ = 273,
    TOK_REF = 274,
    TOK_FIELD = 275,
    TOK_BASE = 276,
    TOK_TYPEOF = 277,
    TOK_NULL = 278,
    TOK_UNDEFINED = 279,
    TOK_EMPTY = 280,
    TOK_TRUE = 281,
    TOK_FALSE = 282,
    TOK_PROTO = 283,
    TOK_FID = 284,
    TOK_SCOPE = 285,
    TOK_CONSTRUCTID = 286,
    TOK_PRIMVALUE = 287,
    TOK_TARGETFUNCTION = 288,
    TOK_CLASS = 289,
    TOK_NUM_TO_STRING = 290,
    TOK_STRING_TO_NUM = 291,
    TOK_NUM_TO_INT32 = 292,
    TOK_NUM_TO_UINT32 = 293,
    TOK_MEMBER_REFERENCE = 294,
    TOK_VARIABLE_REFERENCE = 295,
    TOK_T_NULL = 296,
    TOK_T_UNDEFINED = 297,
    TOK_T_BOOLEAN = 298,
    TOK_T_STRING = 299,
    TOK_T_NUMBER = 300,
    TOK_T_BUILTIN_OBJECT = 301,
    TOK_T_USER_OBJECT = 302,
    TOK_T_OBJECT = 303,
    TOK_T_REFERENCE = 304,
    TOK_DEFEQ = 305,
    TOK_LEQ = 306,
    TOK_AND = 307,
    TOK_OR = 308,
    TOK_SUBTYPE_OF = 309,
    TOK_LEFT_SHIFT = 310,
    TOK_SIGNED_RIGHT_SHIFT = 311,
    TOK_UNSIGNED_RIGHT_SHIFT = 312,
    TOK_NOT = 313,
    TOK_IDENTIFIER = 314,
    TOK_FLOATING = 315,
    TOK_STRING = 316,
    TOK_BUILTIN_LOC = 317,
    TOK_BUILTIN_IDENTIFIER = 318,
    TOK_SPEC_IDENTIFIER = 319
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yyjsillval;

int yyjsilparse (void);

#endif /* !YY_YYJSIL_JSIL_Y_TAB_H_INCLUDED  */



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
#define YYFINAL  10
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   858

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  84
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  109
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  192

#define YYUNDEFTOK  2
#define YYMAXUTOK   319


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
       2,     2,     2,    83,     2,     2,     2,    78,    80,     2,
      65,    66,    76,    74,    69,    75,     2,    77,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    79,     2,
      73,    72,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    70,     2,    71,    82,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    67,    81,    68,     2,     2,     2,     2,
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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   121,   121,   124,   125,   128,   161,   162,   167,   171,
     177,   178,   187,   190,   193,   198,   206,   209,   212,   217,
     224,   228,   234,   241,   246,   255,   259,   264,   272,   273,
     287,   292,   299,   304,   311,   318,   328,   331,   338,   341,
     344,   349,   356,   357,   364,   365,   370,   374,   384,   391,
     398,   407,   408,   412,   416,   420,   424,   428,   429,   435,
     436,   437,   440,   444,   448,   452,   456,   460,   464,   470,
     471,   472,   473,   476,   480,   484,   490,   494,   498,   502,
     506,   512,   516,   520,   524,   530,   534,   538,   542,   546,
     550,   556,   560,   564,   568,   572,   576,   580,   586,   590,
     594,   598,   602,   606,   610,   614,   618,   619,   626,   630
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOK_SCANNER_ERROR", "\"<newline>\"",
  "\"procedure\"", "\"returns\"", "\"to\"", "\"throws\"", "\"eval\"",
  "\"label\"", "\"goto\"", "\"skip\"", "\"with\"", "\"new\"",
  "\"hasField\"", "\"delete\"", "\"protoField\"", "\"protoObj\"",
  "\"ref\"", "\"field\"", "\"base\"", "\"typeOf\"", "\"null\"",
  "\"#undefined\"", "\"#empty\"", "\"true\"", "\"false\"", "\"#proto\"",
  "\"#fid\"", "\"#scope\"", "\"#constructid\"", "\"#primvalue\"",
  "\"#targetfunction\"", "\"#class\"", "\"num_to_string\"",
  "\"string_to_num\"", "\"num_to_int32\"", "\"num_to_uint32\"",
  "\"#MemberReference\"", "\"#VariableReference\"", "\"#Null\"",
  "\"#Undefined\"", "\"#Boolean\"", "\"#String\"", "\"#Number\"",
  "\"#BuiltinObject\"", "\"#UserObject\"", "\"#Object\"", "\"#Reference\"",
  "\":=\"", "\"<=\"", "\"and\"", "\"or\"", "\"<:\"", "\"<<\"", "\">>\"",
  "\">>>\"", "\"not\"", "TOK_IDENTIFIER", "TOK_FLOATING", "TOK_STRING",
  "TOK_BUILTIN_LOC", "TOK_BUILTIN_IDENTIFIER", "TOK_SPEC_IDENTIFIER",
  "'('", "')'", "'{'", "'}'", "','", "'['", "']'", "'='", "'<'", "'+'",
  "'-'", "'*'", "'/'", "'%'", "':'", "'&'", "'|'", "'^'", "'!'", "$accept",
  "program", "procedure_decls", "procedure_decl", "proc_ident",
  "proc_ident_expr", "parameters_opt", "parameters", "statements_opt",
  "statements", "statement", "instruction", "rhs", "with_opt",
  "expressions_opt", "expressions", "expression", "atom_expression",
  "literal", "builtin_field", "binary_op", "compare_op", "arithmetic_op",
  "boolean_op", "bitwise_op", "unary_op", "jsil_type", "ref_type", YY_NULLPTR
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
     315,   316,   317,   318,   319,    40,    41,   123,   125,    44,
      91,    93,    61,    60,    43,    45,    42,    47,    37,    58,
      38,   124,    94,    33
};
# endif

#define YYPACT_NINF (-147)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-12)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       0,    -3,     7,     0,  -147,  -147,  -147,  -147,  -147,   -52,
    -147,  -147,   -45,  -147,   -50,   -51,    11,   -39,   -38,  -147,
      12,   -37,    15,   -35,    18,   -33,   -36,    -2,  -147,   -32,
     -55,  -147,   -22,   185,   -34,    -2,  -147,    25,  -147,  -147,
     185,   120,   -30,   -29,   -26,   -25,  -147,  -147,  -147,  -147,
    -147,  -147,  -147,  -147,  -147,  -147,  -147,  -147,  -147,  -147,
    -147,  -147,  -147,  -147,  -147,  -147,  -147,  -147,  -147,  -147,
    -147,  -147,  -147,  -147,  -147,  -147,  -147,  -147,   185,  -147,
    -147,    31,  -147,  -147,  -147,   185,  -147,  -147,  -147,  -147,
    -147,   200,   -24,   -23,   -21,   -20,   -18,   -17,   -16,   185,
    -147,   -15,  -147,   776,   185,   185,   185,   185,   232,  -147,
    -147,  -147,  -147,  -147,  -147,  -147,   185,  -147,  -147,  -147,
    -147,  -147,  -147,  -147,  -147,  -147,  -147,  -147,   185,  -147,
    -147,  -147,  -147,  -147,   -27,   -14,   185,   185,   185,   185,
     264,   185,   296,   328,   360,   392,  -147,   424,  -147,   -11,
    -147,   456,   488,   520,   552,   185,   -13,    -7,   776,   185,
    -147,  -147,  -147,     1,    -5,   185,   185,   185,   185,   584,
      17,   185,   616,   185,  -147,   648,   680,   712,   744,  -147,
      -4,  -147,   776,   -28,   776,  -147,  -147,  -147,  -147,  -147,
       3,  -147
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     2,     3,     7,     6,     8,     9,     0,
       1,     4,    12,    14,     0,    13,     0,     0,     0,    15,
       0,     0,     0,     0,     0,     0,     0,    16,    20,     0,
       0,    25,     0,     0,     0,    17,    18,     0,    22,    23,
       0,     0,     0,     0,     0,     0,    52,    53,    54,    55,
      56,    62,    63,    64,    65,    66,    67,    68,    93,    94,
      95,    96,   108,   109,    98,    99,   100,   101,   102,   103,
     104,   105,   107,    91,    51,    57,    58,    59,     0,    92,
      97,     0,    42,    44,    61,     0,    60,   106,     5,    19,
      21,     0,     0,     0,     0,     0,     0,    51,    58,     0,
      10,     0,    26,    28,     0,     0,     0,     0,     0,    75,
      81,    82,    83,    88,    89,    90,     0,    73,    74,    76,
      77,    78,    79,    80,    84,    85,    86,    87,     0,    69,
      70,    71,    72,    45,     0,     0,     0,     0,     0,     0,
       0,    38,     0,     0,     0,     0,    46,     0,    43,     0,
      30,     0,     0,     0,     0,     0,     0,    39,    40,     0,
      48,    49,    50,     0,     0,     0,     0,     0,     0,     0,
      36,     0,     0,     0,    24,     0,     0,     0,     0,    32,
       0,    29,    41,     0,    27,    31,    33,    34,    35,    37,
       0,    47
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -147,  -147,  -147,    30,     2,  -147,  -147,  -147,  -147,  -147,
      28,  -147,  -147,  -147,  -147,  -147,   -40,   -82,  -147,  -147,
    -147,  -147,  -147,  -147,  -147,  -147,  -147,  -146
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     4,     9,   101,    14,    15,    34,    35,
      36,    37,   102,   181,   156,   157,    81,    82,    83,    84,
     128,   129,   130,   131,   132,    85,    86,    87
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      91,   103,    28,   133,    39,     1,     5,    10,    29,    30,
      31,    62,    63,    12,    13,    40,    16,    18,    17,    21,
      19,    20,    22,    23,    24,    25,    26,    38,    41,    90,
     180,    27,   149,    11,    88,   104,   105,   190,   108,   106,
     107,   135,   136,   100,   137,   138,   148,   139,    -6,   -11,
     141,   173,   150,   170,   174,   189,     6,    32,   164,   140,
       7,     8,   171,    89,   142,   143,   144,   145,    33,   191,
       0,     0,     0,     0,     0,     0,   147,     0,     0,     0,
       0,     0,   109,   110,   111,   112,   113,   114,   115,     0,
       0,     0,     0,     0,     0,     0,   151,   152,   153,   154,
     116,   158,     0,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,     0,   169,     0,     0,     0,   172,
       0,     0,     0,     0,     0,   175,   176,   177,   178,     5,
       0,   182,     0,   184,    92,    93,    94,    95,    96,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
       0,     0,     0,     0,     0,     0,     0,     0,    73,    97,
      75,    98,    77,     7,     8,    78,     0,     0,     0,     0,
      99,     0,     0,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,    80,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,     0,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,     0,     0,
      78,   109,   110,   111,   112,   113,   114,   115,     0,     0,
      79,     0,     0,     0,     0,     0,     0,     0,    80,     0,
       0,   134,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   109,   110,   111,   112,   113,   114,   115,
       0,     0,     0,     0,     0,     0,     0,     0,   146,     0,
       0,     0,     0,     0,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   109,   110,   111,   112,   113,
     114,   115,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   155,     0,     0,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   109,   110,   111,
     112,   113,   114,   115,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   159,     0,     0,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   109,
     110,   111,   112,   113,   114,   115,     0,     0,     0,     0,
       0,     0,     0,     0,   160,     0,     0,     0,     0,     0,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   109,   110,   111,   112,   113,   114,   115,     0,     0,
       0,     0,     0,     0,     0,     0,   161,     0,     0,     0,
       0,     0,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   109,   110,   111,   112,   113,   114,   115,
       0,     0,     0,     0,     0,     0,     0,     0,   162,     0,
       0,     0,     0,     0,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   109,   110,   111,   112,   113,
     114,   115,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   163,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   109,   110,   111,
     112,   113,   114,   115,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   165,     0,     0,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   109,
     110,   111,   112,   113,   114,   115,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   166,     0,     0,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   109,   110,   111,   112,   113,   114,   115,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   167,
       0,     0,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   109,   110,   111,   112,   113,   114,   115,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   168,     0,     0,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   109,   110,   111,   112,   113,
     114,   115,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   179,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   109,   110,   111,
     112,   113,   114,   115,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   183,     0,     0,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   109,
     110,   111,   112,   113,   114,   115,     0,     0,     0,     0,
       0,     0,     0,     0,   185,     0,     0,     0,     0,     0,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   109,   110,   111,   112,   113,   114,   115,     0,     0,
       0,     0,     0,     0,     0,     0,   186,     0,     0,     0,
       0,     0,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   109,   110,   111,   112,   113,   114,   115,
       0,     0,     0,     0,     0,     0,     0,     0,   187,     0,
       0,     0,     0,     0,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   109,   110,   111,   112,   113,
     114,   115,     0,     0,     0,     0,     0,     0,     0,     0,
     188,     0,     0,     0,     0,     0,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   109,   110,   111,
     112,   113,   114,   115,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127
};

static const yytype_int16 yycheck[] =
{
      40,    41,     4,    85,    59,     5,     9,     0,    10,    11,
      12,    39,    40,    65,    59,    70,    66,     6,    69,     7,
      59,    59,    59,     8,    59,     7,    59,    59,    50,     4,
      13,    67,    59,     3,    68,    65,    65,   183,    78,    65,
      65,    65,    65,    41,    65,    65,   128,    65,    65,    65,
      65,    50,    66,    66,    59,    59,    59,    59,    69,    99,
      63,    64,    69,    35,   104,   105,   106,   107,    70,    66,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    51,    52,    53,    54,    55,    56,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   137,   138,   139,
      69,   141,    -1,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,   155,    -1,    -1,    -1,   159,
      -1,    -1,    -1,    -1,    -1,   165,   166,   167,   168,     9,
      -1,   171,    -1,   173,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    59,
      60,    61,    62,    63,    64,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    58,    59,    60,    61,    62,    -1,    -1,
      65,    51,    52,    53,    54,    55,    56,    57,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,
      -1,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    51,    52,    53,    54,    55,    56,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    51,    52,    53,    54,    55,
      56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    -1,    -1,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    51,    52,    53,
      54,    55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    51,
      52,    53,    54,    55,    56,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    51,    52,    53,    54,    55,    56,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    51,    52,    53,    54,    55,    56,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    51,    52,    53,    54,    55,
      56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    51,    52,    53,
      54,    55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    51,
      52,    53,    54,    55,    56,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    51,    52,    53,    54,    55,    56,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      -1,    -1,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    51,    52,    53,    54,    55,    56,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    51,    52,    53,    54,    55,
      56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    51,    52,    53,
      54,    55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    51,
      52,    53,    54,    55,    56,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    51,    52,    53,    54,    55,    56,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    51,    52,    53,    54,    55,    56,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    51,    52,    53,    54,    55,
      56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    51,    52,    53,
      54,    55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     5,    85,    86,    87,     9,    59,    63,    64,    88,
       0,    87,    65,    59,    90,    91,    66,    69,     6,    59,
      59,     7,    59,     8,    59,     7,    59,    67,     4,    10,
      11,    12,    59,    70,    92,    93,    94,    95,    59,    59,
      70,    50,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    58,    59,    60,    61,    62,    65,    75,
      83,   100,   101,   102,   103,   109,   110,   111,    68,    94,
       4,   100,    14,    15,    16,    17,    18,    59,    61,    70,
      88,    89,    96,   100,    65,    65,    65,    65,   100,    51,
      52,    53,    54,    55,    56,    57,    69,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,   104,   105,
     106,   107,   108,   101,    71,    65,    65,    65,    65,    65,
     100,    65,   100,   100,   100,   100,    66,   100,   101,    59,
      66,   100,   100,   100,   100,    69,    98,    99,   100,    69,
      66,    66,    66,    71,    69,    69,    69,    69,    69,   100,
      66,    69,   100,    50,    59,   100,   100,   100,   100,    71,
      13,    97,   100,    69,   100,    66,    66,    66,    66,    59,
     111,    66
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    84,    85,    86,    86,    87,    88,    88,    88,    88,
      89,    89,    90,    90,    91,    91,    92,    92,    93,    93,
      94,    94,    95,    95,    95,    95,    95,    95,    96,    96,
      96,    96,    96,    96,    96,    96,    97,    97,    98,    98,
      99,    99,   100,   100,   101,   101,   101,   101,   101,   101,
     101,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   103,   103,   103,   103,   103,   103,   103,   104,
     104,   104,   104,   105,   105,   105,   106,   106,   106,   106,
     106,   107,   107,   107,   107,   108,   108,   108,   108,   108,
     108,   109,   109,   109,   109,   109,   109,   109,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   111,   111
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,    16,     1,     1,     1,     1,
       1,     1,     0,     1,     1,     3,     0,     1,     1,     2,
       1,     2,     2,     2,     7,     1,     3,     7,     1,     5,
       3,     6,     5,     6,     6,     6,     0,     2,     0,     1,
       1,     3,     1,     3,     1,     2,     3,     8,     4,     4,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1
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
  case 5:
#line 132 "parser.y"
              {
                symbol_exprt proc(to_symbol_expr(parser_stack(yyvsp[-14])));
                code_typet::parameterst parameters;
                forall_operands(it, parser_stack(yyvsp[-12]))
                {
                  symbol_exprt s(to_symbol_expr(*it));
                  code_typet::parametert p(typet{});
                  p.set_identifier(s.get_identifier());
                  parameters.push_back(p);
                }
                proc.type() = code_typet(std::move(parameters), typet());

                symbol_exprt rv(to_symbol_expr(parser_stack(yyvsp[-9])));
                symbol_exprt rl(to_symbol_expr(parser_stack(yyvsp[-7])));

                symbol_exprt tv(to_symbol_expr(parser_stack(yyvsp[-5])));
                symbol_exprt tl(to_symbol_expr(parser_stack(yyvsp[-3])));

                jsil_declarationt decl;
                decl.add_declarator(proc);
                decl.add_returns(rv.get_identifier(), rl.get_identifier());
                decl.add_throws(tv.get_identifier(), tl.get_identifier());
                if(parser_stack(yyvsp[-1]).is_not_nil())
                  decl.add_value(to_code_block(to_code(parser_stack(yyvsp[-1]))));

                PARSER.parse_tree.items.push_back(decl);
              }
#line 1708 "jsil_y.tab.cpp"
    break;

  case 7:
#line 163 "parser.y"
          {
            symbol_exprt e = symbol_exprt::typeless("eval");
            newstack(yyval).swap(e);
          }
#line 1717 "jsil_y.tab.cpp"
    break;

  case 8:
#line 168 "parser.y"
          {
            parser_stack(yyval).set("proc_type", "builtin");
          }
#line 1725 "jsil_y.tab.cpp"
    break;

  case 9:
#line 172 "parser.y"
          {
            parser_stack(yyval).set("proc_type", "spec");
          }
#line 1733 "jsil_y.tab.cpp"
    break;

  case 11:
#line 179 "parser.y"
               {
                 symbol_exprt s = symbol_exprt::typeless(
                   to_string_constant(parser_stack(yyval)).get_value());
                 parser_stack(yyval).swap(s);
               }
#line 1743 "jsil_y.tab.cpp"
    break;

  case 12:
#line 187 "parser.y"
              {
                newstack(yyval);
              }
#line 1751 "jsil_y.tab.cpp"
    break;

  case 14:
#line 194 "parser.y"
          {
            newstack(yyval).id(ID_parameters);
            parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[0])));
          }
#line 1760 "jsil_y.tab.cpp"
    break;

  case 15:
#line 199 "parser.y"
          {
            yyval=yyvsp[-2];
            parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[0])));
          }
#line 1769 "jsil_y.tab.cpp"
    break;

  case 16:
#line 206 "parser.y"
              {
                newstack(yyval);
              }
#line 1777 "jsil_y.tab.cpp"
    break;

  case 18:
#line 213 "parser.y"
          {
            code_blockt b({static_cast<codet &>(parser_stack(yyvsp[0]))});
            newstack(yyval).swap(b);
          }
#line 1786 "jsil_y.tab.cpp"
    break;

  case 19:
#line 218 "parser.y"
          {
            yyval=yyvsp[-1];
            parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[0])));
          }
#line 1795 "jsil_y.tab.cpp"
    break;

  case 20:
#line 225 "parser.y"
         {
           newstack(yyval)=code_skipt();
         }
#line 1803 "jsil_y.tab.cpp"
    break;

  case 21:
#line 229 "parser.y"
         {
           yyval=yyvsp[-1];
         }
#line 1811 "jsil_y.tab.cpp"
    break;

  case 22:
#line 235 "parser.y"
           {
             code_labelt l(
               to_symbol_expr(parser_stack(yyvsp[0])).get_identifier(),
               code_skipt());
             newstack(yyval).swap(l);
           }
#line 1822 "jsil_y.tab.cpp"
    break;

  case 23:
#line 242 "parser.y"
           {
             code_gotot g(to_symbol_expr(parser_stack(yyvsp[0])).get_identifier());
             newstack(yyval).swap(g);
           }
#line 1831 "jsil_y.tab.cpp"
    break;

  case 24:
#line 247 "parser.y"
           {
             code_gotot lt(to_symbol_expr(parser_stack(yyvsp[-2])).get_identifier());
             code_gotot lf(to_symbol_expr(parser_stack(yyvsp[0])).get_identifier());

             code_ifthenelset ite(parser_stack(yyvsp[-4]), std::move(lt), std::move(lf));

             newstack(yyval).swap(ite);
           }
#line 1844 "jsil_y.tab.cpp"
    break;

  case 25:
#line 256 "parser.y"
           {
             newstack(yyval)=code_skipt();
           }
#line 1852 "jsil_y.tab.cpp"
    break;

  case 26:
#line 260 "parser.y"
           {
             code_assignt a(parser_stack(yyvsp[-2]), parser_stack(yyvsp[0]));
             newstack(yyval).swap(a);
           }
#line 1861 "jsil_y.tab.cpp"
    break;

  case 27:
#line 265 "parser.y"
           {
             index_exprt i(parser_stack(yyvsp[-5]), parser_stack(yyvsp[-3]));
             code_assignt a(i, parser_stack(yyvsp[0]));
             newstack(yyval).swap(a);
           }
#line 1871 "jsil_y.tab.cpp"
    break;

  case 29:
#line 274 "parser.y"
   {
     side_effect_expr_function_callt f(parser_stack(yyvsp[-4]), {}, typet{}, {});
     if(parser_stack(yyvsp[-2]).is_not_nil())
       f.arguments().swap(parser_stack(yyvsp[-2]).operands());

     newstack(yyval).swap(f);

     if(parser_stack(yyvsp[0]).is_not_nil())
     {
       with_exprt w(parser_stack(yyval), parser_stack(yyvsp[0]), nil_exprt());
       parser_stack(yyval).swap(w);
     }
   }
#line 1889 "jsil_y.tab.cpp"
    break;

  case 30:
#line 288 "parser.y"
   {
     exprt n("new");
     newstack(yyval).swap(n);
   }
#line 1898 "jsil_y.tab.cpp"
    break;

  case 31:
#line 293 "parser.y"
   {
     exprt has_field("hasField");
     has_field.add_to_operands(std::move(parser_stack(yyvsp[-3])), std::move(parser_stack(yyvsp[-1])));

     newstack(yyval).swap(has_field);
   }
#line 1909 "jsil_y.tab.cpp"
    break;

  case 32:
#line 300 "parser.y"
   {
     index_exprt i(parser_stack(yyvsp[-3]), parser_stack(yyvsp[-1]));
     newstack(yyval).swap(i);
   }
#line 1918 "jsil_y.tab.cpp"
    break;

  case 33:
#line 305 "parser.y"
   {
     exprt d("delete");
     d.add_to_operands(std::move(parser_stack(yyvsp[-3])), std::move(parser_stack(yyvsp[-1])));

     newstack(yyval).swap(d);
   }
#line 1929 "jsil_y.tab.cpp"
    break;

  case 34:
#line 312 "parser.y"
   {
     exprt proto_field("protoField");
     proto_field.add_to_operands(std::move(parser_stack(yyvsp[-3])), std::move(parser_stack(yyvsp[-1])));

     newstack(yyval).swap(proto_field);
   }
#line 1940 "jsil_y.tab.cpp"
    break;

  case 35:
#line 319 "parser.y"
   {
     exprt proto_obj("protoObj");
     proto_obj.add_to_operands(std::move(parser_stack(yyvsp[-3])), std::move(parser_stack(yyvsp[-1])));

     newstack(yyval).swap(proto_obj);
   }
#line 1951 "jsil_y.tab.cpp"
    break;

  case 36:
#line 328 "parser.y"
        {
          newstack(yyval);
        }
#line 1959 "jsil_y.tab.cpp"
    break;

  case 37:
#line 332 "parser.y"
        {
          yyval=yyvsp[0];
        }
#line 1967 "jsil_y.tab.cpp"
    break;

  case 38:
#line 338 "parser.y"
               {
                 newstack(yyval);
               }
#line 1975 "jsil_y.tab.cpp"
    break;

  case 40:
#line 345 "parser.y"
           {
             newstack(yyval).id(ID_expression_list);
             parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[0])));
           }
#line 1984 "jsil_y.tab.cpp"
    break;

  case 41:
#line 350 "parser.y"
           {
             yyval=yyvsp[-2];
             parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[0])));
           }
#line 1993 "jsil_y.tab.cpp"
    break;

  case 43:
#line 358 "parser.y"
          {
            yyval=yyvsp[-1];
            parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[-2])), std::move(parser_stack(yyvsp[0])));
          }
#line 2002 "jsil_y.tab.cpp"
    break;

  case 45:
#line 366 "parser.y"
               {
                 yyval=yyvsp[-1];
                 parser_stack(yyval).add_to_operands(std::move(parser_stack(yyvsp[0])));
               }
#line 2011 "jsil_y.tab.cpp"
    break;

  case 46:
#line 371 "parser.y"
               {
                 yyval=yyvsp[-1];
               }
#line 2019 "jsil_y.tab.cpp"
    break;

  case 47:
#line 375 "parser.y"
               {
                 exprt ref("ref");
                 ref.add_to_operands(
                   std::move(parser_stack(yyvsp[-5])),
                   std::move(parser_stack(yyvsp[-3])),
                   std::move(parser_stack(yyvsp[-1])));

                 newstack(yyval).swap(ref);
               }
#line 2033 "jsil_y.tab.cpp"
    break;

  case 48:
#line 385 "parser.y"
               {
                 exprt field("field");
                 field.add_to_operands(std::move(parser_stack(yyvsp[-1])));

                 newstack(yyval).swap(field);
               }
#line 2044 "jsil_y.tab.cpp"
    break;

  case 49:
#line 392 "parser.y"
               {
                 exprt base(ID_base);
                 base.add_to_operands(std::move(parser_stack(yyvsp[-1])));

                 newstack(yyval).swap(base);
               }
#line 2055 "jsil_y.tab.cpp"
    break;

  case 50:
#line 399 "parser.y"
               {
                 exprt typeof_expr(ID_typeof);
                 typeof_expr.add_to_operands(std::move(parser_stack(yyvsp[-1])));

                 newstack(yyval).swap(typeof_expr);
               }
#line 2066 "jsil_y.tab.cpp"
    break;

  case 52:
#line 409 "parser.y"
       {
         newstack(yyval).id(ID_null);
       }
#line 2074 "jsil_y.tab.cpp"
    break;

  case 53:
#line 413 "parser.y"
       {
         newstack(yyval).id("undefined");
       }
#line 2082 "jsil_y.tab.cpp"
    break;

  case 54:
#line 417 "parser.y"
       {
         newstack(yyval).id(ID_empty);
       }
#line 2090 "jsil_y.tab.cpp"
    break;

  case 55:
#line 421 "parser.y"
       {
         newstack(yyval) = true_exprt();
       }
#line 2098 "jsil_y.tab.cpp"
    break;

  case 56:
#line 425 "parser.y"
       {
         newstack(yyval) = false_exprt();
       }
#line 2106 "jsil_y.tab.cpp"
    break;

  case 58:
#line 430 "parser.y"
       {
         constant_exprt c(to_string_constant(parser_stack(yyval))
           .get_value(), string_typet());
         parser_stack(yyval).swap(c);
       }
#line 2116 "jsil_y.tab.cpp"
    break;

  case 62:
#line 441 "parser.y"
             {
               newstack(yyval).id("proto");
             }
#line 2124 "jsil_y.tab.cpp"
    break;

  case 63:
#line 445 "parser.y"
             {
               newstack(yyval).id("fid");
             }
#line 2132 "jsil_y.tab.cpp"
    break;

  case 64:
#line 449 "parser.y"
             {
               newstack(yyval).id("scope");
             }
#line 2140 "jsil_y.tab.cpp"
    break;

  case 65:
#line 453 "parser.y"
             {
               newstack(yyval).id("constructid");
             }
#line 2148 "jsil_y.tab.cpp"
    break;

  case 66:
#line 457 "parser.y"
             {
               newstack(yyval).id("primvalue");
             }
#line 2156 "jsil_y.tab.cpp"
    break;

  case 67:
#line 461 "parser.y"
             {
               newstack(yyval).id("targetfunction");
             }
#line 2164 "jsil_y.tab.cpp"
    break;

  case 68:
#line 465 "parser.y"
             {
               newstack(yyval).id(ID_class);
             }
#line 2172 "jsil_y.tab.cpp"
    break;

  case 73:
#line 477 "parser.y"
          {
            newstack(yyval).id(ID_equal);
          }
#line 2180 "jsil_y.tab.cpp"
    break;

  case 74:
#line 481 "parser.y"
          {
            newstack(yyval).id(ID_lt);
          }
#line 2188 "jsil_y.tab.cpp"
    break;

  case 75:
#line 485 "parser.y"
          {
            newstack(yyval).id(ID_le);
          }
#line 2196 "jsil_y.tab.cpp"
    break;

  case 76:
#line 491 "parser.y"
             {
               newstack(yyval).id(ID_plus);
             }
#line 2204 "jsil_y.tab.cpp"
    break;

  case 77:
#line 495 "parser.y"
             {
               newstack(yyval).id(ID_minus);
             }
#line 2212 "jsil_y.tab.cpp"
    break;

  case 78:
#line 499 "parser.y"
             {
               newstack(yyval).id(ID_mult);
             }
#line 2220 "jsil_y.tab.cpp"
    break;

  case 79:
#line 503 "parser.y"
             {
               newstack(yyval).id(ID_div);
             }
#line 2228 "jsil_y.tab.cpp"
    break;

  case 80:
#line 507 "parser.y"
             {
               newstack(yyval).id(ID_mod);
             }
#line 2236 "jsil_y.tab.cpp"
    break;

  case 81:
#line 513 "parser.y"
          {
            newstack(yyval).id(ID_and);
          }
#line 2244 "jsil_y.tab.cpp"
    break;

  case 82:
#line 517 "parser.y"
          {
            newstack(yyval).id(ID_or);
          }
#line 2252 "jsil_y.tab.cpp"
    break;

  case 83:
#line 521 "parser.y"
          {
            newstack(yyval).id("subtype_of");
          }
#line 2260 "jsil_y.tab.cpp"
    break;

  case 84:
#line 525 "parser.y"
          {
            newstack(yyval).id(ID_concatenation);
          }
#line 2268 "jsil_y.tab.cpp"
    break;

  case 85:
#line 531 "parser.y"
          {
            newstack(yyval).id(ID_bitand);
          }
#line 2276 "jsil_y.tab.cpp"
    break;

  case 86:
#line 535 "parser.y"
          {
            newstack(yyval).id(ID_bitor);
          }
#line 2284 "jsil_y.tab.cpp"
    break;

  case 87:
#line 539 "parser.y"
          {
            newstack(yyval).id(ID_bitxor);
          }
#line 2292 "jsil_y.tab.cpp"
    break;

  case 88:
#line 543 "parser.y"
          {
            newstack(yyval).id(ID_shl);
          }
#line 2300 "jsil_y.tab.cpp"
    break;

  case 89:
#line 547 "parser.y"
          {
            newstack(yyval).id(ID_shr);
          }
#line 2308 "jsil_y.tab.cpp"
    break;

  case 90:
#line 551 "parser.y"
          {
            newstack(yyval).id(ID_lshr);
          }
#line 2316 "jsil_y.tab.cpp"
    break;

  case 91:
#line 557 "parser.y"
        {
          newstack(yyval).id(ID_not);
        }
#line 2324 "jsil_y.tab.cpp"
    break;

  case 92:
#line 561 "parser.y"
        {
          newstack(yyval).id(ID_unary_minus);
        }
#line 2332 "jsil_y.tab.cpp"
    break;

  case 93:
#line 565 "parser.y"
        {
          newstack(yyval).id("num_to_string");
        }
#line 2340 "jsil_y.tab.cpp"
    break;

  case 94:
#line 569 "parser.y"
        {
          newstack(yyval).id("string_to_num");
        }
#line 2348 "jsil_y.tab.cpp"
    break;

  case 95:
#line 573 "parser.y"
        {
          newstack(yyval).id("num_to_int32");
        }
#line 2356 "jsil_y.tab.cpp"
    break;

  case 96:
#line 577 "parser.y"
        {
          newstack(yyval).id("num_to_uint32");
        }
#line 2364 "jsil_y.tab.cpp"
    break;

  case 97:
#line 581 "parser.y"
        {
          newstack(yyval).id(ID_bitnot);
        }
#line 2372 "jsil_y.tab.cpp"
    break;

  case 98:
#line 587 "parser.y"
         {
           newstack(yyval).id("null_type");
         }
#line 2380 "jsil_y.tab.cpp"
    break;

  case 99:
#line 591 "parser.y"
         {
           newstack(yyval).id("undefined_type");
         }
#line 2388 "jsil_y.tab.cpp"
    break;

  case 100:
#line 595 "parser.y"
         {
           newstack(yyval).id(ID_boolean);
         }
#line 2396 "jsil_y.tab.cpp"
    break;

  case 101:
#line 599 "parser.y"
         {
           newstack(yyval).id(ID_string);
         }
#line 2404 "jsil_y.tab.cpp"
    break;

  case 102:
#line 603 "parser.y"
         {
           newstack(yyval).id("number");
         }
#line 2412 "jsil_y.tab.cpp"
    break;

  case 103:
#line 607 "parser.y"
         {
           newstack(yyval).id("builtin_object");
         }
#line 2420 "jsil_y.tab.cpp"
    break;

  case 104:
#line 611 "parser.y"
         {
           newstack(yyval).id("user_object");
         }
#line 2428 "jsil_y.tab.cpp"
    break;

  case 105:
#line 615 "parser.y"
         {
           newstack(yyval).id("object");
         }
#line 2436 "jsil_y.tab.cpp"
    break;

  case 107:
#line 620 "parser.y"
         {
           newstack(yyval).id(ID_pointer);
           newstack(yyval).set(ID_C_reference, true);
         }
#line 2445 "jsil_y.tab.cpp"
    break;

  case 108:
#line 627 "parser.y"
        {
          newstack(yyval).id(ID_member);
        }
#line 2453 "jsil_y.tab.cpp"
    break;

  case 109:
#line 631 "parser.y"
        {
          newstack(yyval).id("variable");
        }
#line 2461 "jsil_y.tab.cpp"
    break;


#line 2465 "jsil_y.tab.cpp"

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
#line 636 "parser.y"

