// A Bison parser, made by GNU Bison 3.5.1.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.





#include "cat_parser.tab.h"


// Unqualified %code blocks.
#line 23 "cat_parser.ypp"

 /*
  * KATER -- Automating Weak Memory Model Metatheory
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program; if not, you can access it online at
  * http://www.gnu.org/licenses/gpl-3.0.html.
  */

  #include "cat_parsing_driver.h"
  #include "cat_type.h"
  #include <iostream>
  #include <memory>
  #include <unordered_map>
  #include <vector>

#line 72 "cat_parser.tab.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 163 "cat_parser.tab.cpp"


  /// Build a parser object.
  parser::parser (cat_parsing_drivert& drv_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      drv (drv_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_number_type
  parser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[+state];
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 43: // rel
        value.YY_MOVE_OR_COPY< cat_relationt > (YY_MOVE (that.value));
        break;

      case 39: // ID
      case 40: // QPATH
      case 42: // qualified_ID
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 43: // rel
        value.move< cat_relationt > (YY_MOVE (that.value));
        break;

      case 39: // ID
      case 40: // QPATH
      case 42: // qualified_ID
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 43: // rel
        value.copy< cat_relationt > (that.value);
        break;

      case 39: // ID
      case 40: // QPATH
      case 42: // qualified_ID
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 43: // rel
        value.move< cat_relationt > (that.value);
        break;

      case 39: // ID
      case 40: // QPATH
      case 42: // qualified_ID
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
#if defined __GNUC__ && ! defined __clang__ && ! defined __ICC && __GNUC__ * 100 + __GNUC_MINOR__ <= 408
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
#endif
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (drv));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case 43: // rel
        yylhs.value.emplace< cat_relationt > ();
        break;

      case 39: // ID
      case 40: // QPATH
      case 42: // qualified_ID
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2:
#line 112 "cat_parser.ypp"
                                { }
#line 599 "cat_parser.tab.cpp"
    break;

  case 3:
#line 113 "cat_parser.ypp"
                                { }
#line 605 "cat_parser.tab.cpp"
    break;

  case 4:
#line 116 "cat_parser.ypp"
                                           { }
#line 611 "cat_parser.tab.cpp"
    break;

  case 5:
#line 117 "cat_parser.ypp"
          {}
#line 617 "cat_parser.tab.cpp"
    break;

  case 6:
#line 118 "cat_parser.ypp"
                                           { drv.rename_relation(std::move(yystack_[0].value.as < cat_relationt > ()), yystack_[2].value.as < std::string > (), false); }
#line 623 "cat_parser.tab.cpp"
    break;

  case 7:
#line 119 "cat_parser.ypp"
                                                 { drv.rename_relation(std::move(yystack_[0].value.as < cat_relationt > ()), yystack_[2].value.as < std::string > (), true); }
#line 629 "cat_parser.tab.cpp"
    break;

  case 8:
#line 120 "cat_parser.ypp"
                                     { drv.make_dummy_relation(std::move(yystack_[2].value.as < std::string > ())); }
#line 635 "cat_parser.tab.cpp"
    break;

  case 10:
#line 124 "cat_parser.ypp"
                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 641 "cat_parser.tab.cpp"
    break;

  case 11:
#line 125 "cat_parser.ypp"
                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + "::" + yystack_[0].value.as < std::string > (); }
#line 647 "cat_parser.tab.cpp"
    break;

  case 12:
#line 128 "cat_parser.ypp"
                                        { drv.addConstraint(rel_axiomt::EMPTY, std::move(yystack_[0].value.as < cat_relationt > ())); }
#line 653 "cat_parser.tab.cpp"
    break;

  case 13:
#line 129 "cat_parser.ypp"
                                { drv.addConstraint(rel_axiomt::ACYCLIC, std::move(yystack_[0].value.as < cat_relationt > ())); }
#line 659 "cat_parser.tab.cpp"
    break;

  case 14:
#line 130 "cat_parser.ypp"
                                { drv.addConstraint(rel_axiomt::IRREFLEXIVE, std::move(yystack_[0].value.as < cat_relationt > ())); }
#line 665 "cat_parser.tab.cpp"
    break;

  case 15:
#line 131 "cat_parser.ypp"
                        { drv.addConstraint(rel_axiomt::EMPTY, std::move(yystack_[2].value.as < cat_relationt > ())); }
#line 671 "cat_parser.tab.cpp"
    break;

  case 16:
#line 132 "cat_parser.ypp"
                                { drv.addConstraint(rel_axiomt::ACYCLIC, std::move(yystack_[2].value.as < cat_relationt > ())); }
