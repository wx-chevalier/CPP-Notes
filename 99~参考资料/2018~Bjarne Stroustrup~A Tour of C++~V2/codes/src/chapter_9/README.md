# Overview
## Strings and Regular Expressions
Text manipulation is a big part in many C++ programs. The C++ standard library
offers a `string` type to save most users from C-style manipulation of arrays
of characters through pointers.

A `string_view` allows the manipulation of a sequence of characters without
regard on how they are stored (e.g., `std::string` and `char[]`). In addition,
regular expressions are offered to find patterns in text. Strings and regex
object can use a variety of character types (e.g., Unicode).
