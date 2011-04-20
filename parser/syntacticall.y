%{
#include "syn-include.h"
%}

%union {
    int indent_type;
    int line_num_type;

    parser::op_img* op_type;
    parser::identifier* ident_type;
    parser::param_names* param_names_type;
    parser::arg_list* args_type;

    grammar::expr_base* expr_node;
}

%type <indent_type> indent

%type <line_num_type> eol

%type <ident_type> ident

%type <param_names_type> param_list
%type <param_names_type> additional_param

%type <args_type> actual_param_list
%type <args_type> additional_actual_param

%type <expr_node> cond
%type <expr_node> conj_cond
%type <expr_node> nega_cond
%type <expr_node> comp
%type <expr_node> expr
%type <expr_node> term
%type <expr_node> unary_factor
%type <expr_node> factor
%type <expr_node> ref
%type <expr_node> call

%type <op_type> add_op
%type <op_type> mul_op
%type <op_type> cmp_op
%type <op_type> pm_sign

%token INDENT EOL
%token KW_FUNC KW_IF KW_IFNOT KW_ELSE KW_RETURN
%token KW_TRUE KW_FALSE
%token LE GE NE AND OR
%token COMMA COLON
%token BOOL_TRUE BOOL_FALSE
%token INT_LITERAL DOUBLE_LITERAL
%token IDENT

%%

root:
    stmt_list
;

indent:
    INDENT
    {
        $$ = parser::last_indent;
    }
    |
    {
        $$ = 0;
    }
;

eol:
   EOL
   {
        $$ = yylineno;
        ++yylineno;
   }
;

stmt_list:
    stmt_list stmt {}
    |
    stmt_list clue {}
    |
    {}
;

stmt:
    INDENT eol {}
    |
    eol {}
    |
    var_def {}
    |
    arithmetics {}
    |
    func_return {}
;

clue:
    func_clue {}
    |
    if_clue {}
    |
    ifnot_clue {}
    |
    else_clue {}
;

var_def:
    indent ident ':' cond eol
    {
        parser::builder.add_var_def($1, $2->id, std::move(util::mkptr($4)));
        delete $2;
    }
;

arithmetics:
    indent cond eol
    {
        parser::builder.add_arith($1, std::move(util::mkptr($2)));
    }
;

func_return:
    indent KW_RETURN cond eol
    {
        parser::builder.add_return($1, std::move(util::mkptr($3)));
    }
    |
    indent KW_RETURN eol
    {
        parser::builder.add_return_nothing($1, parser::here($3));
    }
;

func_clue:
    indent KW_FUNC ident '(' param_list ')' eol
    {
        parser::builder.add_function($1, parser::here($7), $3->id, $5->get());
        delete $3;
        delete $5;
    }
;

param_list:
    additional_param ident
    {
        $$ = $1->add($2->id);
        delete $2;
    }
    |
    {
        $$ = new parser::param_names;
    }
;

additional_param:
    param_list ','
    {
        $$ = $1;
    }
    |
    {
        $$ = new parser::param_names;
    }
;

if_clue:
    indent KW_IF cond eol
    {
        parser::builder.add_if($1, std::move(util::mkptr($3)));
    }
;

ifnot_clue:
    indent KW_IFNOT cond eol
    {
        parser::builder.add_ifnot($1, std::move(util::mkptr($3)));
    }
;

else_clue:
    indent KW_ELSE eol
    {
        parser::builder.add_else($1, parser::here($3));
    }
;

ref:
    ident
    {
        $$ = new grammar::reference($1->pos, $1->id);
        delete $1;
    }
    |
    ident '@' INT_LITERAL
    {
        $$ = new grammar::func_reference($1->pos, $1->id, atoi(yytext));
        delete $1;
    }
;

ident:
    IDENT
    {
        $$ = new parser::identifier(parser::here(), yytext);
    }
;

