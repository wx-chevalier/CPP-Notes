#include <iostream>

int main()
{
	std::cout << "Chapter 8\n";

	// 8.2 Standard-Library Components
	// The facilities provided by the standard library can be classified as:
	//		Runtime language support
	//		The C Standard Library
	//		Strings (character sets, localization, and read-only views 
	//		of substrings).
	//		Support for regular expression matching.
	//		I/O streams in an extensible framework for input and output to
	//		which users can add their own elements.
	//		A framework of containers.
	//		Support for numerical computation.
	//		Support for concurrent programming (threads and locks too).
	//		Parallel version of most STL algorithms.
	//		Utilities to support template programming.
	//		Support for efficient and safe management of resources (optional
	//		garbage collectors).
	//		"Smart pointers" for resource management.
	//		Special purpose containers (array, bitset, tuple)
	//		Suffixes for popular units. (ms, i -> imaginary number)
	// 
	// Essentially the C++ standard library provides the most common
	// fundamental data structures together with the fundamental algorithms
	// used on them.
	// 8.2 Standard-Library Components

	// 8.3 Standard-Library Headers and Namespace
	// Every standard-library facility is provided through some standard header.
	//		#include <string>
	//		#include <list>
	// 
	// The standard library is defined in the std namespace. Use the std:: to
	// access facilities.
	//		std::string sheep {"Four legs Good; two legs Baaad!"};
	// 
	// It's generally poor taste to dump every name from a namespace into the 
	// global namespace.
	// 
	// Headers from the C standard library, such as <stdlib.h> are provided.
	// For each such header there is also a version with its name prefixed by
	// c and .h removed. These versions place their declarations in the std
	// namespace.
	// 8.3 Standard-Library Headers and Namespace

	// 8.4 Advice
	// [1] Don’t reinvent the wheel; use libraries; §8.1; [CG: SL.1.]
	// [3] Do not think that the standard library is ideal for 
	// everything; §8.1.
	// 8.4 Advice
}
