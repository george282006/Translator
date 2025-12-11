#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "token.h"
#include "stack.h"
#include <string>
#include <sstream>
#include <stdexcept>

class Evaluator {
public:
    static double eval(const std::string& postfix) {
        std::istringstream in(postfix);
        std::string lex;
        Stack<double> st;

        while (in >> lex) {
            if (lex == "+" || lex == "-" || lex == "*" || lex == "/") {
                if (st.size() < 2) throw std::runtime_error("Not enough operands");
                double b = st.pop();
                double a = st.pop();
                if (lex == "+") st.push(a + b);
                else if (lex == "-") st.push(a - b);
                else if (lex == "*") st.push(a * b);
                else { // "/"
                    if (b == 0.0) throw std::runtime_error("Division by zero");
                    st.push(a / b);
                }
            } else {

                double val = std::stod(lex);
                st.push(val);
            }
        }

        if (st.size() != 1) throw std::runtime_error("Error");
        return st.pop();
    }
};

#endif // EVALUATOR_H
