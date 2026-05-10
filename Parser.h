#ifndef Parser_h
#define Parser_h

#include <string>
#include <vector>
#include "FormulaTree.h"

struct Token
{
    enum class TokenType { VARIABLE, CONSTANT, OPERATOR, UNARY_FUN, BINARY_FUN, PARENTHESIS_L, PARENTHESIS_R, COMMA };

    TokenType token_type;
    std::string val;
    Token(Token::TokenType t_type = TokenType::CONSTANT, const std::string& str = "") : token_type(t_type), val(str) {};
    Token(const Token& t) : token_type(t.token_type), val(t.val) {};
    Token& operator=(const Token& t)
    {
        if (this == &t)
            return *this;

        token_type = t.token_type;
        val = t.val;
        return *this;
    }
};

bool is_numeric(const std::string& str);

bool OperatorSign(const char& sign);

bool CheckToken(const std::string& str, Token& token);

Token CreateOperatorToken(const char sign);

int getPriority(const std::string& str);

std::unique_ptr<Node> ModifyToRPN(std::vector<Token>& tokens);

std::unique_ptr<Node> CreateBinFunNode(const std::string& str, std::unique_ptr<Node> left, std::unique_ptr<Node> right);

std::unique_ptr<Node> CreateUnFunNode(const std::string& str, std::unique_ptr<Node> child);

std::unique_ptr<Node> CreateFormulaTree(std::vector<Token>& RPN);

#endif // !Parser_h
