// A Bison parser, made by GNU Bison 3.0.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2013 Free Software Foundation, Inc.

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

// Take the name prefix into account.
#define yylex   gspnlex

// First part of user declarations.

#line 39 "Gspn-parser.cc" // lalr1.cc:399

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "Gspn-parser.hh"

// User implementation prologue.

#line 53 "Gspn-parser.cc" // lalr1.cc:407
// Unqualified %code blocks.
#line 41 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:408

#include "Gspn-Reader.hpp"
#include <set>
#include <vector>

  vector<string> Par;
  std::string distrib;
  Eval Evaluate;


  int NbServers;
  bool SingleService;
  bool MarkingDependent;
  bool AgeMemory;

 

#line 73 "Gspn-parser.cc" // lalr1.cc:408


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
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

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
      *yycdebug_ << std::endl;                  \
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
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyempty = true)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace gspn {
#line 159 "Gspn-parser.cc" // lalr1.cc:474

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Gspn_parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
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
              // Fall through.
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  Gspn_parser::Gspn_parser (Gspn_Reader& Reader_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      Reader (Reader_yyarg)
  {}

  Gspn_parser::~Gspn_parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/

  inline
  Gspn_parser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  Gspn_parser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  Gspn_parser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
    value = other.value;
  }


  template <typename Base>
  inline
  Gspn_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  inline
  Gspn_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  inline
  Gspn_parser::basic_symbol<Base>::~basic_symbol ()
  {
  }

  template <typename Base>
  inline
  void
  Gspn_parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
    value = s.value;
    location = s.location;
  }

  // by_type.
  inline
  Gspn_parser::by_type::by_type ()
     : type (empty)
  {}

  inline
  Gspn_parser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  Gspn_parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  Gspn_parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.type = empty;
  }

  inline
  int
  Gspn_parser::by_type::type_get () const
  {
    return type;
  }


  // by_state.
  inline
  Gspn_parser::by_state::by_state ()
    : state (empty)
  {}

  inline
  Gspn_parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  Gspn_parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.state = empty;
  }

  inline
  Gspn_parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  Gspn_parser::symbol_number_type
  Gspn_parser::by_state::type_get () const
  {
    return state == empty ? 0 : yystos_[state];
  }

  inline
  Gspn_parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  Gspn_parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
    value = that.value;
    // that is emptied.
    that.type = empty;
  }

  inline
  Gspn_parser::stack_symbol_type&
  Gspn_parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  Gspn_parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    switch (yysym.type_get ())
    {
            case 3: // str

#line 112 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:602
        { delete (yysym.value.name); }
#line 378 "Gspn-parser.cc" // lalr1.cc:602
        break;


      default:
        break;
    }
  }

#if YYDEBUG
  template <typename Base>
  void
  Gspn_parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    switch (yytype)
    {
            case 3: // str

#line 111 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:617
        { debug_stream () << *(yysym.value.name); }
#line 405 "Gspn-parser.cc" // lalr1.cc:617
        break;

      case 5: // rval

#line 115 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:617
        { debug_stream () << (yysym.value.RealVal); }
#line 412 "Gspn-parser.cc" // lalr1.cc:617
        break;

      case 6: // ival

#line 114 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:617
        { debug_stream () << (yysym.value.IntVal); }
#line 419 "Gspn-parser.cc" // lalr1.cc:617
        break;


      default:
        break;
    }
    yyo << ')';
  }
#endif

  inline
  void
  Gspn_parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  Gspn_parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  Gspn_parser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Gspn_parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Gspn_parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Gspn_parser::debug_level_type
  Gspn_parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Gspn_parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline Gspn_parser::state_type
  Gspn_parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  Gspn_parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  Gspn_parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Gspn_parser::parse ()
  {
    /// Whether yyla contains a lookahead.
    bool yyempty = true;

    // State.
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

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyempty)
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location, Reader));
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
        yyempty = false;
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Discard the token being shifted.
    yyempty = true;

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;

      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 127 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {sprintf((yylhs.value.expression),"%d",(yystack_[0].value.IntVal));}
#line 646 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 3:
#line 128 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {
  if(Reader.MyGspn.PlacesId.find(*(yystack_[0].value.name))!=Reader.MyGspn.PlacesId.end())
    {std::ostringstream s; s<<" Marking.P->_PL_"<< (yystack_[0].value.name)->c_str() <<" ";
      sprintf((yylhs.value.expression), "%s",(s.str()).c_str());
    }
  else if(Reader.MyGspn.IntConstant.find(*(yystack_[0].value.name))!=Reader.MyGspn.IntConstant.end())
    {std::ostringstream s; s<<Reader.MyGspn.IntConstant[*(yystack_[0].value.name)];
      sprintf((yylhs.value.expression), "%s",(s.str()).c_str());

    }
  else{cout<<"'"<<*(yystack_[0].value.name)<<"' has not been declared"<<endl;YYABORT;}}
#line 662 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 4:
#line 139 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {sprintf((yylhs.value.expression),"( %s )", (yystack_[1].value.expression));       }
#line 668 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 5:
#line 140 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {sprintf((yylhs.value.expression),"%s + %s", (yystack_[2].value.expression), (yystack_[0].value.expression));  }
#line 674 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 6:
#line 141 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {sprintf((yylhs.value.expression),"%s - %s", (yystack_[2].value.expression), (yystack_[0].value.expression));  }
#line 680 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 7:
#line 142 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {sprintf((yylhs.value.expression),"%s * %s", (yystack_[2].value.expression), (yystack_[0].value.expression));  }
#line 686 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 8:
#line 143 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {sprintf((yylhs.value.expression),"pow(%s , %s)", (yystack_[2].value.expression), (yystack_[0].value.expression));  }
#line 692 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 9:
#line 144 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {sprintf((yylhs.value.expression),"floor( %s )", (yystack_[1].value.expression));  }
#line 698 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 10:
#line 145 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {sprintf((yylhs.value.expression),"floor( %s /double(%s)", (yystack_[3].value.expression),(yystack_[1].value.expression));  }
#line 704 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 11:
#line 146 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {sprintf((yylhs.value.expression),"min(%s , %s)", (yystack_[3].value.expression), (yystack_[1].value.expression));  }
#line 710 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 12:
#line 147 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {sprintf((yylhs.value.expression),"max(%s , %s)", (yystack_[3].value.expression), (yystack_[1].value.expression));  }
#line 716 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 13:
#line 150 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {sprintf((yylhs.value.expression), "%f",(yystack_[0].value.RealVal));}
#line 722 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 14:
#line 151 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {sprintf((yylhs.value.expression),"%d",(yystack_[0].value.IntVal));}
#line 728 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 15:
#line 152 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {
  if(Reader.MyGspn.PlacesId.find(*(yystack_[0].value.name))!=Reader.MyGspn.PlacesId.end())
    {std::ostringstream s; s<<" Marking.P->_PL_"<<(yystack_[0].value.name)->c_str()<<" ";
      sprintf((yylhs.value.expression), "%s",(s.str()).c_str());
    }
  else if(Reader.MyGspn.RealConstant.find(*(yystack_[0].value.name))!=Reader.MyGspn.RealConstant.end())
    {std::ostringstream s; s<<Reader.MyGspn.RealConstant[*(yystack_[0].value.name)];
      sprintf((yylhs.value.expression), "%s",(s.str()).c_str());
    }
  else{cout<<"'"<<*(yystack_[0].value.name)<<"' has not been declared"<<endl;YYABORT;}}
