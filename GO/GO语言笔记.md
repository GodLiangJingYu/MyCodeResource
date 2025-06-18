# GO语言基础

## 一、GO语言结构

例子：

```go
package main

import "fmt"

func main() {
   /* 这是我的Hello World */
   fmt.Println("Hello, World!")
}
```

go组成部分：

- 包声明
- 引入包
- 函数

### 1.1包名

```go
package PackageName
```

通过这种方式定义包名

必须在源文件第一行说明这个文件属于哪个包（类似JAVA）

**package main**表示此文件是一个可独立执行的程序，每个go程序都至少含有一个main包

### 1.2导入包