%{
#include "lex-include.h"
%}

%option nounput

%%
\r\n {
    ++yylineno;
    return EOL;
}

\r {
    ++yylineno;
    return EOL;
}

\n {
    ++yylineno;
    return EOL;
}

^[ ]+ {
    if (0 != yyleng % parser::SPACES_PER_INDENT) {
        parser::bad_indent(parser::here());
    }
    return INDENT;
}

^[ |\t]+ {
    parser::tab_as_indent(parser::here());
    return INDENT;
}

[\ ]+ {}
[\t]+ {}

func {
    return KW_FUNC;
}

ifnot {
    return KW_IFNOT;
}

if {
    return KW_IF;
}

else {
    return KW_ELSE;
}

while {
    return KW_WHILE;
}

return {
    return KW_RETURN;
}

[0-9]+ {
    return INT_LITERAL;
}

[0-9]*\.[0-9]+ {
    return DOUBLE_LITERAL;
}

[_a-zA-Z][_a-zA-Z0-9]* {
    return IDENT;
}

[<]= {
    return LE;
}

[>]= {
    return GE;
}

!= {
    return NE;
}

&& {
    return AND;
}

[\|][\|] {
    return OR;
}

[-+*/%=<>!()\[\],:] {
    return *yytext;
}

. {
    parser::invalid_char(parser::here(), *yytext);
}
%%
