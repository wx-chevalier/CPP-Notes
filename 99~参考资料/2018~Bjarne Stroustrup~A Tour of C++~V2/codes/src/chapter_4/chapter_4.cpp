#include "circle.hpp"
#include "complex.hpp"
#include "smiley.hpp"
#include "vector.hpp"

#include <iostream>
#include <vector>


namespace DDB
{
    class Smiley;
}

// 4.2.1 An Arithmetic Type
void test(DDB::Complex z)
{
    DDB::Complex a {2.3};
    DDB::Complex b {1 / a};
    DDB::Complex c {a + z * DDB::Complex{1, 2.3}};

    if (c != b)
        c = -(b / a) + 2 * b;
}
// 4.2.1 An Arithmetic Type

// 4.3 Abstract Types
// This class represents a pure interface to containers defined later.
class Container
{
public:
    // *virtual* means "may be defined later", these are virtual functions.
    // = 0 indicates *pure virtual*; classes derived from Container must
    // define that specific function.
    virtual double& operator[](int) const = 0;
    virtual int size() const = 0;
    // Container does not have a constructor but it does have a virtual
    // destructor. Derived classes can provide an implementations for it.
    virtual ~Container() {}
};

// ": public Container" indicates VectorContainer is derived from the base
// class Container. We'd say Container is the *superclass* and VectorContainer
// is the *subclass*. This is referred to as *inheritance*
class VectorContainer : public Container
{
public:
    VectorContainer(int s) : v(s) {}
    // explicit use of *override* allows the compiler to catch mistakes.
    ~VectorContainer() override {}
    double& operator[](int i) const override { return v[i]; }
    int size() const override { return v.size(); }
private:
    DDB::V2::Vector v;
};
// 4.3 Abstract Types

// 4.4 Virtual Functions
void use(Container& c)
{
    const int sz {c.size()};

    for (int i {}; i != sz; ++i)
        std::cout << c[i] << '\n';
}
// 4.4 Virtual Functions

// 4.5 Class Hierarchies
void rotateAll(std::vector<DDB::Shape*>& v, int angle)
{
    for (auto ps : v)
        ps->rotate(angle);
}
// 4.5 Class Hierarchies

