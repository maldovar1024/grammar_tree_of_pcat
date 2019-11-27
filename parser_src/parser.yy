%skeleton "lalr1.cc"
%require "3.4"
%defines
%defines "include/parser.hh"

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  #include "ast/body.hh"
  #include "ast/expression.hh"
  #include "ast/type.hh"
  class driver;
}

%param { driver& drv }

%locations
%define api.location.include {"location.hh"}

%define parse.trace
%define parse.error verbose

%code {
  #include "driver/driver.hh"
}

%define api.token.prefix {TOK_}
%token
  EOF 0 "end of file"
  // keywords
  AND
  ARRAY
  BEGIN
  BY
  DIV
  DO
  ELSE
  ELSIF
  END
  EXIT
  FOR
  IF
  IN
  IS
  LOOP
  MOD
  NOT
  OF
  OR
  OUT
  PROCEDURE
  PROGRAM
  READ
  RECORD
  RETURN
  THEN
  TO
  TYPE
  VAR
  WHILE
  WRITE
  // operator
  ASSIGN      ":="
  PLUS        "+"
  MINUS       "-"
  TIMES       "*"
  DIVIDE      "/"
  GT          ">"
  LT          "<"
  EQ          "="
  GE          ">="
  LE          "<="
  NE          "<>"
  //delimiter
  COLON       ":"
  COMMA       ","
  SEMICOLON   ";"
  LPAREN      "("
  RPAREN      ")"
  DOT         "."
  LBRAC       "["
  RBRAC       "]"
  LBRACE      "{"
  RBRACE      "}"
  BRA         "[<"
  KET         ">]"
  SLASH       "\\"
;

%token <sptr<Identifier>> ID
%token <sptr<Constant>>   NUMBER
%token <sptr<Constant>>   STRING

%type <sptr<Body>> body

%type <sptr<Type>> type
%type <std::vector<RecordType::RecordComponent>> components

%type <sptr<DeclList>> declarations
%type <sptr<DeclList>> declaration
%type <sptr<DeclList>> var_decls
%type <sptr<VarDecl>>  var_decl
%type <sptr<Type>>     type_spec
%type <sptr<DeclList>> type_decls
%type <sptr<TypeDecl>> type_decl
%type <sptr<DeclList>> proc_decls
%type <sptr<ProcDecl>> proc_decl
%type <std::vector<ProcDecl::FPSection>> parameters
%type <std::vector<sptr<Identifier>>>    id_list

%type <sptr<StmtList>> statements
%type <sptr<Stmt>>     statement
%type <Write::WriteExpr> write_param
%type <std::vector<Write::WriteExpr>> write_params
%type <std::vector<sptr<Lvalue>>> read_params
%type <sptr<Expr>> for_step
%type <std::vector<If::IfComponent>> elsif_part
%type <sptr<StmtList>> else_part

%type <sptr<Expr>>     expression
%type <sptr<Lvalue>>   l_value
%type <sptr<ProcCall>> procedure_call
%type <std::vector<sptr<Expr>>> arguments
%type <std::vector<CompValue::Component>> comp_values
%type <std::vector<ArrayValue::ArrayComp>> array_values
%type <ArrayValue::ArrayComp> array_value


%%
%start program;

program: PROGRAM IS body ";" { drv.program = $body; } ;

body: declarations BEGIN statements END
  { $$ = std::make_shared<Body>(@$, $declarations, $statements); }
;

// declaration

declarations: %empty       { $$ = std::make_shared<DeclList>(); }
| declarations declaration { $$ = $1; $$->addDeclList($declaration); $$->setLocation(@$); }
;

declaration:
  VAR var_decls { $$ = $var_decls; $$->setLocation(@$); }
| PROCEDURE proc_decls { $$ = $proc_decls; $$->setLocation(@$); }
| TYPE type_decls { $$ = $type_decls; $$->setLocation(@$); }
;

var_decls:
  var_decl  { $$ = std::make_shared<DeclList>(@$); $$->addDecl($1); }
| var_decls var_decl { $$ = $1; $$->addDecl($var_decl); $$->setLocation(@$); }
;

