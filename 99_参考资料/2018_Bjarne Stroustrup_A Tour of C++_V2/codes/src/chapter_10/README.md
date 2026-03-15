# Overview
## Input and Output
The I/O stream library provides formatted and unformatted buffered I/O of text
and numeric values. An `std::ostream` converts typed objects to a stream of
characters (bytes). An `std::istream` converts a stream of characters (bytes)
to a typed object. The operations of the objects are type safe, type sensitive,
and extensible to handle user-defined types.

Other types of user interaction such as graphical I/O, are handled through 
libraries outside the ISO standard. These streams can be used for binary I/O,
a variety of character types, be locale specific, and use advanced buffering
strategies.

