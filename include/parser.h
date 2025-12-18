#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "stack.h"
#include <sstream>
#include <stdexcept>

class Parser {
public:
    static std::string toPostfix(const std::string& expr) {
        Lexer lexer(expr);
        std::vector<Token> tokens = lexer.tokenize();
        tokens = handleUnaryOperators(tokens);
        validateExpression(tokens);

        Stack<Token> opStack;
        std::ostringstream output;
        bool firstToken = true;

        for (const auto& tok : tokens) {
            if (tok.type == TokenType::End) break;

            if (tok.type == TokenType::Number) {
                if (!firstToken) output << ' ';
                output << tok.lexeme;
                firstToken = false;
            }
            else if (tok.type == TokenType::LParen) {
                opStack.push(tok);
            }
            else if (tok.type == TokenType::RParen) {
                while (!opStack.empty() && opStack.top().type != TokenType::LParen) {
                    output << ' ' << opStack.pop().lexeme;
                }
                if (opStack.empty()) {
                    throw std::runtime_error("Mismatched parentheses");
                }
                opStack.pop();
            }
            else {
                while (!opStack.empty() &&
                       opStack.top().type != TokenType::LParen &&
                       precedence(opStack.top().type) >= precedence(tok.type)) {
                    output << ' ' << opStack.pop().lexeme;
                }
                opStack.push(tok);
            }
        }

        while (!opStack.empty()) {
            if (opStack.top().type == TokenType::LParen) {
                throw std::runtime_error("Mismatched parentheses");
            }
            output << ' ' << opStack.pop().lexeme;
        }

        return output.str();
    }

private:
    static int precedence(TokenType type) {
        switch (type) {
            case TokenType::Plus:
            case TokenType::Minus:
                return 1;
            case TokenType::Mul:
            case TokenType::Div:
                return 2;
            default:
                return 0;
        }
    }

    static bool isOperator(TokenType type) {
        return type == TokenType::Plus || type == TokenType::Minus ||
               type == TokenType::Mul || type == TokenType::Div;
    }

    static std::vector<Token> handleUnaryOperators(const std::vector<Token>& tokens) {
        std::vector<Token> result;

        for (size_t i = 0; i < tokens.size(); ++i) {
            const Token& tok = tokens[i];

            if (tok.type == TokenType::End) {
                result.push_back(tok);
                break;
            }

            if ((tok.type == TokenType::Plus || tok.type == TokenType::Minus)) {
                bool isUnary = result.empty() ||
                              result.back().type == TokenType::LParen ||
                              isOperator(result.back().type);

                if (isUnary) {
                    int sign = 1;
                    size_t j = i;
                    while (j < tokens.size() &&
                           (tokens[j].type == TokenType::Plus || tokens[j].type == TokenType::Minus)) {
                        if (tokens[j].type == TokenType::Minus) {
                            sign = -sign;
                        }
                        ++j;
                    }

                    if (j >= tokens.size() ||
                        (tokens[j].type != TokenType::Number && tokens[j].type != TokenType::LParen)) {
                        throw std::runtime_error("Invalid expression: operator without operand");
                    }

                    if (tokens[j].type == TokenType::Number) {
                        std::string numValue = tokens[j].lexeme;
                        if (sign == -1) {
                            if (numValue[0] == '-') {
                                numValue = numValue.substr(1);
                            } else {
                                numValue = "-" + numValue;
                            }
                        }
                        result.push_back({TokenType::Number, numValue});
                        i = j;
                    } else {
                        if (sign == -1) {
                            result.push_back({TokenType::Number, "0"});
                            result.push_back({TokenType::Minus, "-"});
                        }
                        i = j - 1;
                    }
                    continue;
                }
            }

            result.push_back(tok);
        }

        return result;
    }

    static void validateExpression(const std::vector<Token>& tokens) {
        if (tokens.empty() || tokens[0].type == TokenType::End) {
            throw std::runtime_error("Empty expression");
        }

        int parenBalance = 0;
        bool expectOperand = true;

        for (size_t i = 0; i < tokens.size(); ++i) {
            const Token& tok = tokens[i];

            if (tok.type == TokenType::End) {
                if (expectOperand) {
                    throw std::runtime_error("Expression ends with operator");
                }
                break;
            }

            if (tok.type == TokenType::Number) {
                if (!expectOperand) {
                    throw std::runtime_error("Unexpected number");
                }
                expectOperand = false;
            }
            else if (tok.type == TokenType::LParen) {
                if (!expectOperand) {
                    throw std::runtime_error("Missing operator before '('");
                }
                parenBalance++;
                expectOperand = true;
            }
            else if (tok.type == TokenType::RParen) {
                if (expectOperand) {
                    // Пустые скобки или оператор перед )
                    throw std::runtime_error("Invalid expression: empty parentheses or operator before ')'");
                }
                parenBalance--;
                if (parenBalance < 0) {
                    throw std::runtime_error("Mismatched parentheses");
                }
                expectOperand = false;
            }
            else if (isOperator(tok.type)) {
                if (expectOperand) {
                    throw std::runtime_error("Unexpected operator");
                }
                expectOperand = true;
            }
        }

        if (parenBalance != 0) {
            throw std::runtime_error("Mismatched parentheses");
        }
    }
};

#endif //PARSER_H