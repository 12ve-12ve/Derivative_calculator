#ifndef FormulaTree_h
#define FormulaTree_h

#include <cmath>
#include <sstream>
#include <memory>

/**
 * @brief Base node class for the mathematical formula tree.
 * * This class defines the interface required for evaluating the formula,
 * duplicating the tree structure, and generating a derivative tree.
 */
class Node
{
public:
	/**
	 * @brief Calculates the value of the function at a given point.
	 * * First, it recursively calls the Calculate method of its child nodes (if they exist),
	 * and then performs its own mathematical operation using those returned values.
	 * * @param x The input value for the variable x.
	 * @return float The calculated result of the function.
	 */
	virtual float Calculate(float x) const = 0;

	/**
	 * @brief Creates a deep copy of the node.
	 * * @return std::unique_ptr<Node> A smart pointer to the cloned node.
	 */
	virtual std::unique_ptr<Node> Clone() const = 0;

	/**
	 * @brief Generates the derivative tree for this node.
	 * * Applies derivation rules to construct a new syntax tree representing
	 * the mathematical derivative of the function.
	 * * @return std::unique_ptr<Node> A smart pointer to the root of the derivative tree.
	 */
	virtual std::unique_ptr<Node> GetDerivative() const = 0;

	/**
	 * @brief Converts the tree structure into a string.
	 * @return std::string The string representation of the formula.
	 */
	virtual std::string ToString() const = 0;

	virtual ~Node() = default;
};

/**
 * @brief Node of the formula tree for constant value.
 */
class ConstNode :public Node
{
	float constant;	///< The value of the constant node
public:
	ConstNode(float val) : constant(val) {};
	ConstNode(std::string str) {
		if (str == "PI") {
			constant = 3.14159265f;
		}
		else if (str == "e") {
			constant = 2.718281828f;
		}
		else {
			std::istringstream input(str);
			input >> constant;
		}
	}
	float Calculate(float x) const override { return constant; }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<ConstNode>(this->constant); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<ConstNode>(0.0f); }
	std::string ToString() const override { 
		if (constant == 3.14159265f)
			return "π";
		else if (constant == 2.718281828f)
			return "e";
		else
		{
			std::ostringstream oss;
			oss << constant;
			return oss.str();
		}
	}
};

/**
 * @brief Node of the formula tree for variable x.
 */
class VariableNode :public Node
{
public:
	float Calculate(float x) const override { return x; }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<VariableNode>(); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<ConstNode>(1.0f); }
	std::string ToString() const override { return "x"; }
};

/**
 * @brief Abstract base class representing binary mathematical operations.
 */
class BinaryNode :public Node
{
protected:
	std::unique_ptr<Node> left_child;
	std::unique_ptr<Node> right_child;
public:
	BinaryNode(std::unique_ptr<Node> left_node = nullptr, std::unique_ptr<Node> right_node = nullptr) :left_child(std::move(left_node)), right_child(std::move(right_node)) {};
	virtual float Calculate(float x) const = 0;
	virtual std::unique_ptr<Node> Clone() const = 0;
	virtual std::unique_ptr<Node> GetDerivative() const = 0;
	virtual std::string ToString() const = 0;
};

/**
 * @brief Node representing an addition operation.
 */
class AddNode :public BinaryNode
{
public:
	using BinaryNode::BinaryNode;
	float Calculate(float x) const override { return left_child->Calculate(x) + right_child->Calculate(x); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<AddNode>(left_child->Clone(), right_child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<AddNode>(left_child->GetDerivative(), right_child->GetDerivative()); }
	std::string ToString() const override { 
		if (dynamic_cast<ConstNode*>(right_child.get()) != nullptr && right_child->Calculate(0.0f) == 0.0f)
			return left_child->ToString();
		if (dynamic_cast<ConstNode*>(left_child.get()) != nullptr && left_child->Calculate(0.0f) == 0.0f)
			return right_child->ToString();
		return "(" + left_child->ToString() + "+" + right_child->ToString() + ")";
	}
};

/**
 * @brief Node representing a substraction operation.
 */
class SubNode :public BinaryNode
{
public:
	using BinaryNode::BinaryNode;
	float Calculate(float x) const override { return left_child->Calculate(x) - right_child->Calculate(x); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<SubNode>(left_child->Clone(), right_child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<SubNode>(left_child->GetDerivative(), right_child->GetDerivative()); }
	std::string ToString() const override {
		if (dynamic_cast<ConstNode*>(right_child.get()) != nullptr && right_child->Calculate(0.0f) == 0.0f)
			return left_child->ToString();
		if (dynamic_cast<ConstNode*>(left_child.get()) != nullptr && left_child->Calculate(0.0f) == 0.0f)
			return right_child->ToString();
		return "(" + left_child->ToString() + "-" + right_child->ToString() + ")";
	}
};

/**
 * @brief Node representing a multiplication operation.
 */
class MultNode :public BinaryNode
{
public:
	using BinaryNode::BinaryNode;
	float Calculate(float x) const override { return left_child->Calculate(x) * right_child->Calculate(x); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<MultNode>(left_child->Clone(), right_child->Clone()); }