var_decl: id_list type_spec ":=" expression ";"
  { $$ = std::make_shared<VarDecl>(@$, $id_list, $type_spec, $expression); }
;

type_decls:
  type_decl { $$ = std::make_shared<DeclList>(@$); $$->addDecl($1); }
| type_decls type_decl { $$ = $1; $$->addDecl($type_decl); $$->setLocation(@$); }
;

type_decl: ID IS type ";" { $$ = std::make_shared<TypeDecl>(@$, $ID, $type); };

proc_decls:
  proc_decl { $$ = std::make_shared<DeclList>(@$); $$->addDecl($proc_decl); }
| proc_decls proc_decl { $$ = $1; $$->addDecl($proc_decl); $$->setLocation(@$); }
;

proc_decl: ID "(" parameters ")" type_spec IS body ";"
  { $$ = std::make_shared<ProcDecl>(@$, $ID, $parameters, $type_spec, $body); }
;

type_spec: %empty { $$ = nullptr; }
| ":" type  { $$ = $type; }
;

parameters: %empty { }
| id_list ":" type  { $$.emplace_back(std::move($id_list), $type); }
| parameters ";" id_list ":" type { $$ = std::move($1); $$.emplace_back($id_list, $type); }
;


id_list:
  ID          { $$.push_back($ID); }
| id_list "," ID { $$ = $1; $$.push_back($ID); }
;

// expression

%nonassoc  ">" "<" "=" ">=" "<=" "<>";
%left "+" "-" OR;
%left "*" "/" MOD DIV AND;
%right uop NOT;

expression:
  NUMBER { $$ = $1; }
| l_value { $$ = $1; }
| "(" expression ")" { $$ = $2; }
| procedure_call { $$ = $1; }
| ID "{" comp_values "}" { $$ = std::make_shared<CompValue>(@$, $ID, $comp_values); }
| ID "[<" array_values ">]" { $$ = std::make_shared<ArrayValue>(@$, $ID, $array_values); }
| "+" expression %prec uop { $$ = std::make_shared<Unary>(@$, Operation::POS, $2); }
| "-" expression %prec uop { $$ = std::make_shared<Unary>(@$, Operation::NEG, $2); }
| NOT expression           { $$ = std::make_shared<Unary>(@$, Operation::NOT, $2); }
| expression "+"  expression { $$ = std::make_shared<Binary>(@$, $1, Operation::PLUS, $3);   }
| expression "-"  expression { $$ = std::make_shared<Binary>(@$, $1, Operation::MINUS, $3);  }
| expression "*"  expression { $$ = std::make_shared<Binary>(@$, $1, Operation::TIMES, $3);  }
| expression "/"  expression { $$ = std::make_shared<Binary>(@$, $1, Operation::DIVIDE, $3); }
| expression DIV  expression { $$ = std::make_shared<Binary>(@$, $1, Operation::DIV, $3);    }
| expression MOD  expression { $$ = std::make_shared<Binary>(@$, $1, Operation::MOD, $3);    }
| expression AND  expression { $$ = std::make_shared<Binary>(@$, $1, Operation::AND, $3);    }
| expression OR   expression { $$ = std::make_shared<Binary>(@$, $1, Operation::OR, $3);     }
| expression ">"  expression { $$ = std::make_shared<Binary>(@$, $1, Operation::GT, $3);     }
| expression "<"  expression { $$ = std::make_shared<Binary>(@$, $1, Operation::LT, $3);     }
| expression "="  expression { $$ = std::make_shared<Binary>(@$, $1, Operation::EQ, $3);     }
| expression ">=" expression { $$ = std::make_shared<Binary>(@$, $1, Operation::GE, $3);     }
| expression "<=" expression { $$ = std::make_shared<Binary>(@$, $1, Operation::LE, $3);     }
| expression "<>" expression { $$ = std::make_shared<Binary>(@$, $1, Operation::NE, $3);     }
;

comp_values:
  ID ":=" expression { $$.emplace_back($ID, $expression); }
