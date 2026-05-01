#ifndef FormulaTree_h
#define FormulaTree_h

#include <cmath>

class Node
{
public:
	virtual float Calculate(float x) const = 0;
	virtual ~Node() = default;
};

class ConstNode :public Node
{
	float constant;
public:
	ConstNode(float val) : constant(val) {};
	float Calculate(float x) const override { return constant; }
};

class VariableNode :public Node
{
public:
	float Calculate(float x) const override { return x; }
};

class UnaryNode :public Node
{
protected:
	Node* child;
public:
	UnaryNode(Node* node = nullptr) :child(node) {};
	virtual float Calculate(float x) const = 0;
	~UnaryNode() { delete child; }
};

class SinNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return sin(child->Calculate(x)); }
};

class CosNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return cos(child->Calculate(x)); }
};

class TanNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return tan(child->Calculate(x)); }
};

class AsinNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return asin(child->Calculate(x)); }
};

class AcosNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return acos(child->Calculate(x)); }
};

class AtanNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return atan(child->Calculate(x)); }
};

class SinhNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return sinh(child->Calculate(x)); }
};

class CoshNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return cosh(child->Calculate(x)); }
};

class TanhNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return tanh(child->Calculate(x)); }
};

class AsinhNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return asinh(child->Calculate(x)); }
};

class AcoshNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return acosh(child->Calculate(x)); }
};

class AtanhNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return atanh(child->Calculate(x)); }
};

class LnNode :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return log(child->Calculate(x)); }
};

class Log10Node :public UnaryNode
{
public:
	using UnaryNode::UnaryNode;
	float Calculate(float x) const override { return log10(child->Calculate(x)); }
};

class BinaryNode :public Node
{
protected:
	Node* left_child;
	Node* right_child;
public:
	BinaryNode(Node* left_node = nullptr, Node* right_node = nullptr) :left_child(left_node), right_child(right_node) {};
	virtual float Calculate(float x) const = 0;
	~BinaryNode() { delete left_child; delete right_child; }
};

class AddNode :public BinaryNode
{
public:
	using BinaryNode::BinaryNode;
	float Calculate(float x) const override { return left_child->Calculate(x) + right_child->Calculate(x); }
};

class SubNode :public BinaryNode
{
public:
	using BinaryNode::BinaryNode;
	float Calculate(float x) const override { return left_child->Calculate(x) - right_child->Calculate(x); }
};

class MultNode :public BinaryNode
{
public:
	using BinaryNode::BinaryNode;
	float Calculate(float x) const override { return left_child->Calculate(x) * right_child->Calculate(x); }
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
};

class PowNode :public BinaryNode
{
public:
	using BinaryNode::BinaryNode;
	float Calculate(float x) const override { return pow(left_child->Calculate(x), right_child->Calculate(x)); }
};

class LogNode :public BinaryNode
{
public:
	using BinaryNode::BinaryNode;
	float Calculate(float x) const override { return log(right_child->Calculate(x)) / log(left_child->Calculate(x)); }
};

#endif // !FromulaTree_h