	/**
	 * @brief Generates the derivative tree for this node.
	 * Uses the formula (f * g)' = f' * g + f * g'.
	 * * @return std::unique_ptr<Node> A smart pointer to an addition node.
	 */
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<AddNode>(std::make_unique<MultNode>(left_child->GetDerivative(), right_child->Clone()), std::make_unique<MultNode>(left_child->Clone(), right_child->GetDerivative())); }
	std::string ToString() const override { 
		if (dynamic_cast<ConstNode*>(right_child.get()) != nullptr && right_child->Calculate(0.0f) == 1.0f)
			return left_child->ToString();
		if (dynamic_cast<ConstNode*>(left_child.get()) != nullptr && left_child->Calculate(0.0f) == 1.0f)
			return right_child->ToString();
		if (dynamic_cast<ConstNode*>(right_child.get()) != nullptr && right_child->Calculate(0.0f) == 0.0f)
			return "0";
		if (dynamic_cast<ConstNode*>(left_child.get()) != nullptr && left_child->Calculate(0.0f) == 0.0f)
			return "0";
		return "(" + left_child->ToString() + "×" + right_child->ToString() + ")"; 
	}
};

/**
 * @brief Node representing a division operation.
 */
class DivNode :public BinaryNode
{
public:
	using BinaryNode::BinaryNode;

	/**
	 * @brief Calculates the division of the left child by the right child.
	 * * Protects against division by zero by returning NAN.
	 * * @param x The input value for the variable x.
	 * @return float The result of division, or NAN if the denominator is 0.
	 */
	float Calculate(float x) const override
	{
		float denominator = right_child->Calculate(x);
		if (denominator == 0.0f)
			return NAN;
		return left_child->Calculate(x) / denominator;
	}
	std::unique_ptr<Node> Clone() const override { return std::make_unique<DivNode>(left_child->Clone(), right_child->Clone()); }

	/**
	 * @brief Generates the derivative tree for this node.
	 * Uses the formula (f / g)' = (f' * g - f * g') / g^2.
	 * * @return std::unique_ptr<Node> A smart pointer to a division node.
	 */
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<DivNode>(std::make_unique<SubNode>(std::make_unique<MultNode>(left_child->GetDerivative(), right_child->Clone()), std::make_unique<MultNode>(left_child->Clone(), right_child->GetDerivative())), std::make_unique<MultNode>(right_child->Clone(), right_child->Clone())); }
	std::string ToString() const override { 
		if (dynamic_cast<ConstNode*>(right_child.get()) != nullptr && right_child->Calculate(0.0f) == 1.0f)
			return left_child->ToString();
		else if (dynamic_cast<ConstNode*>(left_child.get()) != nullptr && right_child->Calculate(0.0f) == 0.0f)
			return right_child->ToString();
		return "(" + left_child->ToString() + "÷" + right_child->ToString() + ")"; 
	}
};

/**
 * @brief Abstract base class representing unary mathematical operations.
 */
class UnaryNode :public Node
{
protected:
	std::unique_ptr<Node> child;
public:
	UnaryNode(std::unique_ptr<Node> node = nullptr) :child(std::move(node)) {};
	virtual float Calculate(float x) const = 0;
	virtual std::unique_ptr<Node> Clone() const = 0;
	virtual std::unique_ptr<Node> GetDerivative() const = 0;
	virtual std::string ToString() const = 0;
};

