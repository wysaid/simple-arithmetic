#include <graphics.h> // 引用图形库

int main()
{
    initgraph(640, 480);   // 初始化，显示一个窗口，这里和 TC 略有区别
    circle(200, 200, 200); // 画圆，圆心(200, 200)，半径 100
    getch();               // 暂停一下等待用户按键
    closegraph();          // 关闭图形界面
    return 0;
}