#line 743 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 16:
#line 162 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {sprintf((yylhs.value.expression),"( %s )", (yystack_[1].value.expression));       }
#line 749 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 17:
#line 163 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {sprintf((yylhs.value.expression),"%s /double(%s)", (yystack_[2].value.expression), (yystack_[0].value.expression));  }
#line 755 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 18:
#line 164 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {sprintf((yylhs.value.expression),"%s + %s", (yystack_[2].value.expression), (yystack_[0].value.expression));  }
#line 761 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 19:
#line 165 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {sprintf((yylhs.value.expression),"%s - %s", (yystack_[2].value.expression), (yystack_[0].value.expression));  }
#line 767 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 20:
#line 166 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {sprintf((yylhs.value.expression),"%s * %s", (yystack_[2].value.expression), (yystack_[0].value.expression));  }
#line 773 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 21:
#line 167 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {sprintf((yylhs.value.expression),"pow(%s , %s)", (yystack_[2].value.expression), (yystack_[0].value.expression));  }
#line 779 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 22:
#line 168 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {sprintf((yylhs.value.expression),"floor( %s )", (yystack_[1].value.expression));  }
#line 785 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 23:
#line 169 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {sprintf((yylhs.value.expression),"min(%s , %s)", (yystack_[3].value.expression), (yystack_[1].value.expression));  }
#line 791 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 24:
#line 170 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {sprintf((yylhs.value.expression),"max(%s , %s)", (yystack_[3].value.expression), (yystack_[1].value.expression));  }
#line 797 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 33:
#line 189 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {if(Reader.MyGspn.RealConstant.find(*(yystack_[3].value.name))!=Reader.MyGspn.RealConstant.end())
    {cout<<"Constant "<<*(yystack_[3].value.name)<<" already defined."<<endl; YYABORT;}
  else {string st=(yystack_[1].value.expression);
	if (Reader.P.constants.count(*(yystack_[3].value.name))>0)st = Reader.P.constants[*(yystack_[3].value.name)];
	  
    Evaluate.parse(st);
    Reader.MyGspn.IntConstant[*(yystack_[3].value.name)]=Evaluate.IntResult;Reader.MyGspn.RealConstant[*(yystack_[3].value.name)]=Evaluate.RealResult;}
}
#line 810 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 34:
#line 198 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {if(Reader.MyGspn.RealConstant.find(*(yystack_[3].value.name))!=Reader.MyGspn.RealConstant.end())
    {cout<<"Constant "<<*(yystack_[3].value.name)<<" already defined."<<endl; YYABORT;}
  else {string st=(yystack_[1].value.expression);
	  if (Reader.P.constants.count(*(yystack_[3].value.name))>0)st = Reader.P.constants[*(yystack_[3].value.name)];
    Evaluate.parse(st);
    Reader.MyGspn.RealConstant[*(yystack_[3].value.name)]=Evaluate.RealResult;}
}
#line 822 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 37:
#line 209 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {Reader.MyGspn.pl=(yystack_[1].value.IntVal);

 }
#line 830 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 38:
#line 212 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {if(Reader.MyGspn.IntConstant.find(*(yystack_[1].value.name))==Reader.MyGspn.IntConstant.end())
     {
       std::cout<<*(yystack_[1].value.name)<<" was not declared"<<std::endl;
       YYABORT;
     }
   Reader.MyGspn.pl=Reader.MyGspn.IntConstant[*(yystack_[1].value.name)];

 }
#line 843 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 39:
#line 221 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {Reader.MyGspn.tr=(yystack_[1].value.IntVal);

 }
#line 851 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 40:
#line 224 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {if(Reader.MyGspn.IntConstant.find(*(yystack_[1].value.name))==Reader.MyGspn.IntConstant.end())
     {
       std::cout<<*(yystack_[1].value.name)<<" was not declared"<<std::endl;
       YYABORT;
     }
   Reader.MyGspn.tr=Reader.MyGspn.IntConstant[*(yystack_[1].value.name)];

 }
#line 864 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 41:
#line 233 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {
  if(Reader.MyGspn.PlacesId.size()!=Reader.MyGspn.pl){
    std::cout<<"Place label missing or redeclared, please check your places list"<<std::endl;
    YYABORT;
  }
  
  Reader.MyGspn.Marking= vector<string>(Reader.MyGspn.pl, " ");
  Reader.MyGspn.InitialMarking = vector<vector<coloredToken>>(Reader.MyGspn.pl);

  MarkingDependent=false;
  AgeMemory=false;


 }
#line 883 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 42:
#line 248 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {
	place p;
	p.name = *(yystack_[0].value.name);
	p.id = Reader.MyGspn.placeStruct.size();
	Reader.MyGspn.placeStruct.push_back(p);
    int sz=Reader.MyGspn.PlacesId.size();
    Reader.MyGspn.PlacesId[*(yystack_[0].value.name)]=sz;
 }
#line 896 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 43:
#line 256 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {
	place p;
	p.name = *(yystack_[0].value.name);
	p.id = Reader.MyGspn.placeStruct.size();
	Reader.MyGspn.placeStruct.push_back(p);
	int sz=Reader.MyGspn.PlacesId.size();
    Reader.MyGspn.PlacesId[*(yystack_[0].value.name)]=sz;
 }
#line 909 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 44:
#line 265 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {
  if(Reader.MyGspn.TransList.size()!=Reader.MyGspn.tr){
    std::cout<<"Transition label missing or redeclared, please check your transitions list"<<std::endl;
    YYABORT;
  }

 }
#line 921 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 45:
#line 273 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {Reader.MyGspn.TransList.insert(*(yystack_[0].value.name));
   /*int sz=Reader.MyGspn.TransId.size();
   Reader.MyGspn.TransId[*$1]=sz;*/

 }
