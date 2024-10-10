%{
#include "token.hpp"
#include <iostream>
#include <iomanip>
#include <string>
using std::string;
using namespace Token;
static int cur_line_num = 1;
void init_scanner();
%}

/* Definitions, Note: \042 is '"' */
INTEGER             ([0-9]+)
FLOAT               ([0-9]+\.[0-9]+)
CHARACTER           (\'[^\'\n]?\')
UNTERM_STRING       (\042[^\042\n]*)
STRING              (\042[^\042\n]*\042)
IDENTIFIER          ([_a-zA-Z][_a-zA-Z0-9]*)
OPERATOR            ([+*-/%=,;!(){}\[\]])
SINGLE_COMMENT      (\/\/[^\n]*)           
MULTI_COMMENT       (\/\*.*?\*\/)

%%

[\n]                { cur_line_num++;                       }
[ \t\r\a]+          { /* ignore all spaces */               }
{SINGLE_COMMENT}    { /* skip for single line comment */    }
{MULTI_COMMENT}     { /* skip for multi comment */          }

{OPERATOR}          { return yytext[0];         }   

"<="                { return Token::TokenType::LE;              }
">="                { return Token::TokenType::GE;              }
"=="                { return Token::TokenType::EQ;              }
"!="                { return Token::TokenType::NE;              }
"<"                 { return Token::TokenType::LT;              }
">"                 { return Token::TokenType::GT;              }
"++"                { return Token::TokenType::INC;              }
"--"                { return Token::TokenType::DEC;              }
"void"              { return Token::TokenType::VOID;            }
"int"               { return Token::TokenType::INT;             }
"float"             { return Token::TokenType::FLOAT;           }
"char"              { return Token::TokenType::CHAR;            }
"while"             { return Token::TokenType::WHILE;           }
"if"                { return Token::TokenType::IF;              }
"else"              { return Token::TokenType::ELSE;            }
"for"               { return Token::TokenType::FOR;             }
"return"            { return Token::TokenType::RETURN;          }
"break"             { return Token::TokenType::BREAK;           }
"continue"          { return Token::TokenType::CONTINUE;        }

{INTEGER}           { return Token::TokenType::CONST_INT;     }
{FLOAT}             { return Token::TokenType::CONST_FLOAT;   }
{CHARACTER}         { return Token::TokenType::CONST_CHAR; }
{STRING}            { return Token::TokenType::CONST_STRING; }
{IDENTIFIER}        { return Token::TokenType::CONST_ID;      }

<<EOF>>             { return 0; }

{UNTERM_STRING}     { std::cout << "Line Number " << cur_line_num << ": Unterminated string constant: \n" << yytext << std::endl; }
.                   { std::cout << "Line Number " << cur_line_num << ": Unrecognized character: " << yytext << std::endl; }

%%

int main(int argc, char* argv[]) {
    int token;
    init_scanner();
    while ((token = yylex())) {
         print_token(static_cast<Token::TokenType>(token)); 
        std::cout << yytext << std::endl;
    }
    return 0;
}

void init_scanner() {
    std::cout << std::left << std::setw(20) << "TOKEN-TYPE" << "TOKEN-VALUE" << std::endl;
    std::cout << "-------------------------------" << std::endl;
}

int yywrap() {
    return 1;
}
