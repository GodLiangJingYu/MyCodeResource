# C++基础

## 一、基本语法与程序结构

实例代码：

```c++
#include <iostream>
using namespace std:

int main(){
    cout<<"Hello World";
    return 0;
}
```

### 1.命名空间

using namespace std表示使用了命名空间为std的命名空间

命名空间是一个声明性的区域，给变量、函数、类等提供了一个作用域，相当于给所有代码提供了一个姓氏

通过不同的命名空间，相同函数名可以实现不同的操作（类似多态）

using namespace std为标准命名空间

#### 1.1自定义命名空间

```c++
namespace MyCompany {
    class Logger { /* ... */ };
    void helperFunction() { /* ... */ }
}

// 使用
MyCompany::Logger log;
```

### 2.C++执行过程

#### 2.1  预处理

- 插入头文件
- 展开宏定义
- 条件编译
- 删除注释

生成.i文件

#### 2.2  编译

输出汇编代码 .s 文件

#### 2.3  汇编

将汇编指令转换为二进制指令 .o 文件

#### 2.4  链接

将多个目标文件和库文件输出为可执行文件

- 静态链接
  - 将库代码直接复制到可执行文件
  - 不依赖外部库但文件较大
- 动态链接
  - 程序运行时才加载共享库
  - 节省内存空间但需要系统拥有相应库

### 3.C++独有的类型

1. auto  自动推断类型

   例如： auto x =10;此处的auto为int

2. decltype  获取表达式类型

   decltype(x) y;

   表示y的类型将与x相同

3. nullptr  空指针**常量**

4. std::tuple  可以存储不同常量的元组

   例如：std::tuple<int ,float ,char> x(1,1.2,a);

5. **引用**

   表示已经存在的变量的别名，引用无法被转移给其他变量

   使用方法：

   ​	类型& 引用名 = 被引用名

   ​	例如：

   ```c++
   int a = 5;
   int& b=a;
   b=100;//这里就是给a重新赋了100的值
   ```

   1. 引用传参

      ```c++
      //引用
      void test(int& num){
          num++;
      }
      
      //指针
      void test2(int* num){
          *num++;
      }
      
      int main(){
          int a=1;
          test(a);
          cout<<a<<endl;//a=2
          test2(&a);
          cout<<a<<endl;//a=3
      }
      ```

   2. 引用与指针的区别

      |            |     引用     |     指针     |
      | :--------: | :----------: | :----------: |
      |   初始化   |     需要     | 可以不初始化 |
      | 是否能为空 |      ×       | √（nullptr） |
      |  内存占用  |     占用     |    不占用    |
      |  地址操作  | 不能获取地址 | 可以获取地址 |
      |  数组访问  | 不能表示数组 | 可以表示数组 |
      |    多级    |    不支持    |     支持     |

6. 别名

   1. typedef int Myint；
   2. using Myint = int；

7. 动态数组vector

   内存中连续存储，访问速度快

   vector<int> v;

   vector<int> v={1,2,33};

   添加元素：

   ​	vectorName.push_back(7);

8. 固定大小数组array

   array<int,4> v ={1,2,3,4};

9. 存储两个数pair

   pair<int,float> p(1,2.0);

   使用：p.first,p.second

10. 键值对map

    map<int,char> m;

    使用：m[1] = “one”;m[2] = “two”;

### 4.生成随机数

j=rand();

### 

