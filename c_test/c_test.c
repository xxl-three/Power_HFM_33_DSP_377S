* #include<stdio.h>
int main() {
    int a, b;
    char c;
    c = getchar();
    scanf_s("%d %d", &a, &b, &c);
    printf("����%d\n%d", a + b, c);
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
/*ʹ�ÿɱ�������ֶ�*/


/*#include<stdio.h>
int main(void)
{
    unsigned width, precision;          // �����޷������ͱ���width��precision�����ڴ洢�û�����Ŀ�Ⱥ;���
    int number = 256;                   // �������ͱ���number����ֵΪ256�����ڴ洢һ������
    double weight = 25.5;               // ����˫���ȸ����ͱ���weight����ֵΪ25.5�����ڴ洢һ��С��

    printf("Please input number's width:\n");             // ��ʾ�û��������ֵĿ��
    scanf_s("%d", &width);                                // ���û������ж�ȡһ��������������洢��width������
    printf("The number is: %*d\n", width, number);        // ʹ��*��ʽ�����ţ������û�����Ŀ�ȴ�ӡnumber��ֵ
    printf("Then please input width and precision:\n");   // ��ʾ�û������Ⱥ;���
    scanf_s("%d %d", &width, &precision);                 // ���û������ж�ȡһ��������һ���������������Ƿֱ�洢��width��precision������
    printf("Weight = %*.*f\n", width, precision, weight); // ʹ��*��ʽ�����ţ������û�����Ŀ�Ⱥ;��ȴ�ӡweight��ֵ

    return 0;                          // ����0����ʾ����ִ�гɹ�
}
/#include<stdio.h>
int main()
{
    int a = 12; // �������ͱ���a����ֵΪ12
    float b = 3.1415; // ���帡���ͱ���b����ֵΪ3.1415
    char c = 'A'; // �����ַ��ͱ���c����ֵΪ'A'
    printf("%d\n", a); // ��ӡ���ͱ���a��ֵ
    printf("0%o\n", a);
    printf("%o\n", a);// ��ӡ���ͱ���a�İ˽��Ʊ�ʾ
    printf("0x%x\n", a);
    printf("%x\n", a); // ��ӡ���ͱ���a��ʮ�����Ʊ�ʾ
    printf("%4.3f\n", b);
    printf("%10.3f\n", b);
    printf("%3.2f\n", b); // ��ӡ�����ͱ���b��ֵ��������λС��
    rintf("%c\n", c); // ��ӡ�ַ��ͱ���c��ֵ
    getchar(); // �ȴ��û�����һ���ַ�
    return 0;
}*/
//���������ʾ

/*��*����scanf���յ�����*/
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
//    printf("int �洢��С : %lu \n", sizeof(int));
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
//    printf("Line 1 - c ��ֵ�� %d\n", c);
//    c = a - b;
//    printf("Line 2 - c ��ֵ�� %d\n", c);
//    c = a * b;
//    printf("Line 3 - c ��ֵ�� %d\n", c);
//    c = a / b;
//    printf("Line 4 - c ��ֵ�� %d\n", c);
//    c = a % b;
//    printf("Line 5 - c ��ֵ�� %d\n", c);
//    c = a++;  // ��ֵ���ټ� 1 ��c Ϊ 21��a Ϊ 22
//    printf("Line 6 - c ��ֵ�� %d\n", c);
//    c = a--;  // ��ֵ���ټ� 1 ��c Ϊ 22 ��a Ϊ 21
//    printf("Line 7 - c ��ֵ�� %d\n", c);
//
//}
/*
#include <stdio.h>

int main()
{
    int c;
    int a = 10;
    c = a++;
    printf("�ȸ�ֵ�����㣺\n");
    printf("Line 1 - c ��ֵ�� %d\n", c);
    printf("Line 2 - a ��ֵ�� %d\n", a);
    a = 10;
    c = a--;
    printf("Line 3 - c ��ֵ�� %d\n", c);
    printf("Line 4 - a ��ֵ�� %d\n", a);

    printf("�������ֵ��\n");
    a = 10;
    c = ++a;
    printf("Line 5 - c ��ֵ�� %d\n", c);
    printf("Line 6 - a ��ֵ�� %d\n", a);
    a = 10;
    c = --a;
    printf("Line 7 - c ��ֵ�� %d\n", c);
    printf("Line 8 - a ��ֵ�� %d\n", a);

}
*/
//#include <stdio.h>
//
//int main()
//{
//    int a = 21; // �������ͱ���a����ֵΪ21
//    int b = 10; // �������ͱ���b����ֵΪ10
//    int c; // �������ͱ���c
//
//    c = a + b; // ��a��b�ĺ͸�ֵ��c
//    printf("Line 1 - c ��ֵ�� %d\n", c); // ��ӡc��ֵ
//    c = a - b; // ��a��b�Ĳֵ��c
//    printf("Line 2 - c ��ֵ�� %d\n", c); // ��ӡc��ֵ
//    c = a * b; // ��a��b�Ļ���ֵ��c
//    printf("Line 3 - c ��ֵ�� %d\n", c); // ��ӡc��ֵ
//    c = a / b; // ��a����b���̸�ֵ��c
//    printf("Line 4 - c ��ֵ�� %d\n", c); // ��ӡc��ֵ
//    c = a % b; // ��a����b��������ֵ��c
//    printf("Line 5 - c ��ֵ�� %d\n", c); // ��ӡc��ֵ
//    c = a++; // �Ƚ�a��ֵ����c��Ȼ��a��ֵ��1
//    printf("Line 6 - c ��ֵ�� %d\n", c); // ��ӡc��ֵ
//    c = a--; // �Ƚ�a��ֵ����c��Ȼ��a��ֵ��1
//    printf("Line 7 - c ��ֵ�� %d\n", c); // ��ӡc��ֵ
//
//    return 0; // ����0����ʾ����ִ�гɹ�
//}
//#include <stdio.h>
//
//int main()
//{
//    int a = 21; // �������ͱ���a����ֵΪ21
//    int b = 10; // �������ͱ���b����ֵΪ10
//    int c;
//
//    // �ж�a�Ƿ����b
//    if (a == b)
//    {
//        printf("Line 1 - a ���� b\n");
//    }
//    else
//    {
//        printf("Line 1 - a ������ b\n");
//    }
//
//    // �ж�a�Ƿ�С��b
//    if (a < b)
//    {
//        printf("Line 2 - a С�� b\n");
//    }
//    else
//    {
//        printf("Line 2 - a ��С�� b\n");
//    }
//
//    // �ж�a�Ƿ����b
//    if (a > b)
//    {
//        printf("Line 3 - a ���� b\n");
//    }
//    else
//    {
//        printf("Line 3 - a ������ b\n");
//    }
//
//    // �ı�a��b��ֵ
//    a = 5;
//    b = 20;
//
//    // �ж�a�Ƿ�С�ڻ����b
//    if (a <= b)
//    {
//        printf("Line 4 - a С�ڻ���� b\n");
//    }
//
//    // �ж�b�Ƿ���ڻ����a
//    if (b >= a)
//    {
//        printf("Line 5 - b ���ڻ���� a\n");
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
//    printf("����һ������ : ");
//    scanf_s("%d", &num);
//
//    (num % 2 == 0) ? printf("ż��") : printf("����");
//	return 0;
//}���������

//#include <stdio.h>

//// ��������
//int max(int num1, int num2);
//
//// ������
//int main() {
//    int num1, num2;
//    printf("������������: ");
//    scanf_s("%d %d", &num1, &num2);  // ʹ��scanf����ȡ����
//    printf("�ϴ�ֵ��: %d\n", max(num1, num2));
//    return 0;
//}
//
//// max ��������
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
//    // ����ö��Ԫ��
//    for (day = MON; day <= SUN; day++) {
//        printf("ö��Ԫ�أ�%d \n", day);
//    }
//}
//#include <stdio.h>
//
//int main()
//{
//    int  var1;
//    char var2[10];
//
//    printf("var1 �����ĵ�ַ�� %p\n", &var1);
//    printf("var2 �����ĵ�ַ�� %p\n", &var2);
//
//    return 0
//
//#include "F28x_Project.h"   // �����豸ͷ�ļ���ʾ���ļ�
//
//// ��ʼ��GPIO����
//void InitGpioLeds(void)
//{
//    EALLOW;
//
//    // ����GPIO92ΪGPIO����
//    GpioCtrlRegs.GPBGMUX1.bit.GPIO92 = 0; // ѡ��GPIO����
//    GpioCtrlRegs.GPBMUX1.bit.GPIO92 = 0;  // ѡ��GPIO����
//    GpioCtrlRegs.GPBDIR.bit.GPIO92 = 1;   // ����Ϊ���
//    GpioDataRegs.GPBSET.bit.GPIO92 = 1;   // ���øߵ�ƽ������LED
//
//    // ����GPIO99ΪGPIO����
//    GpioCtrlRegs.GPBGMUX2.bit.GPIO99 = 0; // ѡ��GPIO����
//    GpioCtrlRegs.GPBMUX2.bit.GPIO99 = 0;  // ѡ��GPIO����
//    GpioCtrlRegs.GPBDIR.bit.GPIO99 = 1;   // ����Ϊ���
//    GpioDataRegs.GPBSET.bit.GPIO99 = 1;   // ���øߵ�ƽ������LED
//
//    // ����GPIO133ΪGPIO����
//    GpioCtrlRegs.GPCGMUX1.bit.GPIO133 = 0; // ѡ��GPIO����
//    GpioCtrlRegs.GPCMUX1.bit.GPIO133 = 0;  // ѡ��GPIO����
//    GpioCtrlRegs.GPCDIR.bit.GPIO133 = 1;   // ����Ϊ���
//    GpioDataRegs.GPCSET.bit.GPIO133 = 1;   // ���øߵ�ƽ������LED
//
//    EDIS;
//}
//
//void main(void)
//{
//    // ��ʼ��ϵͳ����: ʱ��, PLL, ���ӹ�, ����ʱ��
//    InitSysCtrl();
//
//    // ��ʼ��GPIO
//    InitGpio();
//
//    // ��ʼ��LED GPIO
//    InitGpioLeds();
//
//    // ���ÿ��Ź�
//    EALLOW;
//    SysCtrlRegs.WDCR = 0x68;
//    EDIS;
//
//    // ��ѭ��
//    while (1)
//    {
//        // �ڴ˴����������������
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
    Gustav Mahler      ����1�δ�ӡ������
Gustav             ����2�δ�ӡ������
Mahler             �����ǵ�2�δ�ӡ������
Gustav Mahler      ����3�κ͵�4�δ�ӡ������
}*/
/* nogood.c -- �д���ĳ��� */
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
/* print1.c - ��ʾprintf()��һЩ���� */
#include <stdio.h>
int main(void)
{
    int ten = 10;
    int two = 2;

    printf("Doing it right: ");
    printf("%d minus %d is %d\n", ten, 2, ten - two);
    printf("Doing it wrong: ");
    printf("%d minus %d is %d\n", ten);  // ��©2������

    return 0;
}