#line 931 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 46:
#line 278 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {Reader.MyGspn.TransList.insert(*(yystack_[0].value.name));
   /*int sz=Reader.MyGspn.TransId.size();
   Reader.MyGspn.TransId[*$3]=sz;*/

 }
#line 941 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 49:
#line 288 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {}
#line 947 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 52:
#line 294 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    { if(Reader.MyGspn.PlacesId.find(*(yystack_[4].value.name))==Reader.MyGspn.PlacesId.end())
    {cout<<"'"<<*(yystack_[4].value.name)<<"' has not been declared"<<endl;
      YYABORT;
    }
  string st=(yystack_[2].value.expression);
  if(Evaluate.parse(st)){
    std::cout<<"Initial Marking is not marking dependent"<<std::endl;
    YYABORT;
  }
  stringstream ss;
  ss << Evaluate.IntResult;
  Reader.MyGspn.Marking[Reader.MyGspn.PlacesId[*(yystack_[4].value.name)]]=ss.str();
  Reader.MyGspn.InitialMarking[Reader.MyGspn.PlacesId[*(yystack_[4].value.name)]].push_back(coloredToken(ss.str()));
}
#line 966 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 54:
#line 312 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {}
#line 972 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 55:
#line 313 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {}
#line 978 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 56:
#line 315 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {
  if(Reader.MyGspn.TransList.find(*(yystack_[8].value.name))==Reader.MyGspn.TransList.end())
    {
      std::cout<<"Transition: "<<*(yystack_[8].value.name)<<" was not declared"<<std::endl;
      YYABORT;
    }
  transition *trans = new transition();
  trans->label = (yystack_[8].value.name)->c_str();
  trans->type = Timed;
  trans->dist.name = distrib;
  for(auto &s:Par)trans->dist.Param.push_back(expr(s));
  trans->priority = (yystack_[4].value.expression);
  trans->weight = (yystack_[2].value.expression);
  trans->singleService = true;
  trans->markingDependant = false;
  trans->ageMemory = false;
  trans->nbServers = 1;
  trans->id = Reader.MyGspn.transitionStruct.size();
  int sz=Reader.MyGspn.TransId.size();
  Reader.MyGspn.TransId[*(yystack_[8].value.name)]=sz;
  Reader.MyGspn.transitionStruct.push_back(*trans);

  Par.clear();
 }
#line 1007 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 57:
#line 339 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {
  if(Reader.MyGspn.TransList.find(*(yystack_[10].value.name))==Reader.MyGspn.TransList.end())
    {
      std::cout<<"Transition: "<<*(yystack_[10].value.name)<<" was not declared"<<std::endl;
      YYABORT;
    }
  transition *trans = new transition();
  trans->label = (yystack_[10].value.name)->c_str();
  trans->type = Timed;
  trans->dist.name = distrib;
  for(auto &s:Par)trans->dist.Param.push_back(expr(s));
  trans->priority = (yystack_[6].value.expression);
  trans->weight = (yystack_[4].value.expression);
  trans->singleService = true;
  trans->markingDependant = false;
  trans->ageMemory = AgeMemory;
  trans->nbServers = 1;
  trans->id = Reader.MyGspn.transitionStruct.size();
	int sz=Reader.MyGspn.TransId.size();
	Reader.MyGspn.TransId[*(yystack_[10].value.name)]=sz;
  Reader.MyGspn.transitionStruct.push_back(*trans);

  Par.clear();
  AgeMemory=false;
 }
#line 1037 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 58:
#line 364 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {
  if(Reader.MyGspn.TransList.find(*(yystack_[13].value.name))==Reader.MyGspn.TransList.end())
    {
      std::cout<<"Transition: "<<*(yystack_[13].value.name)<<" was not declared"<<std::endl;
      YYABORT;
    }
  if(!SingleService) MarkingDependent=true;
  string st=(yystack_[9].value.expression);
  vector<string> v(1);
  if(Evaluate.parse(st))
    {MarkingDependent=true;v[0]=st;
    }
  else{ if (Evaluate.RealResult<= 0 ) {
      cout << "In exponential ditribution Lambda > 0" << endl;
      YYABORT;
    }
    std::ostringstream s;s<<Evaluate.RealResult;
    v[0]=s.str();
  }
  
  transition *trans = new transition();
  trans->label = (yystack_[13].value.name)->c_str();
  trans->type = Timed;
  trans->dist.name = "EXPONENTIAL";
  for(auto &s:v)trans->dist.Param.push_back(expr(s));
  trans->priority = (yystack_[6].value.expression);
  trans->weight = (yystack_[4].value.expression);
  trans->singleService = SingleService;
  trans->markingDependant = MarkingDependent;
  trans->ageMemory = false;
  trans->nbServers = NbServers;
  trans->id = Reader.MyGspn.transitionStruct.size();
	int sz=Reader.MyGspn.TransId.size();
	Reader.MyGspn.TransId[*(yystack_[13].value.name)]=sz;
  Reader.MyGspn.transitionStruct.push_back(*trans);

  MarkingDependent=false;

 }
#line 1081 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 59:
#line 404 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {
  if(Reader.MyGspn.TransList.find(*(yystack_[15].value.name))==Reader.MyGspn.TransList.end())
    {
      std::cout<<"Transition: "<<*(yystack_[15].value.name)<<" was not declared"<<std::endl;
      YYABORT;
    }

  if(!SingleService) MarkingDependent=true;
  string st=(yystack_[11].value.expression);
  vector<string> v(1);
  if(Evaluate.parse(st))
    {MarkingDependent=true;v[0]=st;
    }
  else{ if (Evaluate.RealResult<= 0 ) {
      cout << "In exponential ditribution Lambda > 0" << endl;
      YYABORT;
    }
    std::ostringstream s;s<<Evaluate.RealResult;
    v[0]=s.str();
  }
  
  transition *trans = new transition();
  trans->label = (yystack_[15].value.name)->c_str();
  trans->type = Timed;
  trans->dist.name = "EXPONENTIAL";
  for(auto &s:v)trans->dist.Param.push_back(expr(s));
  trans->priority = (yystack_[8].value.expression);
  trans->weight = (yystack_[6].value.expression);
  trans->singleService = SingleService;
  trans->markingDependant = MarkingDependent;
  trans->ageMemory = AgeMemory;
  trans->nbServers = NbServers;
  trans->id = Reader.MyGspn.transitionStruct.size();
  Reader.MyGspn.transitionStruct.push_back(*trans);
	int sz=Reader.MyGspn.TransId.size();
	Reader.MyGspn.TransId[*(yystack_[15].value.name)]=sz;
  MarkingDependent=false;
  AgeMemory=false;
 }
