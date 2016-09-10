/*
* main.cpp
*
*  Created on: 2016-9-10
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#define SHOW_CONSOLE
#include "../arithmetic.h"
#include <iostream>
using namespace std;

int main()
{
	string s = "1+1";
	ArithmeticNode* node = parseEquation(s);

	cout << node->value() << endl;;

	return 0;
}