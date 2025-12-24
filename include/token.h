#ifndef TOKEN_H
#define TOKEN_H
#include <string>

enum class TokenType {
    Number,
    Plus, Minus, Mul, Div,
    LParen, RParen,
    End
};

struct Token {
    TokenType type;
    std::string lexeme;
};

#endif //TOKEN_H
