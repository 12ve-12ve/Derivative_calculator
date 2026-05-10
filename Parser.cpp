#include "Parser.h"
#include <sstream>
#include <stack>
#include "FormulaTree.h"
#include <iostream>

bool is_numeric(const std::string& str)
{
    double result;
    std::istringstream input(str);
    input >> result;
    return !input.fail() && input.eof();
}

bool OperatorSign(const char& sign)
{
    if (sign == '-' || sign == '+' || sign == '/' || sign == '*' || sign == '(' || sign == ')' || sign == ',')
        return true;
    return false;
}

bool CheckToken(const std::string& str, Token& token)
{
    if (str == "x")
    {
        token.val = str;
        token.token_type = Token::TokenType::VARIABLE;
        return true;
    }
    if (is_numeric(str) || str == "e" || str == "PI")
    {
        token.val = str;
        token.token_type = Token::TokenType::CONSTANT;
        return true;
    }
    if (str == "sin" || str == "cos" || str == "tan" || str == "arcsin" || str == "arccos" || str == "arctan" ||
        str == "sinh" || str == "cosh" || str == "tanh" || str == "arcsinh" || str == "arccosh" || str == "arctanh" ||
        str == "sqrt" || str == "ln" || str == "log10")
    {
        token.val = str;
        token.token_type = Token::TokenType::UNARY_FUN;
        return true;
    }
    if (str == "pow" || str == "log")
    {
        token.val = str;
        token.token_type = Token::TokenType::BINARY_FUN;
        return true;
    }
    return false;
}

Token CreateOperatorToken(const char sign)
{
    if (sign == '+' || sign == '-' || sign == '*' || sign == '/')
        return Token(Token::TokenType::OPERATOR, std::string(1, sign));
    else if (sign == '(')
        return Token(Token::TokenType::PARENTHESIS_L, std::string(1, sign));
    else if (sign == ')')
        return Token(Token::TokenType::PARENTHESIS_R, std::string(1, sign));
    else
        return Token(Token::TokenType::COMMA, std::string(1, sign));
}

int getPriority(const std::string& str)
{
    if (str == "+" || str == "-")
        return 1;
    if (str == "*" || str == "/")
        return 2;
    if (str == "pow")
        return 3;
    return 0;
}

std::unique_ptr<Node> ModifyToRPN(std::vector<Token>& tokens)
{
    std::vector<Token> RPN_output;
    std::vector<Token> operators;

    for (int i = 0; i < tokens.size(); ++i)
    {
        if (tokens[i].token_type == Token::TokenType::VARIABLE || tokens[i].token_type == Token::TokenType::CONSTANT)
            RPN_output.push_back(tokens[i]);
        else if (tokens[i].token_type == Token::TokenType::UNARY_FUN || tokens[i].token_type == Token::TokenType::BINARY_FUN ||
            tokens[i].token_type == Token::TokenType::PARENTHESIS_L)
            operators.push_back(tokens[i]);
        else if (tokens[i].token_type == Token::TokenType::PARENTHESIS_R || tokens[i].token_type == Token::TokenType::COMMA)
        {
            while (!operators.empty() && operators.back().token_type != Token::TokenType::PARENTHESIS_L)
            {
                RPN_output.push_back(operators.back());
                operators.pop_back();
            }
            if(tokens[i].token_type == Token::TokenType::PARENTHESIS_R)
            {
                operators.pop_back();

                if (!operators.empty() && (operators.back().token_type == Token::TokenType::UNARY_FUN || operators.back().token_type == Token::TokenType::BINARY_FUN))
                {
                    RPN_output.push_back(operators.back());
                    operators.pop_back();
                }
            }
        }
        else if (tokens[i].token_type == Token::TokenType::OPERATOR)
        {
            while (!operators.empty() && operators.back().token_type == Token::TokenType::OPERATOR && getPriority(operators.back().val) >= getPriority(tokens[i].val))
            {
                RPN_output.push_back(operators.back());
                operators.pop_back();
            }
            operators.push_back(tokens[i]);
        }
    }
    while (!operators.empty())
    {
        RPN_output.push_back(operators.back());
        operators.pop_back();
    }

    return CreateFormulaTree(RPN_output);
}

