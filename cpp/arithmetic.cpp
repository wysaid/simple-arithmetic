/*
* arithmetic.cpp
*
*  Created on: 2016-9-10
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "arithmetic.h"
#include <cmath>
#include <vector>

#define LEFT_VAR m_childNode.front()->value()
#define RIGHT_VAR m_childNode.back()->value()
#define HELPER_2_ARG(op) \
return LEFT_VAR op RIGHT_VAR \

bool ArithmeticNodeOperatorImp::check()
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

double ArithmeticNodeOperatorImp::value()
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
	case OP_ABS: return fabs(LEFT_VAR);
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

class Parser
{
public:

	ArithmeticNode* parseNode(const std::string& equation)
	{
		mTmpNodes.clear();
		mBuffer[0] = '\0';
		return parse(equation);
	}
	
protected:
	
	ArithmeticNode* parse(const std::string& equation)
	{
		using namespace std;

		auto brIndex = equation.find_last_of('(');
		if (brIndex != string::npos)
		{
			auto endIndex = equation.find(')', brIndex + 1);
			if (string::npos == endIndex)
				return nullptr; //Invalid Equation

			if (brIndex == 0 || isOperator(equation[brIndex - 1]))
			{
				const string& braceIn = equation.substr(brIndex + 1, endIndex - brIndex - 1);
				mTmpNodes.push_back(parse(braceIn));

				sprintf(mBuffer, "@%d", (int)mTmpNodes.size() - 1);
				const string& braceOut = equation.substr(0, brIndex) + mBuffer + equation.substr(endIndex + 1, equation.size() - endIndex - 1);
				return parse(braceOut);
			}
			else
			{

			}
		}

		return nullptr;
	}

	bool isOperator(char c)
	{
        return false;
	}

protected:
	char mBuffer[1024]; //for temp usage.

	std::vector<ArithmeticNode*> mTmpNodes;
};



ArithmeticNode* parseEquation(const std::string& equation)
{
	using namespace std;
	
	string eq = equation;
	auto pos = eq.find('=');
	if (pos != string::npos)
	{
		string left = eq.substr(0, pos);
		string right = eq.substr(pos + 1);
		eq = "(";
		eq += left + ")" + "-(" + right + ")";
	}

	//trim
	for (auto it = eq.begin(); it != eq.end();)
	{
		if (isblank(*it))
		{
			it = eq.erase(it);
		}
		else ++it;
	}

	Parser parser;
	return parser.parseNode(eq);
}
