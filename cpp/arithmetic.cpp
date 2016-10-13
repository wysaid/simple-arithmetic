/*
 * arithmetic.cpp
 *
 *  Created on: 2016-9-10
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 *        Blog: wysaid.org
 */

#include "arithmetic.h"
#include <cmath>
#include <vector>

#ifndef M_E
#define M_E        2.71828182845904523536   // e
#endif

#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif

#define LEFT_VAR m_childNode.front()->value()
#define RIGHT_VAR m_childNode.back()->value()
#define HELPER_2_ARG(op) \
return LEFT_VAR op RIGHT_VAR \

#define BUFFER_SIZE 1024

using namespace std;

bool ArithmeticNodeOperatorImp::isValid()
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
    if (!isValid())
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
        case OP_COS: return cos(LEFT_VAR);
        case OP_ARCCOS: return acos(LEFT_VAR);
        case OP_TAN: return tan(LEFT_VAR);
        case OP_ARCTAN: return atan(LEFT_VAR);
        case OP_LOG: return log(LEFT_VAR);
        case OP_LOG2: return log2(LEFT_VAR);
        case OP_LOG10: return log10(LEFT_VAR);
        default:
            return 0.0;
    }
}

static const char* s_functions[] = {
    "exp",
    "abs",
    "sign",
    "sqrt",
    "sin",
    "asin",
    "arcsin",
    "cos",
    "acos",
    "arccos",
    "tan",
    "atan",
    "arctan",
    "ln",
    "log",
    "log2",
    "lg",
    "log10",
};

static ArithmeticNode::OperatorType s_opType[] =
{
    ArithmeticNode::OP_EXP, //exp
    ArithmeticNode::OP_ABS, //abs
    ArithmeticNode::OP_SIGN, //sign
    ArithmeticNode::OP_SQRT, //sqrt
    ArithmeticNode::OP_SIN, //sin
    ArithmeticNode::OP_ARCSIN, //asin
    ArithmeticNode::OP_ARCSIN, //arcsin
    ArithmeticNode::OP_COS, //cos
    ArithmeticNode::OP_ARCCOS, //acos
    ArithmeticNode::OP_ARCCOS, //arccos
    ArithmeticNode::OP_TAN, //tan
    ArithmeticNode::OP_ARCTAN, //atan
    ArithmeticNode::OP_ARCTAN, //arctan
    ArithmeticNode::OP_LOG, //ln
    ArithmeticNode::OP_LOG, //log
    ArithmeticNode::OP_LOG2, //log2
    ArithmeticNode::OP_LOG10, //lg
    ArithmeticNode::OP_LOG10, //log10
};

ArithmeticNode::OperatorType getOpTypeByName(const char* name)
{
    static_assert(sizeof(s_functions) / sizeof(*s_functions) == sizeof(s_opType) / sizeof(*s_opType), "Invalid Rule Strings!");
    
    auto type = ArithmeticNode::OP_INVALID;
    
    for(int i = 0; i != sizeof(s_functions) / sizeof(*s_functions); ++i)
    {
        if(strcmp(name, s_functions[i]) == 0)
        {
            return s_opType[i];
        }
    }
    
    return type;
}

const char* getOpNameByType(ArithmeticNode::OperatorType opType)
{
	static_assert(sizeof(g_functions) / sizeof(*g_functions) == sizeof(g_opType) / sizeof(*g_opType), "Invalid Rule Strings!");

	for(int i = 0; i != sizeof(g_opType) / sizeof(*g_opType); ++i)
	{
		if (opType == g_opType[i])
		{
			return g_functions[i];
		}
	}

	return nullptr;
}

class Parser
{
public:
    Parser() : mOperators("+-*/^") {}
    
    ArithmeticNode* parseNode(const string& equation)
    {
        mTmpEquations.clear();
        mBuffer[0] = '\0';
        
        puts(equation.c_str());
        
        string result = extractEquation(equation);
        
        int index = 0;
        for(auto& s : mTmpEquations)
        {
            printf("@%d = %6s,\n", index++, s.c_str());
        }
        
        printf("@%d = %6s,\n", index, result.c_str());
        
        return parse(result);
    }
    
protected:
    
