%{
#include <iostream>
#include <cstdio>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <string>
#include "driver/driver.hh"
#include "driver/keywords.hh"
#include "parser.hh"
%}

%option noyywrap nounput noinput batch debug

%{
  void skip_COMMENT(const char* comment, yy::location& loc);

  yy::parser::symbol_type
  make_IDENTIFIER(const std::string& s, const yy::parser::location_type& loc);

  yy::parser::symbol_type
  make_INTEGER(const std::string& s, const yy::parser::location_type& loc);

  yy::parser::symbol_type
  make_STRING(const std::string& s, const yy::parser::location_type& loc);
%}

BLANK          ([ \t\r])
INTEGER        ([0-9]+)
REAL           ([0-9]+\.[0-9]*)
IDENTIFIER     ([a-zA-Z][a-zA-Z0-9]*)
STRING         (\"[^\"\n]*\")
UNTERM_STRING  (\"[^\"\n]*)
COMMENT        (\(\*(([^\*]*(\*[^\)])?)*)\*\))
UNTERM_COMMENT ("(*"(([^\*]*(\*[^\)])?)*))

%{
  # define YY_USER_ACTION  loc.step(); loc.columns(yyleng);
%}

%%

%{
  yy::location& loc = drv.location;
%}

{BLANK}+         { }
\n+              { loc.lines(yyleng); }
{COMMENT}        { skip_COMMENT(yytext, loc); }

{REAL}           { return yy::parser::make_NUMBER(
                    std::make_shared<Constant>(loc, yytext, Constant::REAL), loc); }
{INTEGER}        { return make_INTEGER(yytext, loc); }
{STRING}         { return make_STRING(std::string(yytext), loc); }
{IDENTIFIER}     { return make_IDENTIFIER(yytext, loc); }

":="    { return yy::parser::make_ASSIGN(loc); }
"+"     { return yy::parser::make_PLUS(loc);   }
"-"     { return yy::parser::make_MINUS(loc);  }
"*"     { return yy::parser::make_TIMES(loc);  }
"/"     { return yy::parser::make_DIVIDE(loc); }
">"     { return yy::parser::make_GT(loc);     }
"<"     { return yy::parser::make_LT(loc);     }
"="     { return yy::parser::make_EQ(loc);     }
">="    { return yy::parser::make_GE(loc);     }
"<="    { return yy::parser::make_LE(loc);     }
"<>"    { return yy::parser::make_NE(loc);     }

":"     { return yy::parser::make_COLON(loc);     }
","     { return yy::parser::make_COMMA(loc);     }
";"     { return yy::parser::make_SEMICOLON(loc); }
"("     { return yy::parser::make_LPAREN(loc);    }
")"     { return yy::parser::make_RPAREN(loc);    }
"."     { return yy::parser::make_DOT(loc);       }
"["     { return yy::parser::make_LBRAC(loc);     }
"]"     { return yy::parser::make_RBRAC(loc);     }
"{"     { return yy::parser::make_LBRACE(loc);    }
"}"     { return yy::parser::make_RBRACE(loc);    }
"[<"    { return yy::parser::make_BRA(loc);       }
">]"    { return yy::parser::make_KET(loc);       }
"\\"    { return yy::parser::make_SLASH(loc);     }

{UNTERM_COMMENT} { skip_COMMENT(yytext, loc);
                   throw yy::parser::syntax_error(loc, "unterminated comment"); }
{UNTERM_STRING}  { throw yy::parser::syntax_error(loc, "unterminated string"); }
.                { throw yy::parser::syntax_error(loc, "invalid character: "
                   + std::string(yytext)); }

<<EOF>>          { return yy::parser::make_EOF(loc); }

%%

void skip_COMMENT(const char* comment, yy::location& loc) {
  int pos = -1;
  int nlines = 0;
  int i = 0;
  // 寻找最后一个'\n'
  while (comment[i]) {
    if (comment[i] == '\n') {
      pos = i;
      nlines++;
    }
    i++;
  }
    
  if (nlines != 0) { // 多行注释
    loc.lines(nlines);
    loc.columns(i - pos);
  }
}

yy::parser::symbol_type
make_IDENTIFIER(const std::string& s, const yy::parser::location_type& loc) {
  if (s.size() > 255) {
    throw yy::parser::syntax_error(loc, "too long identifier: " + s);
  }

  auto item = keywords.find(s);
  if (item != keywords.end()) {
    return (item->second)(loc);
  }
  else {
    return yy::parser::make_ID(std::make_shared<Identifier>(loc, s), loc);
  }
}

yy::parser::symbol_type
make_INTEGER(const std::string& s, const yy::parser::location_type& loc) {
  try {
    std::stoi(s);
    return yy::parser::make_NUMBER(std::make_shared<Constant>(loc, s, Constant::INTEGER), loc);
  }
  catch(const std::out_of_range& e) {
    throw yy::parser::syntax_error(loc, "integer out of range");
  }
}

yy::parser::symbol_type
make_STRING(const std::string& s, const yy::parser::location_type& loc) {
  if (s.size() > 255 + 2) {
    throw yy::parser::syntax_error(loc, "too long string: " + s);
  }
  else if (s.find('\t') != std::string::npos) {
    throw yy::parser::syntax_error(loc, "tab in string: " + s);
  }

  return yy::parser::make_STRING(std::make_shared<Constant>(loc, s, Constant::STRING), loc);
}

void driver::begin_scan() {
  using namespace std;
  yy_flex_debug = debug_mode;
  yyin = fopen(file.c_str(), "r");
  if (!yyin) {
    cerr << "Cannot open " << file << ": " << strerror(errno) << endl;
    exit(EXIT_FAILURE);
  }
}

void driver::end_scan() {
  std::fclose(yyin);
}
