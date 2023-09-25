# 原始指针

指针是一种变量。它将对象的地址存储在内存中，并用于访问该对象。原始指针是指其生存期不受封装对象控制的指针。可以为原始指针分配另一个非指针变量的地址，也可以为其分配 nullptr 值。未分配值的指针包含随机数据。还可以取消引用指针以检索其指向的对象的值。成员访问运算符提供对对象成员的访问权限。

```cpp
int* p = nullptr; // declare pointer and initialize it
                      // so that it doesn't store a random address
int i = 5;
p = &i; // assign pointer to address of object
int j = *p; // dereference p to retrieve the value at its address
```

指针可以指向类型化对象或指向 void。当程序在内存中的堆上分配对象时，它会以指针的形式接收该对象的地址。此类指针称为“拥有指针”。当不再需要堆分配的对象时，必须使用拥有指针（或其副本）显式释放该对象。未能释放内存会导致内存泄漏，并使该内存位置无法供计算机上的任何其他程序使用。必须使用 delete（或 delete[]）释放使用 new 分配的内存。

```cpp
MyClass* mc = new MyClass(); // allocate object on the heap
mc->print(); // access class member
delete mc; // delete object (please don't forget!)
```

指针（如果未声明为 const）可以递增或递减，以指向内存中的另一个位置。此操作称为“指针算术”。它用于在 C 风格编程中循环访问数组或其他数据结构中的元素。const 指针不能指向不同的内存位置，从这个意义上说，它类似于引用。

```cpp
// declare a C-style string. Compiler adds terminating '\0'.
const char* str = "Hello world";

const int c = 1;
const int* pconst = &c; // declare a non-const pointer to const int
const int c2 = 2;
pconst = &c2;  // OK pconst itself isn't const
const int* const pconst2 = &c;
// pconst2 = &c2; // Error! pconst2 is const.
```

在 64 位操作系统上，指针的大小为 64 位。系统的指针大小决定了它可以拥有的可寻址内存量。指针的所有副本都指向同一内存位置。指针（以及引用）在 C++ 中广泛用于向/从函数传递较大的对象。复制对象的地址通常比复制整个对象更加高效。定义函数时，除非希望函数修改对象，否则请将指针参数指定为 const。通常，const 引用是将对象传递给函数的首选方式，除非对象的值可能为 nullptr。

# 初始化和成员访问

下面的示例展示了如何声明、初始化和使用原始指针。它使用 new 初始化，以指向堆上分配的对象，必须显式删除 (delete) 该对象。该示例还展示了与原始指针相关的一些危险。

```cpp
#include <iostream>
#include <string>

class MyClass
{
public:
    int num;
    std::string name;
    void print() { std::cout << name << ":" << num << std::endl; }
};

// Accepts a MyClass pointer
void func_A(MyClass* mc)
{
    // Modify the object that mc points to.
    // All copies of the pointer will point to
    // the same modified object.
    mc->num = 3;
}

// Accepts a MyClass object
void func_B(MyClass mc)
{
    // mc here is a regular object, not a pointer.
    // Use the "." operator to access members.
    // This statement modifies only the local copy of mc.
    mc.num = 21;
    std::cout << "Local copy of mc:";
    mc.print(); // "Erika, 21"
}


int main()
{
    // Use the * operator to declare a pointer type
    // Use new to allocate and initialize memory
    MyClass* pmc = new MyClass{ 108, "Nick" };

    // Prints the memory address. Usually not what you want.
    std:: cout << pmc << std::endl;

    // Copy the pointed-to object by dereferencing the pointer
    // to access the contents of the memory location.
    // mc is a separate object, allocated here on the stack
    MyClass mc = *pmc;

    // Declare a pointer that points to mc using the addressof operator
    MyClass* pcopy = &mc;

    // Use the -> operator to access the object's public members
    pmc->print(); // "Nick, 108"

    // Copy the pointer. Now pmc and pmc2 point to same object!
    MyClass* pmc2 = pmc;

    // Use copied pointer to modify the original object
    pmc2->name = "Erika";
    pmc->print(); // "Erika, 108"
    pmc2->print(); // "Erika, 108"

    // Pass the pointer to a function.
    func_A(pmc);
    pmc->print(); // "Erika, 3"
    pmc2->print(); // "Erika, 3"

    // Dereference the pointer and pass a copy
    // of the pointed-to object to a function
    func_B(*pmc);
    pmc->print(); // "Erika, 3" (original not modified by function)

    delete(pmc); // don't forget to give memory back to operating system!
   // delete(pmc2); //crash! memory location was already deleted
}
```

# 指针算术和数组

