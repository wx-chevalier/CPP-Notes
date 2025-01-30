# 成员访问控制

通过访问控制，可以将类的 public 接口与 private 实现详细信息和仅供派生类使用的 protected 成员分离开来。访问说明符应用于在它之后声明的所有成员，直到遇到下一个访问说明符。

```cpp
class Point
{
public:
    Point( int, int ) // Declare public constructor.;
    Point();// Declare public default constructor.
    int &x( int ); // Declare public accessor.
    int &y( int ); // Declare public accessor.

private:                 // Declare private state variables.
    int _x;
    int _y;

protected:      // Declare protected function for derived classes only.
    Point ToWindowCoords();
};
```

默认访问是类中的 private，以及结构或联合中的 public。类中的访问说明符可按任何顺序使用任意次数。类类型的对象的存储分配是依赖于实现的。但是，编译器必须保证在访问说明符之间将成员分配到更高的连续内存地址。

| 访问类型                                                                             | 含义                                                                                                        |
| :----------------------------------------------------------------------------------- | :---------------------------------------------------------------------------------------------------------- |
| [`private`](https://learn.microsoft.com/zh-cn/cpp/cpp/private-cpp?view=msvc-170)     | 声明为 **`private`** 的类成员只能由类的成员函数和友元（类或函数）使用。                                     |
| [`protected`](https://learn.microsoft.com/zh-cn/cpp/cpp/protected-cpp?view=msvc-170) | 声明为 **`protected`** 的类成员可由类的成员函数和友元（类或函数）使用。此外，它们还可由派生自该类的类使用。 |
| [`public`](https://learn.microsoft.com/zh-cn/cpp/cpp/public-cpp?view=msvc-170)       | 声明为 **`public`** 的类成员可由任意函数使用。                                                              |

访问控制有助于阻止通过不适当的方式使用对象。在执行显式类型转换（强制转换）时，此保护将丢失。

# 派生类中的访问控制

两个因素控制基类的哪些成员可在派生类中访问；这些相同的因素控制对派生类中的继承成员的访问：

- 派生类是否使用 **`public`** 访问说明符声明基类。
- 基类中对成员的访问权限如何。

下表显示了这些因素之间的交互以及如何确定基类成员访问。

| **`private`**                    | **`protected`**                                               | **`public`**                                                  |
| :------------------------------- | :------------------------------------------------------------ | :------------------------------------------------------------ |
| 始终无法通过任何派生访问进行访问 | 如果使用 **`private`** 派生，则在派生类中为 **`private`**     | 如果使用 **`private`** 派生，则在派生类中为 **`private`**     |
|                                  | 如果使用 **`protected`** 派生，则在派生类中为 **`protected`** | 如果使用 **`protected`** 派生，则在派生类中为 **`protected`** |
|                                  | 如果使用 **`public`** 派生，则在派生类中为 **`protected`**    | 如果使用 **`public`** 派生，则在派生类中为 **`public`**       |

以下示例演示了访问派生：

```cpp
// access_specifiers_for_base_classes.cpp
class BaseClass
{
public:
    int PublicFunc(); // Declare a public member.
protected:
    int ProtectedFunc(); // Declare a protected member.
private:
    int PrivateFunc(); // Declare a private member.
};

// Declare two classes derived from BaseClass.
class DerivedClass1 : public BaseClass
{
    void foo()
    {
        PublicFunc();
        ProtectedFunc();
        PrivateFunc(); // function is inaccessible
    }
};

class DerivedClass2 : private BaseClass
{
    void foo()
    {
        PublicFunc();
        ProtectedFunc();
        PrivateFunc(); // function is inaccessible
    }
};

int main()
{
    DerivedClass1 derived_class1;
    DerivedClass2 derived_class2;
    derived_class1.PublicFunc();
    derived_class2.PublicFunc(); // function is inaccessible
}
```

在 `DerivedClass1` 中，成员函数 `PublicFunc` 是 **`public`** 成员，`ProtectedFunc` 是 **`protected`** 成员，因为 `BaseClass` 是 **`public`** 基类。`PrivateFunc` 对于 `BaseClass` 是 **`private`** 的，因此任何派生类都无法访问它。

在 `DerivedClass2` 中，函数 `PublicFunc` 和 `ProtectedFunc` 被视为 **`private`** 成员，因为 `BaseClass` 是 **`private`** 基类。同样，`PrivateFunc` 对于 `BaseClass` 是 **`private`** 的，因此任何派生类都无法访问它。

您可以声明派生类而不使用基类访问说明符。在这种情况下，如果派生类声明使用 **`class`** 关键字，则将派生视为 **`private`**。如果派生类声明使用 **`struct`** 关键字，则将派生视为 **`public`**。例如，以下代码：

```cpp
class Derived : Base
...
```

等效于：

```cpp
class Derived : private Base
...
```

同样，以下代码：

```cpp
struct Derived : Base
...
```

等效于：

```cpp
struct Derived : public Base
...
```

声明为拥有 **`private`** 访问权限的成员对函数或派生类是不可访问的，除非使用基类中的 **`friend`** 声明来声明这些函数或类。**`union`** 类型不能有基类。

# 访问控制和静态成员

在将基类指定为 private 时，它只影响非静态成员。在派生类中，公共静态成员仍是可访问的。但是，使用指针、引用或对象访问基类的成员需要转换，此时将再次应用访问控制。请考虑以下示例：

```cpp
// access_control.cpp
class Base
{
public:
    int Print();             // Nonstatic member.
    static int CountOf();    // Static member.
};

// Derived1 declares Base as a private base class.
class Derived1 : private Base
{
};

// Derived2 declares Derived1 as a public base class.
class Derived2 : public Derived1
{
    int ShowCount();    // Nonstatic member.
};

// Define ShowCount function for Derived2.
int Derived2::ShowCount()
{
    // Call static member function CountOf explicitly.
    int cCount = ::Base::CountOf();     // OK.

    // Call static member function CountOf using pointer.
    cCount = this->CountOf();  // C2247: 'Base::CountOf'
                               // not accessible because
                               // 'Derived1' uses 'private'
                               // to inherit from 'Base'
    return cCount;
}
```

在前面的代码中，访问控制禁止从指向 `Derived2` 的指针转换为指向 `Base` 的指针。**`this`** 指针是 `Derived2 *` 类型的隐式表示形式。若要选择 `CountOf` 函数，必须将 **`this`** 转换为 `Base *` 类型。不允许执行此类转换，因为 `Base` 是 `Derived2` 的 private 间接基类。到 private 基类类型的转换只适用于指向立即派生类的指针。这就是为什么可以将 `Derived1 *` 类型的指针转换为 `Base *` 类型。

显式调用 `CountOf` 函数，而不使用指针、引用或对象来选择它，意味着没有转换。这就是允许调用的原因。派生类 `T` 的成员和友元可以将指向 `T` 的指针转换为指向 `T` 的 private 直接基类的指针。

# 对虚函数的访问

适用于 [`virtual`](https://learn.microsoft.com/zh-cn/cpp/cpp/virtual-cpp?view=msvc-170) 函数的访问控制是由用于进行函数调用的类型决定的。重写函数的声明不会影响给定类型的访问控制。例如：

```cpp
// access_to_virtual_functions.cpp
class VFuncBase
{
public:
    virtual int GetState() { return _state; }
protected:
    int _state;
};

class VFuncDerived : public VFuncBase
{
private:
    int GetState() { return _state; }
};

int main()
{
   VFuncDerived vfd;             // Object of derived type.
   VFuncBase *pvfb = &vfd;       // Pointer to base type.
   VFuncDerived *pvfd = &vfd;    // Pointer to derived type.
   int State;

   State = pvfb->GetState();     // GetState is public.
   State = pvfd->GetState();     // C2248 error expected; GetState is private;
}
```

在前面的示例中，使用指向 `VFuncBase` 类型的指针调用虚函数 `GetState` 将调用 `VFuncDerived::GetState`，并且会将 `GetState` 视为 **`public`**。但是，使用指向 `VFuncDerived` 类型的指针调用 `GetState` 是一种访问控制冲突，因为 `GetState` 在 `VFuncDerived` 类中被声明为 **`private`**。

# 具有多重继承的访问控制

在涉及虚拟基类的多重继承方格中，可通过多个路径到达给定的名称。由于可沿着这些不同的路径应用不同的访问控制，因此该编译器选择允许大多数访问的路径。请参阅下图：

![显示沿继承图路径进行访问的关系图。](https://learn.microsoft.com/zh-cn/cpp/cpp/media/vc38v91.gif?view=msvc-170)

此图显示了以下继承层次结构：VBase 类是基类。使用虚拟 private VBase 从 VBase 继承的 LeftPath 类。类 RightPath 也继承自 VBase，但使用的是虚拟 public VBase。最后，派生类使用 public LeftPath、RightPath 从类 LeftPath 和类 RightPath public 继承。

在该图中，通过类 `VBase` 始终到达类 `RightPath` 中声明的名称。右路径更易于访问，因为 `RightPath` 将 `VBase` 声明为 **`public`** 基类，而 `LeftPath` 将 `VBase` 声明为 **`private`**。
