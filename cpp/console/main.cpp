/*
* main.cpp
*
*  Created on: 2016-9-10
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*        Blog: wysaid.org
*/

#include "../arithmetic.h"
#include <iostream>
using namespace std;

int main()
{
	string s = "(1+1+1+1) ^ (((2+2)+(3-2)) * 0.5) + 5*(6-2*4)";
	ArithmeticNode* node = parseEquation(s);

    node->setValue(ArithmeticNode::VARIABLE_X, 55);
    node->setValue(ArithmeticNode::VARIABLE_Y, 88);
    
    if(node != nullptr && node->isValid())
        printf("\n%s = %g\n", s.c_str(), node->value());
    else puts("Invalid Equation!");

    delete node;
	return 0;
}
