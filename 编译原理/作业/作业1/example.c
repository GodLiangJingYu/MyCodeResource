#include <stdio.h>

// 宏定义 - 预处理阶段会展开
#define MAX_SIZE 100
#define SQUARE(x) ((x) * (x))

// 全局变量
int globalVar = 42;

// 函数声明
int add(int a, int b);
void printMessage(const char* msg);

// 主函数
int main() {
    int x = 10;
    int y = 20;
    int result;
    
    // 使用宏
    int arr[MAX_SIZE];
    int squared = SQUARE(5);
    
    // 函数调用
    result = add(x, y);
    
    // 条件语句
    if (result > 25) {
        printMessage("Result is greater than 25");
    } else {
        printMessage("Result is 25 or less");
    }
    
    // 循环语句
    for (int i = 0; i < 5; i++) {
        arr[i] = i * 2;
    }
    
    printf("x = %d, y = %d, result = %d\n", x, y, result);
    printf("squared = %d, globalVar = %d\n", squared, globalVar);
    
    return 0;
}

// 函数定义
int add(int a, int b) {
    return a + b;
}

void printMessage(const char* msg) {
    printf("%s\n", msg);
}