#line 1125 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 60:
#line 444 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {
  if(Reader.MyGspn.TransList.find(*(yystack_[8].value.name))==Reader.MyGspn.TransList.end())
    {
      std::cout<<"Transition: "<<*(yystack_[8].value.name)<<" was not declared"<<std::endl;
      YYABORT;
    }

  transition *trans = new transition();
  trans->label = (yystack_[8].value.name)->c_str();
  trans->type = unTimed;
  trans->dist.name = "IMMEDIATE";
  trans->priority = (yystack_[4].value.expression);
  trans->weight = (yystack_[2].value.expression);
  trans->singleService = true;
  trans->markingDependant = false;
  trans->ageMemory = false;
  trans->nbServers = 1;
  trans->id = Reader.MyGspn.transitionStruct.size();
	int sz=Reader.MyGspn.TransId.size();
	Reader.MyGspn.TransId[*(yystack_[8].value.name)]=sz;
  Reader.MyGspn.transitionStruct.push_back(*trans);

 }
#line 1153 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 61:
#line 473 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {string st=(yystack_[0].value.expression);
  if(Evaluate.parse(st))
    {cout<<"Weight is not marking dependent: '"<<st<<"'"<<endl;YYABORT; }
  else{ if(Evaluate.RealResult<0)
      {cout<<"Weight is a positive value: '"<<st<<"'"<<endl; YYABORT;}
    else{std::ostringstream s;s<<Evaluate.RealResult;
      sprintf((yylhs.value.expression), "%s",(s.str()).c_str());
    }
  }
}
#line 1168 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 62:
#line 484 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {string st=(yystack_[0].value.expression);
  if(Evaluate.parse(st))
    {cout<<"Priority is not marking dependent: '"<<st<<"'"<<endl;YYABORT; }
  else{ if(Evaluate.RealResult<0)
      {cout<<"Priority is a positive value: '"<<st<<"'"<<endl; YYABORT;}
    else{std::ostringstream s;s<<Evaluate.RealResult;
      sprintf((yylhs.value.expression), "%s",(s.str()).c_str());
    }
  }
}
#line 1183 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 63:
#line 495 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {SingleService=true; NbServers=1;}
#line 1189 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 64:
#line 496 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {SingleService=false;NbServers=INT_MAX;}
#line 1195 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 65:
#line 497 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {NbServers= (yystack_[1].value.IntVal);
   if(NbServers<1)
     { cout<<"Number of servers should be at least one"<<endl;
       YYABORT;
     }
   if(NbServers==1) SingleService=true;
   else {SingleService=false;}


 }
#line 1210 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 66:
#line 507 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {
  if(Reader.MyGspn.IntConstant.find(*(yystack_[1].value.name))!=Reader.MyGspn.IntConstant.end()){
    NbServers=Reader.MyGspn.IntConstant[*(yystack_[1].value.name)];
    if(NbServers<1)
      { cout<<"Number of servers should be at least one"<<endl;
        YYABORT;
      }
    if(NbServers==1) SingleService=true;
    else {SingleService=false;}
  }
  else{cout<<*(yystack_[1].value.name)<<" not defined or not defined like an integer constant "<<endl;}

 }
#line 1228 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 67:
#line 521 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {AgeMemory=true;}
#line 1234 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 68:
#line 522 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {AgeMemory=false;}
#line 1240 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 69:
#line 525 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {
  distrib=*(yystack_[3].value.name);
  int np=Par.size();
  switch (Reader.IndexDist[distrib]) {
    case 1:
      {
        if (np != 2){
          cout << "Uniform ditribution has two parameters: min and max where 0<=min<max" << endl;
          YYABORT;
        }
        double p1,p2;
        string st=Par[0];
        Evaluate.parse(st);p1=Evaluate.RealResult;
        st=Par[1];
        Evaluate.parse(st);p2=Evaluate.RealResult;
        if ((p1>=p2) || (p1<0)) {
          cout << "In uniform ditribution 0 <= min< max " << endl;
          YYABORT;
        }
        break;
      }

    case 2:
      {
        if (np != 1) {
          cout << "exponential ditribution has one parameter: Lambda > 0" << endl;
          YYABORT;
        }

        double p1;
        string st=Par[0];
        Evaluate.parse(st);p1=Evaluate.RealResult;
        if (p1<= 0 ) {
          cout << "In exponential ditribution Lambda > 0" << endl;
          YYABORT;
        }
        break;
      }

    case 3:
      {
        if (np != 1) {
          cout << "Deterministic ditribution has one parameter:  T >= 0" << endl;
          YYABORT;
        }

        double p1;
        string st=Par[0];
        Evaluate.parse(st);p1=Evaluate.RealResult;
        if (p1<0 ) {
          cout << "In Deterministic ditribution Lambda > 0" << endl;
          YYABORT;
        }
        break;
      }

    case 4:
      {
        if (np != 2) {
          cout << "Lognormal ditribution has two parameters: mu and sigma^2, where mu > 0 sigma^2 > 0" << endl;
          YYABORT;
        }

        double p1,p2;
        string st=Par[0];
        Evaluate.parse(st);p1=Evaluate.RealResult;
        st=Par[1];
        Evaluate.parse(st);p2=Evaluate.RealResult;
        if ((p1<=0) || (p2<=0)) {
          cout << "In Lognormal ditribution mu > 0 sigma^2 > 0" << endl;
          YYABORT;
        }
        break;
      }

    case 5:
      {
        if (np != 3) {
          cout << "Triangle ditribution has three parameters: 0 <= b <= c <= a" << endl;
          YYABORT;
        }
        double p1,p2,p3;
        string st=Par[0];
        Evaluate.parse(st);p1=Evaluate.RealResult;
        st=Par[1];
        Evaluate.parse(st);p2=Evaluate.RealResult;
        st=Par[2];
        Evaluate.parse(st);p3=Evaluate.RealResult;
        if ((p1<0) || (p2<p1) || (p3<p2)) {
          cout << "In Triangle ditribution: 0 <= b <= c<= a" << endl;
          YYABORT;
        }


        break;
      }

    case 6:
      {
        if (np != 2) {
          cout << "Geometric ditribution has two parameters: 0 <= p <= 1 (probability of success) and T > 0 (duration of one step)" << endl;
          YYABORT;
        }
        double p1,p2;
        string st=Par[0];
        Evaluate.parse(st);p1=Evaluate.RealResult;
        st=Par[1];
        Evaluate.parse(st);p2=Evaluate.RealResult;
        if ((p1<0) || (p1>1) || p2<=0) {
          cout << "In Geometric ditribution 0 <= p <= 1 (probability of success) and T > 0 (duration of one step)" << endl;
          YYABORT;
        }
        break;
      }
    case 7:
      {
        if (np != 2) {
          cout << "Erlang distribution has two parameters: Shape and Rate, where Shape is a positive integer and Rate > 0" << endl;
          YYABORT;
        }


        string st=Par[0];
        Evaluate.parse(st);
        if(Evaluate.RealResult != Evaluate.IntResult || Evaluate.IntResult < 1){
          cout << "In Erlang distribution Shape is a positive integer" << endl;
          YYABORT;
        }

        st=Par[1];
        Evaluate.parse(st);
        if (Evaluate.RealResult<=0) {
          cout << "In Erlang distribution Rate > 0" << endl;
          YYABORT;
        }
        break;
      }
    case 8:
      {
        if (np != 2) {
          cout << "Gamma distribution has two parameters: Shape>0 and Scale>0" << endl;
          YYABORT;
        }


        string st=Par[0];
        Evaluate.parse(st);
        if(Evaluate.RealResult<=0){
          cout << "In Gamma distribution Shape > 0" << endl;
          YYABORT;
        }

        st=Par[1];
        Evaluate.parse(st);
        if (Evaluate.RealResult<=0) {
          cout << "In Gamma distribution Scale > 0" << endl;
          YYABORT;
        }
        break;
      }


    default: cout << "\nUnknown distribution !" << endl;
      YYABORT;

  }
 }