#line 677 "cat_parser.tab.cpp"
    break;

  case 17:
#line 133 "cat_parser.ypp"
                                        { drv.addConstraint(rel_axiomt::IRREFLEXIVE, std::move(yystack_[2].value.as < cat_relationt > ())); }
#line 683 "cat_parser.tab.cpp"
    break;

  case 18:
#line 134 "cat_parser.ypp"
                        { drv.addConstraint(rel_axiomt::NOT_EMPTY, std::move(yystack_[0].value.as < cat_relationt > ())); }
#line 689 "cat_parser.tab.cpp"
    break;

  case 19:
#line 135 "cat_parser.ypp"
                                { drv.addConstraint(rel_axiomt::NOT_EMPTY, std::move(yystack_[2].value.as < cat_relationt > ())); }
#line 695 "cat_parser.tab.cpp"
    break;

  case 20:
#line 138 "cat_parser.ypp"
                                    { yylhs.value.as < cat_relationt > () = drv.make_base("id"); }
#line 701 "cat_parser.tab.cpp"
    break;

  case 21:
#line 139 "cat_parser.ypp"
                                { yylhs.value.as < cat_relationt > () = cat_relationt(yystack_[0].value.as < std::string > (), rel_opt::TERMINAL, std::vector<std::string>{}); }
#line 707 "cat_parser.tab.cpp"
    break;

  case 22:
#line 140 "cat_parser.ypp"
                                  { yylhs.value.as < cat_relationt > () = std::move(yystack_[1].value.as < cat_relationt > ()); }
#line 713 "cat_parser.tab.cpp"
    break;

  case 23:
#line 141 "cat_parser.ypp"
                                  { yylhs.value.as < cat_relationt > () = drv.make_relation(rel_opt::BRACKET, std::vector<std::string>{yystack_[1].value.as < cat_relationt > ().name}); }
#line 719 "cat_parser.tab.cpp"
    break;

  case 24:
#line 142 "cat_parser.ypp"
                                      { yylhs.value.as < cat_relationt > () = drv.make_base("id"); }
#line 725 "cat_parser.tab.cpp"
    break;

  case 25:
#line 143 "cat_parser.ypp"
                                  { yylhs.value.as < cat_relationt > () = drv.make_relation(rel_opt::ALT, std::vector<std::string>{yystack_[2].value.as < cat_relationt > ().name, yystack_[0].value.as < cat_relationt > ().name}); }
#line 731 "cat_parser.tab.cpp"
    break;

  case 26:
#line 144 "cat_parser.ypp"
                                  { yylhs.value.as < cat_relationt > () = drv.make_relation(rel_opt::SEQ, std::vector<std::string>{yystack_[2].value.as < cat_relationt > ().name, yystack_[0].value.as < cat_relationt > ().name}); }
#line 737 "cat_parser.tab.cpp"
    break;

  case 27:
#line 145 "cat_parser.ypp"
                                  { yylhs.value.as < cat_relationt > () = drv.make_relation(rel_opt::AND, std::vector<std::string>{yystack_[2].value.as < cat_relationt > ().name, yystack_[0].value.as < cat_relationt > ().name}); }
#line 743 "cat_parser.tab.cpp"
    break;

  case 28:
#line 146 "cat_parser.ypp"
                          { yylhs.value.as < cat_relationt > () = drv.make_relation(rel_opt::SUB, std::vector<std::string>{yystack_[2].value.as < cat_relationt > ().name, yystack_[0].value.as < cat_relationt > ().name}); }
#line 749 "cat_parser.tab.cpp"
    break;

  case 29:
#line 147 "cat_parser.ypp"
                                  { yylhs.value.as < cat_relationt > () = drv.make_relation(rel_opt::PROD, std::vector<std::string>{yystack_[2].value.as < cat_relationt > ().name, yystack_[0].value.as < cat_relationt > ().name}); }
#line 755 "cat_parser.tab.cpp"
    break;

  case 30:
#line 148 "cat_parser.ypp"
                                      { yylhs.value.as < cat_relationt > () = drv.make_relation(rel_opt::PLUS, std::vector<std::string>{yystack_[1].value.as < cat_relationt > ().name}); }
#line 761 "cat_parser.tab.cpp"
    break;

  case 31:
#line 149 "cat_parser.ypp"
                    { yylhs.value.as < cat_relationt > () = drv.make_star(yystack_[1].value.as < cat_relationt > ().name); }
#line 767 "cat_parser.tab.cpp"
    break;

  case 32:
