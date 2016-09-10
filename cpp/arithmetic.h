/*
* arithmetic.h
*
*  Created on: 2016-9-10
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#ifndef _ARITHMETIC_H_
#define _ARITHMETIC_H_

#include <list>
#include <string>

class ArithmeticNode
{
public:

	enum NodeType {
		INVALID, 
		OPERATOR,
		CONSTANT,		
		VARIABLE_X,
		VARIABLE_Y,
	};

	virtual NodeType nodeType() const { return INVALID; }
	virtual double value() = 0;

	virtual void setValue(NodeType type, double v) {}
};

template <typename ConstantType>
class ArithmeticNodeConstant : public ArithmeticNode
{
public:
	ArithmeticNodeConstant(ConstantType c) : m_constant(c) {}
	NodeType nodeType() { return CONSTANT; }
	double value() { return (double)m_constant; }  //A constant must be compitable with 'double'.
	inline const ConstantType constant() const { return m_constant; }
private:
	ConstantType m_constant;
};

template<int VariableType>
class ArithmeticNodeVariable : public ArithmeticNode
{
public:
	ArithmeticNodeVariable() { static_assert(VariableType == VARIABLE_X || VariableType == VARIABLE_X, "Invalid Variable Type!"); };
	NodeType nodeType() { return VariableType; }
	double value() { return m_variable; }
	void setValue(NodeType type, double v) {
		if (type == VariableType)
			m_variable = v;
	}

private:
	double m_variable;
};

class ArithmeticNodeOperatorInterface : public ArithmeticNode
{
public:

	NodeType nodeType() const { return OPERATOR; }

	inline std::list<ArithmeticNode*>& childNode() { return m_childNode; }
	inline const std::list<ArithmeticNode*>& childNode() const { return m_childNode; }
	inline void addChildNode(ArithmeticNode* node) { m_childNode.push_back(node); }

protected:
	std::list<ArithmeticNode*> m_childNode;
};

class ArithmeticNodeOperatorBasic : public ArithmeticNodeOperatorInterface
{
public:

	enum OperatorType
	{
		OP_PLUS = '   +',
		OP_MINUS = '   -',
		OP_MULTIPLY = '   *',
		OP_DIVID = '   /',
		OP_EXP = ' exp',
		OP_POWER = ' pow',
		OP_ABS = ' abs',
		OP_SIGN = 'sign',
		OP_SQRT = 'sqrt',

		OP_SIN = ' sin',
		OP_ARCSIN = 'asin',
		OP_ARCCOS = 'acos',
		OP_TAN = ' tan',
		OP_ATAN = 'atan',
		OP_LOG = ' log',
		OP_LOG2 = 'log2',
		OP_LOG10 = '  lg',
	};


	ArithmeticNodeOperatorBasic(OperatorType op) : m_op(op) {}
	
	

	bool check();
	double value();

private:
	OperatorType m_op;
};

ArithmeticNode* parseEquation(const std::string& equation);

#endif // !_ARITHMETIC_H_