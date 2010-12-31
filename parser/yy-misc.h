#ifndef __STACKENING_PARSER_YY_MISC_H__
#define __STACKENING_PARSER_YY_MISC_H__

#include <string>

#include "../grammar/clause-builder.h"
#include "../misc/pos-type.h"

void yyerror(std::string const& msg);
int yyparse();
int yylex();
extern "C" int yywrap(void);

extern char* yytext;
extern int yylineno;
extern int yyleng;

namespace parser {

    int const SPACES_PER_INDENT = 4;

    extern grammar::clause_builder builder;

    misc::pos_type here();

}

#endif /* __STACKENING_PARSER_YY_MISC_H__ */
