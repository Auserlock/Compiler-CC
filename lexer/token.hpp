#ifndef __TOKEN_H_
#define __TOKEN_H_

#include <iostream>
#include <string>
#include <array>
#include <iomanip>

namespace Token
{
    enum TokenType
    {
        LE = 256, //<=
        GE,       //>=
        EQ,       //==
        NE,       //!=
        LT,       //<
        GT,       //>
        INC,
        DEC,
        CONST_INT,
        CONST_FLOAT,
        CONST_CHAR,
        CONST_STRING,
        CONST_ID,
        VOID,
        INT,
        FLOAT,
        CHAR,
        WHILE,
        FOR,
        IF,
        ELSE,
        RETURN,
        BREAK,
        CONTINUE
    };
    template <TokenType... Tokens>
    struct TokenStringMap;

    template <>
    struct TokenStringMap<TokenType::LE, TokenType::GE, TokenType::EQ, TokenType::NE,
                          TokenType::LT, TokenType::GT, TokenType::INC, TokenType::DEC,
                          TokenType::CONST_INT, TokenType::CONST_FLOAT, TokenType::CONST_CHAR,
                          TokenType::CONST_STRING, TokenType::CONST_ID,
                          TokenType::VOID, TokenType::INT, TokenType::FLOAT,
                          TokenType::CHAR, TokenType::WHILE, TokenType::FOR,
                          TokenType::IF, TokenType::ELSE, TokenType::RETURN,
                          TokenType::BREAK, TokenType::CONTINUE>
    {
        static constexpr std::array<const char *, 24> strings = {
            "LE", "GE", "EQ", "NE", "LT", "GT", "INC", "DEC", "CONST_INT",
            "CONST_FLOAT", "CONST_CHAR", "CONST_STRING", "CONST_ID",
            "VOID", "INT", "FLOAT", "CHAR", "WHILE", "FOR",
            "IF", "ELSE", "RETURN", "BREAK", "CONTINUE"};
    };

    constexpr const char *get_token_string(TokenType token)
    {
        if (static_cast<int>(token) < 256)
        {
            return nullptr; // 不支持的类型
        }
        else
        {
            return TokenStringMap<TokenType::LE, TokenType::GE, TokenType::EQ, TokenType::NE,
                                  TokenType::LT, TokenType::GT, TokenType::INC, TokenType::DEC,
                                  TokenType::CONST_INT, TokenType::CONST_FLOAT, TokenType::CONST_CHAR,
                                  TokenType::CONST_STRING, TokenType::CONST_ID,
                                  TokenType::VOID, TokenType::INT, TokenType::FLOAT,
                                  TokenType::CHAR, TokenType::WHILE, TokenType::FOR,
                                  TokenType::IF, TokenType::ELSE, TokenType::RETURN,
                                  TokenType::BREAK, TokenType::CONTINUE>::strings[static_cast<int>(token) - 256];
        }
    }

    void print_token(TokenType token)
    {
        const char *token_str = get_token_string(token);
        if (token_str)
        {
            std::cout << std::setw(20) << token_str;
        }
        else
        {
            std::cout << std::setw(20) << static_cast<char>(token);
        }
    }

} // namespace Token
#endif