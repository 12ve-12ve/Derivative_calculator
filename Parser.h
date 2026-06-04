#ifndef Parser_h
#define Parser_h

#include <string>
#include <vector>
#include "FormulaTree.h"

/**
 * @brief Represents a single token generated during the parsing of a formula.
 * * The tokenizer breaks down the user input into a sequence of tokens,
 * which are later processed to build the Abstract Syntax Tree (AST).
 */
struct Token
{
    /**
     * @brief Defines the specific types of tokens recognized by the parser.
     */
    enum class TokenType
    {
        VARIABLE,      ///< Represents the variable 'x'.
        CONSTANT,      ///< Represents a constant number.
        OPERATOR,      ///< Represents a basic arithmetic operator (+, -, *, /).
        UNARY_FUN,     ///< Represents a function taking a single argument.
        BINARY_FUN,    ///< Represents a function taking two arguments.
        PARENTHESIS_L, ///< Represents a left opening parenthesis '('.
        PARENTHESIS_R, ///< Represents a right closing parenthesis ')'.
        COMMA          ///< Represents a comma separator for multi-argument functions.
    };

    TokenType token_type;   ///< The type of this token.
    std::string val;    ///< The representation of the token.

    /**
     * @brief Default and parameterized constructor for the Token.
     * * @param t_type The type of the token. Defaults to TokenType::CONSTANT.
     * @param str The string value of the token. Defaults to an empty string.
     */
    Token(Token::TokenType t_type = TokenType::CONSTANT, const std::string& str = "") : token_type(t_type), val(str) {};

    /**
     * @brief Copy constructor.
     * * @param t Existing Token to copy from.
     */
    Token(const Token& t) : token_type(t.token_type), val(t.val) {};

    /**
     * @brief Copy assignment operator
     * * @param t Token to assign from.
     * @return Token& A reference to this Token.
     */
    Token& operator=(const Token& t)
    {
        if (this == &t)
            return *this;

        token_type = t.token_type;
        val = t.val;
        return *this;
    }
};

/**
 * @brief Checks if the string contains a constant.
 * * @param str String that may contain a constant.
 * @return bool Returns true if string contains a constant value.
 */
bool is_numeric(const std::string& str);

/**
 * @brief Checks if the string contains one of the sings : '-', '+', '*', '/', '(', ')', ','.
 * * @param str String that may contain a sign.
 * @return bool Returns true if string contains a sign.
 */
bool OperatorSign(const char& sign);

/**
 * @brief Assigns a token a proper TokenType based on the given string.
 * * @param str Part of the user input from which the TokenType is based.
 * @param token Token to which it will be assigned a proper TokenType.
 * @return bool Returns true if the TokenType was assigned successfully.
 */
bool CheckToken(const std::string& str, Token& token);

/**
 * @brief Creates one of the signs token: '-', '+', '*', '/', '(', ')', ','.
 * * Based on the parameter it creates a proper Token.
 * * @param sign Symbol that indicates which Token should be created.
 * @return Token Created Token based on a sign.
 */
Token CreateOperatorToken(const char sign);

/**
 * @brief Determines order of operation.
 * * Returns an integer indicating how hisg on the priority in the order
 * of mathematical operations is given operation.
 * The higher the number the higher the priority.
 * * @param str Operation that needs to be given a priority.
 * @return int Priority number.
 */
int getPriority(const std::string& str);

/**
 * @brief Modifies a token vector to a Reverse Polish Notation order.
 * * This function takes previously created a vector of Tokens
 * and modfies the order so it can be read using the RPN.
 * It deletes not needed tokens such as parenthesis or comma tokens.
 * * @param tokens Vector of tokens that were created based on the user text input.
 * @return std::unique_ptr<Node> Root of formula tree.
 */
std::unique_ptr<Node> ModifyToRPN(std::vector<Token>& tokens);

/**
 * @brief Creates a tree node for a binary function or an arythmetic operator.
 * * A proper node is created based on the string parameter. This node then points
 * at left and right as its left and right child.
 * * @param str Function or an arythmetic operation.
 * @param Left Left child of the node.
 * @param Right child of the node.
 * @return std::unique_ptr<Node> Created node.
 */
std::unique_ptr<Node> CreateBinFunNode(const std::string& str, std::unique_ptr<Node> left, std::unique_ptr<Node> right);

/**
 * @brief Creates a tree node for an unary function.
 * * A proper node is created based on the string parameter. This node has a pointer
 * set on child parameter.
 * * @param str Unary function.
 * @param child Child of the node.
 * @return std::unique_ptr<Node> Created node.
 */
std::unique_ptr<Node> CreateUnFunNode(const std::string& str, std::unique_ptr<Node> child);

/**
 * @brief Creates an Abstract Syntax Tree as a formula tree.
 * * Takes a vector of tokens of functions, constants and variables that are in 
 * Reverse Polish Notation order. Next it creates an AST out of it to create a formula tree.
 * * @param RPN vector of tokens in RPN order.
 * @return std::unique_ptr<Node> Root of the formula tree.
 */
std::unique_ptr<Node> CreateFormulaTree(std::vector<Token>& RPN);

#endif // !Parser_h
