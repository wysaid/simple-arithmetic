/*
* main.cpp
*
*  Created on: 2016-10-10
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*        Blog: wysaid.org
*/

#define SHOW_CONSOLE

#include "graphics.h"
#include "../arithmetic.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

bool dealMsg(int msg, ArithmeticExpression*& expression)
{
	using namespace std;
	const int BUFFER_SIZE = 1024;
	char buffer[1024] = "";
	int ret;

	flushkey();

	switch (msg)
	{
	case key_space:case '\r': case '\n':
		ret = inputbox_getline("Input your equation:", "Support: +, -, *, /, ^, sin, cos, arcsin, arccos, tan, arctan, exp, abs, sign, sqrt, ln, log2, lg(log10)\nx(variable), y(variable)\n\n e.g: 2*3^4*5 + (sin(3.14/3) - sqrt(2)) * 2 + x^2 * (2 / y)", buffer, BUFFER_SIZE);

		//Valid Input
		if (ret != 0)
		{
			expression = new ArithmeticExpression(buffer);

			if (expression->isValid())
			{
				if (expression->canReduce())
					printf("Result: %s = %g\n", buffer, expression->value());
				else
				{
					puts("Variables are detected, please set them!");
					dealMsg('x', expression);
					dealMsg('y', expression);
					printf("Result: %s = %g\n", buffer, expression->value());
				}
				return true;
			}
			else
			{
				string msg = "Invalid equation: ";
				msg += buffer;
				MessageBoxA(getHWnd(), msg.c_str(), "Invalid equation!", MB_OK);
				delete expression;
				expression = nullptr;
				return false;
			}
		}
		break;
	case 'x': case 'y':
		if (expression != nullptr)
		{
			bool isX = msg == 'x';
			string title = "Input value of ";
			title += isX ? 'x' : 'y';
			string content = title + "\nFor example: 3.14";
			ret = inputbox_getline(title.c_str(), content.c_str(), buffer, BUFFER_SIZE);
			if (ret != 0)
			{
				double value;
				if (sscanf(buffer, "%lf", &value) == 1 || sscanf(buffer, "%*[^0-9.-]%lf", &value) == 1)
				{
					printf("%c = %g\n", msg, value);
					if(isX) expression->setX(value);
					else expression->setY(value);
				}
				return true;
			}
			else
			{
				sprintf(buffer, "%c is not set, default to 0", msg);
				MessageBoxA(getHWnd(), buffer, "Tips", MB_OK);
				return false;
			}
		}
		break;
	default:
		break;
	}

	return false;
}

void drawNodes(ArithmeticNode* node, int x, int y, int depth)
{
	switch (node->nodeType())
	{
	case ArithmeticNode::CONSTANT:
		setcolor(0xff);
		setfillcolor(WHITE);
		bar(x - 20, y - 10, x + 30, y + 10);
		xyprintf(x - 12, y - 8, "%g", node->value());
		break;
	case ArithmeticNode::OPERATOR:
	{
		ArithmeticNode::OperatorType opType = node->operatorType();

		setcolor(0);
		setfillcolor(0xff0000);
		fillellipse(x, y, 20, 20);

		if (opType <= ArithmeticNode::OP_POWER)
		{
			setcolor(RED);
			outtextxy(x - 5, y - 8, (char)opType);
		}
		else
		{
			const char* name = ArithmeticNode::getOpNameByType(node->operatorType());
			if (name == nullptr)
				break;

			setcolor(RED);
			outtextxy(x - 12, y - 8, name);
		}

		auto* childNodes = node->childNode();
		if (childNodes != nullptr)
		{
			const int sz = childNodes->size();
			const int halfSz = sz / 2;
			const int len = 80 * powf(2, depth);
			int i = 0;
			for (auto* childNode : *childNodes)
			{
				const int nx = !(sz & 1) ? x + (i - halfSz + 0.5) * len : x + (i - halfSz) * len;
				const int ny = y + 50;
				setcolor(0x00ff00);
				line(x, y + 15, nx, ny);
				drawNodes(childNode, nx, ny, depth - 1);
				++i;
			}
		}
	}
		break;
	case ArithmeticNode::VARIABLE_X:case ArithmeticNode::VARIABLE_Y:
		setcolor(0);
		setfillcolor(WHITE);
		bar(x - 20, y - 10, x + 20, y + 10);
		outtextxy(x - 12, y - 8, node->nodeType() == ArithmeticNode::VARIABLE_X ? 'x' : 'y');
		break;
	default:
		break;
	}
}

int getDepth(ArithmeticNode* node, int depth = 0)
{
	if(node->nodeType() == ArithmeticNode::OPERATOR)
	{
		int newDepth = depth + 1;
		int maxDepth = newDepth;
		auto* childNodes = node->childNode();
		if (childNodes != nullptr)
		{
			for (auto* childNode : *childNodes)
			{
				int d = getDepth(childNode, newDepth);
				if (d > maxDepth)
					maxDepth = d;
			}
		}
		return maxDepth;
	}

	return depth + 1;
}

int main()
{
	initgraph(SCR_WIDTH, SCR_HEIGHT, INIT_RENDERMANUAL);
	ArithmeticExpression* expression = nullptr;
	setbkmode(TRANSPARENT);
	setlinewidth(2);
	bool shouldRedraw = true;
	int lastx, lasty;
	int x = SCR_WIDTH / 2, y = SCR_HEIGHT / 2;
	bool isMoving = false;
	int depth = 0;
	
	for (; is_run(); delay_fps(60))
	{
		if (kbhit())
		{
			int ch = getch();
			if(dealMsg(ch, expression))
			{
				x = SCR_WIDTH / 2;
				y = SCR_HEIGHT / 2;
				if (expression != nullptr && expression->isValid())
				{
					depth = getDepth(expression->node());
					shouldRedraw = true;
				}
			}
			else
			{
				puts("Deal msg: false is returned");
			}

			flushkey();
		}

		if (mousemsg())
		{
			do 
			{
				mouse_msg msg = getmouse();
				if (msg.is_down())
				{
					isMoving = true;
					lastx = msg.x;
					lasty = msg.y;
				}
				else if (msg.is_up())
				{
					isMoving = false;
				}

				if (isMoving && msg.is_move())
				{
					x += msg.x - lastx;
					y += msg.y - lasty;
					lastx = msg.x;
					lasty = msg.y;
				}

			} while (mousemsg());

			shouldRedraw = true;
		}

		//降低cpu使用率
		if (shouldRedraw)
		{
			cleardevice();
			setcolor(0xffffff);
			outtextxy(10, 20, "按下空格键输入表达式, 如: 1+1, 之后可使用鼠标拖拽移动图形");

			if (expression != nullptr && expression->isValid())
			{
				drawNodes(expression->node(), x, y, depth - 2);
			}

			shouldRedraw = false;
		}
	}

	closegraph();
	return 0;
}
