## 一 拉格朗日插值法

### **1. 程序源代码**

```c++
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
```
### **2. 测试**
**测试用例:**

函数为ln(x)，选取4个插值节点:
```
1 0
2 0.693
3 1.099
4 1.386
```
**测试结果:**

![图片1](L1.png)

实际结果：`ln(1.5) = 0.4054651`

## 二 牛顿插值法
### **1. 程序源代码**
```c++
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
```

### **2. 测试**

**测试用例**

*与拉格朗日插值法相同*

函数为ln(x)，选取4个插值节点
```
1 0
2 0.693
3 1.099
4 1.386
```

**测试结果**

计算`x=1.5`：
![图片2](N1.png)

真实结果：`ln(1.5) = 0.4054651`

## 三 总结

本次实现了Lagrange插值与Newton插值。两者虽同为插值算法，但是计算过程并不相同。
+ Lagrange算法采用了基函数，简单易懂，但计算结果不能重复使用；
+ Newton算法基于差商表(采用一个二维数组保存)，差商表由**递推**得到，故中间结果可以重复使用。

另外，由于Lagrange插值与Newton插值实际上代表了同一个多项式。所以算出来的结果是相同的，程序结果也验证了这一点，都为`0.392875`
