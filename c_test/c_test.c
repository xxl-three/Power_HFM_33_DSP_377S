* #include<stdio.h>
int main() {
    int a, b;
    char c;
    c = getchar();
    scanf_s("%d %d", &a, &b, &c);
    printf("答案是%d\n%d", a + b, c);
    printf("%d", c);
    return 0;
}
#include<stdio.h>
/*int main()
{
    int a = 12;
    float b = 3.1415;
    char c = 'A';
    printf("%d\n", a);
    printf("0%o\n", a);
    printf("0x%x\n", a);
    printf("3.2f\n", b);
    printf("%c\n", c);
    getchar();
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
int main()
{
    float a = 0.302;
    float b = 128.101;
    double c = 123;
    float d = 112.64E3;
    double e = 0.7623e-2;
    float f = 1.23002398;
    printf("a=%e \nb=%f \nc=%lf \nd=%lE \ne=%lf \nf=%f\n", a, b, c, d, e, f);

    return 0;
}
/*使用可变宽度输出字段*/


/*#include<stdio.h>
int main(void)
{
    unsigned width, precision;          // 声明无符号整型变量width和precision，用于存储用户输入的宽度和精度
    int number = 256;                   // 声明整型变量number并赋值为256，用于存储一个整数
    double weight = 25.5;               // 声明双精度浮点型变量weight并赋值为25.5，用于存储一个小数

    printf("Please input number's width:\n");             // 提示用户输入数字的宽度
    scanf_s("%d", &width);                                // 从用户输入中读取一个整数，并将其存储在width变量中
    printf("The number is: %*d\n", width, number);        // 使用*格式化符号，根据用户输入的宽度打印number的值
    printf("Then please input width and precision:\n");   // 提示用户输入宽度和精度
    scanf_s("%d %d", &width, &precision);                 // 从用户输入中读取一个整数和一个整数，并将它们分别存储在width和precision变量中
    printf("Weight = %*.*f\n", width, precision, weight); // 使用*格式化符号，根据用户输入的宽度和精度打印weight的值

    return 0;                          // 返回0，表示程序执行成功
}
/#include<stdio.h>
int main()
{
    int a = 12; // 定义整型变量a并赋值为12
    float b = 3.1415; // 定义浮点型变量b并赋值为3.1415
    char c = 'A'; // 定义字符型变量c并赋值为'A'
    printf("%d\n", a); // 打印整型变量a的值
    printf("0%o\n", a);
    printf("%o\n", a);// 打印整型变量a的八进制表示
    printf("0x%x\n", a);
    printf("%x\n", a); // 打印整型变量a的十六进制表示
    printf("%4.3f\n", b);
    printf("%10.3f\n", b);
    printf("%3.2f\n", b); // 打印浮点型变量b的值，保留两位小数
    rintf("%c\n", c); // 打印字符型变量c的值
    getchar(); // 等待用户输入一个字符
    return 0;
}*/
//变量输出表示

/*用*跳过scanf接收的数字*/
//#include<stdio.h>
//
//int main(void)
//{
//    int num,a,b;
//    printf("please enter three numbers:\n");
//    scanf_s("%*d %*d %d", &num, &a, &b);
//    printf("the last number is %d\n", num);	
//	printf("the second number is %d*\n", a);
//	printf("the first number is %d\n", b);
//    return 0;
//}

