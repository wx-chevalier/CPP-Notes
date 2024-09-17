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
