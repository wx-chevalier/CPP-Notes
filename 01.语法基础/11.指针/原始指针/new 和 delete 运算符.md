# new 和 delete 运算符

C++ 支持使用 new 和 delete 运算符动态分配和解除分配对象。这些运算符为来自称为“自由存储”（也称为“堆”）的池中的对象分配内存。new 运算符调用特殊函数 operator new，delete 运算符调用特殊函数 operator delete。

# new 运算符

编译器将如下语句转换为对函数 operator new 的调用：

```cpp
char *pch = new char[BUFFER_SIZE];
```

如果请求的存储空间为零字节，operator new 将返回指向不同对象的指针。也就是说，重复调用 operator new 会返回不同的指针。如果分配请求的内存不足，operator new 会引发 std::bad_alloc 异常。或者，如果使用了 placement 形式 new(std::nothrow)，或者链接在非引发的 operator new 支持中，它将返回 nullptr。

## operator new 函数的范围

operator new 的第一个自变量必须为 size_t 类型，且返回类型始终为 `void*`。在使用 new 运算符分配内置类型的对象、不包含用户定义的 operator new 函数的类类型的对象和任何类型的数组时，将调用全局 operator new 函数。在使用 new 运算符分配类类型的对象时（其中定义了 operator new），将调用该类的 operator new。

为类定义的 operator new 函数是静态成员函数（不能是虚函数），该函数隐藏此类类型的对象的全局 operator new 函数。考虑 new 用于分配内存并将内存设为给定值的情况：

```cpp
#include <malloc.h>
#include <memory.h>

class Blanks
{
public:
    Blanks(){}
    void *operator new( size_t stAllocateBlock, char chInit );
};
void *Blanks::operator new( size_t stAllocateBlock, char chInit )
{
    void *pvTemp = malloc( stAllocateBlock );
    if( pvTemp != 0 )
        memset( pvTemp, chInit, stAllocateBlock );
    return pvTemp;
}
// For discrete objects of type Blanks, the global operator new function
// is hidden. Therefore, the following code allocates an object of type
// Blanks and initializes it to 0xa5
int main()
{
   Blanks *a5 = new(0xa5) Blanks;
   return a5 != 0;
}
```

用括号包含的提供给 new 的自变量将作为 chInit 自变量传递给 Blanks::operator new。但是，全局 operator new 函数将被隐藏，从而导致以下代码生成错误：

```cpp
Blanks *SomeBlanks = new Blanks;
```

编译器在类声明中支持成员数组 new 和 delete 运算符。例如：

```cpp
class MyClass
{
public:
   void * operator new[] (size_t)
   {
      return 0;
   }
   void   operator delete[] (void*)
   {
   }
};

int main()
{
   MyClass *pMyClass = new MyClass[5];
   delete [] pMyClass;
}
```

### 分配失败行为