| comp_values ";" ID ":=" expression { $$ = std::move($1); $$.emplace_back($ID, $expression); }
;

array_values:
  array_value { $$.push_back($1); }
| array_values "," array_value { $$ = std::move($1); $$.push_back($array_value); }
;

array_value:
  expression  { $$.first = $expression; $$.second = nullptr; }
| expression[n] OF expression[v] { $$.first = $n; $$.second = $v; }
;

type:
  ID { $$ = std::make_shared<TypeId>(@$, $ID); }
| ARRAY OF type[base] { $$ = std::make_shared<ArrayType>(@$, $base); }
| RECORD components END { $$ = std::make_shared<RecordType>(@$, $components); }
;

components:
  ID ":" type ";" { $$.emplace_back($ID, $type); }
| components ID ":" type ";" { $$ = std::move($1); $$.emplace_back($ID, $type); }
;

l_value:
  ID { $$ = std::make_shared<IdAccess>(@$, $1); }
| l_value "[" expression "]" { $$ = std::make_shared<ArrayAccess>(@$, $1, $expression); }
| l_value "." ID { $$ = std::make_shared<RecordAccess>(@$, $1, $ID); }
;

procedure_call: ID "(" arguments ")"  { $$ = std::make_shared<ProcCall>(@$, $ID, $arguments); };

arguments: %empty { }
| expression  { $$.push_back($expression); }
| arguments "," expression { $$ = std::move($1); $$.push_back($expression); }
;

// statement

statements:
  statement            { $$ = std::make_shared<StmtList>(@$); $$->addStmt($statement); }
| statements statement { $$ = $1; $$->addStmt($statement); $$->setLocation(@$); }
;

statement:
  WRITE "(" write_params ")" ";" { $$ = std::make_shared<Write>(@$, $write_params); }
| READ "(" read_params ")" ";" { $$ = std::make_shared<Read>(@$, $read_params); }
| l_value ":=" expression ";" { $$ = std::make_shared<Assign>(@$, $l_value, $expression); }
| procedure_call ";"          { $$ = std::make_shared<ProcCallStmt>(@$, $1); }
| RETURN ";"                  { $$ = std::make_shared<Return>(@$); }
| RETURN expression ";"       { $$ = std::make_shared<Return>(@$, $expression); }
| WHILE expression DO statements END ";" { $$ = std::make_shared<While>(@$, $expression, $statements); }
| FOR ID ":=" expression[start] TO expression[end] for_step DO statements END ";"
  { $$ = std::make_shared<For>(@$, $ID, $start, $end, $for_step, $statements); }
| LOOP statements END ";" { $$ = std::make_shared<Loop>(@$, $statements); }
| EXIT ";" { $$ = std::make_shared<Exit>(@$); }
| IF expression THEN statements else_part END ";"
  { $$ = std::make_shared<If>(@$, std::pair($expression, $statements), $else_part); }
| IF expression THEN statements elsif_part else_part END ";"
  { $$ = std::make_shared<If>(@$, std::pair($expression, $statements), $elsif_part, $else_part); }
;

elsif_part:
  ELSIF expression THEN statements { $$.emplace_back($expression, $statements); }
| elsif_part ELSIF expression THEN statements
  { $$ = std::move($1); $$.emplace_back($expression, $statements); }
;

else_part: %empty { $$ = nullptr; }
| ELSE statements { $$ = $statements; };

for_step: %empty { $$ = nullptr; }
| BY expression  { $$ = $expression; }
;

write_params: %empty           {  }
| write_param                  { $$.push_back($write_param); }
| write_params "," write_param { $$ = std::move($1); $$.push_back($write_param); }
;

write_param:
  STRING    { $$ = $1; }
| expression  { $$ = $1; }
;

read_params:
  l_value { $$.push_back($1); }
| read_params "," l_value { $$ = std::move($1); $$.push_back($l_value);   }
;

%%

void yy::parser::error(const location_type& l, const std::string& m) {
#define RED "\033[31m"
#define RESET "\033[0m"
  std::cerr << RED << l << ": " << m << RESET << std::endl;
}
