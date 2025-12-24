#ifndef LEXER_H
#define LEXER_H
#include "token.h"
#include <vector>
#include <stdexcept>
#include <cctype>

class Lexer {
public:
    explicit Lexer(const std::string &input) : text(input), pos(0) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        enum class State { Start, InNumber } state = State::Start;
        std::string num;
        bool hasDot = false;

        while (true) {
            char c = (pos < text.size()) ? text[pos] : '\0';

            if (state == State::Start) {
                hasDot = false;

                if (c == '\0') {
                    tokens.push_back({TokenType::End, ""});
                    return tokens;
                }
                if (std::isspace(static_cast<unsigned char>(c))) {
                    ++pos;
                    continue;
                }

                if (std::isdigit(static_cast<unsigned char>(c))) {
                    num.clear();
                    num.push_back(c);
                    ++pos;
                    state = State::InNumber;
                    continue;
                }

                if (c == '+') { tokens.push_back({TokenType::Plus, "+"});  ++pos; continue; }
                if (c == '-') { tokens.push_back({TokenType::Minus, "-"}); ++pos; continue; }
                if (c == '*') { tokens.push_back({TokenType::Mul, "*"});   ++pos; continue; }
                if (c == '/') { tokens.push_back({TokenType::Div, "/"});   ++pos; continue; }
                if (c == '(') { tokens.push_back({TokenType::LParen, "("});++pos; continue; }
                if (c == ')') { tokens.push_back({TokenType::RParen, ")"});++pos; continue; }

                throw std::runtime_error("Invalid character");
            } else {

                if (std::isdigit(static_cast<unsigned char>(c))) {
                    num.push_back(c);
                    ++pos;
                } else if (c == '.') {
                    if (hasDot) {
                        throw std::runtime_error("Incorrect number format");
                    }
                    hasDot = true;
                    num.push_back(c);
                    ++pos;
                } else {
                    if (!num.empty() && num.back() == '.') {
                        throw std::runtime_error("Incorrect number format");
                    }
                    tokens.push_back({TokenType::Number, num});
                    state = State::Start;
                }
            }
        }
    }

private:
    std::string text;
    std::size_t pos;
};

#endif //LEXER_H