    //消除所有优先级
    string extractEquation(string equation)
    {
        
        //最高优先级为括号
        while(1)
        {
            auto startIndex = equation.find_last_of('(');
            if(startIndex == string::npos)
                break;
            
            auto endIndex = equation.find(')', startIndex + 1);
            if(endIndex == string::npos)
            {
                puts("括号不匹配!");
                equation = "";
                break;
            }
            
            string eq = equation.substr(startIndex + 1, endIndex - startIndex - 1);
            eq = extractOP(eq);
            sprintf(mBuffer, "@%d", (int)mTmpEquations.size());
            mTmpEquations.push_back(eq);
            
            //检查括号是否前置三角函数等
            if(startIndex != 0 && isalpha(equation[startIndex - 1]))
            {
                int opIndex = (int)startIndex - 2;
                while(opIndex >= 0 && isalpha(equation[opIndex])) --opIndex;
                if(opIndex < 0 || !isalpha(equation[opIndex])) ++opIndex;
                
                eq = equation.substr(opIndex, startIndex - opIndex);
                sprintf(mBuffer, "%s(@%d)", eq.c_str(), (int)mTmpEquations.size() - 1);
                mTmpEquations.push_back(mBuffer);
                sprintf(mBuffer, "@%d", (int)mTmpEquations.size() - 1);
                
                equation = equation.substr(0, opIndex) + mBuffer + equation.substr(endIndex + 1, equation.size() - endIndex - 1);
            }
            else
            {
                equation = equation.substr(0, startIndex) + mBuffer + equation.substr(endIndex + 1, equation.size() - endIndex - 1);
            }
        }
        
        equation = extractOP(equation);
        
        return equation;
    }
    
    string extractOP(string equation)
    {
        //乘方
        equation = parseOperator(equation, "^");
        //乘除
        equation = parseOperator(equation, "*/");
        //加减
        equation = parseOperator(equation, "+-");
        return equation;
    }
    
    //op必须为同一优先级.
    string parseOperator(string equation, string op)
    {
        while(1)
        {
            int opIndex = -1, opCount = 0, sz = (int)equation.size();
            
            for(int i = 0; i != sz; ++i)
            {
                if(opIndex < 0 && op.find_first_of(equation[i]) != string::npos)
                {
                    opIndex = i;
                }
                
                if(mOperators.find_first_of(equation[i]) != string::npos)
                    ++opCount;
            }
            
            if(opIndex < 0 || opCount == 1)
                break;
            
            int before, after;
            
            for(before = (int)opIndex - 1; before >= 0; --before)
            {
                int c = equation[before];
                if(!(isdigit(c) || c == '.' || c == '@' || c == 'x' || c == 'y'))
                    break;
            }
            
            for(after = (int)opIndex + 1; after < equation.size(); ++after)
            {
                int c = equation[after];
                if(!(isdigit(c) || c == '.' || c == '@' || c == 'x' || c == 'y'))
                    break;
            }
            
            if(before == opIndex - 1 || after == opIndex + 1)
            {
                puts("错误的表达式!");
                equation = "";
                break;
            }
            
            sprintf(mBuffer, "@%d", (int)mTmpEquations.size());
            mTmpEquations.push_back(equation.substr(before + 1, after - before - 1));
            
            string eq;
            if(before >= 0) eq += equation.substr(0, before + 1);
            eq += mBuffer;
            if(after < equation.size()) eq += equation.substr(after, equation.size() - after);
            
            equation = eq;
        }
        
        return equation;
    }
    