#line 150 "cat_parser.ypp"
                                      { yylhs.value.as < cat_relationt > () = drv.make_qmark(yystack_[1].value.as < cat_relationt > ().name); }
#line 773 "cat_parser.tab.cpp"
    break;

  case 33:
#line 151 "cat_parser.ypp"
                                { yylhs.value.as < cat_relationt > () = drv.make_relation(rel_opt::FLIP, std::vector<std::string>{yystack_[2].value.as < cat_relationt > ().name}); }
#line 779 "cat_parser.tab.cpp"
    break;

  case 34:
#line 152 "cat_parser.ypp"
                    { yylhs.value.as < cat_relationt > () = drv.make_fencerel(yystack_[0].value.as < cat_relationt > ().name); }
#line 785 "cat_parser.tab.cpp"
    break;

  case 35:
#line 153 "cat_parser.ypp"
                  { yylhs.value.as < cat_relationt > () = drv.make_free(); }
#line 791 "cat_parser.tab.cpp"
    break;


#line 795 "cat_parser.tab.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[+yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yy_error_token_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yy_error_token_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
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


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (state_type, const symbol_type&) const
  {
    return YY_("syntax error");
  }


  const signed char parser::yypact_ninf_ = -38;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
     -38,     9,   -38,    23,    23,    23,    -7,   -37,   -12,   -38,
     -38,   -38,   -38,   -38,    23,     3,    23,   -17,   -38,    37,
      57,    69,    23,   -38,   -32,   -13,   101,   -38,   113,   -38,
     -19,   -18,    23,    23,    23,    23,   -38,    23,   -38,    -3,
      -5,     4,    89,    11,    13,   -38,   -38,   -38,   -38,    41,
      77,   134,    77,    41,   -38,   -38,   -38,     5,    23,   -38,
     125,   -38,   125
  };

  const signed char
  parser::yydefact_[] =
  {
       2,     0,     1,     0,     0,     0,     0,     0,     0,     5,
       3,     9,    35,    20,     0,     0,     0,    10,    21,    13,
      14,    12,     0,     4,     0,     0,     0,    24,     0,    34,
       0,     0,     0,     0,     0,     0,    30,    31,    32,     0,
       0,     0,    18,     0,     0,    22,    23,    11,    16,    26,
      27,    25,    28,    29,    33,    17,    15,     0,     0,     8,
       6,    19,     7
  };

  const signed char
  parser::yypgoto_[] =
  {
     -38,   -38,    -4,   -38,   -38,   -38
  };

  const signed char
  parser::yydefgoto_[] =
  {
      -1,    18,    19,     1,    10,    11
  };

  const signed char
  parser::yytable_[] =
  {
      20,    21,    22,    23,    24,    30,    12,    43,    44,     2,
      26,    28,    29,     3,     4,    13,    12,     5,    42,     6,
      47,    48,    54,     7,     8,    13,    12,    25,    49,    50,
      51,    52,    58,    53,    55,    13,    14,    59,    15,    27,
      60,    16,    17,    56,    61,     0,    14,     0,    15,     9,
      31,    16,    17,     0,    62,     0,    14,     0,    15,     0,
       0,    16,    17,    32,    33,    34,    35,    36,    37,    38,
      40,    36,    37,    38,    39,     0,     0,     0,    39,     0,
       0,     0,    41,    32,    33,    34,    35,    36,    37,    38,
       0,     0,     0,     0,    39,    32,    33,    34,    35,    36,
      37,    38,    57,    32,     0,     0,    39,    36,    37,    38,
       0,     0,     0,     0,    39,    32,    33,    34,    35,    36,
      37,    38,     0,     0,     0,     0,    39,    32,    33,    34,
      35,    36,    37,    38,     0,    45,     0,     0,    39,    32,
      33,    34,    35,    36,    37,    38,     0,     0,     0,    46,
      39,    32,    33,    34,    35,    36,    37,    38,     0,     0,
      32,    33,    39,    35,    36,    37,    38,     0,     0,     0,
       0,    39
  };

  const signed char
  parser::yycheck_[] =
  {
       4,     5,     9,    40,    16,    22,     3,    39,    21,     0,
      14,    15,    16,     4,     5,    12,     3,     8,    22,    10,
      39,    39,    25,    14,    15,    12,     3,    39,    32,    33,
      34,    35,    21,    37,    39,    12,    33,    24,    35,    36,
      44,    38,    39,    39,    39,    -1,    33,    -1,    35,    40,
      13,    38,    39,    -1,    58,    -1,    33,    -1,    35,    -1,
      -1,    38,    39,    26,    27,    28,    29,    30,    31,    32,
      13,    30,    31,    32,    37,    -1,    -1,    -1,    37,    -1,
      -1,    -1,    13,    26,    27,    28,    29,    30,    31,    32,
      -1,    -1,    -1,    -1,    37,    26,    27,    28,    29,    30,
      31,    32,    13,    26,    -1,    -1,    37,    30,    31,    32,
      -1,    -1,    -1,    -1,    37,    26,    27,    28,    29,    30,
      31,    32,    -1,    -1,    -1,    -1,    37,    26,    27,    28,
      29,    30,    31,    32,    -1,    34,    -1,    -1,    37,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    -1,    36,
      37,    26,    27,    28,    29,    30,    31,    32,    -1,    -1,
      26,    27,    37,    29,    30,    31,    32,    -1,    -1,    -1,
      -1,    37
  };

  const signed char
  parser::yystos_[] =
  {
       0,    44,     0,     4,     5,     8,    10,    14,    15,    40,
      45,    46,     3,    12,    33,    35,    38,    39,    42,    43,
      43,    43,     9,    40,    16,    39,    43,    36,    43,    43,
      22,    13,    26,    27,    28,    29,    30,    31,    32,    37,
      13,    13,    43,    39,    21,    34,    36,    39,    39,    43,
      43,    43,    43,    43,    25,    39,    39,    13,    21,    24,
      43,    39,    43
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    41,    44,    44,    45,    45,    45,    45,    45,    45,
      42,    42,    46,    46,    46,    46,    46,    46,    46,    46,
      43,    43,    43,    43,    43,    43,    43,    43,    43,    43,
      43,    43,    43,    43,    43,    43
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     0,     2,     2,     1,     4,     5,     4,     1,
       1,     3,     2,     2,     2,     4,     4,     4,     3,     5,
       1,     1,     3,     3,     2,     3,     3,     3,     3,     3,
       2,     2,     2,     3,     2,     1
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "YYEOF", "error", "$undefined", "\"free\"", "\"acyclic\"",
  "\"irreflexive\"", "\"declare\"", "\"declare-loc\"", "\"empty\"",
  "\"~empty\"", "\"flag\"", "\"error\"", "\"id\"", "\"as\"", "\"include\"",
  "\"let\"", "\"rec\"", "\"save\"", "\"total\"", "\"view\"", "\"warning\"",
  "\"=\"", "\"::\"", "\",\"", "\"0\"", "\"-1\"", "\";\"", "\"&\"", "\"|\"",
  "\"\\\\\"", "\"+\"", "\"*\"", "\"?\"", "\"(\"", "\")\"", "\"[\"",
  "\"]\"", "\"^\"", "\"fencerel\"", "ID", "QPATH", "$accept",
  "qualified_ID", "rel", "main", "decl", "constraint", YY_NULLPTR
  };


  const unsigned char
  parser::yyrline_[] =
  {
       0,   112,   112,   113,   116,   117,   118,   119,   120,   121,
     124,   125,   128,   129,   130,   131,   132,   133,   134,   135,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153
  };

  // Print the state stack on the debug stream.
  void
  parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  parser::yy_reduce_print_ (int yyrule)
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


} // yy
#line 1150 "cat_parser.tab.cpp"

