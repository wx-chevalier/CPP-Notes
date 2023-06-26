# 引用

与指针相似的是，引用将存储位于内存中其他位置的对象的地址。 与指针不同的是，初始化之后的引用无法引用不同的对象或设置为 null。 有两种引用：引用命名变量的 lvalue 引用和引用临时对象的 rvalue 引用。 & 运算符表示 lvalue 引用，&& 运算符表示 rvalue 引用或通用引用（rvalue 或 lvalue），具体取决于上下文。

可以通过以下语法声明引用：

```sh
[storage-class-specifiers] [cv-qualifiers] type-specifiers [ms-modifier] declarator [=expression];
```

可以使用指定引用的任何有效声明符。除非引用是对函数或数组类型的引用，否则应用以下简化语法：

```sh
[storage-class-specifiers] [cv-qualifiers] type-specifiers [& or &&] [cv-qualifiers] identifier [=expression];
```

指向数组和函数的指针的更复杂的声明符形式也适用于对数组和函数的引用。多个声明符和初始值设定项可能出现在一个声明说明符后面的逗号分隔的列表中。例如：

```cpp
int &i;
int &i, &j;

// 引用、指针和对象可以一起声明：
int &ref, *ptr, k;

// references.cpp
#include <stdio.h>
struct S {
   short i;
};

int main() {
   S  s;   // Declare the object.
   S& SRef = s;   // Declare and initialize the reference.
   s.i = 3;

   printf_s("%d\n", s.i);
   printf_s("%d\n", SRef.i);

   SRef.i = 4;
   printf_s("%d\n", s.i);
   printf_s("%d\n", SRef.i);
}
```