    ArithmeticNode* parseSimpleNode(const string& s)
    {
        ArithmeticNode* node = nullptr;
        
        if(s.size() == 1)
        {
            if(s[0] == 'x')
                return new ArithmeticNodeVariable<ArithmeticNode::VARIABLE_X>();
            
            if(s[0] == 'y')
                return new ArithmeticNodeVariable<ArithmeticNode::VARIABLE_Y>();
            
            if(s[0] == 'e')
            {
                return new ArithmeticNodeConstant<double>(M_E);
            }
        }
        
        if(s.compare("pi") == 0)
        {
            return new ArithmeticNodeConstant<double>(M_PI);
        }
        
        auto opIndex = s.find_first_of('(');
        
        if(opIndex != string::npos)
        {
            string opName = s.substr(0, opIndex);
            auto opType = getOpTypeByName(opName.c_str());
            if(opType != ArithmeticNode::OP_INVALID)
            {
                string opChild = s.substr(opIndex + 1, s.size() - opIndex - 2);
                auto* childNode = parse(opChild);
                
                if(childNode == nullptr)
                {
                    printf("Invalid node: %s\n", opChild.c_str());
                    return nullptr;
                }
                
                auto* opNode = new ArithmeticNodeOperatorImp(opType);
                opNode->addChildNode(childNode);
                return opNode;
            }
        }
        
        if(s[0] == '@')
        {
            int index;
            if(sscanf(s.c_str() + 1, "%d", &index) == 1 && index < mTmpEquations.size())
                return parse(mTmpEquations[index]);
            return nullptr;
        }
        else if(isdigit(s[0]) || s[0] == '.' || s[0] == '-')
        {
            double value;
            sscanf(s.c_str(), "%lf", &value);
            node = new ArithmeticNodeConstant<double>(value);
        }
        else
        {
            printf("Invalid node: %s\n", s.c_str());
        }
        
        return node;
    }
    
    //Recursive parse for the equation tree.
    ArithmeticNode* parse(const string& equation)
    {
        if(equation.empty())
            return nullptr; //Invalid case.
        
        char op, left[128], right[128];
        if(sscanf(equation.c_str(), "%127[^+-*/^]%c%127s", left, &op, right) == 3)
        {
            ArithmeticNode *leftNode, *rightNode;
            leftNode = parseSimpleNode(left);
            rightNode = parseSimpleNode(right);
            ArithmeticNodeOperatorImp* opNode = new ArithmeticNodeOperatorImp((ArithmeticNodeOperatorImp::OperatorType)op);
            opNode->addChildNode(leftNode);
            opNode->addChildNode(rightNode);
            return opNode;
        }
        
        return parseSimpleNode(equation);
    }
    
protected:
    char mBuffer[BUFFER_SIZE]; //for temp usage.
    
    vector<string> mTmpEquations;
    string mOperators;
};


ArithmeticNode* parseEquation(const string& equation)
{
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
        *it = tolower(*it);
        if(*it == '[' || *it == '{') *it = '(';
        else if(*it == ']' || *it == '}') *it = ')';
        
        if (isblank(*it))
        {
            it = eq.erase(it);
        }
        else ++it;
    }
    
    Parser parser;
    return parser.parseNode(eq);
}

void reduceNode(ArithmeticNode** pnode)
{
    ArithmeticNode* node = *pnode;
    
    if(node->canReduce())
    {
        auto* newNode = new ArithmeticNodeConstant<double>(node->value());
        delete *pnode;
        *pnode = newNode;
        return;
    }
    
    auto childNodes = node->childNode();
    
    if(childNodes != nullptr && !childNodes->empty())
    {
        for(auto& childNode : *childNodes)
        {
            if(childNode->canReduce())
            {
                auto* newNode = new ArithmeticNodeConstant<double>(childNode->value());
                delete childNode;
                childNode = newNode;
            }
            else
            {
                auto* grandChildNodes = childNode->childNode();
                if(grandChildNodes != nullptr && !grandChildNodes->empty())
                {
                    for(auto& grandChildNode : *grandChildNodes)
                    {
                        reduceNode(&grandChildNode);
                    }
                }
            }
        }
    }
}





