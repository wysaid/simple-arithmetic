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

class ArithMeticNode
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

template <class ConstantType = double>
class ArithMeticNodeConstant : public ArithMeticNode
{
public:
	ArithMeticNodeConstant(ConstantType c) : m_constant(c) {}
	NodeType nodeType() { return CONSTANT; }
	double value() { return (double)constant; }  //A constant must be compitable with 'double'.
	inline const ConstantType constant() const { return m_constant; }
private:
	ConstantType m_constant;
};

template<int VariableType>
class ArithMeticNodeVariable : public ArithMeticNode
{
public:
	ArithMeticNodeVariable() { static_assert(VariableType == VARIABLE_X || VariableType == VARIABLE_X, "Invalid Variable Type!"); };
	NodeType nodeType() { return VariableType; }
	double value() { return m_variable; }
	void setValue(NodeType type, double v) {
		if (type == VariableType)
			m_variable = v;
	}

private:
	double m_variable;
};


class ArithMeticNodeOperatorInterface : public ArithMeticNode
{
public:

	NodeType nodeType() const { return OPERATOR; }

	inline std::list<ArithMeticNode>& childNode() { return m_childNode; }
	inline const std::list<ArithMeticNode>& childNode() const { return m_childNode; }

protected:
	std::list<ArithMeticNode> m_childNode;
};


#endif // !_ARITHMETIC_H_