/**
 * @brief Node representing a square root operation.
 */
class SqrtNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return sqrt(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<SqrtNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<DivNode>(child->GetDerivative(), std::make_unique<MultNode>(std::make_unique<ConstNode>(2.0f), this->Clone())); }
	std::string ToString() const override { return "sqrt(" + child->ToString() + ")"; }
};

/**
 * @brief Node representing a dnatural logarithm operation.
 */
class LnNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return log(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<LnNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<DivNode>(child->GetDerivative(), child->Clone()); }
	std::string ToString() const override { return "ln(" + child->ToString() + ")"; }
};

class CosNode;

/**
 * @brief Node representing a sine operation.
 */
class SinNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return sin(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<SinNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override;
	std::string ToString() const override { return "sin(" + child->ToString() + ")"; }
};

/**
 * @brief Node representing a cosine operation.
 */
class CosNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return cos(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<CosNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<MultNode>(std::make_unique<SubNode>(std::make_unique<ConstNode>(0.0f), std::make_unique<SinNode>(child->Clone())), child->GetDerivative()); }
	std::string ToString() const override { return "cos(" + child->ToString() + ")"; }
};

inline std::unique_ptr<Node> SinNode::GetDerivative() const { return std::make_unique<MultNode>(std::make_unique<CosNode>(child->Clone()), child->GetDerivative()); }

/**
 * @brief Node representing a tangent operation.
 */
class TanNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return tan(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override{ return std::make_unique<TanNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<DivNode>(child->GetDerivative(), std::make_unique<MultNode>(std::make_unique<CosNode>(child->Clone()), std::make_unique<CosNode>(child->Clone()))); }
	std::string ToString() const override { return "tan(" + child->ToString() + ")"; }
};

/**
 * @brief Node representing an arcsine operation.
 */
class AsinNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return asin(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<AsinNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<DivNode>(child->GetDerivative(), std::make_unique<SqrtNode>(std::make_unique<SubNode>(std::make_unique<ConstNode>(1.0f), std::make_unique<MultNode>(child->Clone(), child->Clone())))); }
	std::string ToString() const override { return "arcsin(" + child->ToString() + ")"; }
};

/**
 * @brief Node representing an arccosine operation.
 */
class AcosNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return acos(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<AcosNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<SubNode>(std::make_unique<ConstNode>(0.0f), std::make_unique<DivNode>(child->GetDerivative(), std::make_unique<SqrtNode>(std::make_unique<SubNode>(std::make_unique<ConstNode>(1.0f), std::make_unique<MultNode>(child->Clone(), child->Clone()))))); }
	std::string ToString() const override { return "arccos(" + child->ToString() + ")"; }
};

/**
 * @brief Node representing an arctangent operation.
 */
class AtanNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return atan(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<AtanNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<DivNode>(child->GetDerivative(), std::make_unique<AddNode>(std::make_unique<ConstNode>(1.0f), std::make_unique<MultNode>(child->Clone(), child->Clone()))); }
	std::string ToString() const override { return "arctan(" + child->ToString() + ")"; }
};

class CoshNode;

/**
 * @brief Node representing a hyperbolic sine operation.
 */
class SinhNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return sinh(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<SinhNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override;
	std::string ToString() const override { return "sinh(" + child->ToString() + ")"; }
};

/**
 * @brief Node representing a hyperbolic cosine operation.
 */
class CoshNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return cosh(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override{ return std::make_unique<CoshNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<MultNode>(std::make_unique<SinhNode>(child->Clone()), child->GetDerivative()); }
	std::string ToString() const override { return "cosh(" + child->ToString() + ")"; }
};

inline std::unique_ptr<Node> SinhNode::GetDerivative() const { return std::make_unique<MultNode>(std::make_unique<CoshNode>(child->Clone()), child->GetDerivative()); }

/**
 * @brief Node representing a hyperbolic tangent operation.
 */
class TanhNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return tanh(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<TanhNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<DivNode>(child->GetDerivative(), std::make_unique<MultNode>(std::make_unique<CoshNode>(child->Clone()), std::make_unique<CoshNode>(child->Clone()))); }
	std::string ToString() const override { return "tanh(" + child->ToString() + ")"; }
};

