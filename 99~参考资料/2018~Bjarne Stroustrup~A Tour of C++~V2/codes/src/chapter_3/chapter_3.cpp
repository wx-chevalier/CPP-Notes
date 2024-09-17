import Vector;

#include "vector.hpp"

#include <cmath>
#include <complex>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

// 3.2 Separate Compilation
double sqrtSum(DDB::Vector& vec)
{
    double sum {};
    for (int i {}; i != vec.size(); ++i)
        sum += std::sqrt(vec[i]);
    return sum;
}
// 3.2 Separate Compilation

// 3.3 Modules (C++20)
double sqrtSum(Modules::Vector& vec)
{
    double sum {};
    for (int i {}; i != vec.size(); ++i)
        sum += std::sqrt(vec[i]);
    return sum;
}
// 3.3 Modules (C++20)

// 3.6.3 Structured Binding
struct Entry
{
    std::string name {};
    int value {};
};

Entry readEntry(std::istream& is)
{
    std::string s;
    int i;
    is >> s >> i;
    return {s, i};
}
// 3.6.3 Structured Binding

int main()
{
    // 3.2 Separate Compilation
    // C++ supports the notion of separate compilation where user code
    // sees only declarations of the types and functions used.
    //
    // Definitions of those types are held in separate source files and
    // are compiled separately.
    //
    // These features can be used to organize a program into a set of
    // semi-independent code fragments. A library is often a collection
    // of separately compiled code fragments (e.g., functions).
    DDB::Vector vec1 {4};
    for (int i {}; i != vec1.size(); ++i)
        vec1[i] = 4;
    std::cout << sqrtSum(vec1) << '\n';
    // vector.cpp and chapter_3.cpp are sharing the vector interface (header)
    // but are otherwise independent.
    // A .cpp file that is compiled by itself (including the header files it
    // includes) is called a translation unit. A program can consist of
    // thousands of translation units.
    // 3.2 Separate Compilation

    // 3.3 Modules (C++20)
    // The use of #includes is very old, error-prone, and a rather expensive
    // way of composing programs out of parts.
    //
    // Including the same header in many different translation units will have
    // that header file's text processed by the compiler that many times.
    // How about if you include one header file before another. That header
    // might affect the declarations in the next header file.
    //
    // This is not ideal, and in fact is a major source of cost and bugs.
    //
    // A better way of expressing physical modules is the use of the C++
    // language feature, modules.
    Modules::Vector vec2 {4};
    for (int i {}; i != vec2.size(); ++i)
        vec2[i] = 9;
    std::cout << sqrtSum(vec2) << '\n';
    // The difference between headers and modules is not just syntactic.
    //      A module is compiled once only.
    //      Two modules can be imported in either order without changing
    //      their meaning.
    //      If you import something into a module, users of the module
    //      do not implicitly gain access to what is imported.
    // 3.3 Modules (C++20)

    // 3.4 Namespaces
    // C++ offers namespaces as a mechanism for expressing that declarations
    // belong together and their names shouldn't clash with other names.
    //
    // For example:
    // You might want to create experiments emulating types in the std library.
    // The problem is using the names from the std library will create
    // collisions. A namespace dodges this problem by requiring a name to be
    // qualified with the namespace name.
    //      MyCode::main();
    //
    // If this ever becomes tedious, a name can be brought to scope with a
    // using-declaration.
    //      using std::swap;
    // It makes the name appear as if it was declared in the scope in which the
    // using-declaration appears.
    //
    // To gain access to a whole namespace append namespace to the 
    // using-directive.
    //      using namespace std;
    // This should be used carefully as it removes the ability to selectively
    // use names.
    //
    // Namespace definition:
    //      namespace MyCode
    //      {
    //      }
    // 3.4 Namespaces

    // 3.5 Error Handling
    // Large and complex topic, but C++ provides a few features to help.
    // Instead of painstakingly building up applications from built-in types,
    // build types and algorithms that are appropriate (e.g., sort(), regex).
    //
    // These higher-level constructs simplify programming and limit opportunity
    // for mistakes, and increase compiler's chances of catching errors.
    // 3.5 Error Handling

    // 3.5.1 Exceptions
    // What if the user tries to read a vector out-of-bounds?
    //      The writer of Vector doesn't know what the user wants.
    //      The user cannot consistently detect this problem.
    //
    // Let's assume we want to recover from this error. The solution is to
    // have the Vector implementation detect the out-of-range access.
    try
    {
        vec1[123] = 0; // will cause std::out_of_range exception
    }
    catch (std::out_of_range& err)
    {
        std::cerr << err.what() << '\n';
    }
    // try-block will encompass the potential exception and the catch-block
    // will provide a handler for exceptions of type out_of_range. Standard
    // library containers also use <stdexcept> classes.
    //
    // Do not overuse try-statements.
    //
    // A function that should never throw an exception can be declared noexcept
    //      void user(int sz) noexcept {}
    // If all planning fails, std::terminate() is called to immediately
    // to terminate the program.
    // 3.5.1 Exceptions

    // 3.5.2 Invariants
    // Whenever we define a function, we should consider what its preconditions
    // are and consider whether to test them. A set of conditions that need to
    // remain true through the life-cycle of a class is called an invariant.
    // The constructor is typically responsible for establishing the invariant
    // for its class (so member functions can rely on it). Functions should
    // also consider whether to test them.
    try
    {
        DDB::Vector vec3{-27};
    }
    catch (std::length_error& err)
    {
        std::cout << err.what() << '\n';
    }
    // 3.5.2 Invariants

    // 3.5.3 Error-Handling Alternatives
    // There are a variety of approaches to error handling. Throwing an
    // exception is C++'s most general mechanism for that.
    //
    // In C++ exceptions are designed to be used to report failure to complete
    // a given task.
    //
    // There are other ways of reporting an error that cannot be handled
    // locally.
    //      Somehow return a value indicating failure.
    //      Terminate the program (by invoking a function like terminate(),
    //      exit(), or abort()).
    // Error codes can be used when a failure is normal and expected or an the
    // caller can be expected to handle the error.
    // We throw an exception when the error is rare,
    //      not handled by the caller,
    //      errors can be added in lower-modules of an app,
    //      no suitable path for error code is available,
    //      return path is complicated,
    //      transmits up through a call chain to the original caller,
    //      recovery depends on multiple function calls,
    //      function argument found the error,
    //      or implies an undo action is needed.
    // Terminate when:
    //      System cannot recover,
    //      or error-handling is based on restarting a thread, process, or
    //      computer.
    //
    // Ensure termination by adding noexcept to a function so any throw
    // turns into terminate();
    //
    // Often prefer exceptions because their use scales better, and don't
    // require external tools to check that all errors are handled.
    // 3.5.3 Error-Handling Alternatives

    // 3.5.4 Contracts
    // There is no general and standard way of writing optional run-time tests
    // of invariants, preconditions, etc.
    // C++20 proposes a contract mechanism to support users who want to rely on
    // testing to get programs right. (didn't happen :p)
    // For now, ad hoc mechanisms will have to do. We use a command-line macro
    // to control a run-time check:
    //      if (RANGE_CHECK && (i < 0 || size() <= i))
    //
    // The standard library offers the debug macro, assert(), to assert that
    // a condition must hold at run time. If this condition fails in debug mode
    // the program terminates.
    //      assert(p != nullptr);
    // 3.5.4 Contracts

    // 3.5.5 Static Assertions
    // Exceptions report errors found at run time. If an error can be found at
    // compile time, it is usually preferable to do so.
    // However we can also perform simple check on most properties at compile
    // time and report failures to meet expectations as compiler err messages.
    constexpr auto C = 299792.458;
    constexpr auto D = 123445.12;
    static_assert(D < C, "D is too big!");
    // You can also leave out S and have the default message printed.
    // static_assert is important in generic programming asserting the types
    // used as parameters.
    // 3.5.5 Static Assertions

    // 3.6.1 Function Arguments and Return Values
    // By default we copy "pass-by-value" into a function.
    // If we refer to an object we use a reference "pass-by-reference".
    // In regard to performance, pass small values by-value, larger
    // ones by reference.
    //      Small here means something cheap to copy. This depends on
    //      the machine architecture, but "the size of or three pointers or
    //      less" is good rule.
    //          void test(int i, vector<int>& rv);
    // Also, it's not uncommon for a function argument to have a default value.
    //      void print(int value, int base = 10);
    // It's a simpler alternative to overloading a function.
    // 3.6.1 Function Arguments and Return Values

    // 3.6.2 Value Return
    // The default when returning a value is to copy. We return "by reference"
    // to grant the caller access to something not local to the function.
    // Local variables disappear when the function returns, so we should not
    // return a pointer or reference to it.
    //
    // What if we want to transfer large amount of information from a function
    // though?
    //      Use a move constructor. (5.2.2)
    //
    // The return type a function can be deduced, especially convenient for
    // generic functions:
    //      auto mul(int i, double d) { return i*d; }
    // 3.6.2 Value Return

    // 3.6.3 Structured Binding
    // A function return a class object with many members. This allows us
    // to return multiple values.
    // We can use structured bindings to give local names to members of a
    // class.
    auto [n,v] = readEntry(std::cin);
    std::cout << '{' << n << ", " << v << "}\n";

    std::map<std::string, int> m;
    m.insert({"num0", 0});
    for (const auto [key, value] : m)
        std::cout << '{' << key << ", " << value << "}\n";
    // We've only shown structures with public data members. But it's also
    // possible to gain access through member functions.
    // 3.6.3 Structured Binding

    // 3.7 Advice
    // [4] Avoid non-inline function definitions in headers; §3.2; [CG: SF.2].
    // [5] Prefer modules over headers (where modules are supported); §3.3.
    // [6] Use namespaces to express logical structure; §3.4; [CG: SF.20].
    // [8] Don’t put a using-directive in a header file; §3.4; [CG: SF.7].
    // [9] Throw an exception to indicate that you cannot perform an assigned
    // task; §3.5; [CG:E.2].
    // [13] If in doubt whether to use an exception or an error code, prefer
    // exceptions; §3.5.3.
    // [18] If your function may not throw, declare it noexcept; §3.5;
    // [CG: E.12].
    // [19] Let a constructor establish an invariant, and throw if it cannot;
    // §3.5.2; [CG:E.5].
    // [21] What can be checked at compile time is usually best checked at
    // compile time; §3.5.5[CG:P.4][CG:P.5].
    // [24] Return values as function-return values (rather than by out-
    // parameters); §3.6.2; [CG:F.20] [CG:F.21] .
    // [26] Don’t overuse structured binding; using a named return type is often
    // clearer documentation; §3.6.3.
    // 3.7 Advice
}
