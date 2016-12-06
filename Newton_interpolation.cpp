#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char const *argv[]) {
    //缺少参数，报错
    if(argc < 2)
    {
        cout << "ERROR: Too few arguments" << endl;
        return 0;
    }

    int n;      // 插值点节点个数
    double x;   // 待求的点的x坐标

    FILE *fp = fopen(argv[1], "r");
    //文件无法打开，出错
    if(fp == NULL)
    {
        cout << "ERROR: File not exist" << endl;
    }

    //读入
    fscanf(fp, "%lf\n", &x);，
    fscanf(fp, "%d\n", &n);

    printf("x = %.4lf\n\n", x);

    double X[n];        // 插值节点横坐标
    double Y[n];        // 插值节点纵坐标
    double diff_array[n][n];   // 差商，第i行代表第i阶差商

    printf("共%d个插值点,如下:\n", n);
    //读入并打印插值节点
    for (int i = 0; i < n; i++) {
        fscanf(fp, "%lf %lf\n", X+i, Y+i);
        printf("%8.4lf %8.4lf\n", X[i], Y[i]);
    }

    //计算差商表
    for (int  i = 0; i < n; i++) {
        diff_array[0][i] = Y[i];  // 将第0阶差商即f(x)放入差商数组
    }
    for(int i = 1; i < n; i++){
        for(int j = 0; j < n-i; j++){
            //计算差商，公式为:
            //f[x0, x1 ... xj] = ( f[x1, x2 ... xj] - f[x0, x1 ... xj-1] ) / (xj - x0)
            diff_array[i][j] = ( diff_array[i-1][j+1]-diff_array[i-1][j] ) / (X[j+i] - X[j]);
        }
    }

    //打印差商表
    printf("\n\n差商表如下\n");
    for(int i = 0; i < n; i++){
        printf("%d阶差商: ", i);
        for(int j = 0; j < n-i; j++){
            printf("%-10.5lf", diff_array[i][j]);
        }
        printf("\n");
    }

    //计算(x-x0)(x-x1)...(x - xi)
    double coef[n]; // 第i项为(x-x0)(x-x1)...(x - xi), 特别的第0项为1，便于运算
    coef[0] = 1;
    for(int i = 1; i < n; i++){
        //递推求(x-x0)(x-x1)...(x - xi)
        coef[i] = coef[i-1] *(x - X[i-1]);
    }

    // 利用Newton插值公式计算所求值
    double sum = 0;
    for(int i = 0; i < n; i++){
        // sum += f[x0, x1, x2..., xni](x-x0)(x-x1)...(x-xi-1)
        sum += coef[i] * diff_array[i][0];
    }

    printf("\n插值结果: %.8lf\n", sum);

    return 0;
}