C++ 标准库中的 **`new`** 函数支持自 C++98 以来在 C++ 标准中指定的行为。如果分配请求的内存不足，**`operator new`** 会引发 [`std::bad_alloc`](https://learn.microsoft.com/zh-cn/cpp/standard-library/bad-alloc-class?view=msvc-170) 异常。

较旧的 C++ 代码会为失败的分配返回 null 指针。如果你的代码需要非引发版本的 **`new`**，请将程序链接到 _`nothrownew.obj`_。_`nothrownew.obj`_ 文件将全局 **`operator new`** 替换为分配失败时返回 **`nullptr`** 的版本。**`operator new`** 不再引发 `std::bad_alloc`。有关 _`nothrownew.obj`_ 和其他链接器选项文件的详细信息，请参阅[链接选项](https://learn.microsoft.com/zh-cn/cpp/c-runtime-library/link-options?view=msvc-170)。

不能将检查全局 **`operator new`** 异常的代码与检查同一个应用程序中的 null 指针的代码混合使用。但是，仍可以创建不同行为的类本地 **`operator new`**。这种可能性意味着编译器在默认情况下必须以防御方式行事，并在 **`new`** 调用中包含对 null 指针返回的检查。有关优化这些编译器检查的方法的详细信息，请参阅 [`/Zc:throwingnew`](https://learn.microsoft.com/zh-cn/cpp/build/reference/zc-throwingnew-assume-operator-new-throws?view=msvc-170)。

### 处理内存不足

从 **`new`** 表达式中测试失败分配的方式取决于是使用标准异常机制还是使用 **`nullptr`** 返回。标准 C++ 要求分配器引发 `std::bad_alloc` 或派生自 `std::bad_alloc` 的类。可以处理此类异常，如以下示例所示：

```cpp
#include <iostream>
#include <new>
using namespace std;
#define BIG_NUMBER 10000000000LL
int main() {
   try {
      int *pI = new int[BIG_NUMBER];
   }
   catch (bad_alloc& ex) {
      cout << "Caught bad_alloc: " << ex.what() << endl;
      return -1;
   }
}
```

使用 `nothrow` 格式的 **`new`** 时，可以测试分配失败，如以下示例所示：

```cpp
#include <iostream>
#include <new>
using namespace std;
#define BIG_NUMBER 10000000000LL
int main() {
   int *pI = new(nothrow) int[BIG_NUMBER];
   if ( pI == nullptr ) {
      cout << "Insufficient memory" << endl;
      return -1;
   }
}
```

使用 _`nothrownew.obj`_ 文件替换全局 **`operator new`** 时，可以测试失败的内存分配，如下所示：

```cpp
#include <iostream>
#include <new>
using namespace std;
#define BIG_NUMBER 10000000000LL
int main() {
   int *pI = new int[BIG_NUMBER];
   if ( !pI ) {
      cout << "Insufficient memory" << endl;
      return -1;
   }
}
```

可以为失败的内存分配请求提供处理程序。可以编写自定义恢复例程来处理此类失败。例如，它可以释放一些保留内存，然后允许分配再次运行。有关详细信息，请参阅 [`_set_new_handler`](https://learn.microsoft.com/zh-cn/cpp/c-runtime-library/reference/set-new-handler?view=msvc-170)。

## `delete` 运算符

可使用 **`delete`** 运算符释放使用 **`new`** 运算符动态分配的内存。delete 运算符调用 **`operator delete`** 函数，该函数将内存释放回可用池。使用 **`delete`** 运算符也会导致调用类析构函数（如果存在）。

存在全局和类范围的 **`operator delete`** 函数。只能为给定类定义一个 **`operator delete`** 函数；如果定义了该函数，它会隐藏全局 **`operator delete`** 函数。始终为所有类型的数组调用全局 **`operator delete`** 函数。

全局 **`operator delete`** 函数。全局 **`operator delete`** 函数和类成员 **`operator delete`** 函数存在两种形式：

```cpp
void operator delete( void * );
void operator delete( void *, size_t );
```

给定类中只存在前面两种形式中的一个。第一种形式采用 **`void *`** 类型的单个自变量，其中包含指向要解除分配的对象的指针。第二个形式（大小经过调整的解除分配）采用两个自变量，第一个是指向要解除分配的内存块的指针，第二个是解除分配的字节数。这两种形式的返回类型为 **`void`**（**`operator delete`** 无法返回值）。

第二种形式的意图是加快搜索要删除的对象的正确大小类别。此信息通常不会存储在分配本身附近，并且可能未缓存。当基类中的 **`operator delete`** 函数用于删除派生类的对象时，第二个形式非常有用。**`operator delete`** 函数是静态的，因此它不能是虚拟的，**`operator delete`** 函数服从访问控制。

以下示例显示了旨在记录内存的分配和解除分配的用户定义的 **`operator new`** 和 **`operator delete`** 函数：

```cpp
#include <iostream>
using namespace std;

int fLogMemory = 0;      // Perform logging (0=no; nonzero=yes)?
int cBlocksAllocated = 0;  // Count of blocks allocated.

// User-defined operator new.
void *operator new( size_t stAllocateBlock ) {
   static int fInOpNew = 0;   // Guard flag.

   if ( fLogMemory && !fInOpNew ) {
      fInOpNew = 1;
      clog << "Memory block " << ++cBlocksAllocated
          << " allocated for " << stAllocateBlock
          << " bytes\n";
      fInOpNew = 0;
   }
   return malloc( stAllocateBlock );
}

// User-defined operator delete.
void operator delete( void *pvMem ) {
   static int fInOpDelete = 0;   // Guard flag.
   if ( fLogMemory && !fInOpDelete ) {
      fInOpDelete = 1;
      clog << "Memory block " << cBlocksAllocated--
          << " deallocated\n";
      fInOpDelete = 0;
   }

   free( pvMem );
}

int main( int argc, char *argv[] ) {
   fLogMemory = 1;   // Turn logging on
   if( argc > 1 )
      for( int i = 0; i < atoi( argv[1] ); ++i ) {
         char *pMem = new char[10];
         delete[] pMem;
      }
   fLogMemory = 0;  // Turn logging off.
   return cBlocksAllocated;
}
```

前面的代码可用于检测“内存溢出”，即在自由储存中分配但从未释放过的内存。若要检测泄漏，应重新定义全局 **`new`** 和 **`delete`** 运算符以计算内存的分配和解除分配。

编译器在类声明中支持成员数组 **`new`** 和 **`delete`** 运算符。例如：

```cpp
// spec1_the_operator_delete_function2.cpp
// compile with: /c
class X  {
public:
   void * operator new[] (size_t) {
      return 0;
   }
   void operator delete[] (void*) {}
};

void f() {
   X *pX = new X[5];
   delete [] pX;
}
```