#line 1412 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 72:
#line 700 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {string st=(yystack_[0].value.expression);
   if(Evaluate.parse(st))
     {cout<<"The exponential ditribution is the only marking dependent distribution: '"<<st<<"'"<<endl;YYABORT; }
   else{std::ostringstream s;s<<Evaluate.RealResult;
     Par.push_back(s.str());
   }
 }
#line 1424 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 73:
#line 709 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {}
#line 1430 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 74:
#line 711 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {}
#line 1436 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 75:
#line 712 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {}
#line 1442 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 76:
#line 714 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {
  if(Reader.MyGspn.PlacesId.find(*(yystack_[6].value.name))==Reader.MyGspn.PlacesId.end())
    {
      std::cout<<"Place: "<<*(yystack_[6].value.name)<<" was not declared"<<std::endl;
      YYABORT;
    }
  if(Reader.MyGspn.TransList.find(*(yystack_[4].value.name))==Reader.MyGspn.TransList.end())
    {
      std::cout<<"Transition: "<<*(yystack_[4].value.name)<<" was not declared"<<std::endl;
      YYABORT;
    }
  string st=(yystack_[2].value.expression);

  if(Evaluate.parse(st)){
    Reader.MyGspn.inArcsStruct.insert(make_pair<pair<size_t,size_t>,arc>(Reader.MyGspn.arckey(Reader.MyGspn.TransId[*(yystack_[4].value.name)],Reader.MyGspn.PlacesId[*(yystack_[6].value.name)]), arc(st)));
  }else Reader.MyGspn.inArcsStruct.insert(make_pair<pair<size_t,size_t>,arc>(Reader.MyGspn.arckey(Reader.MyGspn.TransId[*(yystack_[4].value.name)],Reader.MyGspn.PlacesId[*(yystack_[6].value.name)]),arc(Evaluate.IntResult)));

 }
#line 1465 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 77:
#line 732 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {
  if(Reader.MyGspn.PlacesId.find(*(yystack_[4].value.name))==Reader.MyGspn.PlacesId.end())
    {
      std::cout<<"Place: "<<*(yystack_[4].value.name)<<" was not declared"<<std::endl;
      YYABORT;
    }
  if(Reader.MyGspn.TransList.find(*(yystack_[2].value.name))==Reader.MyGspn.TransList.end())
    {
      std::cout<<"Transition: "<<*(yystack_[2].value.name)<<" was not declared"<<std::endl;
      YYABORT;
    }
  Reader.MyGspn.inArcsStruct.insert(make_pair<pair<size_t,size_t>,arc>(Reader.MyGspn.arckey(Reader.MyGspn.TransId[*(yystack_[2].value.name)],Reader.MyGspn.PlacesId[*(yystack_[4].value.name)]),arc(1)));

 }
#line 1484 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 78:
#line 747 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {}
#line 1490 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 79:
#line 749 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {}
#line 1496 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 80:
#line 750 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {}
#line 1502 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 81:
#line 752 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {
  if(Reader.MyGspn.TransList.find(*(yystack_[6].value.name))==Reader.MyGspn.TransList.end())
    {
      std::cout<<"Transition: "<<*(yystack_[6].value.name)<<" was not declared"<<std::endl;
      YYABORT;
    }
  if(Reader.MyGspn.PlacesId.find(*(yystack_[4].value.name))==Reader.MyGspn.PlacesId.end())
    {
      std::cout<<"Place: "<<*(yystack_[4].value.name)<<" was not declared"<<std::endl;
      YYABORT;
    }
  string st=(yystack_[2].value.expression);

  if(Evaluate.parse(st)){
	  Reader.MyGspn.outArcsStruct.insert(make_pair<pair<size_t,size_t>,arc>(Reader.MyGspn.arckey(Reader.MyGspn.TransId[*(yystack_[6].value.name)],Reader.MyGspn.PlacesId[*(yystack_[4].value.name)]),arc(st)));
  }
  else Reader.MyGspn.outArcsStruct.insert(make_pair<pair<size_t,size_t>,arc>(Reader.MyGspn.arckey(Reader.MyGspn.TransId[*(yystack_[6].value.name)],Reader.MyGspn.PlacesId[*(yystack_[4].value.name)]),arc(Evaluate.IntResult)));

 }
#line 1526 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 82:
#line 771 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {
  if(Reader.MyGspn.TransList.find(*(yystack_[4].value.name))==Reader.MyGspn.TransList.end())
    {
      std::cout<<"Transition: "<<*(yystack_[4].value.name)<<" was not declared"<<std::endl;
      YYABORT;
    }
  if(Reader.MyGspn.PlacesId.find(*(yystack_[2].value.name))==Reader.MyGspn.PlacesId.end())
    {
      std::cout<<"Place: "<<*(yystack_[2].value.name)<<" was not declared"<<std::endl;
      YYABORT;
    }
  Reader.MyGspn.outArcsStruct.insert(make_pair<pair<size_t,size_t>,arc>(Reader.MyGspn.arckey(Reader.MyGspn.TransId[*(yystack_[4].value.name)],Reader.MyGspn.PlacesId[*(yystack_[2].value.name)]),arc(1)));
 }
