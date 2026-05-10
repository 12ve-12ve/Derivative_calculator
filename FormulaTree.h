#ifndef FormulaTree_h
#define FormulaTree_h

#include <cmath>
#include <sstream>
#include <memory>

class Node
{
public:
	virtual float Calculate(float x) const = 0;
	virtual std::unique_ptr<Node> Clone() const = 0;
	virtual std::unique_ptr<Node> GetDerivative() const = 0;
	virtual ~Node() = default;
};

class ConstNode :public Node
{
	float constant;
public:
	ConstNode(const float& val) : constant(val) {};
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
};

class VariableNode :public Node
{
public:
	float Calculate(float x) const override { return x; }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<VariableNode>(); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<ConstNode>(1.0f); }
};

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
};

class AddNode :public BinaryNode
{
public:
	using BinaryNode::BinaryNode;
	float Calculate(float x) const override { return left_child->Calculate(x) + right_child->Calculate(x); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<AddNode>(left_child->Clone(), right_child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<AddNode>(left_child->GetDerivative(), right_child->GetDerivative()); }
};

class SubNode :public BinaryNode
{
public:
	using BinaryNode::BinaryNode;
	float Calculate(float x) const override { return left_child->Calculate(x) - right_child->Calculate(x); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<SubNode>(left_child->Clone(), right_child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<SubNode>(left_child->GetDerivative(), right_child->GetDerivative()); }
};

class MultNode :public BinaryNode
{
public:
	using BinaryNode::BinaryNode;
	float Calculate(float x) const override { return left_child->Calculate(x) * right_child->Calculate(x); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<MultNode>(left_child->Clone(), right_child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<AddNode>(std::make_unique<MultNode>(left_child->GetDerivative(), right_child->Clone()), std::make_unique<MultNode>(left_child->Clone(), right_child->GetDerivative())); }
};

class DivNode :public BinaryNode
{
public:
	using BinaryNode::BinaryNode;
	float Calculate(float x) const override
	{
		float denominator = right_child->Calculate(x);
		if (denominator == 0.0f)
			return NAN;
		return left_child->Calculate(x) / denominator;
	}
	std::unique_ptr<Node> Clone() const override { return std::make_unique<DivNode>(left_child->Clone(), right_child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<DivNode>(std::make_unique<SubNode>(std::make_unique<MultNode>(left_child->GetDerivative(), right_child->Clone()), std::make_unique<MultNode>(left_child->Clone(), right_child->GetDerivative())), std::make_unique<MultNode>(right_child->Clone(), right_child->Clone())); }
};

class UnaryNode :public Node
{
protected:
	std::unique_ptr<Node> child;
public:
	UnaryNode(std::unique_ptr<Node> node = nullptr) :child(std::move(node)) {};
	virtual float Calculate(float x) const = 0;
	virtual std::unique_ptr<Node> Clone() const = 0;
	virtual std::unique_ptr<Node> GetDerivative() const = 0;
};

class SqrtNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return sqrt(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<SqrtNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<DivNode>(child->GetDerivative(), std::make_unique<MultNode>(std::make_unique<ConstNode>(2.0f), this->Clone())); }
};

class LnNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return log(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<LnNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<DivNode>(child->GetDerivative(), child->Clone()); }
};

class CosNode;

class SinNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return sin(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<SinNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override;
};

class CosNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return cos(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<CosNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<MultNode>(std::make_unique<SubNode>(std::make_unique<ConstNode>(0.0f), std::make_unique<SinNode>(child->Clone())), child->GetDerivative()); }
};

inline std::unique_ptr<Node> SinNode::GetDerivative() const { return std::make_unique<MultNode>(std::make_unique<CosNode>(child->Clone()), child->GetDerivative()); }

class TanNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return tan(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override{ return std::make_unique<TanNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<DivNode>(child->GetDerivative(), std::make_unique<MultNode>(std::make_unique<CosNode>(child->Clone()), std::make_unique<CosNode>(child->Clone()))); }
};

class AsinNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return asin(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<AsinNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<DivNode>(child->GetDerivative(), std::make_unique<SqrtNode>(std::make_unique<SubNode>(std::make_unique<ConstNode>(1.0f), std::make_unique<MultNode>(child->Clone(), child->Clone())))); }
};

class AcosNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return acos(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<AcosNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<SubNode>(std::make_unique<ConstNode>(0.0f), std::make_unique<DivNode>(child->GetDerivative(), std::make_unique<SqrtNode>(std::make_unique<SubNode>(std::make_unique<ConstNode>(1.0f), std::make_unique<MultNode>(child->Clone(), child->Clone()))))); }
};

class AtanNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return atan(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<AtanNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<DivNode>(child->GetDerivative(), std::make_unique<AddNode>(std::make_unique<ConstNode>(1.0f), std::make_unique<MultNode>(child->Clone(), child->Clone()))); }
};

class CoshNode;

class SinhNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return sinh(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<SinhNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override;
};

class CoshNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return cosh(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override{ return std::make_unique<CoshNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<MultNode>(std::make_unique<SinhNode>(child->Clone()), child->GetDerivative()); }
};

inline std::unique_ptr<Node> SinhNode::GetDerivative() const { return std::make_unique<MultNode>(std::make_unique<CoshNode>(child->Clone()), child->GetDerivative()); }

class TanhNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return tanh(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<TanhNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<DivNode>(child->GetDerivative(), std::make_unique<MultNode>(std::make_unique<CoshNode>(child->Clone()), std::make_unique<CoshNode>(child->Clone()))); }
};

class AsinhNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return asinh(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<AsinhNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<DivNode>(child->GetDerivative(), std::make_unique<SqrtNode>(std::make_unique<AddNode>(std::make_unique<MultNode>(child->Clone(), child->Clone()), std::make_unique<ConstNode>(1.0f)))); }
};

class AcoshNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return acosh(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<AcoshNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<DivNode>(child->GetDerivative(), std::make_unique<SqrtNode>(std::make_unique<SubNode>(std::make_unique<MultNode>(child->Clone(), child->Clone()), std::make_unique<ConstNode>(1.0f)))); }
};

class AtanhNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return atanh(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<AtanhNode>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<DivNode>(child->GetDerivative(), std::make_unique<SubNode>(std::make_unique<ConstNode>(1.0f), std::make_unique<MultNode>(child->Clone(), child->Clone()))); }
};

class Log10Node :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return log10(child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<Log10Node>(child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<DivNode>(child->GetDerivative(), std::make_unique<MultNode>(child->Clone(), std::make_unique<LnNode>(std::make_unique<ConstNode>(10.0f)))); }
};

class PowNode :public BinaryNode
{
public:
	using BinaryNode::BinaryNode;
	float Calculate(float x) const override { return pow(left_child->Calculate(x), right_child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<PowNode>(left_child->Clone(), right_child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override { return std::make_unique<MultNode>(this->Clone(), std::make_unique<AddNode>(std::make_unique<MultNode>(right_child->GetDerivative(), std::make_unique<LnNode>(left_child->Clone())), std::make_unique<MultNode>(right_child->Clone(), std::make_unique<DivNode>(left_child->GetDerivative(), left_child->Clone())))); }
};

class LogNode :public BinaryNode
{
public:
	using BinaryNode::BinaryNode;
	float Calculate(float x) const override { return log(right_child->Calculate(x)) / log(left_child->Calculate(x)); }
	std::unique_ptr<Node> Clone() const override { return std::make_unique<LogNode>(left_child->Clone(), right_child->Clone()); }
	std::unique_ptr<Node> GetDerivative() const override{ return std::make_unique<DivNode>(std::make_unique<LnNode>(right_child->Clone()), std::make_unique<LnNode>(left_child->Clone()))->GetDerivative(); }
};

#endif // !FromulaTree_h