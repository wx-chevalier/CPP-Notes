# Overview

## User-Defined Types

Types that can be built from the fundamental types, const
modifier and declarator operators are called _built-in
types_. Most of C++'s set of built-in types is
deliberately low-level. They don't provide the programmer
with high-level facilities to conveniently write
advanced applications. Abstraction mechanisms is our way
around this.

C++ abstraction mechanisms allow programmers to design
and implement their own types, with suitable operations
and representations. User-defined types are types built
from other types using C++'s abstraction mechanisms.
The standard library is constructed user-defined types.
User-defined types are preferred over built-in types
because they are easier to use, less error-prone, and
typically as efficient for what they do or even faster.
