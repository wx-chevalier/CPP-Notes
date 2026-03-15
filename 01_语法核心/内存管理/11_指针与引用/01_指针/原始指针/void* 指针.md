# `void*` 指针

指向 **`void`** 的指针仅指向原始内存位置。有时需要使用 `void*` 指针，例如在 C++ 代码和 C 函数之间传递时。

将类型化指针强制转换为 `void` 指针时，内存位置的内容保持不变。但是，类型信息会丢失，因此无法执行递增或递减操作。例如，可以将内存位置从 `MyClass*` 强制转换为 `void*`，然后再转换回 `MyClass*`。此类操作很容易出错，需要非常小心避免 (avoid) 出错。新式 C++ 几乎在所有情况下都不鼓励使用 `void` 指针。

```cpp
//func.c
void func(void* data, int length)
{
    char* c = (char*)(data);

    // fill in the buffer with data
    for (int i = 0; i < length; ++i)
    {
        *c = 0x41;
        ++c;
    }
}

// main.cpp
#include <iostream>

extern "C"
{
    void func(void* data, int length);
}

class MyClass
{
public:
    int num;
    std::string name;
    void print() { std::cout << name << ":" << num << std::endl; }
};

int main()
{
    MyClass* mc = new MyClass{10, "Marian"};
    void* p = static_cast<void*>(mc);
    MyClass* mc2 = static_cast<MyClass*>(p);
    std::cout << mc2->name << std::endl; // "Marian"
    delete(mc);

    // use operator new to allocate untyped memory block
    void* pvoid = operator new(1000);
    char* pchar = static_cast<char*>(pvoid);
    for(char* c = pchar; c < pchar + 1000; ++c)
    {
        *c = 0x00;
    }
    func(pvoid, 1000);
    char ch = static_cast<char*>(pvoid)[0];
    std::cout << ch << std::endl; // 'A'
    operator delete(pvoid);
}
```