std::unique_ptr<Node> CreateBinFunNode(const std::string& str, std::unique_ptr<Node> left, std::unique_ptr<Node> right)
{
    if (str == "+")
        return std::make_unique<AddNode>(std::move(left), std::move(right));
    else if (str == "-")
        return std::make_unique<SubNode>(std::move(left), std::move(right));
    else if (str == "*")
        return std::make_unique<MultNode>(std::move(left), std::move(right));
    else if (str == "/")
        return std::make_unique<DivNode>(std::move(left), std::move(right));
    else if (str == "pow")
        return std::make_unique<PowNode>(std::move(left), std::move(right));
    else if (str == "log")
        return std::make_unique<LogNode>(std::move(left), std::move(right));
    else
        return nullptr;
}

std::unique_ptr<Node> CreateUnFunNode(const std::string& str, std::unique_ptr<Node> child)
{
    if (str == "sin")
        return std::make_unique<SinNode>(std::move(child));
    else if (str == "cos")
        return std::make_unique<CosNode>(std::move(child));
    else if (str == "tan")
        return std::make_unique<TanNode>(std::move(child));
    else if (str == "sqrt")
        return std::make_unique<SqrtNode>(std::move(child));
    else if (str == "ln")
        return std::make_unique<LnNode>(std::move(child));
    else if (str == "log10")
        return std::make_unique<Log10Node>(std::move(child));
    else if (str == "arcsin")
        return std::make_unique<AsinNode>(std::move(child));
    else if (str == "arccos")
        return std::make_unique<AcosNode>(std::move(child));
    else if (str == "arctan")
        return std::make_unique<AtanNode>(std::move(child));
    else if (str == "sinh")
        return std::make_unique<SinhNode>(std::move(child));
    else if (str == "cosh")
        return std::make_unique<CoshNode>(std::move(child));
    else if (str == "tanh")
        return std::make_unique<TanhNode>(std::move(child));
    else if (str == "arcsinh")
        return std::make_unique<AsinhNode>(std::move(child));
    else if (str == "arccosh")
        return std::make_unique<AcoshNode>(std::move(child));
    else if (str == "arctanh")
        return std::make_unique<AtanhNode>(std::move(child));
    else
        return nullptr;
}

std::unique_ptr<Node> CreateFormulaTree(std::vector<Token>& RPN)
{
    std::vector<std::unique_ptr<Node>> tree;
    for (int i = 0; i < RPN.size(); ++i)
    {
        if (RPN[i].token_type == Token::TokenType::CONSTANT)
            tree.push_back(std::make_unique<ConstNode>(RPN[i].val));
        else if (RPN[i].token_type == Token::TokenType::VARIABLE)
            tree.push_back(std::make_unique<VariableNode>());
        else if (RPN[i].token_type == Token::TokenType::OPERATOR || RPN[i].token_type == Token::TokenType::BINARY_FUN)
        {
            if (tree.size() > 1)
            {
                std::unique_ptr<Node> right = std::move(tree.back());
                tree.pop_back();

                std::unique_ptr<Node> left = std::move(tree.back());
                tree.pop_back();

                std::unique_ptr<Node> tmp = CreateBinFunNode(RPN[i].val, std::move(left), std::move(right));
                if (tmp != nullptr)
                    tree.push_back(std::move(tmp));
                else 
                    return nullptr;
            }
            else
                return nullptr;
        }
        else if (RPN[i].token_type == Token::TokenType::UNARY_FUN)
        {
            if (!tree.empty())
            {
                std::unique_ptr<Node> child = std::move(tree.back());
                tree.pop_back();

                std::unique_ptr<Node> tmp = CreateUnFunNode(RPN[i].val, std::move(child));
                if (tmp != nullptr)
                    tree.push_back(std::move(tmp));
                else
                    return nullptr;
            }
            else
                return nullptr;
        }
    }
    if (!tree.empty())
        return std::move(tree[0]);
    return nullptr;
}