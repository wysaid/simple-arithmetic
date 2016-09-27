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
	string s = "1 + 2 + (1+1+1+1+1) ^ (((2+2)+(3-2)) + 4) * 2+1+1+1";
	ArithmeticNode* node = parseEquation(s);

//	cout << node->value() << endl;

    delete node;
	return 0;
}
