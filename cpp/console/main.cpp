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

int main(int argc, char **argv)
{
    string s;// = "(1+1+1+1) ^ tan(((2+2)+(3-2)) * 0.5) + 5*(6-2*4) + cos(x^2) + sin(y)^2";

	if (argc == 1)
	{
		puts("Input your equation:");
		getline(cin, s);
	}
	else
	{
		s = argv[1];
	}

	ArithmeticNode* node = parseEquation(s);

    if(node != nullptr && node->isValid())
    {
        double x = 55, y = 88;
        node->setValue(ArithmeticNode::VARIABLE_X, x);
        node->setValue(ArithmeticNode::VARIABLE_Y, y);
        printf("\nx=%g\ny=%g\n%s = %g\n", x, y, s.c_str(), node->value());
    }
    else
    {
        puts("Invalid Equation!");
    }
    
    delete node;
	return 0;
}