int main()
{
    // When we refer to the representation, we're talking about the data
    // members of a class.

    // 4.2 Concrete Types
    // Concrete types behave just like built-in types but have their own
    // semantics and sets of operations.
    // For example, vectors and strings are much like built-in arrays, except
    // they are better behaved.
    // The defining characteristic of a concrete type is that its
    // representation is part of its definition. In other words, the way the
    // object is represented in memory is fixed.
    // Therefore, if the representation changes, a user must recompile.
    // To increase flexibility, a concrete type can keep major parts of its
    // representation on free store (dynamic memory, heap) and access them
    // through the part stored in the class object itself.
    // Unlike abstract types concrete types can be instantiated.
    // 4.2 Concrete Types

    // 4.2.1 An Arithmetic Type
    // An arithmetic-type defines a conventional set of operators and remains
    // efficient for inline use for those particular operator overloaded
    // functions.
    //
    // Simple operations in the complex class remain efficient so they can be
    // inline. That means the generated machine code will be implemented
    // without function calls.
    const DDB::Complex cz {1, 2};
    DDB::Complex complex {};
    double x = cz.real();
    test(cz);
    // Functions defined in a class are inline by default. It is possible to
    // explicitly request an inline by preceding a function declaration
    // with the `inline` keyword. Here DDB::Complex is carefully implemented
    // to do appropriate inlining.
    // 
    // A constructor invoked without an argument is called a *default
    // constructor*. It eliminates the possibility of uninitialized variables
    // of the user-defined type.
    //
    // Syntax for overloaded operators is fixed by the language, so you can't
    // define a unary / operator. Also, you can redefine the meaning of
    // existing operators for built-in types.
    // 4.2.1 An Arithmetic Type

    // 4.2.2 A Container
    // Our Vector type comes with set of useful features, but it has a fatal
    // flaw. It allocates elements using new but never deallocates them.
    // Although C++ defines an interface for a garbage collection, it doesn't
    // guarantee that it will make unused memory available for new objects.
    // Therefore we need a mechanism to ensure that the memory allocated by
    // the constructor is deallocated; the mechanism is a destructor.
    //
    // The technique of acquiring resources in a constructor and releasing
    // them in a destructor is known as *Resource Acquisition Is
    // Initialization* or *RAII*. It buries allocation in general code in
    // the implementation of well-behaved abstractions.
    // 4.2.2 A Container

    // 4.2.3 Initializing Containers
    // Containers exist to hold elements, so there's need to be a convenient
    // way of getting elements into them. Vector provides the ability to
    // create an appropriate number of elements and then assign to them, but
    // there are typically other more elegant ways.
    //
    // *initializer-list constructor*: Initialize with a list of elements.
    // push_back(): Add a new element at the end of the sequence.
    //
    // We'll focus on initializer lists which are defined by {}-list (e.g.,
    // {1,2,3,4}).
    DDB::V2::Vector v1 = {1, 2, 3, 4}; // contains 4 elements
    for (int i {}; i < v1.size(); ++i)
        std::cout << v1[i] << '\n';
    // 4.2.3 Initializing Containers

    // 4.3 Abstract Types
    // An abstract type insulates a user of implementation details. It gives up
    // genuine local variables and decouples the interface.
    //
    // Since the representation of an abstract type is not known, objects are
    // allocated on free store and accessed through references and pointers.
    //
    // Container c; // No objects from the abstract class.
    // // Container is an interface for VectorContainer object.
    Container* p {new VectorContainer {10}};
    // Since the abstract class can take in any kind of object, a function arg
    // using the abstract type can be blind to the implementation of the 
    // subclass container and still use it the Container functions.
    //
    // A class with a pure virtual function is called an *abstract class*.
    // A class that provides the interface to a variety of other classes is
    // often a *polymorphic type*.
    // 4.3 Abstract Types

    // 4.4 Virtual Functions
    use(*p);
    // How is the following function above resolved? It uses a Container type
    // but it knows to call a subclass's function. A Container object must
    // contain information to allow it to select the right function to call at
    // runtime.
    // 
    // A typical compiler implementation involves the use of a vbtl table 
    // (*virtual function table*). The name of a virtual function is turned
    // into an index into a table of pointers to functions. Each subclass has
    // a separate table.
    // 
    // The functions in the vbtl allow the caller to use the object correctly
    // even if the layout and data information is missing. The virtual call
    // mechanism can be made almost as efficient as a normal function call
    // (within 25%).
    // 4.4 Virtual Functions

    // 4.5 Class Hierarchies
    // Container is a simple example of class hierarchy.
    // 
    // Class Hierarchy is a set of classes ordered in a lattice created by
    // derivation (e.g., : public).
    // 
    // Let's create a semi-realistic classic example with the Shape abstract
    // class.
    // 4.5 Class Hierarchies

    // 4.5.1 Benefits from Hierarchies
    // Interface Inheritance: An object of a derived class can be used anywhere
    // an object of the base class is required.
    //
    // Implementation Inheritance: A base class provides functions or data that
    // simplifies the implementation of derived classes.
    // 4.5.1 Benefits from Hierarchies

    // 4.5.2 Hierarchy Navigation
    // What do we do if we want to use a member function that is only provided
    // by a particular derived class? We can use *dynamic_cast*
    DDB::Shape* ps {new DDB::Circle {{0,0},5}};
    if (const auto* smiley = dynamic_cast<DDB::Smiley*>(ps); smiley != nullptr)
    {
        // is smiley
    }
    else
    {
        // not smiley
    }
    // The above basically tests to see if the result is nullptr.
    //
    // This test can be placed in to the initialization of a variable.
    DDB::Shape* ps2 {new DDB::Smiley{{0,0},15}};
    DDB::Smiley& r {dynamic_cast<DDB::Smiley&>(*ps2)};
    // This is susceptible to throw std::bad_cast if object is not the expected
    // type. Operations similar to dynamic_cast are known as "is kind of" and
    // "is instance of" operations. This can lead to small and efficient code,
    // but there is the chance of losing type information. Such as a system
    // that takes in an interface type and then passes that same interface
    // type back.
    // 4.5.2 Hierarchy Navigation

    // 4.5.3 Avoiding Resource Leaks
    // At the end of the day, pointers to objects allocated on the free store
    // is dangerous. They shouldn't be used to represent ownership.
    // (They have the possibility to leak memory).
    //
    // A simple solution to this is the standard-library unique_ptr rather than
    // "naked pointer" when deletion is required.
    std::vector<std::unique_ptr<DDB::Shape>> eyes;
    // When used within a class it has the side effect of no longer requiring
    // us to define a destructor for smiley. Plus, the code using the unique
    // pointer will be just as efficient as using the raw pointers correctly.
    //
    // Objects owned by a unique_ptr will be deleted when unique_ptr goes out
    // of scope. In order to use functions that originally took naked pointers
    // we must rewrite them to use unique_ptr.
    // 4.5.3 Avoiding Resource Leaks

    // 4.6 Advice
    // [2] A concrete type is the simplest kind of class. Where applicable,
    // prefer a concrete type over more complicated classes and over plain
    // data structures; §4.2; [CG:C.10]
    // [4] Prefer concrete classes over class hierarchies for performance
    // -critical components; §4.2.
    // [6] Make a function a member only if it needs direct access to the
    // representation of a class; §4.2.1; [CG:C.4] .
    // [7] Define operators primarily to mimic conventional usage; §4.2.1;
    // [CG:C.160].
    // [8] Use nonmember functions for symmetric operators; §4.2.1; [CG:
    // C.161].
    // [9] Declare a member function that does not modify the state of its
    // object const; §4.2.1.
    // [10] If a constructor acquires a resource, its class needs a destructor
    // to release the resource; §4.2.2; [CG:C.20].
    // [11] Avoid “naked” new and delete operations; §4.2.2; [CG: R.11].
    // (use in general code)
    // [12] Use resource handles and RAII to manage resources; §4.2.2;
    // [CG: R.1].
    // [13] If a class is a container, give it an initializer-list
    // constructor; §4.2.3; [CG:C.103].
    // [14] Use abstract classes as interfaces when complete separation of
    // interface and implementation is needed; §4.3; [CG:C.122]
    // [15] Access polymorphic objects through pointers and references; §4.3.
    // [18] A class with a virtual function should have a virtual destructor;
    // §4.5; [CG:C.127].
    // [19] Use override to make overriding explicit in large class hierarchies;
    // §4.5.1; [CG:C.128].
    // [21] Use dynamic_cast where class hierarchy navigation is unavoidable;
    // §4.5.2; [CG:C.146].
    // [24] Use unique_ptr or shared_ptr to avoid forgetting to delete objects
    // created using new; §4.5.3; [CG:C.149] .
    // 4.6 Advice
}
