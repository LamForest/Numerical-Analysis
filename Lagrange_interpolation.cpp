#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

/********
输入：X 插值节点横坐标， Y 插值节点纵坐标， n 插值节点个数， x 所求节点x坐标
输出：所求节点的值
********/
double Lagrange(double *X, double *Y, double n, double x){

    double ff = 0; //所求节点的值
    for (int i = 0; i < n; ++i) {


        double numerator = 1;   // numerator 为 插值基函数li(x) 的 分子
        double denominator = 1; // denominator 为 插值基函数li(x) 的 分母
        for (int j = 0; j < n; ++j) {
            // i == j时跳过
            if(i == j)
                continue;

            // 分母的计算
            denominator *= X[i] - X[j];

            // 不能继续下去，因为分母为0，原因是插值节点有重复，程序中止
            if(denominator == 0){
                cout << "ERROR: Divided by zero, may be x(i) == x(j)" << endl;
                exit(-1);
            }

            // 分子的计算
            numerator *= x - X[j];
        }
        // 每次循环加上li(x) * y(i)
        ff += numerator * Y[i] / denominator;
    }

    return  ff;
}

int main(int argc, char const *argv[]) {
    //缺少参数，报错
    if(argc < 2)
    {
        cout << "ERROR: Too few arguments" << endl;
        return 0;
    }

    int n;          // 插值点节点个数
    double x;       // 待求的点的x坐标

    FILE *fp = fopen(argv[1], "r");
    //文件无法打开，出错
    if(fp == NULL)
    {
        cout << "ERROR: File not exist" << endl;
    }

    //读入
    fscanf(fp, "%lf\n", &x);
    fscanf(fp, "%d\n", &n);
    printf("x = %.4lf\n\n", x);

    double X[n];  // 插值节点横坐标
    double Y[n];  // 插值节点纵坐标

    printf("共%d个插值点,如下:\n", n);
    //读入并打印插值节点
    for (int i = 0; i < n; i++) {
        fscanf(fp, "%lf %lf\n", X+i, Y+i);
        printf("%-8.4lf %-8.4lf\n", X[i], Y[i]);
    }

    //计算
    double fx = Lagrange(X, Y, n, x);

    printf("\n插值结果: %.8lf\n", fx);

    return 0;
}