//#include <stdio.h>
//#include <limits.h>
//
//int main()
//{
//    printf("int 存储大小 : %lu \n", sizeof(int));
//
//    return 0;
//}
//#include <stdio.h>
//
//int main()
//{
//    int a = 21;
//    int b = 10;
//    int c;
//
//    c = a + b;
//    printf("Line 1 - c 的值是 %d\n", c);
//    c = a - b;
//    printf("Line 2 - c 的值是 %d\n", c);
//    c = a * b;
//    printf("Line 3 - c 的值是 %d\n", c);
//    c = a / b;
//    printf("Line 4 - c 的值是 %d\n", c);
//    c = a % b;
//    printf("Line 5 - c 的值是 %d\n", c);
//    c = a++;  // 赋值后再加 1 ，c 为 21，a 为 22
//    printf("Line 6 - c 的值是 %d\n", c);
//    c = a--;  // 赋值后再减 1 ，c 为 22 ，a 为 21
//    printf("Line 7 - c 的值是 %d\n", c);
//
//}
/*
#include <stdio.h>

int main()
{
    int c;
    int a = 10;
    c = a++;
    printf("先赋值后运算：\n");
    printf("Line 1 - c 的值是 %d\n", c);
    printf("Line 2 - a 的值是 %d\n", a);
    a = 10;
    c = a--;
    printf("Line 3 - c 的值是 %d\n", c);
    printf("Line 4 - a 的值是 %d\n", a);

    printf("先运算后赋值：\n");
    a = 10;
    c = ++a;
    printf("Line 5 - c 的值是 %d\n", c);
    printf("Line 6 - a 的值是 %d\n", a);
    a = 10;
    c = --a;
    printf("Line 7 - c 的值是 %d\n", c);
    printf("Line 8 - a 的值是 %d\n", a);

}
*/
//#include <stdio.h>
//
//int main()
//{
//    int a = 21; // 定义整型变量a并赋值为21
//    int b = 10; // 定义整型变量b并赋值为10
//    int c; // 定义整型变量c
//
//    c = a + b; // 将a和b的和赋值给c
//    printf("Line 1 - c 的值是 %d\n", c); // 打印c的值
//    c = a - b; // 将a和b的差赋值给c
//    printf("Line 2 - c 的值是 %d\n", c); // 打印c的值
//    c = a * b; // 将a和b的积赋值给c
//    printf("Line 3 - c 的值是 %d\n", c); // 打印c的值
//    c = a / b; // 将a除以b的商赋值给c
//    printf("Line 4 - c 的值是 %d\n", c); // 打印c的值
//    c = a % b; // 将a除以b的余数赋值给c
//    printf("Line 5 - c 的值是 %d\n", c); // 打印c的值
//    c = a++; // 先将a的值赋给c，然后将a的值加1
//    printf("Line 6 - c 的值是 %d\n", c); // 打印c的值
//    c = a--; // 先将a的值赋给c，然后将a的值减1
//    printf("Line 7 - c 的值是 %d\n", c); // 打印c的值
//
//    return 0; // 返回0，表示程序执行成功
//}
//#include <stdio.h>
//
//int main()
//{
//    int a = 21; // 定义整型变量a并赋值为21
//    int b = 10; // 定义整型变量b并赋值为10
//    int c;
//
//    // 判断a是否等于b
//    if (a == b)
//    {
//        printf("Line 1 - a 等于 b\n");
//    }
//    else
//    {
//        printf("Line 1 - a 不等于 b\n");
//    }
//
//    // 判断a是否小于b
//    if (a < b)
//    {
//        printf("Line 2 - a 小于 b\n");
//    }
//    else
//    {
//        printf("Line 2 - a 不小于 b\n");
//    }
//
//    // 判断a是否大于b
//    if (a > b)
//    {
//        printf("Line 3 - a 大于 b\n");
//    }
//    else
//    {
//        printf("Line 3 - a 不大于 b\n");
//    }
//
//    // 改变a和b的值
//    a = 5;
//    b = 20;
//
//    // 判断a是否小于或等于b
//    if (a <= b)
//    {
//        printf("Line 4 - a 小于或等于 b\n");
//    }
//
//    // 判断b是否大于或等于a
//    if (b >= a)
//    {
//        printf("Line 5 - b 大于或等于 a\n");
//    }
//
//    return 0;
//}
//#include<stdio.h>
//
//int main()
//{
//    int num;
//
//    printf("输入一个数字 : ");
//    scanf_s("%d", &num);
//
//    (num % 2 == 0) ? printf("偶数") : printf("奇数");
//	return 0;
//}三分运算符

//#include <stdio.h>

//// 函数声明
//int max(int num1, int num2);
//
//// 主函数
//int main() {
//    int num1, num2;
//    printf("输入两个整数: ");
//    scanf_s("%d %d", &num1, &num2);  // 使用scanf来读取输入
//    printf("较大值是: %d\n", max(num1, num2));
//    return 0;
//}
//
//// max 函数定义
//int max(int num1, int num2) {
//    int result;
//    if (num1 > num2)
//        result = num1;
//    else
//        result = num2;
//    return result;
//}