/**
 * @brief Node representing an inverse hyperbolic sine operation.
 */
class AsinhNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return asinh(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<AsinhNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<DivNode>(child->GetDerivative(), std::make_unique<SqrtNode>(std::make_unique<AddNode>(std::make_unique<MultNode>(child->Clone(), child->Clone()), std::make_unique<ConstNode>(1.0f)))); }
	std::string ToString() const override { return "arcsinh(" + child->ToString() + ")"; }
};

/**
 * @brief Node representing an inverse hyperbolic cosine operation.
 */
class AcoshNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return acosh(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<AcoshNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<DivNode>(child->GetDerivative(), std::make_unique<SqrtNode>(std::make_unique<SubNode>(std::make_unique<MultNode>(child->Clone(), child->Clone()), std::make_unique<ConstNode>(1.0f)))); }
	std::string ToString() const override { return "arccosh(" + child->ToString() + ")"; }
};

/**
 * @brief Node representing an inverse hyperbolic tangent operation.
 */
class AtanhNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return atanh(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<AtanhNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<DivNode>(child->GetDerivative(), std::make_unique<SubNode>(std::make_unique<ConstNode>(1.0f), std::make_unique<MultNode>(child->Clone(), child->Clone()))); }
	std::string ToString() const override { return "arctanh(" + child->ToString() + ")"; }
};

/**
 * @brief Node representing a logarithm with a base 10 operation.
 */
class Log10Node :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return log10(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<Log10Node>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<DivNode>(child->GetDerivative(), std::make_unique<MultNode>(child->Clone(), std::make_unique<LnNode>(std::make_unique<ConstNode>(10.0f)))); }
	std::string ToString() const override { return "log10(" + child->ToString() + ")"; }
};

/**
 * @brief Node representing a exponentiation operation.
 */
class PowNode :public BinaryNode
{
public:
	using BinaryNode::BinaryNode;
	float Calculate(float x) const override { return pow(left_child->Calculate(x), right_child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<PowNode>(left_child->Clone(), right_child->Clone()); }

	/**
	 * @brief Generates the derivative tree for the power operation.
	 * * This method checks if the right child is a constant. If so, it applies the standard
	 * power rule b * a^(b-1) * a'. Otherwise, it uses the generalized 
	 * logarithmic differentiation formula a ^ b * ((b / a) * a' + b' * ln(a)).
	 * * @return std::unique_ptr<Node> A smart pointer to the root of the derivative tree.
	 */
	std::unique_ptr<Node> GetDerivative() const override 
	{ 
		if (dynamic_cast<ConstNode*>(right_child.get()) != nullptr)
			return std::make_unique<MultNode>(std::make_unique<MultNode>(right_child->Clone(),std::make_unique<PowNode>(left_child->Clone(),std::make_unique<ConstNode>(right_child->Calculate(0.0f) - 1.0f))),left_child->GetDerivative());
		return std::make_unique<MultNode>(this->Clone(), std::make_unique<AddNode>(std::make_unique<MultNode>(right_child->GetDerivative(), std::make_unique<LnNode>(left_child->Clone())), std::make_unique<MultNode>(right_child->Clone(), std::make_unique<DivNode>(left_child->GetDerivative(), left_child->Clone())))); 
	}
	std::string ToString() const override { return "pow(" + left_child->ToString() + ", " + right_child->ToString() + ")"; }
};

/**
 * @brief Node representing a logarithm operation with any base.
 */
class LogNode :public BinaryNode
{
public:
	using BinaryNode::BinaryNode;

	/**
	 * @brief Calculates the value of the logarithm with any base at a given point.
	 * * This uses a logarithmic formula: log(a, b) = ln(a) / ln(b).
	 * * @return float The calculated result of the function.
	 */
	float Calculate(float x) const override { return log(right_child->Calculate(x)) / log(left_child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<LogNode>(left_child->Clone(), right_child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override{ return std::make_unique<DivNode>(std::make_unique<LnNode>(right_child->Clone()), std::make_unique<LnNode>(left_child->Clone()))->GetDerivative(); }
	std::string ToString() const override { return "log(" + left_child->ToString() + ", " + right_child->ToString() + ")"; }
};

#endif // !FromulaTree_h