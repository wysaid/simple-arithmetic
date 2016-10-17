#define SHOW_CONSOLE

#include "../arithmetic.h"
#include "graphics.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

class ElementInterface
{
public:

	ElementInterface(float x, float y, float radius) : m_x(x), m_y(y), m_radius(radius) {}

	void moveTo(float x, float y)
	{
		m_x = x;
		m_y = y;
	}

	virtual void render(float x, float y) = 0;

protected:

	float m_x, m_y;
	float m_radius;
};

class OperatorElement : public ElementInterface
{
public:

	void render(float x, float y)
	{
		fillellipsef(x + m_x, y + m_y, m_radius, m_radius);
		outtextxy(x + m_x - m_radius / 2, y + m_y, m_opName);
	}

private:
	const char* m_opName;
};

class ConstantElement : public ElementInterface
{
public:	

	void render(float x, float y)
	{
		fillellipsef(x + m_x, y + m_y, m_radius, m_radius);
		xyprintf(x + m_x - m_radius / 2, y + m_y, "%g", m_value);
	}


private:
	double m_value;
};

class VariableElement : public ElementInterface
{
public:

	void render(float x, float y)
	{
		fillellipsef(x + m_x, y + m_y, m_radius, m_radius);
		xyprintf(x + m_x - m_radius / 2, y + m_y, "%c=%g", m_varName, m_varValue);
	}

private:
	char m_varName;
	double m_varValue;
};

void drawArithMeticNodes(int x, int y)
{

}

bool dealMsg(int msg, ArithmeticNode*& node)
{
	using namespace std;
	const int BUFFER_SIZE = 1024;
	char buffer[1024] = "";
	int ret;

	flushkey();

	switch (msg)
	{
	case key_space:
		ret = inputbox_getline("Input your equation:", "Support: +, -, *, /, ^, sin, cos, arcsin, arccos, tan, arctan, exp, abs, sign, sqrt, ln, log2, lg(log10)\nx(variable), y(variable)\n\n e.g: 2*3^4*5 + (sin(3.14/3) - sqrt(2)) * 2 + x^2 * (2 / y)", buffer, BUFFER_SIZE);

		//Valid Input
		if (ret != 0)
		{
			if (node != nullptr)
				delete node;
			node = parseEquation(buffer);

			if (node != nullptr && node->isValid())
			{
				if (node->canReduce())
					printf("Result: %s = %g\n", buffer, node->value());
				else
				{
					puts("Variables are detected, please set them!");
					dealMsg('x', node);
					dealMsg('y', node);
					printf("Result: %s = %g\n", buffer, node->value());
				}
				return true;
			}
			else
			{
				string msg = "Invalid equation: ";
				msg += buffer;
				MessageBoxA(getHWnd(), msg.c_str(), "Invalid equation!", MB_OK);
				delete node;
				node = nullptr;
				return false;
			}
		}
		break;
	case 'x': case 'y':
		if (node != nullptr)
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
					node->setValue(isX ? ArithmeticNode::VARIABLE_X : ArithmeticNode::VARIABLE_Y, value);
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

void drawNodes(ArithmeticNode* node, int x, int y, PIMAGE pimg)
{
	switch (node->nodeType())
	{
	case ArithmeticNode::CONSTANT:
		setcolor(WHITE);
		settarget(pimg);
		xyprintf(x, y, "%g", node->value());
		settarget(nullptr);
		break;
	case ArithmeticNode::OPERATOR:
	{
		ArithmeticNode::OperatorType opType = node->operatorType();

		if (opType <= ArithmeticNode::OP_POWER)
		{
			outtextxy(x, y, (char)opType, pimg);
		}
		else
		{
			const char* name = getOpNameByType(node->operatorType());
			if (name == nullptr)
				break;

			setcolor(RED);
			outtextxy(x, y, name, pimg);
		}

		auto* childNodes = node->childNode();
		if (childNodes != nullptr)
		{
			for (auto* node : *childNodes)
			{

			}
		}
	}
		break;
	case ArithmeticNode::VARIABLE_X:case ArithmeticNode::VARIABLE_Y:
		setcolor(GREEN);
		outtextxy(x, y, node->nodeType() == ArithmeticNode::VARIABLE_X ? 'x' : 'y', pimg);
		break;
	default:
		break;
	}
}

int main()
{
	initgraph(SCR_WIDTH, SCR_HEIGHT, INIT_RENDERMANUAL);
	ArithmeticNode* node = nullptr;
	//PIMAGE pimg = newimage()
	
	for (; is_run(); delay_fps(60))
	{
		cleardevice();

		if (kbhit())
		{
			int ch = getch();
			if(dealMsg(ch, node))
			{

			}

			flushkey();
		}
	}

	closegraph();
	return 0;
}
