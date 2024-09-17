#include <iostream>
#include <string>
#include <string_view>
#include <regex>

// 9.2 Strings
// e.g. Concatenation Example
std::string compose(const std::string& name, const std::string& domain)
{
	// Addition of strings mean concatenation. The standard string has a move
	// constructor, so returning even long strings by value is efficient.
	return name + '@' + domain;
}
// 9.2 Strings

// 9.2.1 string Implementation
// To handle multiple character sets, string is really an alias for a 
// general template basic_string with the character type char:
template <typename Char>
class basic_string
{ /*string of Char*/
};
using string = basic_string<char>;
// A user can define strings of arbitrary character tyoes. e.g., japanese
// character type Jchar, we can write:
// using jstring = basic_char<jchar>;
// Now we can do all the usual string operations on Jstring, a string of
// Japanese characters.
// 9.2.1 string Implementation

// 9.3 String Views
// Consider a simple function concatenating two strings:
std::string cat(std::string_view sv1, std::string_view sv2)
{
	std::string res(sv1.length() + sv2.length(), ' ');
	char* p = &res[0];
	for (char ch : sv1)
		*p++ = ch;
	copy(sv2.begin(), sv2.end(), p);
	return res;
};
// 9.3 String Views

int main()
{
	std::cout << "Chapter 9\n";

	// 9.2 Strings
	// The standard library provides a string type to complement string 
	// literals. It provides a Regular type for owning and manipulating
	// a sequence of characters from various character types.
	// 
	// Initialized to the character sequence "dmr@bell-labs.com"
	std::string addr {compose("dmr", "bell-labs.com")};
	std::cout << addr << '\n';
	// Alternativately += is available to concatenate items to the end
	// of a string.
	// 
	// In addition to = and +=, subscripting (using []) and substring
	// operations are supported. 
	// e.g. Subscripting
	std::string name {"Niels Stroustrup"};
	auto m3 = [&]()
	{
		std::string s {name.substr(6, 10)}; // s = Stroustrop
		name.replace(0, 5, "nicholas");
		name[0] = std::toupper(name[0]);
	};
	// In the lambda substr and replace take two main paramaters in terms
	// of positioning. The starting index and length. Replace has the
	// additional value argument to replace the substring. (does not need
	// to match length).
	m3();
	std::cout << name << '\n';
	// Naturally, strings can be compared against each other, against C-style
	// strings, and against string literals. 
	// In addition, strings support lexographical ordering (<, >, <=, >=),
	// iteration (using iterators as for vector), input, and streaming.
	// 
	// If a C-style string is necssary (a zero-terminated array of char), string
	// offers read-only access to its contained characters.
	std::string s {"c-string"};
	auto print = [&s]()
	{
		printf("For people who like printf: %s\n", s.c_str());
	};
	print();
	// A string literal is by definition a const char*. To get a literal type
	// of std::string use s suffix.
	// auto s = "Cat"s;
	// auto c = "Dog";
	// The s suffix is contained in the std::literals::string_literals namespace.
	// 9.2 Strings

	// 9.2.1 string Implementation
	// These days, string is usually implemented using the short-string
	// optimization. That is, short string values are kept in the string object
	// itself and only longer strings are placed on free store. Consider:
	std::string s1 {"Annemarie"};
	std::string s2 {"Annemarie Stroustrup"};
	// When string change from a short to a long string (and vice versa) its
	// representation adjusts appropriately. How many characters a short string
	// can have is implementation defined. A combination of memory 
	// fragmenation, memory allocations costs, and run time environments have
	// made the short-string optimization a ubiquitous implementation.
	// 9.2.1 string Implementation

	// 9.3 String Views
	// There are extra complexities when passing substrings to a function.
	// To address this, the standard library offers string_view; basically
	// a (pointer,length) pair denoting a sequence of characters.
	// 
	// A string_view gives access to a contigious sequence of characters.
	// They are stored in many possible ways, including in a string/c-style str.
	// It's like a pointer or a reference in that it doesn't own the characters
	// it points to. In that, it resembles an STL pair of iterators.
	// 
	std::string king {"Harold"};
	auto s3 = cat(king, "William"); // string and const char*
	// cat() has three advantages over the compose() that takes const.
	//		Can be used with character sequences managed in different ways.
	//		No temporary string objects are created for c-style strings.
	//		We can easily pass substrings.
	using namespace std::literals::string_view_literals;
	auto s4 = cat({&king[0], 2}, "Henry"sv);
	// Thea above avoids counting the characters at from const char*
	// and instead uses sv to compute the length at compile time.
	// 
	// When returning a string_view remember that very much like a pointer
	// it has to point to something. If we return a string within a function
	// as a string_view it will be destroyed and string_view will be left
	// pointing to nothing.
	// 
	// One significant restriction of string_view is that it's read-only.
	// You cannot use string_view to pass to a function that modifies
	// its arguments. You might consider gsl::span or gsl::string_span for
	// that. Out of range access of a string_view is unspecified. For guaranteed
	// range checking use at(), which throws out_of_range for attempted 
	// out-of-range access.
	// 9.3 String Views

	// 9.4 Regular Expressions
	// Regular expressions are a powerful tool for text processing. They provide
	// a way to simply and tersely describe patterns in text. (e.g. US postal 
	// codes).
	// 
	// In <regex>, the standard library provides support for regular expressions
	// in the form of the std::regex class and its supporting functions.
	// 
	// Defining a print pattern:
	// US Postal Code Pattern: XXddddd-dddd and variants
	std::regex pat {R"(\w{2}\s*\d{5}(-\d{4})?)"}; 
	// To express the pattern, a raw string literal is used. This allows back
	// slashes and quotes to be used directly in the string. Suitable for regex
	// cause of the number of backslashes they contain.
	// 
	// In <regex>, the standard library provides support for regular
	// expressions:
	//		regex_match(): match regex against a string.
	//		regex_search(): search for a string that matches regex in long 
	//		stream of data.
	//		regex_replace(): regex_search() and replace string.
	//		regex_iterator: iterate over matches and submatches.
	//		regex_token_iterator: iterate over non-matches.
	// 9.4 Regular Expressions

	// 9.4.1 Searching
	// The simplest way of using a pattern is to search for it in a stream:
	/*int lineno {};
	for (std::string line {}; std::getline(std::cin, line);)
	{
		++lineno;
		std::smatch matches {};
		if (std::regex_search(line, matches, pat))
			std::cout << lineno << ": " << matches[0] << '\n';
	}*/
	// regex_search searches the line for anything that matches the regular
	// expression stored in pat and it finds any matches, it stores them in
	// matches.If no matches are found, it returns false.
	//
	// The matches variable is smatch. The s stands for sub or string, and
	// an smatch is a vector of submatches of type string. The first
	// element matches[0] is the complete match.
	//
	// The regular expression syntax and semantics are designed so that
	// regular expressions can be compiled into state machines for
	// efficient execution. The regex type performs this compilation at
	// runtime.
	// 9.4.1 Searching

	// 9.4.2 Regular Expression Notation
	// The regex library can recognize several variants of the notation for
	// regular expressions. Here we used the default notation, a variant
	// of the ECMA standard for javascript.
	//
	// For full portability, use the character class names rather than
	// abbreviations. (e.g. [:digit:] instead of \d).
	//
	// Function that uses the simplest version of regex_match() to test
	// whether a string is an identifier:
	auto is_identifier = [](const std::string& s)
	{
		std::regex pat2 {"[_[:alpha:]\\w*"};
		return std::regex_match(s, pat2);
	};
	// A group (sub-pattern) potentially to be presented by a sub_match is
	// delimited by parentheses. Parentheses that should not define a
	// sub-pattern use "(?:" rather than a plain "(".
	// 9.4.2 Regular Expression Notation

	// 9.4.3 Iterators
	// We can define a regex_iterator for iterating over a sequence of characters
	// finding matches for a pattern. For example, using sregex_iterator to
	// output all whitespace-separated words in a string:
	auto test = [&]()
	{
		std::string input {"aa as; asd ++e^asdf asdfg"};
		const std::regex pat3 {R"(\s+(\w+))"};
		for (std::sregex_iterator p(input.begin(), input.end(), pat3);
			p != std::sregex_iterator {}; ++p)
			std::cout << (*p)[1] << '\n';
	};
	test();
	// A regex_iterator is a bidirectional iterator, so we cannot directly
	// iterator over an istream (only offers input iterator). We also can't write
	// through one. "regex_iterator {}" is the only possible end of the sequence.
	// 9.4.3 Iterators

	// 9.5 Advice
	// [1] Use std::string to own character sequences; §9.2; [CG: SL.str.1].
	// [2] Prefer string operations to C-style string functions; §9.1.
	// Use string to declare variables and members rather than as a base
	// class; §9.2.S
	// [4] Return strings by value (rely on move semantics); §9.2, §9.2.1.
	// [5] Directly or indirectly, use substr() to read substrings and
	// replace() to write substrings; §9.2.
	// [7] Use at() rather than iterators or [ ] when you want range checking;
	// §9.2.
	// [12] A basic_string can be used to make strings of characters on any
	// type; §9.2.1.
	// [13] Use the s suffix for string literals meant to be standard-library
	// strings; §9.3[CG:SL.str.12].
	// [25] Note that \i allows you to express a subpattern in terms of a previous
	// subpattern; §9.4.2.
	// [27] Use regex_iterators for iterating over a stream looking for a pattern;
	// §9.4.3.
	// 9.5 Advice
}
