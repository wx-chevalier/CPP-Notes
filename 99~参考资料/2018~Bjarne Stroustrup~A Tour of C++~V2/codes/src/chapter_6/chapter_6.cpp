#include "vector.hpp"

#include <iostream>
#include <list>


// 6.3.1 Function Templates
// Calculate the sum of the element values of any sequence
// that a range-for can traverse (a container).
template <typename Sequence, typename Value>
Value sum(const Sequence& s, Value v)
{
    for (const auto& x : s)
        v += x;
    return v;
}
// 6.3.1 Function Templates

// 6.4.1 Variable Templates
template <typename T>
constexpr T viscosity = 0.4;
// You can also use a user-defined class templates too.
// 6.4.1 Variable Templates

int main()
{
    // 6.2 Parameterized Types
    // template <typename T>
    // The prefix makes a T parameter of the declaration it prefixes.
    // Using class instead of typename is equivalent. (template <class t>)
    //
    // In combination with member functions definitions:
    // template <typename T>
    // Vector<T>::Vector(int s) {}
    // We gain the ability to define Vector's as follows:
    // Vector<double> vc {200};
    // 
    // Templates are a compile time mechanism, there is no runtime overhead
    // incurred when using them. In fact, the new Vector implementation
    // may result in the same machine code as the original double Vector.
    // 
    // Templates plus a set of template args is called an instantiation or
    // specialization. Code is generated for each instantiation used.
    // It's also type checked to be as safe as hand written code.
    const DDB::V4::Vector<int> vec {4};
    vec[0] = 'h';
    vec[1] = 'a';
    vec[2] = 'i';
    vec[3] = 'r';
    for (const auto& r : vec)
    {
        std::cout << r;
        if (&r != vec.end() - 1)
            std::cout << ' ';
    }
    std::cout << '\n';

    DDB::V4::Vector<std::string> vs {17};
    DDB::V4::Vector<std::list<int>> vli {71};
    DDB::V4::Vector<int> vi {4};
    // 6.2 Parameterized Types

    // 6.2.1 Constrained Template Arguments (C++20)
    // Often templates will only make sense for template arguments that 
    // meet certain criteria. For example, a Vector typically offers
    // a copy operation, so it must require elements that must be copyable.
    //      In other words, the template's argument is not just a typename
    //      but an "Element". Where "Element" specifies a list of requirements
    //      of a type that can be an element.
    // 
    // Code example:
    // template <Element T>
    // class Vector { \*...*\ };
    // 
    // This is C++'s version of mathematics' "for all T such that Element(T)";
    // that is, Element is a predicate that checks whether T has all properties
    // that a Vector requires. We call these concepts.
    // 
    // Template arguments with concepts are referred to as a *constrained
    // argument* and templates with constrained args are called a *constrained
    // template*.
    // 
    // It's a compile-time error to instantiate a template with a type that
    // does not meet it's requirements.
    //      Vector<thread> v2; // for example.
    //
    // Since C++ does not officially support concepts before C++20, older code
    // uses unconstrained template arguments and leaves requirements to
    // documentation.
    // 6.2.1 Constrained Template Arguments (C++20)

    // 6.2.2 Value Template Arguments
    // In addition to to type arguments (typename T) a template can take value
    // args.
    // template <typename T, int N>
    // struct Buffer
    // {
    //      using value_type = T;
    //      constexpr int size() { return N; }
    //      T[N];
    // };
    // 
    // These can be useful in different context. For Buffer, it allows the
    // creation of arbitrarily sized and typed buffers with no use of free
    // store.
    //      Buffer<char, 1024> glob;
    // 6.2.2 Value Template Arguments

    // 6.2.3 Template Argument Deduction
    // std::pair<int, double> p = {1, 5.2};
    // People can find the need to specify template args tedious.
    // The standard library offers a function, make_pair(), that deduces the
    // template arguments of the pair and it returns from its function args.
    //      auto p = std::make_pair(1, 5.2);
    //
    // C++17 provides the ability to deduce these template parameters from
    // constructor args.
    //      std::pair p {1, 5.2};
    //
    // Vector vs{"Hello", "World"};
    // Vector vs2{"Hello"s, "World"s};
    // The first statement creates a vector of const char*, however, using
    // the s suffix we can get std::string.
    //
    // When a template arg cannot be deduced from the constructor args, we
    // can help by providing a deduction guide.
    // Consider the following:
    //      template <typename T>
    //      class Vector2
    //      {
    //          using value_type = T;
    //          public:
    //          template <typename Iter>
    //          Vector2(Iter b, Iter e); // [b:e) range constructor
    //      }
    // If we use the range constructor by using the iterators of another
    // vector let's say vector<int>. The compiler has no idea how to deduce
    // this new vector will be int.
    //
    // So we use a "deduction guide" after the declaration of Vector:
    //      template <typename Iter>
    //      Vector2(Iter, Iter) -> Vector2<typename Iter::value_type>;
    // This basically translates to: if we initialize Vec2 with a pair of
    // iterators, deduce Vec2::value_type to be the iterators' value type.
    //
    // It's best to design class templates so that deduction guides
    // are not needed. However, the std library is full of classes that still
    // don't use concepts and have such ambiguities.
    // 6.2.3 Template Argument Deduction

    // 6.3 Parameterized Operations
    // Templates have more uses than just parameterizing the element type of a
    // container. They are also used for the paramaterization of algorithms
    // and types in the standard library.
    // 
    // Three ways of expressing an operation parameterized by types of values:
    // A function template
    // A function object: an object that can carry data and can be called like
    // a function.
    // A lambda expression: a shorthand notation for a function object.
    // 6.3 Parameterized Operations

    // 6.3.1 Function Templates
    // void user(Vector<int>& vi, list<double>& ld, vector<complex<double>>& vc)
    // {
    //      int x = sum(vi, 0);
    //      double d = sum(vi, 0.0);
    //      double dd = sum(ld, 0.0);
    //      auto z = sum(vc, complex{0.0, 0.0});
    // }
    // In this case we add ints to a double to handle sums larger than what 
    // int can handle. Sequence and Value are deduced from the function 
    // arguments.
    // 
    // This sum() is a simplified version of standard library accumulate().
    // 
    // A function template can be a member function but not a virtual 
    // member. The compiler would not know all instatiations of such a template
    // in a program, so it could not generate a vtbl.
    // 6.3.1 Function Templates

    // 6.3.2 Function Objects
    // A particularly useful function template is function objects (often called
    // functors). These define objects that can be called like functions.
    // template <typename T>
    // class LessThan
    // {
    // public:
    //      LessThan(const T& v) : val {v} {}
    //      bool operator()(const T& x) const { return x < val; }
    // private:
    //      const T val;
    // };
    // The function called operator() implements the "function call", "call",
    // or "application" operator ().
    //      LessThan lti {42};
    //      bool = lti(n);
    // We call the lti object as if it were a function. It compares if n < its
    // representation.
    //
    // These function objects are widely used as arguments to algorithms. For
    // example counting the occurrences of a values for which a predicate
    // returns true. (predicates are invoked and return true or false)
    // template <typename C, typename P>
    // int count(const C& c, P pred)
    // {
    //      int count = 0;
    //      for (const auto& x : c)
    //          if (pred(x))
    //              ++count;
    //      return count;
    // }
    // We would push a LessThan object as the predicate in the example above.
    //
    // Function objects are used to specify the meaning of key operations of a
    // general algorithm (such as LessThan for count()) are often referred to
    // as *policy objects*.
    // 6.3.2 Function Objects

    // 6.3.3 Lambda Expressions
    // It can inconvenient to define a func obj outside its use. We can use a
    // notation to implicitly generate them instead:
    // count(vec, [&](int a){ return a < x; })
    //
    // The second arg is called *lambda expression. It generates a func obj
    // exactly like LessThan<int>{x}. [&] represents the *capture list*, and
    // in this case specifying all local names will be accessed through
    // reference. To specify a specific local var use something similar to
    // [&x]. To generate a copy replace & with =. To capture nothing leave
    // it empty.
    //
    // Lambda's can be obscure so its preferred to name the operations when
    // it comes to non-trivial (more complicated actions).
    //
    // We can also use lambdas as initializers.
    // std::vector<int> v = [&]{ /*switch statement*/ };
    // This can help with conditional initialization.
    // 6.3.3 Lambda Expressions

    // 6.4 Template Mechanisms
    // To define good templates, we need some supporting language facilities:
    //      Values dependent on a type: *variable templates* (6.4.1)
    //      Aliases for types and templates: *alias templates* (6.4.2)
    //      A compile-time selection mechanism: if constexpr (6.4.3)
    //      A compile-time mechanism to inquire about properties of types and
    //      expressions: *require-expressions* (7.2.3)
    // In addition, constexpr functions and static_asserts often take part
    // in template design and use.
    // 6.4 Template Mechanisms

    // 6.4.1 Variable Templates
    // When we use a type, we often want constants and values of types. This
    // is the same for class templates: when we define a C<T>, we often want
    // constants and variables of type T and other types depending on T.
    // // fluid simulation example above.
    // 6.4.1 Variable Templates

    // 6.4.2 Aliases
    // It's often useful to introduce a synonym for a type. (e.g., <cstddef>
    // contains a definition of the alias size_t maybe)
    //      using size_t = unsigned int; // hypothetical
    // The actual type is implementation dependent. In another implementation
    // it may be unsigned long. In the end, having an alias allows the
    // the programmer to write portable code.
    //
    // It's common for parameterized type to provide an alias for types related
    // to their template args.
    // template <typename T>
    // class Vector
    // {
    //      using value_type = T;
    // };
    // Every standard library container provides value_type as the name of its
    // value type (11). It allows the ability to write code that will work for
    // every container that follows the convention.
    // Example
    //      template <typename C>
    //      // type of C's elements
    //      using ValueType = typename C::value_type;
    //
    //      template <typename Container>
    //      void algo(Container& c)
    //      {
    //          Vector<ValueType<Container>> vec;
    //      }
    //
    // The aliasing mechanism can be used to define a new template by binding
    // some of all template arguments. For example:
    // template <typename Key, typename Value>
    // class Map
    // {
    //      // ...
    // };
    //
    // template <typename Value>
    // using StringMap = Map<std::string, Value>;
    //
    // StringMap<int> m;
    // 6.4.2 Aliases

    // 6.4.3 Compile-time if
    // C++17 provides the ability to optionally use performant code with a
    // compile-time if. 
    // template <typename T>
    // void update(T& target)
    // {
    //      // pod = plain old data
    //      if constexpr(isPod<T>::value)
    //          simple_and_fast(target);
    //      else
    //          slow_and_safe(target);  
    // }
    // isPod<T> tells us if a type can be trivially copied. Only the selected
    // branch is instantiated.
    // 
    // if constexpr cannot break general syntax, type and scope rules.
    // 6.4.3 Compile-time if 

    // 6.5 Advice
    // [1] Use templates to express algorithms that apply to many argument
    // types; §6.1; [CG:T.2].
    // [2] Use templates to express containers; §6.2; [CG: T.3].
    // [4] Templates are type safe, but checking happens too late; §6.2.
    // [5] Let constructors or function templates deduce class template 
    // argument types; §6.2.3.
    // [6] Use function objects as arguments to algorithms; §6.3.2; [CG: T.40].
    // [8] A virtual function member cannot be a template member function;
    // §6.3.1.
    // [9] Use template aliases to simplify notation and hide implementation
    // details; §6.4.2.
    // [10] To use a template, make sure its definition (not just its 
    // declaration) is in scope; §7.5.
    // [12] There is no separate compilation of templates: #include template
    // definitions in every translation unit that uses them.
    // 6.5 Advice
}