指针和数组密切相关。当数组按值传递给函数时，它将作为指向第一个元素的指针传递。以下示例演示了指针和数组的以下重要属性：

- **`sizeof`** 运算符返回数组的总大小（以字节为单位）
- 若要确定元素数目，请将总字节数除以一个元素的大小
- 当数组被传递给函数时，它会衰减为指针类型
- 当 **`sizeof`** 运算符应用于指针时，它将返回指针大小，例如，x86 上为 4 个字节，x64 上为 8 个字节

```cpp
#include <iostream>

void func(int arr[], int length)
{
    // returns pointer size. not useful here.
    size_t test = sizeof(arr);

    for(int i = 0; i < length; ++i)
    {
        std::cout << arr[i] << " ";
    }
}

int main()
{

    int i[5]{ 1,2,3,4,5 };
    // sizeof(i) = total bytes
    int j = sizeof(i) / sizeof(i[0]);
    func(i,j);
}
```

某些算术运算可用于非 `const` 指针，以使其指向另一个内存位置。指针使用 **`++`**、**`+=`**、**`-=`** 和 **`--`** 运算符递增和递减。此方法可用于数组，在非类型化数据的缓冲区中尤其有用。`void*` 按一个 **`char`** 的大小（1 个字节）递增。类型化指针按其指向的类型的大小递增。

以下示例演示如何使用指针算术访问 Windows 上位图中的单个像素。请注意 new 和 delete 以及取消引用运算符的使用。

```cpp
#include <Windows.h>
#include <fstream>

using namespace std;

int main()
{

    BITMAPINFOHEADER header;
    header.biHeight = 100; // Multiple of 4 for simplicity.
    header.biWidth = 100;
    header.biBitCount = 24;
    header.biPlanes = 1;
    header.biCompression = BI_RGB;
    header.biSize = sizeof(BITMAPINFOHEADER);

    constexpr int bufferSize = 30000;
    unsigned char* buffer = new unsigned char[bufferSize];

    BITMAPFILEHEADER bf;
    bf.bfType = 0x4D42;
    bf.bfSize = header.biSize + 14 + bufferSize;
    bf.bfReserved1 = 0;
    bf.bfReserved2 = 0;
    bf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); //54

    // Create a gray square with a 2-pixel wide outline.
    unsigned char* begin = &buffer[0];
    unsigned char* end = &buffer[0] + bufferSize;
    unsigned char* p = begin;
    constexpr int pixelWidth = 3;
    constexpr int borderWidth = 2;

    while (p < end)
    {
            // Is top or bottom edge?
        if ((p < begin + header.biWidth * pixelWidth * borderWidth)
            || (p > end - header.biWidth * pixelWidth * borderWidth)
            // Is left or right edge?
            || (p - begin) % (header.biWidth * pixelWidth) < (borderWidth * pixelWidth)
            || (p - begin) % (header.biWidth * pixelWidth) > ((header.biWidth - borderWidth) * pixelWidth))
        {
            *p = 0x0; // Black
        }
        else
        {
            *p = 0xC3; // Gray
        }
        p++; // Increment one byte sizeof(unsigned char).
    }

    ofstream wf(R"(box.bmp)", ios::out | ios::binary);

    wf.write(reinterpret_cast<char*>(&bf), sizeof(bf));
    wf.write(reinterpret_cast<char*>(&header), sizeof(header));
    wf.write(reinterpret_cast<char*>(begin), bufferSize);

    delete[] buffer; // Return memory to the OS.
    wf.close();
}
```

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

# 指向函数的指针

在 C 风格的编程中，函数指针主要用于将函数传递给其他函数。此方法使调用方能够在不修改函数的情况下自定义函数的行为。在新式 C++ 中，lambda 表达式提供了相同的功能，并且提供了更高的类型安全性和其他优势。函数指针声明指定指向函数必须具有的签名：

```cpp
// Declare pointer to any function that...

// ...accepts a string and returns a string
string (*g)(string a);

// has no return value and no parameters
void (*x)();

// ...returns an int and takes three parameters
// of the specified types
int (*i)(int i, string s, double d);
```

以下示例展示了函数 combine，该函数将接受 std::string 并返回 std::string 的任何函数作为参数。根据传递给 combine 的函数，它将在前面或后面添加字符串。

```cpp
#include <iostream>
#include <string>

using namespace std;

string base {"hello world"};

string append(string s)
{
    return base.append(" ").append(s);
}

string prepend(string s)
{
    return s.append(" ").append(base);
}

string combine(string s, string(*g)(string a))
{
    return (*g)(s);
}

int main()
{
    cout << combine("from MSVC", append) << "\n";
    cout << combine("Good morning and", prepend) << "\n";
}
```