#line 156 "cat_parser.ypp"


void yy::parser::error(const location_type &l, const std::string &m)
{
	std::cerr << "\n" << l << ": " << m << "\n";
}

/* void yy::parser::report_syntax_error(const yy::parser::context &ctx) const
{
	std::cerr << ctx.location() << ": error";

	{
		enum { TOKENMAX = 5 };
		symbol_kind_type expected[TOKENMAX];

		auto n = ctx.expected_tokens(expected, TOKENMAX);
		for (auto i = 0; i < n; i++)
			std::cerr << (i == 0 ? ": expected \'" : "\' or \'") << symbol_name(expected[i]);
		if (n > 0)
			std::cerr << "\'";
	}

	{
		auto lookahead = ctx.token();
		if (lookahead != symbol_kind::S_YYEMPTY)
			std::cerr << " before \'" << symbol_name (lookahead) << "\'";
	}

	std::cerr << '\n';

	{
		auto &loc = ctx.location();
		std::cerr << current_line;
		for (auto i = 0; i < loc.begin.column-1; i++)
			std::cerr << " ";
		std::cerr << "^";
		for (auto i = loc.begin.column+1; i < loc.end.column; i++)
			std::cerr << "~";
		std::cerr << "\n";
		reset_current_line();
	}
} */
