/*
* arithmetic.cpp
*
*  Created on: 2016-9-10
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#include "arithmetic.h"
#include <cmath>

#define LEFT_VAR m_childNode.front()->value()
#define RIGHT_VAR m_childNode.back()->value()
#define HELPER_2_ARG(op) \
return LEFT_VAR op RIGHT_VAR \

bool ArithmeticNodeOperatorBasic::check()
{
	switch (m_op)
	{
	case OP_PLUS: 
	case OP_MINUS: 
	case OP_MULTIPLY: 
	case OP_DIVID: 
	case OP_POWER:
		return m_childNode.size() >= 2;
	default:
		return m_childNode.size() >= 1;
	}
}

double ArithmeticNodeOperatorBasic::value()
{
	if (!check())
		return 0.0;

	switch (m_op)
	{
	case OP_PLUS: HELPER_2_ARG(+);
	case OP_MINUS: HELPER_2_ARG(-);
	case OP_MULTIPLY: HELPER_2_ARG(*);
	case OP_DIVID: HELPER_2_ARG(/);
	case OP_POWER: return pow(LEFT_VAR, RIGHT_VAR);

	case OP_EXP: return exp(LEFT_VAR);
	case OP_ABS: return abs(LEFT_VAR);
	case OP_SIGN: return (LEFT_VAR > 0.0) - (LEFT_VAR < 0.0);
	case OP_SQRT: return sqrt(LEFT_VAR);
	case OP_SIN: return sin(LEFT_VAR);
	case OP_ARCSIN: return asin(LEFT_VAR);
	case OP_ARCCOS: return acos(LEFT_VAR);
	case OP_TAN: return tan(LEFT_VAR);
	case OP_ATAN: return atan(LEFT_VAR);
	case OP_LOG: return log(LEFT_VAR);
	case OP_LOG2: return log2(LEFT_VAR);
	case OP_LOG10: return log10(LEFT_VAR);
	default:
		return 0.0;
	}
}

ArithmeticNode* parseNode(const std::string& equation)
{
	using namespace std;



}

ArithmeticNode* parseEquation(const std::string& equation)
{
	using namespace std;
	
	auto pos = equation.find('=');
	if (pos != string::npos)
	{
		string left = equation.substr(0, pos);
		//string right = equation.substr()
	}

	return new ArithmeticNodeConstant<double>(0.0);
}