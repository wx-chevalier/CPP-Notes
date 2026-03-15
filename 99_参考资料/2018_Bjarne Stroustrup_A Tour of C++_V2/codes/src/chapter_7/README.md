# Overview
## Concepts and Generic Programming
There are a couple of things template offer:
- The ability to pass types (as well as values and templates) as arguments
without the loss of information.
- Opportunities to weave together information from different contexts at 
instantiation time.
- The ability to pass constant arguments as arguments.

In other words they add compile-time computation and type manipulation that
leads to compact and efficient code.

The first and most common use for templates is supporting generic programming.
That is, programming focused on the design, implementation and use of general
algorithms. "General" referring to the ability to accept a long range of types
as long as they meet the argument requirements.