#line 1544 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 83:
#line 787 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {}
#line 1550 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 84:
#line 789 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {}
#line 1556 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 85:
#line 790 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {}
#line 1562 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 86:
#line 792 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {
  if(Reader.MyGspn.PlacesId.find(*(yystack_[6].value.name))==Reader.MyGspn.PlacesId.end())
    {
      std::cout<<"Place: "<<*(yystack_[6].value.name)<<" was not declared"<<std::endl;
      YYABORT;
    }
  if(Reader.MyGspn.TransList.find(*(yystack_[4].value.name))==Reader.MyGspn.TransList.end())
    {
      std::cout<<"Transition: "<<*(yystack_[4].value.name)<<" was not declared"<<std::endl;
      YYABORT;
    }
  string st=(yystack_[2].value.expression);

  if(Evaluate.parse(st)){
    Reader.MyGspn.inhibArcsStruct.insert(make_pair<pair<size_t,size_t>,arc>(Reader.MyGspn.arckey(Reader.MyGspn.TransId[*(yystack_[4].value.name)],Reader.MyGspn.PlacesId[*(yystack_[6].value.name)]),arc(st)));
  }
  else Reader.MyGspn.inhibArcsStruct.insert(make_pair<pair<size_t,size_t>,arc>(Reader.MyGspn.arckey(Reader.MyGspn.TransId[*(yystack_[4].value.name)],Reader.MyGspn.PlacesId[*(yystack_[6].value.name)]),arc(Evaluate.IntResult)));

 }
#line 1586 "Gspn-parser.cc" // lalr1.cc:847
    break;

  case 87:
#line 812 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:847
    {
  if(Reader.MyGspn.PlacesId.find(*(yystack_[4].value.name))==Reader.MyGspn.PlacesId.end())
    {
      std::cout<<"Place: "<<*(yystack_[4].value.name)<<" was not declared"<<std::endl;
      YYABORT;
    }
  if(Reader.MyGspn.TransList.find(*(yystack_[2].value.name))==Reader.MyGspn.TransList.end())
    {
      std::cout<<"Transition: "<<*(yystack_[2].value.name)<<" was not declared"<<std::endl;
      YYABORT;
    }
  Reader.MyGspn.inhibArcsStruct.insert(make_pair<pair<size_t,size_t>,arc>(Reader.MyGspn.arckey(Reader.MyGspn.TransId[*(yystack_[2].value.name)],Reader.MyGspn.PlacesId[*(yystack_[4].value.name)]),arc(1)));

 }
#line 1605 "Gspn-parser.cc" // lalr1.cc:847
    break;


