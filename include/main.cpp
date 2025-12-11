#include <iostream>
#include <string>
#include <iomanip>
#include "lexer.h"
#include "parser.h"
#include "evaluator.h"
#include <algorithm>

int main() {
    std::cout << "Write the expression. Сommand: exit.\n";
    std::string expr;

    while (true) {
        std::cout << ">>> ";
        if (!std::getline(std::cin, expr)) break;

        auto not_space = [](unsigned char ch){ return !std::isspace(ch); };
        if (expr.empty() || std::find_if(expr.begin(), expr.end(), not_space) == expr.end())
            continue;

        if (expr == "exit") break;

        try {
            std::string postfix = Parser::toPostfix(expr);
            double result = Evaluator::eval(postfix);

            std::cout << "RPN: " << postfix << "\n";
            std::cout << std::setprecision(15) << "Аnswer: " << result << "\n";
        } catch (const std::exception& e) {
            std::cout << "ERROR: " << e.what() << "\n";
        }
    }

    return 0;
}