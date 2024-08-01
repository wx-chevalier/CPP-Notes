> [Original article](https://dhashe.com/how-to-build-highly-debuggable-c-binaries.html) to build highly-debuggable C++ binaries, follow these steps:
>
> 1.  **Enable Sanitizers:** Use `-fsanitize=address,undefined` for debugging memory and undefined behavior issues.
> 2.  **Enable Debug Modes in Standard Libraries:** Use `-D_GLIBCXX_DEBUG` or `_LIBCPP_HARDENING_MODE` for enhanced checks.
> 3.  **Generate Debug Info for Macros:** Add `-ggdb3` to capture macro definitions.
> 4.  **Use Frame Pointers:** Add `-fno-omit-frame-pointer`.
> 5.  **Enable Asynchronous Unwind Tables:** Add `-fasynchronous-unwind-tables`.

# How to build highly-debuggable C++ binaries