#line 1609 "Gspn-parser.cc" // lalr1.cc:847
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
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
        error (yyla.location, yysyntax_error_ (yystack_[0].state,
                                           yyempty ? yyempty_ : yyla.type_get ()));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyempty)
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyempty = true;
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
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
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
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
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyempty)
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
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyempty)
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  Gspn_parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  Gspn_parser::yysyntax_error_ (state_type yystate, symbol_number_type yytoken) const
  {
    std::string yyres;
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yytoken) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (yytoken != yyempty_)
      {
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short int Gspn_parser::yypact_ninf_ = -210;

  const signed char Gspn_parser::yytable_ninf_ = -1;

  const short int
  Gspn_parser::yypact_[] =
  {
      -7,    44,    62,     7,    13,    35,    48,    73,    58,    69,
      85,    59,   155,   120,   127,  -210,   162,   147,   147,    73,
     169,   175,  -210,   158,   176,  -210,  -210,  -210,   188,   195,
     196,   202,   199,   206,   183,   213,  -210,  -210,   194,  -210,
     203,   204,  -210,  -210,  -210,  -210,  -210,  -210,    55,    51,
     222,   207,   234,   223,   245,   246,  -210,  -210,    55,   241,
     242,   243,     9,  -210,  -210,  -210,    51,   244,   247,   248,
      56,   251,     0,  -210,   249,   215,   250,   230,  -210,    -5,
    -210,    -2,   161,    55,    55,    55,  -210,    55,    55,    55,
      55,    64,    51,    51,    51,  -210,    51,    51,    51,    51,
      51,   252,   255,  -210,   260,     1,  -210,   256,   226,   253,
    -210,   264,   261,   266,   263,  -210,   101,   105,   112,   257,
       2,     2,  -210,  -210,    83,    92,   119,   258,   258,   217,
     217,  -210,    55,  -210,   267,   265,  -210,   268,     5,  -210,
     269,   236,  -210,  -210,  -210,  -210,    55,    55,  -210,    55,
      51,    51,  -210,   168,    -1,  -210,   271,   270,  -210,   277,
      11,  -210,   272,   177,   184,   191,   129,   136,   275,   274,
     276,   278,   280,   282,  -210,   281,   279,  -210,   287,    12,
    -210,  -210,  -210,  -210,  -210,  -210,  -210,    51,    51,    51,
      51,    -3,   288,  -210,   284,   286,  -210,   141,   224,  -210,
     141,   289,   153,   290,    55,   292,   227,   291,  -210,    51,
    -210,    51,   293,    51,   198,  -210,    55,   295,   228,  -210,
     141,   285,    51,   231,   296,   205,  -210,    55,   297,   298,
     299,   100,   301,  -210,   302,   212,  -210,  -210,    51,  -210,
    -210,   300,  -210,  -210,   304,   305,   307,  -210,    77,  -210,
    -210,  -210,   303,   232,   182,   100,   308,   306,   309,   310,
    -210,  -210,  -210,   311,  -210
  };

  const unsigned char
  Gspn_parser::yydefact_[] =
  {
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
       0,     0,     0,     0,     0,     1,     0,    25,     0,     0,
       0,     0,    28,     0,     0,    30,    31,    32,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    26,     0,    27,
       0,     0,    35,    36,    38,    37,    40,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     3,     2,     0,     0,
       0,     0,     0,    15,    13,    14,     0,     0,     0,     0,
       0,     0,     0,    50,     0,     0,     0,    47,    42,     0,
      45,     0,     0,     0,     0,     0,    33,     0,     0,     0,
       0,     0,     0,     0,     0,    34,     0,     0,     0,     0,
       0,     0,     0,    51,     0,     0,    54,     0,     0,     0,
      48,     0,     0,     0,     0,     4,     0,     0,     0,     7,
       5,     6,     8,    16,     0,     0,     0,    17,    20,    18,
      19,    21,     0,    49,     0,     0,    55,     0,     0,    74,
       0,     0,    43,    41,    46,    44,     0,     0,     9,     0,
       0,     0,    22,     0,     0,    53,     0,     0,    75,     0,
       0,    79,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    73,     0,     0,    80,     0,     0,
      84,    11,    12,    10,    23,    24,    52,     0,     0,     0,
       0,     0,     0,    78,     0,     0,    85,    72,     0,    70,
      62,     0,     0,     0,     0,     0,     0,     0,    83,     0,
      69,     0,     0,     0,     0,    77,     0,     0,     0,    71,
      61,     0,     0,     0,     0,     0,    82,     0,     0,     0,
       0,     0,     0,    76,     0,     0,    87,    60,     0,    68,
      67,     0,    56,    81,     0,     0,     0,    86,     0,    57,
      63,    64,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    66,    65,     0,    59
  };

  const short int
  Gspn_parser::yypgoto_[] =
  {
    -210,   -57,   -49,  -210,  -210,   313,   294,  -210,   312,   314,
     316,   315,  -210,   273,  -210,  -210,  -210,  -210,   254,   317,
    -210,   201,  -209,  -182,  -210,    67,  -210,  -210,   108,  -210,
    -210,   185,  -210,  -210,   167,  -210,  -210,   149,  -210
  };

  const short int
  Gspn_parser::yydefgoto_[] =
  {
      -1,    62,   200,     4,     5,     6,     7,     8,    22,     9,
      10,    23,    79,    24,    81,    17,    18,    72,    73,    36,
     105,   106,   221,   201,   253,   241,   172,   198,   199,    53,
     138,   139,    77,   160,   161,   110,   179,   180,    37
  };

  const unsigned short int
  Gspn_parser::yytable_[] =
  {
      70,    82,   169,   111,   223,   204,   113,   205,   203,    71,
     104,     1,     2,    15,   137,    87,    86,    91,    90,     3,
     159,   178,    87,    88,    89,    90,   116,   117,   118,   245,
     119,   120,   121,   122,    13,    14,   112,   170,   171,   114,
     230,   102,   135,   124,   125,   126,   157,   127,   128,   129,
     130,   131,   176,   195,    63,    11,    64,    65,    56,    16,
      66,    57,    28,    95,    58,    29,     1,     2,    96,    97,
      98,    99,   100,    12,   123,   153,    96,    97,    98,    99,
     100,    67,    68,    69,     3,    59,    60,    61,     2,   163,
     164,   150,   165,    20,    21,    96,    97,    98,    99,   100,
     151,   166,   167,     1,    96,    97,    98,    99,   100,   146,
     250,   251,   252,   147,    87,    88,    89,    90,    87,    88,
      89,    90,   148,    32,   149,    87,    88,    89,    90,   152,
      33,    96,    97,    98,    99,   100,   239,   240,   197,   184,
     202,    96,    97,    98,    99,   100,   185,   214,    96,    97,
      98,    99,   100,    96,    97,    98,    99,   100,    30,   225,
     197,    31,   220,   212,   220,    96,    97,    98,    99,   100,
     235,   115,    35,    34,    87,    88,    89,    90,   168,    21,
      40,    87,    88,    89,    90,   257,    41,   181,   258,   220,
      87,    88,    89,    90,   182,    44,    20,    87,    88,    89,
      90,   183,    45,    46,    87,    88,    89,    90,   224,    47,
      48,    87,    88,    89,    90,   234,    52,    49,    87,    88,
      89,    90,   244,    50,    51,    87,    88,    89,    90,    96,
      97,    71,   209,   100,   210,   216,   227,   217,   228,   231,
     255,   232,   256,    54,    55,    75,    76,    74,    78,    80,
      83,    84,    85,    92,   101,   107,    93,    94,   104,   109,
     132,   108,   133,   134,   141,   137,   140,   142,   143,   144,
     145,   156,   155,    90,   100,   154,   162,   174,   159,   173,
     175,   178,   186,   187,   188,   191,   193,   189,   190,   192,
     194,   206,   207,   208,   218,   229,    42,   211,   213,   215,
      19,   222,   226,   233,   236,   237,   136,   238,   242,   243,
     246,   247,   254,   248,   249,   260,   261,   219,   264,   262,
     263,    25,   259,   158,    27,    26,   103,   177,   196,     0,
       0,    39,     0,     0,     0,    38,     0,     0,     0,    43
  };

  const short int
  Gspn_parser::yycheck_[] =
  {
      49,    58,     3,     8,   213,     8,     8,    10,   190,     9,
       9,    18,    19,     0,     9,    13,     7,    66,    16,    26,
       9,     9,    13,    14,    15,    16,    83,    84,    85,   238,
      87,    88,    89,    90,    27,    28,    41,    38,    39,    41,
     222,    41,    41,    92,    93,    94,    41,    96,    97,    98,
      99,   100,    41,    41,     3,    11,     5,     6,     3,    24,
       9,     6,     3,     7,     9,     6,    18,    19,    12,    13,
      14,    15,    16,    11,    10,   132,    12,    13,    14,    15,
      16,    30,    31,    32,    26,    30,    31,    32,    19,   146,
     147,     8,   149,    20,    21,    12,    13,    14,    15,    16,
       8,   150,   151,    18,    12,    13,    14,    15,    16,     8,
      33,    34,    35,     8,    13,    14,    15,    16,    13,    14,
      15,    16,    10,     3,    12,    13,    14,    15,    16,    10,
       3,    12,    13,    14,    15,    16,    36,    37,   187,    10,
     189,    12,    13,    14,    15,    16,    10,   204,    12,    13,
      14,    15,    16,    12,    13,    14,    15,    16,     3,   216,
     209,     6,   211,    10,   213,    12,    13,    14,    15,    16,
     227,    10,    25,    11,    13,    14,    15,    16,    10,    21,
      11,    13,    14,    15,    16,     3,    11,    10,     6,   238,
      13,    14,    15,    16,    10,     7,    20,    13,    14,    15,
      16,    10,     7,     7,    13,    14,    15,    16,    10,     7,
      11,    13,    14,    15,    16,    10,    22,    11,    13,    14,
      15,    16,    10,    40,    11,    13,    14,    15,    16,    12,
      13,     9,     8,    16,    10,     8,     8,    10,    10,     8,
       8,    10,    10,    40,    40,    11,    23,    40,     3,     3,
       9,     9,     9,     9,     3,    40,     9,     9,     9,    29,
       8,    11,     7,     3,    11,     9,    40,     3,     7,     3,
       7,     3,     7,    16,    16,     8,    40,     7,     9,     8,
       3,     9,     7,     9,     8,     3,     7,     9,     8,     8,
       3,     3,     8,     7,     3,    10,    23,     8,     8,     7,
       6,     8,     7,     7,     7,     7,   105,     8,     7,     7,
      10,     7,     9,     8,     7,     7,    10,   209,     7,    10,
      10,     8,   255,   138,    10,     9,    72,   160,   179,    -1,
      -1,    19,    -1,    -1,    -1,    18,    -1,    -1,    -1,    24
  };

  const unsigned char
  Gspn_parser::yystos_[] =
  {
       0,    18,    19,    26,    45,    46,    47,    48,    49,    51,
      52,    11,    11,    27,    28,     0,    24,    57,    58,    48,
      20,    21,    50,    53,    55,    47,    52,    51,     3,     6,
       3,     6,     3,     3,    11,    25,    61,    80,    61,    50,
      11,    11,    55,    53,     7,     7,     7,     7,    11,    11,
      40,    11,    22,    71,    40,    40,     3,     6,     9,    30,
      31,    32,    43,     3,     5,     6,     9,    30,    31,    32,
      44,     9,    59,    60,    40,    11,    23,    74,     3,    54,
       3,    56,    43,     9,     9,     9,     7,    13,    14,    15,
      16,    44,     9,     9,     9,     7,    12,    13,    14,    15,
      16,     3,    41,    60,     9,    62,    63,    40,    11,    29,
      77,     8,    41,     8,    41,    10,    43,    43,    43,    43,
      43,    43,    43,    10,    44,    44,    44,    44,    44,    44,
      44,    44,     8,     7,     3,    41,    63,     9,    72,    73,
      40,    11,     3,     7,     3,     7,     8,     8,    10,    12,
       8,     8,    10,    43,     8,     7,     3,    41,    73,     9,
      75,    76,    40,    43,    43,    43,    44,    44,    10,     3,
      38,    39,    68,     8,     7,     3,    41,    76,     9,    78,
      79,    10,    10,    10,    10,    10,     7,     9,     8,     9,
       8,     3,     8,     7,     3,    41,    79,    44,    69,    70,
      44,    65,    44,    65,     8,    10,     3,     8,     7,     8,
      10,     8,    10,     8,    43,     7,     8,    10,     3,    70,
      44,    64,     8,    64,    10,    43,     7,     8,    10,    10,
      65,     8,    10,     7,    10,    43,     7,     7,     8,    36,
      37,    67,     7,     7,    10,    64,    10,     7,     8,     7,
      33,    34,    35,    66,     9,     8,    10,     3,     6,    67,
       7,    10,    10,    10,     7
  };

  const unsigned char
  Gspn_parser::yyr1_[] =
  {
       0,    42,    43,    43,    43,    43,    43,    43,    43,    43,
      43,    43,    43,    44,    44,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    45,    45,    46,    46,    47,
      47,    48,    48,    49,    49,    50,    50,    51,    51,    52,
      52,    53,    54,    54,    55,    56,    56,    57,    57,    58,
      59,    59,    60,    61,    62,    62,    63,    63,    63,    63,
      63,    64,    65,    66,    66,    66,    66,    67,    67,    68,
      69,    69,    70,    71,    72,    72,    73,    73,    74,    75,
      75,    76,    76,    77,    78,    78,    79,    79,    80
  };

  const unsigned char
  Gspn_parser::yyr2_[] =
  {
       0,     2,     1,     1,     3,     3,     3,     3,     3,     4,
       6,     6,     6,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     4,     6,     6,     2,     3,     3,     2,     1,
       2,     2,     2,     6,     6,     2,     2,     4,     4,     4,
       4,     6,     1,     3,     6,     1,     3,     4,     5,     6,
       1,     2,     6,     6,     1,     2,    10,    12,    15,    17,
      10,     1,     1,     1,     1,     4,     4,     1,     1,     4,
       1,     3,     1,     6,     1,     2,     8,     6,     6,     1,
       2,     8,     6,     6,     1,     2,     8,     6,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const Gspn_parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "str", "chr", "rval", "ival",
  "SEMICOLON", "COMMA", "LB", "RB", "EQ", "DIV", "MUL", "PLUS", "MINUS",
  "POWER", "GspnName", "NbPl", "NbTr", "PList", "TList", "in", "out",
  "marking", "transitions", "Const", "INT", "DOUBLE", "inhibitor", "MIN",
  "MAX", "FLOOR", "SINGLE", "INFINITE", "MULTIPLE", "ENABLINGMEMORY",
  "AGEMEMORY", "IMDT", "EXPO", "'{'", "'}'", "$accept", "IntStringFormula",
  "RealStringFormula", "GSPN", "declarations", "Constants", "Sizes",
  "Constant", "Lists", "NbPlaces", "NbTransitions", "PlacesList",
  "PLabels", "TransitionsList", "TLabels", "definitions", "PlacesDef",
  "PLACES", "PLACE", "TransitionsDef", "TRANSITIONS", "TRANSITION",
  "WEIGHT", "PRIORITY", "SERVICE", "MEMORY", "dist", "params", "param",
  "InArcs", "incells", "incell", "OutArcs", "outcells", "outcell",
  "Inhibitors", "inhibcells", "inhibcell", "redifinitions", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  Gspn_parser::yyrline_[] =
  {
       0,   127,   127,   128,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   150,   151,   152,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   174,   175,   177,   178,   182,
     183,   185,   186,   188,   197,   206,   207,   209,   212,   221,
     224,   233,   248,   256,   265,   273,   278,   285,   286,   288,
     290,   291,   293,   310,   312,   313,   315,   339,   364,   404,
     444,   472,   483,   495,   496,   497,   507,   521,   522,   525,
     695,   696,   700,   709,   711,   712,   714,   732,   747,   749,
     750,   752,   771,   787,   789,   790,   792,   812,   829
  };

  // Print the state stack on the debug stream.
  void
  Gspn_parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Gspn_parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  // Symbol number corresponding to token number t.
  inline
  Gspn_parser::token_number_type
  Gspn_parser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,    40,     2,    41,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39
    };
    const unsigned int user_token_number_max_ = 294;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }


} // gspn
#line 2207 "Gspn-parser.cc" // lalr1.cc:1155
#line 831 "../../../../src/ModelGenerator/GspnParser/Gspn-parser.yy" // lalr1.cc:1156


void
gspn::Gspn_parser::error (const gspn::Gspn_parser::location_type& l,
                          const std::string& m)
{
  Reader.error (l, m);
}
