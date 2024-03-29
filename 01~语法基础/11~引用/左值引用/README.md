# 左值引用

左值引用保留对象的地址，但行为方式在语法上与对象相似。您可以将左值引用视为对象的另一名称。左值引用声明由说明符的可选列表后跟一个引用声明符组成。引用必须初始化且无法更改。地址可转换为给定指针类型的任何对象也可转换为相似的引用类型。例如，地址可转换为类型 `char *` 的任何对象也可转换为类型 `char &`。

不要将引用声明与 [address-of 运算符](https://learn.microsoft.com/zh-cn/cpp/cpp/address-of-operator-amp?view=msvc-170)的用法混淆。`&`identifier 前面有 **`int`** 或 **`char`** 之类的类型时，identifier 将声明为对该类型的引用。`&`identifier 前面没有类型时，用法就是 address-of 运算符的用法。

以下示例通过声明 Person 对象和对该对象的引用演示了引用声明符。由于 rFriend 是对 myFriend 的引用，因此更新任一变量都将更改同一对象。

```cpp
// reference_declarator.cpp
// compile with: /EHsc
// Demonstrates the reference declarator.
#include <iostream>
using namespace std;

struct Person
{
    char* Name;
    short Age;
};

int main()
{
   // Declare a Person object.
   Person myFriend;

   // Declare a reference to the Person object.
   Person& rFriend = myFriend;

   // Set the fields of the Person object.
   // Updating either variable changes the same object.
   myFriend.Name = "Bill";
   rFriend.Age = 40;

   // Print the fields of the Person object to the console.
   cout << rFriend.Name << " is " << myFriend.Age << endl;
}
```

# 案例

## swap 案例

看看下面的 C 语言实现的交换的错误代码：

```cpp
#include <stdio.h>
#include <stdlib.h>

void swap(int _left, int _right) {
	int temp = _left;
	_left = _right;
	_right = temp;
}

int main() {
	int a = 10;
	int b = 20;
	swap(a, b);
	printf("%d\r\n%d", a, b);
	return 0;
}

/**
10
20
*/
```

在将代码修改为指针方式后：

```cpp
#include <stdio.h>
#include <stdlib.h>

void swap(int* _left, int *_right) {
	int temp = *_left;
	*_left = *_right;
	*_right = temp;
}

int main() {
	int a = 10;
	int b = 20;
	swap(&a, &b);
	printf("%d\r\n%d", a, b);
	return 0;
}
```

这个结果是我们需要的，两个变量的值成功交换了，第一段代码错误的原因是什么？在调用 swap 函数时，a，b 被拷贝了一份，在函数体里面，对 a，b 的操作实际上是对 a，b 副本的操作。第二种方法虽然达到了目的，但是总有麻烦和不美观。我们可以再修改为引用传参：

```cpp
#include <iostream>

void swap(int& _left, int& _right) {
	int temp = _left;
	_left = _right;
	_right = temp;
}

int main() {
	int a = 10;
	int b = 20;
	swap(a, b);
	std::cout << a << std::endl << b << std::endl;
	return 0;
}
```

可以看到，swap 函数只被改动了很小的一部分，但是结果有所不同，因为当 a，b 传入 swap 函数时，a，b 没有被拷贝，在函数体内对 a，b 的操作就是对它们本身进行操作。下面提供了一种 swap 函数的版本：

```cpp
void swap(int& _left, int& _right) {
    int temp = std::move(_left);
	_left = std::move(_right);
	_right = std::move(temp);
}
```

这是标准库实现 swap 的方法，通过右值引用减少不必要的内存大小

## 避免多次构造案例

```cpp
#include <iostream>

class Student {
public:
	Student(int _id = 0)
		: id_(_id) {
		std::cout << "Student()" << std::endl;
	}
	Student(Student& other)
		: id_(other.id_) {
		std::cout << "Student(Student&)" << std::endl;
	}
	Student(Student&& other)
		: id_(other.id_) {
		std::cout << "Student(Student&&)" << std::endl;
	}
	~Student() {
		std::cout << "~Student()" << std::endl;
	}
	int id() {
		return id_;
	}
private:
	int id_;
};


void printId(Student s) {
	std::cout << s.id() << std::endl;
}

int main() {
	Student s;
	printId(s);
	return 0;
}

/**
Student()
Student(Student&)
0
~Student()
~Student()
**/
```

在 main 函数中只有一个 Student 对象，为什么被析构了两次？再看看输出，有一个 Student(Student&)，看来 s 是被拷贝过一份的，但是我们并不需要这种拷贝，拷贝了有什么效果吗，显然在这个程序中没有，所以我们应该使用引用传递参数而不是值传递参数

```cpp
void printId(Student& s) {
	std::cout << s.id() << std::endl;
}
```
