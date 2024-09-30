%{
#include "token.h"
int cur_line_num = 1;
void init_scanner();
void lex_error(char* msg, int line);
%}

/* Definitions, note: \042 is '"' */
INTEGER             ([0-9]+)
UNTERM_STRING       (\042[^\042\n]*)
STRING              (\042[^\042\n]*\042)
IDENTIFIER          ([_a-zA-Z][_a-zA-Z0-9]*)
OPERATOR            ([+*-/%=,;!(){}])
SINGLE_COMMENT1     (\/)(\*).*(\*)(\/)

%%

[\n]                { cur_line_num++;                       }
[ \t\r\a]+          { /* ignore all spaces */               }
{SINGLE_COMMENT1}   { /* skip for single line comment */    }

{OPERATOR}          { return yytext[0];         }   

"<="                { return LE;              }
">="                { return GE;              }
"=="                { return EQ;              }
"!="                { return NE;              }
"<"                { return LT;             }
">"                { return GT;              }
"void"              { return VOID;            }
"int"               { return INT;             }
"while"             { return WHILE;           }
"if"                { return IF;              }
"else"              { return ELSE;            }
"return"            { return RETURN;          }
"break"             { return BREAK;           }
"continue"          { return CONTINUE;        }

{INTEGER}           { return CONST_INT;     }
{IDENTIFIER}        { return CONST_ID;      }

<<EOF>>             { return 0; }

{UNTERM_STRING}     { lex_error("Unterminated string constant", cur_line_num);  }
.                   { lex_error("Unrecognized character", cur_line_num);        }

%%

int main(int argc, char* argv[]) {
    int token;
    init_scanner();
    while (token = yylex()) {
        print_token(token);
        puts(yytext);
    }
    return 0;
}

void init_scanner() {
    printf("%-20s%s\n", "TOKEN-TYPE", "TOKEN-VALUE");
    printf("------------------------\n");
}

void lex_error(char* msg, int line) {
    printf("\nError at line %-3d: %s\n\n", line, msg);
}

int yywrap(void) {
    return 1;
}