cond:
    cond OR conj_cond
    {
        $$ = new grammar::disjunction($1->pos, std::move(util::mkptr($1)), std::move(util::mkptr($3)));
    }
    |
    conj_cond
    {
        $$ = $1;
    }
;

conj_cond:
    conj_cond AND nega_cond
    {
        $$ = new grammar::conjunction($1->pos, std::move(util::mkptr($1)), std::move(util::mkptr($3)));
    }
    |
    nega_cond
    {
        $$ = $1;
    }
;

nega_cond:
    '!' comp
    {
        $$ = new grammar::negation($2->pos, std::move(util::mkptr($2)));
    }
    |
    comp
    {
        $$ = $1;
    }
;

comp:
    comp cmp_op expr
    {
        $$ = new grammar::binary_op($1->pos, std::move(util::mkptr($1)), $2->img, std::move(util::mkptr($3)));
        delete $2;
    }
    |
    expr
    {
        $$ = $1;
    }
;

expr:
    expr add_op term
    {
        $$ = new grammar::binary_op($1->pos, std::move(util::mkptr($1)), $2->img, std::move(util::mkptr($3)));
        delete $2;
    }
    |
    term
    {
        $$ = $1;
    }
;

term:
    term mul_op unary_factor
    {
        $$ = new grammar::binary_op($1->pos, std::move(util::mkptr($1)), $2->img, std::move(util::mkptr($3)));
        delete $2;
    }
    |
    unary_factor
    {
        $$ = $1;
    }
;

unary_factor:
    pm_sign factor
    {
        $$ = new grammar::pre_unary_op($2->pos, $1->img, std::move(util::mkptr($2)));
        delete $1;
    }
    |
    factor
    {
        $$ = $1;
    }
;

factor:
    ref
    {
        $$ = $1;
    }
    |
    BOOL_TRUE
    {
        $$ = new grammar::bool_literal(parser::here(), true);
    }
    |
    BOOL_FALSE
    {
        $$ = new grammar::bool_literal(parser::here(), false);
    }
    |
    INT_LITERAL
    {
        $$ = new grammar::int_literal(parser::here(), yytext);
    }
    |
    DOUBLE_LITERAL
    {
        $$ = new grammar::float_literal(parser::here(), yytext);
    }
    |
    '(' cond ')'
    {
        $$ = $2;
    }
    |
    call
    {
        $$ = $1;
    }
;

cmp_op:
    '<'
    {
        $$ = new parser::op_img(yytext);
    }
    |
    '>'
    {
        $$ = new parser::op_img(yytext);
    }
    |
    GE
    {
        $$ = new parser::op_img(yytext);
    }
    |
    LE
    {
        $$ = new parser::op_img(yytext);
    }
    |
    '='
    {
        $$ = new parser::op_img(yytext);
    }
    |
    NE
    {
        $$ = new parser::op_img(yytext);
    }
;

add_op:
    '+'
    {
        $$ = new parser::op_img(yytext);
    }
    |
    '-'
    {
        $$ = new parser::op_img(yytext);
    }
;

mul_op:
    '*'
    {
        $$ = new parser::op_img(yytext);
    }
    |
    '/'
    {
        $$ = new parser::op_img(yytext);
    }
    |
    '%'
    {
        $$ = new parser::op_img(yytext);
    }
;

pm_sign:
    '+'
    {
        $$ = new parser::op_img(yytext);
    }
    |
    '-'
    {
        $$ = new parser::op_img(yytext);
    }
;

call:
    ident '(' actual_param_list ')'
    {
        $$ = new grammar::call($1->pos, $1->id, $3->deliver_args());
        delete $1;
        delete $3;
    }
;

actual_param_list:
    additional_actual_param cond
    {
        $$ = $1->add($2);
    }
    |
    {
        $$ = new parser::arg_list; 
    }
;

additional_actual_param:
    additional_actual_param cond ','
    {
        $$ = $1->add($2);
    }
    |
    {
        $$ = new parser::arg_list; 
    }
;