//#include<stdio.h>
//
//enum DAY
//{
//    MON = 1, TUE, WED, THU, FRI, SAT, SUN
//} day;
//int main()
//{
//    // 遍历枚举元素
//    for (day = MON; day <= SUN; day++) {
//        printf("枚举元素：%d \n", day);
//    }
//}
//#include <stdio.h>
//
//int main()
//{
//    int  var1;
//    char var2[10];
//
//    printf("var1 变量的地址： %p\n", &var1);
//    printf("var2 变量的地址： %p\n", &var2);
//
//    return 0
//
//#include "F28x_Project.h"   // 包含设备头文件和示例文件
//
//// 初始化GPIO引脚
//void InitGpioLeds(void)
//{
//    EALLOW;
//
//    // 配置GPIO92为GPIO功能
//    GpioCtrlRegs.GPBGMUX1.bit.GPIO92 = 0; // 选择GPIO功能
//    GpioCtrlRegs.GPBMUX1.bit.GPIO92 = 0;  // 选择GPIO功能
//    GpioCtrlRegs.GPBDIR.bit.GPIO92 = 1;   // 设置为输出
//    GpioDataRegs.GPBSET.bit.GPIO92 = 1;   // 设置高电平，点亮LED
//
//    // 配置GPIO99为GPIO功能
//    GpioCtrlRegs.GPBGMUX2.bit.GPIO99 = 0; // 选择GPIO功能
//    GpioCtrlRegs.GPBMUX2.bit.GPIO99 = 0;  // 选择GPIO功能
//    GpioCtrlRegs.GPBDIR.bit.GPIO99 = 1;   // 设置为输出
//    GpioDataRegs.GPBSET.bit.GPIO99 = 1;   // 设置高电平，点亮LED
//
//    // 配置GPIO133为GPIO功能
//    GpioCtrlRegs.GPCGMUX1.bit.GPIO133 = 0; // 选择GPIO功能
//    GpioCtrlRegs.GPCMUX1.bit.GPIO133 = 0;  // 选择GPIO功能
//    GpioCtrlRegs.GPCDIR.bit.GPIO133 = 1;   // 设置为输出
//    GpioDataRegs.GPCSET.bit.GPIO133 = 1;   // 设置高电平，点亮LED
//
//    EDIS;
//}
//
//void main(void)
//{
//    // 初始化系统控制: 时钟, PLL, 监视狗, 外设时钟
//    InitSysCtrl();
//
//    // 初始化GPIO
//    InitGpio();
//
//    // 初始化LED GPIO
//    InitGpioLeds();
//
//    // 禁用看门狗
//    EALLOW;
//    SysCtrlRegs.WDCR = 0x68;
//    EDIS;
//
//    // 主循环
//    while (1)
//    {
//        // 在此处添加您的其他代码
//    }
//}
/*#include <stdio.h>
int main(void)
{
    int dogs;

    printf("How many dogs do you have?\n");
    scanf_s("%d", &dogs);
    printf("So you have %d dog(s)!\n", dogs);

    return 0;
    Gustav Mahler      ←第1次打印的内容
Gustav             ←第2次打印的内容
Mahler             ←仍是第2次打印的内容
Gustav Mahler      ←第3次和第4次打印的内容
}*/
/* nogood.c -- 有错误的程序 */
//#include<stdio.h>
//void one_three(void);
//void two(void);
//int main()
//{
//    printf("starting now\n");
//    one_three();
//    printf("done\n");
//    return 0;
//}
//void one_three(void)
//{
//    printf("one\n");
//    two();
//    printf("three\n");
//        return 0;
//}
//void two(void)
//{
//    printf("twe\n");
//
//    return 0;
//}
/* print1.c - 演示printf()的一些特性 */
#include <stdio.h>
int main(void)
{
    int ten = 10;
    int two = 2;

    printf("Doing it right: ");
    printf("%d minus %d is %d\n", ten, 2, ten - two);
    printf("Doing it wrong: ");
    printf("%d minus %d is %d\n", ten);  // 遗漏2个参数

    return 0;
}