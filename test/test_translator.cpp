
#include "gtest.h"
#include "translator.h"
TEST(LexerTest, NumbersOpsParens) {
    Lexer lx("12+3*(4-5)/6");
    auto tokens = lx.tokenize();
    ASSERT_FALSE(tokens.empty());
    EXPECT_EQ(tokens.back().type, TokenType::End);
}

TEST(LexerTest, InvalidSymbolThrows) {
    Lexer lx("2 + @");
    EXPECT_THROW(lx.tokenize(), std::runtime_error);
}

TEST(ParserTest, SimpleAddition) {
    auto postfix = Parser::toPostfix("2+3");
    EXPECT_EQ("2 3 +", postfix);
    EXPECT_EQ(5, Evaluator::eval(postfix));
}

TEST(ParserTest, PriorityMulOverAdd) {
    auto postfix = Parser::toPostfix("2+3*4");
    EXPECT_EQ("2 3 4 * +", postfix);
    EXPECT_EQ(14, Evaluator::eval(postfix));
}

TEST(ParserTest, ParenthesesChangeOrder) {
    auto postfix = Parser::toPostfix("(2+3)*4");
    EXPECT_EQ("2 3 + 4 *", postfix);
    EXPECT_EQ(20, Evaluator::eval(postfix));
}

TEST(ParserTest, ComplexExpression) {
    auto postfix = Parser::toPostfix("10-2*3+(8/4)");
    EXPECT_EQ("10 2 3 * - 8 4 / +", postfix);
    EXPECT_EQ(10 - 6 + 2, Evaluator::eval(postfix));
}

TEST(ParserTest, WhitespacesAndMultiDigits) {
    auto postfix = Parser::toPostfix("  123   +  456  ");
    EXPECT_EQ("123 456 +", postfix);
    EXPECT_EQ(579, Evaluator::eval(postfix));
}

TEST(ParserTest, UnbalancedParensThrow) {
    EXPECT_THROW(Parser::toPostfix("(2+3"), std::runtime_error);
    EXPECT_THROW(Parser::toPostfix("2+3)"), std::runtime_error);
}

TEST(EvaluatorTest, DivisionByZeroThrows) {
    auto postfix = Parser::toPostfix("5/0");
    EXPECT_THROW(Evaluator::eval(postfix), std::runtime_error);
}

TEST(EvaluatorTest, NotEnoughOperandsThrow) {
    EXPECT_THROW(Evaluator::eval("2 +"), std::runtime_error);
    EXPECT_THROW(Evaluator::eval("+"), std::runtime_error);
}

TEST(EvaluatorTest, ExtraOperandsThrow) {
    EXPECT_THROW(Evaluator::eval("2 3 + 4"), std::runtime_error);
}

TEST(EvaluatorTest, MixedOps) {
    auto postfix = Parser::toPostfix("7+(3*2)-8/4");
    EXPECT_EQ("7 3 2 * + 8 4 / -", postfix);
    EXPECT_EQ(7 + 6 - 2, Evaluator::eval(postfix));
}

TEST(EvaluatorTest, SingleNumber) {
    EXPECT_EQ(42, Evaluator::eval("42"));
}

TEST(EvaluatorTest, NegativeResult) {
    auto postfix = Parser::toPostfix("2-5");
    EXPECT_EQ("2 5 -", postfix);
    EXPECT_EQ(-3, Evaluator::eval(postfix));
}
TEST(TranslatorFloatTest, SimpleAdditionOfFloats) {
    std::string expr = "3.14 + 2.5";
    std::string postfix = Parser::toPostfix(expr);
    EXPECT_NEAR(5.64, Evaluator::eval(postfix), 1e-9);
}

TEST(TranslatorFloatTest, MixedIntAndFloatOps) {
    std::string expr = "2 + 0.5 * 4";
    std::string postfix = Parser::toPostfix(expr);
    EXPECT_NEAR(4.0, Evaluator::eval(postfix), 1e-9);
}

TEST(TranslatorFloatTest, ParenthesesWithFloats) {
    std::string expr = "0.5 * (1.2 + 3)";
    std::string postfix = Parser::toPostfix(expr);
    EXPECT_NEAR(2.1, Evaluator::eval(postfix), 1e-9);
}

TEST(TranslatorFloatTest, DivisionWithFloats) {
    std::string expr = "5.0 / 2";
    std::string postfix = Parser::toPostfix(expr);
    EXPECT_NEAR(2.5, Evaluator::eval(postfix), 1e-9);
}

// Ошибки формата числа: две точки подряд и точка в конце
TEST(TranslatorErrorNumberFormatTest, DoubleDotInsideNumber) {
    std::string expr = "1..2 + 3";
    EXPECT_THROW(Parser::toPostfix(expr), std::runtime_error);
}

TEST(TranslatorErrorNumberFormatTest, TrailingDotInNumber) {
    std::string expr = "3. + 2";
    EXPECT_THROW(Parser::toPostfix(expr), std::runtime_error);
}

TEST(TranslatorErrorNumberFormatTest, MultipleDotsInNumber) {
    std::string expr = "1.2.3";
    EXPECT_THROW(Parser::toPostfix(expr), std::runtime_error);
}

TEST(TranslatorErrorNumberFormatTest, DotAtStartIsInvalid) {
    std::string expr = ".5 + 1";
    EXPECT_THROW(Parser::toPostfix(expr), std::runtime_error);
}

TEST(TranslatorErrorNumberFormatTest, DoubleDotAtEnd) {
    std::string expr = "2..";
    EXPECT_THROW(Parser::toPostfix(expr), std::runtime_error);
}


TEST(LexerFloatTokensTest, TokenizeValidFloat) {
    Lexer lexer("12.34");
    auto tokens = lexer.tokenize();
    ASSERT_GE(tokens.size(), 2);
    EXPECT_EQ(TokenType::Number, tokens[0].type);
    EXPECT_EQ("12.34", tokens[0].lexeme);
    EXPECT_EQ(TokenType::End, tokens[1].type);
}

TEST(LexerFloatTokensTest, TokenizeInvalidDoubleDot) {
    Lexer lexer("7..8");
    EXPECT_THROW(lexer.tokenize(), std::runtime_error);
}

TEST(LexerFloatTokensTest, TokenizeInvalidTrailingDot) {
    Lexer lexer("9.");
    EXPECT_THROW(lexer.tokenize(), std::runtime_error);
}