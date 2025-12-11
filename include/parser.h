
#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "lexer.h"
#include "stack.h"
#include <string>
#include <vector>
#include <stdexcept>


class Parser {
public:
    static std::string toPostfix(const std::string &expr) {
        Lexer lexer(expr);
        auto tokens = lexer.tokenize();

        Stack<Token> ops;
        std::vector<Token> out;

        for (auto &tok : tokens) {
            if (tok.type == TokenType::Number) {
                out.push_back(tok);
            }
            else if (isOp(tok.type)) {
                while (!ops.empty() && isOp(ops.top().type) &&
                       prio(ops.top().type) >= prio(tok.type)) {
                    out.push_back(ops.pop());
                }
                ops.push(tok);
            }
            else if (tok.type == TokenType::LParen) {
                ops.push(tok);
            }
            else if (tok.type == TokenType::RParen) {
                bool found = false;
                while (!ops.empty()) {
                    Token t = ops.pop();
                    if (t.type == TokenType::LParen) {
                        found = true;
                        break;
                    }
                    out.push_back(t);
                }
                if (!found)
                    throw std::runtime_error("Inconsistent parentheses");
            }
            else if (tok.type == TokenType::End) {
                break;
            }
        }

        while (!ops.empty()) {
            Token t = ops.pop();
            if (t.type == TokenType::LParen || t.type == TokenType::RParen)
                throw std::runtime_error("Inconsistent parentheses");
            out.push_back(t);
        }

        std::string res;
        for (size_t i = 0; i < out.size(); ++i) {
            if (out[i].type == TokenType::Number)
                res += out[i].lexeme;
            else
                res += opStr(out[i].type);
            if (i + 1 < out.size())
                res += ' ';
        }
        return res;
    }

private:
    static bool isOp(TokenType t) {
        return t == TokenType::Plus || t == TokenType::Minus ||
               t == TokenType::Mul || t == TokenType::Div;
    }
    static int prio(TokenType t) {
        if (t == TokenType::Plus || t == TokenType::Minus) return 1;
        if (t == TokenType::Mul || t == TokenType::Div) return 2;
        return 0;
    }
    static std::string opStr(TokenType t) {
        if (t == TokenType::Plus) return "+";
        if (t == TokenType::Minus) return "-";
        if (t == TokenType::Mul) return "*";
        if (t == TokenType::Div) return "/";
        return "";
    }
};

#endif //PARSER_H
