> [原文地址](https://gist.github.com/CaglayanDokme/a90aec08b5da8d4edc9fc8a3173bd1f7)

# Is there an ultimate Makefile?: Part 1 - Applications

Have you ever struggled with writing your Makefile manually or dealing with your projects without depending on a specific IDE? If so, this article is for you to break your chains.

Picture this: a Makefile that adapts to your project like a chameleon, requiring only a few parameter tweaks to fit most of your needs. Gone are the days of wrestling with convoluted build scripts or spending hours integrating external libraries. With this Makefile in your arsenal, you'll conquer the build process with ease and finesse.

Who is this article for?

- If you have a GNU Linux development environment.
- If you are using GNU compilers such as `g++`.
- If you don't depend on any IDE for project configurations.

> Before starting, I suggest you to enable showing whitespace characters when dealing with Makefiles in your editor as they are fundamental for distinguishing variable and target declarations.

> Feel free to use the suggestions below as a source of inspiration, but remember that they may not address all of your specific needs. Get creative and adapt them to suit your project requirements.

> Please note: This article focuses solely on the Makefile template and does not compare or discuss other build systems.

## Dealing with Folder Hierarchy

When it comes to working with C++ projects, handling the folder hierarchy is essential for a smooth and organized development experience. In this section, we will explore how our ultimate Makefile tackles the intricacies of folder structures, including subfolders and linked folders.

Let's take a look at a basic project structure to illustrate the concepts we'll be discussing:

```console
[12:17:27]BasicProject$ tree -l
.
├── Makefile
└── SourceCode
    ├── Controllers
    │   ├── MenuPage.cpp
    │   └── MenuPage.hpp
    ├── main.cpp
    ├── main.hpp
    └── Visuals -> ../AnotherProject/SourceCode/Visuals
        ├── Components
        │   └── Text.hpp
        ├── MenuPage.cpp
        └── MenuPage.hpp
```

While this structure may seem simple at first glance, it requires special treatment in our Makefile. One challenge is to detect all the source files in this project accurately. To achieve this, we leverage the power of the find command—a familiar tool in the developer's toolkit.

The key aspect of finding the source files is capturing their paths relative to the main source folder. This relative hierarchy is crucial because we will replicate it when generating the object files. Without preserving the folder hierarchy, conflicts may arise due to files with the same name residing in different folders. Such conflicts can hinder the linker's ability to perform its task effectively. Additionally, we need to address the inclusion of linked files, which can be achieved by using the -L flag with the find command.

Let's see an example of the find command in action, demonstrating how it locates source files in both subfolders and linked folders:

```console
[12:57:37]BasicProject$ find -L SourceCode -type f -name "*.cpp"  -print
SourceCode/Visuals/MenuPage.cpp
SourceCode/main.cpp
SourceCode/Controllers/MenuPage.cpp
```

As you can see, the command successfully identifies the source files, considering both subfolders and linked folders. This comprehensive detection mechanism ensures that our Makefile can handle projects of varying complexity.

Building on these concepts, let's take a closer look at the Makefile we can create to accommodate this folder hierarchy:

```Makefile
PRJ_NAME := BasicProject

### Paths ###
CURRENT_DIR     := $(notdir $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST))))))
PRJ_ROOT        := $(CURRENT_DIR)
PRJ_SOURCE_CODE := ${PRJ_ROOT}/SourceCode
PRJ_OUTPUT_DIR  := ${PRJ_ROOT}/Outputs
PRJ_OBJECTS_DIR := ${PRJ_OUTPUT_DIR}/Objects
INCLUDES        += -I${PRJ_SOURCE_CODE}

### Derived Variables ###
CPP_SRC_FILES   := $(shell find -L $(PRJ_SOURCE_CODE) -type f -name "*.cpp"  -print)                # All C++ files
CPP_OBJECTS     := $(patsubst $(PRJ_SOURCE_CODE)/%.cpp,$(PRJ_OBJECTS_DIR)/%.o, $(CPP_SRC_FILES))    # All C++ objects

### Targets ###
# Complete build
all: $(CPP_OBJECTS)
	@echo "Executing target '$@' for project '${PRJ_NAME}'"

	@g++ -o ${PRJ_OUTPUT_DIR}/${PRJ_NAME}.elf $(CPP_OBJECTS)

	@echo "Finished executing the target '$@' for project '${PRJ_NAME}'"

clean:
	@echo "Executing target '$@' for project '${PRJ_NAME}'"

	rm -rf ${PRJ_OBJECTS_DIR}

	@echo "Finished cleaning the target '$@' for project '${PRJ_NAME}'"

# Generic C/C++ files build target (Object generation)
${PRJ_OBJECTS_DIR}/%.o : $(PRJ_SOURCE_CODE)/%.cpp
	@echo "Building '$<' for project '${PRJ_NAME}'"

	@mkdir -p $(@D)
	@g++ $(INCLUDES) -c $< -o $@

	@echo "Built '$<' for project '${PRJ_NAME}'"
```

Running this Makefile produces the following output folder structure, showcasing the preservation of the folder hierarchy found within the source folder. This approach not only facilitates easier inspection of object files but also enhances maintenance convenience.

```console
[13:03:54]BasicProject$ tree Outputs
Outputs
├── BasicProject.elf
└── Objects
    ├── Controllers
    │   └── MenuPage.o
    ├── main.o
    └── Visuals
        └── MenuPage.o
```

By incorporating these techniques into our Makefile, we ensure that our build system effortlessly adapts to any project's source folder structure, providing a solid foundation for efficient C++ development.

Notice that this Makefile doesn't deal with any libraries, dependencies, etc. In the following sections, we will add more features to this Makefile and ultimately obtain the Makefile in our dreams :)

## Detecting Header Modifications

Ensuring that our Makefile accurately detects modifications to header files is crucial for maintaining a reliable and efficient build process. By default, our Makefile only tracks changes in the source files we made it find, but we can enhance it to address changes in header files as well. To achieve this, we will leverage a feature provided by GNU compilers.

By appending the `-MD -MP` flags to our compiler directive, we enable the compiler to generate a list of dependencies for each source file. This list captures the dependencies of the source file, including header files. Let's consider an example dependency chain created for the `main.cpp` source file:

```Makefile
Outputs/Objects/main.o: SourceCode/main.cpp /usr/include/stdc-predef.h \
 SourceCode/main.hpp SourceCode/Controllers/MenuPage.hpp \
 SourceCode/Visuals/MenuPage.hpp SourceCode/Visuals/Components/Text.hpp
/usr/include/stdc-predef.h:
SourceCode/main.hpp:
SourceCode/Controllers/MenuPage.hpp:
SourceCode/Visuals/MenuPage.hpp:
SourceCode/Visuals/Components/Text.hpp:
```

As you can see, the dependency file reflects the targets that represent each file we depend on. This information becomes invaluable in our situation. To incorporate this feature into our Makefile, we need to make a few adjustments:

1. Append the `-MD -MP` options to the compilation step, as shown below:

   ```Makefile
   g++ $(INCLUDES) -c $< -o $@ -MD -MP
   ```

2. Determine the dependency files that will be referenced. We can achieve this by modifying the following line:

   ```Makefile
   DEP_FILES := $(patsubst $(PRJ_OBJECTS_DIR)/%.o,$(PRJ_OBJECTS_DIR)/%.d, $(CPP_OBJECTS))
   ```

3. Refer to the targets created in the dependency files by adding the following line:
   ```Makefile
   -include $(DEP_FILES)
   ```

> I'm not adding the whole Makefile this time to keep the article shorter.

By implementing these changes, our Makefile will now consider the modifications in both source and header files when determining which targets need to be rebuilt. This ensures that the build process remains accurate and efficient, saving us time and effort.

## Defining Symbols

In many C++ projects, it's common to define symbols or macros that provide configurable parameters or flags during the compilation process. These symbols can be used to control the behavior of the code or enable conditional compilation based on certain conditions. One example is as below.

```C++
#ifdef DEBUG
    // Code to execute when the DEBUG macro is defined
#else
    // Code to execute when the DEBUG macro is not defined
#endif
```

Let's consider an example where we define a symbol `PRJ_NAME` and assign it the value of our project's name `$(PRJ_NAME)`. Our Makefile will look like this:

```Makefile
DEFINES += -DPRJ_NAME=$(PRJ_NAME)

${PRJ_OBJECTS_DIR}/%.o: $(PRJ_SOURCE_CODE)/%.cpp
	@echo "Building '$<' for project '${PRJ_NAME}'"

	@mkdir -p $(@D)
	@g++ $(INCLUDES) $(DEFINES) -c $< -o $@ -MD -MP

	@echo "Built '$<' for project '${PRJ_NAME}'"
```

By utilizing symbol definitions in our Makefile, we can easily customize various aspects of our project without modifying the source code directly. This enhances the flexibility and reusability of our Makefile, making it easier to adapt to different project requirements.

## Leveraging Third-party Libraries

In many C++ projects, it's common to leverage external libraries to access additional functionality or utilize existing frameworks. These libraries provide pre-compiled code that can be linked with our own project during the compilation process.

Let's consider an example where we want to link our project with three external libraries: `pthread`, `rt`, and `cairo`. These libraries provide threading support, real-time functionality, and graphics capabilities, respectively. To incorporate these external libraries into our project, we can make the following changes to our Makefile:

```Makefile
EXTERNAL_LIBS := -lpthread -lrt -lcairo

all: $(CPP_OBJECTS)
	@echo "Executing target '$@' for project '${PRJ_NAME}'"

	@g++ -o ${PRJ_OUTPUT_DIR}/${PRJ_NAME}.elf $(CPP_OBJECTS) $(EXTERNAL_LIBS)

	@echo "Finished cleaning the target '$@' for project '${PRJ_NAME}'"
```

In the example above, we have introduced the `EXTERNAL_LIBS` variable, which contains the flags necessary to link our project with the external libraries. The `-l` prefix indicates that we are linking against a specific library, followed by the library name. In this case, we are linking against `libpthread`, `librt`, and `libcairo` libraries, represented by `-lpthread`, `-lrt`, and `-lcairo`, respectively.

By appending `$(EXTERNAL_LIBS)` to the linker command, we ensure that the external libraries are linked with our project during the final compilation step. This allows us to utilize the functionality provided by these libraries in our C++ code.

## Architecture Dependency

In some cases, it may be necessary to build a project for different architectures. This could be due to target platforms having different processor architectures, such as ARM-based systems. To address this requirement, we can introduce architecture selection in our Makefile.

Let's consider an example where we want to build our project for either the ARM 64-bit architecture (`aarch64`) or the ARM 32-bit architecture (`arm`). We will use the `ARCH_TYPE` variable to control the selected architecture.

```Makefile
ARCH_TYPE ?= arm

ifeq ($(ARCH_TYPE), x86_64)
    $(info x86 64-bit architecture choosen!)

    CC := g++
else ifeq ($(ARCH_TYPE), aarch64)
    $(info ARM 64-bit architecture chosen!)

    CC := aarch64-linux-gnu-g++
else ifeq ($(ARCH_TYPE), arm)
    $(info ARM 32-bit architecture chosen!)

    CC := arm-linux-gnueabihf-g++
else
    $(error Architecture type not recognized! ARCH_TYPE: '${ARCH_TYPE}')
endif

ifeq ($(strip $(shell which ${CC})),)
    $(error Toolchain(${CC}) not found in the path! Check the environment.)
endif

all: $(CPP_OBJECTS)
	@echo "Executing target '$@' for project '${PRJ_NAME}'"

	@$(CC) -o ${PRJ_OUTPUT_DIR}/${PRJ_NAME}.elf $(CPP_OBJECTS) $(EXTERNAL_LIBS)

	@echo "Finished cleaning the target '$@' for project '${PRJ_NAME}'"

${PRJ_OBJECTS_DIR}/%.o : $(PRJ_SOURCE_CODE)/%.cpp
	@echo "Building '$<' for project '${PRJ_NAME}'"

	@mkdir -p $(@D)
	@$(CC) $(INCLUDES) $(DEFINES) -c $< -o $@ -MD -MP

	@echo "Built '$<' for project '${PRJ_NAME}'"
```

In the example above, we use an `ifeq` statement to check the value of the `ARCH_TYPE` variable. If `ARCH_TYPE` is set to `aarch64`, we inform the user that the ARM 64-bit architecture has been chosen, and we set the `CC` variable to `aarch64-linux-gnu-g++`, which is the compiler for the ARM 64-bit architecture. Similarly, if `ARCH_TYPE` is set to `arm`, we inform the user that the ARM 32-bit architecture has been chosen, and we set the `CC` variable to `arm-linux-gnueabihf-g++`, which is the compiler for the ARM 32-bit architecture.

By selecting the appropriate compiler based on the architecture type, we ensure that our project is compiled correctly for the target platform. This flexibility allows us to easily switch between different architectures without modifying the Makefile extensively.

Remember to set the `ARCH_TYPE` variable when invoking `make`, like this:

```bash
make ARCH_TYPE=aarch64
```

### Architecture Specific Utilities

When we select a specific architecture, it's not just the compiler that changes; it's the entire compiler toolchain associated with that architecture. Therefore, it's essential to consider and adapt the utilities used in the build process based on the selected architecture.

For instance, to printout the size of the produced ELF, we should use appropriate size utility.

- `aarch64-linux-gnu-size` for ARM-64 executables.
- `arm-linux-gnu-size` for ARM-32 executables.

### System Root

During cross-compilation, it is often necessary to use libraries specifically built for the target architecture. The `--sysroot` option provided by GNU compilers allows you to specify an alternative system root directory. By utilizing this option, you can override the default system root directory and instruct the compiler to search for system headers and libraries in a different location.

In our Makefile, we can incorporate this awareness by defining the system root based on the selected architecture and then informing the compiler about the changes.

```Makefile
ifeq ($(ARCH_TYPE), x86_64)
    SYSTEM_ROOT_PATH := /
else ifeq ($(ARCH_TYPE), aarch64)
    SYSTEM_ROOT_PATH := /path/to/aarch64-rootfs
else ifeq ($(ARCH_TYPE), arm)
    SYSTEM_ROOT_PATH := /path/to/arm-rootfs
endif

all: $(CPP_OBJECTS)
	@$(CC) --sysroot="$(SYSTEM_ROOT_PATH)" -o ${PRJ_OUTPUT_DIR}/${PRJ_NAME}.elf $(CPP_OBJECTS) $(EXTERNAL_LIBS)
```

By including the `--sysroot` option in our compilation command, we ensure that the compiler is aware of the system root specific to our target architecture, enhancing the cross-compilation process and enabling the seamless integration of target-specific libraries and dependencies.

## Compilation Flags

The compilation flags play a crucial role in determining the behavior of the compiler and the resulting executable. In our built system, it is important to include these flags to control aspects such as optimization levels, debugging information, and compiler warnings.

```Makefile
DEBUG_LEVEL         := 3
OPTIMIZATION_LEVEL  := 0
CPP_FLAGS           := -O${OPTIMIZATION_LEVEL} -g${DEBUG_LEVEL} -Wall -std=c++17


${PRJ_OBJECTS_DIR}/%.o : $(PRJ_SOURCE_CODE)/%.cpp
	@$(CC) ${CPP_FLAGS} $(INCLUDES) $(DEFINES) -c $< -o $@ -MD -MP
```

With the inclusion of compilation flags, we have fine-grained control over aspects such as optimization, debugging, and warning levels, enabling us to tailor the compilation process to our specific requirements. You can extend these flags according to your specific needs.

## Sanitizers

Sanitizers are powerful tools provided by the GNU compiler that help detect various types of bugs and issues in the code during runtime. They are particularly useful for identifying memory errors, undefined behavior, and other programming mistakes that can lead to crashes or vulnerabilities. I prefer enabling the sanitizers when the compilation doesn't have any optimization and debugging level is at its highest.

```Makefile
ifeq (${DEBUG_LEVEL}, 3)
    ifeq (${OPTIMIZATION_LEVEL}, 0)
        $(warning [WARNING] Enabling the sanitizers..)

        # Detect memory related errors
        SANITIZERS += -fsanitize=address

        # Detect undefined behavior
        SANITIZERS += -fsanitize=undefined

        # Detect memory leaks
        SANITIZERS += -fsanitize=leak

        # Detect thread safety issues
        # SANITIZERS += -fsanitize=thread

        # Detect overflows
        SANITIZERS += -fsanitize=signed-integer-overflow

        # Detect out-of-bound accesses
        SANITIZERS += -fsanitize=bounds
    endif
endif

all: $(CPP_OBJECTS)
	@$(CC) --sysroot="$(SYSTEM_ROOT_PATH)" -o ${PRJ_OUTPUT_DIR}/${PRJ_NAME}.elf $(CPP_OBJECTS) $(SANITIZERS) $(EXTERNAL_LIBS)

${PRJ_OBJECTS_DIR}/%.o : $(PRJ_SOURCE_CODE)/%.cpp
	@$(CC) $(CPP_FLAGS) $(SANITIZERS) $(INCLUDES) $(DEFINES) -c $< -o $@ -MD -MP
```

> Notice that the sanitizers option is both given to compiler and linker.

By incorporating these sanitizers into our Makefile, we can enhance the reliability and security of our code by detecting and preventing various types of bugs and vulnerabilities. It is important to note that sanitizers may impose a runtime performance overhead, so they are typically used during development and testing phases rather than in production environments. This is why I choose including them when debugging is enabled.

## Target Awareness

Most of the targets defined in our Makefile have dependencies or are part of a dependency chain. However, there is one special target, clean, which does not depend on any other target or variable. Its sole purpose is to remove the generated object files and clean the project directory. To ensure that the other targets' dependencies and variables are not evaluated when invoking the clean target, we can use conditional logic to exclude them. By wrapping the definitions within the `ifneq ($(MAKECMDGOALS),clean)` condition, we prevent unnecessary processing and execution of code that is not relevant to the clean target. Here's an example of how we can structure the rule:

```Makefile
ifneq ($(MAKECMDGOALS),clean)
    # Architecture Selection
    # ...

    # Include paths
    # ...

    # Symbols
    # ...

    # Compilation Flags
    # ...

    # Sanitizers
    # ...

    # External Libraries
    # ...

    # Derived Variables
    # ...
endif

clean:
	rm -rf ${PRJ_OBJECTS_DIR}
```

By employing this target awareness technique, we can optimize the Makefile's execution by excluding unnecessary processing when the clean target is invoked. This ensures that the clean target operates efficiently and completes its task of removing the generated object files without unnecessary overhead.

## Phony Targets

In the context of Makefiles, phony targets are targets that don't represent actual files or dependencies. Instead, they are used to define actions that should always be executed when the target is invoked, regardless of whether a file with the same name exists.

```Makefile
.PHONY: all clean
```

## Additional Customization

While the provided Makefile serves as a solid foundation for most C++ projects, you can further customize it to incorporate additional functionality and tools based on your project's requirements. Let's explore a few examples:

### Version Control Integration

Currently, our team uses SVN for version controls. So, when determining the version of an executable, we utilize the SVN revision as well. Below is how we integrate include it into the executable.

```Makefile
ifneq ($(strip $(shell which svn)),)
    SVN_REVISION  ?= $(shell svn info --show-item revision)
else
    $(warning SVN command line tools cannot be found, cannot gather SVN revision!)
    SVN_REVISION ?= 0
endif

DEFINES += -DSVN_REVISION=$(SVN_REVISION)
```

### Creating Backups

When working on a project, it's important to have a backup of the previous executable version before generating a new one. This ensures that you don't accidentally overwrite or lose the previous release. Including a backup step in your Makefile can be a simple and effective solution. Here's an example of how you can achieve this:

```Makefile
all: $(CPP_OBJECTS)
	@$(CC) --sysroot="$(SYSTEM_ROOT_PATH)" -o $(EXECUTABLE) $(CPP_OBJECTS) $(SANITIZERS) $(EXTERNAL_LIBS)

#	Create a backup
	@cp $(EXECUTABLE) $(EXECUTABLE)_`date +%Y%m%d_%H%M%S`
```

Below is how they are stored in the outputs directory. Notice that the last created file `BasicProject.elf` has its backup named with the current date and time, _i.e. `BasicProject.elf_20230617_164542`_.

```console
[16:47:09]BasicProject$ ls -l Outputs/*elf*
-rwxrwxr-x 43008 June 17 16:45 Outputs/BasicProject.elf
-rwxrwxr-x 43012 June 17 15:42 Outputs/BasicProject.elf_20230617_154236
-rwxrwxr-x 43138 June 17 16:32 Outputs/BasicProject.elf_20230617_163203
-rwxrwxr-x 43140 June 17 16:40 Outputs/BasicProject.elf_20230617_164044
-rwxrwxr-x 43008 June 17 16:45 Outputs/BasicProject.elf_20230617_164542
```

Remember to regularly transfer your released executables to a secure storage or server for long-term preservation and distribution.

### Code Formatting

Maintaining consistent code formatting across a project is crucial for readability and collaboration. By integrating a code formatting tool like `clang-format` into your build process, you can automatically format your source code according to a predefined style. To add code formatting to the Makefile, you can create a new target called `format`:

```Makefile
format:
    clang-format -i $(shell find $(PRJ_SOURCE_CODE) -name "*.cpp" -o -name "*.hpp")
```

Now, by running `make format`, the `clang-format` tool will be invoked to format all the C++ source files within your project's source code directory.

### Documentation Generation

Generating documentation for your C++ code is essential for providing clear and concise documentation to developers and users. A popular documentation generation tool is `Doxygen`, which can automatically extract comments from your source code and generate comprehensive documentation. To integrate `Doxygen` into your build process, you can add a new target called `docs`:

```Makefile
docs:
    doxygen Doxyfile
```

By running `make docs`, the `Doxygen` tool will be invoked using a configuration file named `Doxyfile`, which you can customize according to your project's needs.

### Code Analysis

Performing static code analysis helps identify potential bugs, code smells, and other issues in your C++ codebase. Tools like `Cppcheck` or `Clang-Tidy` can be integrated into your build process to automate the analysis. To add code analysis to the Makefile, you can create a new target called `analyze`:

```Makefile
analyze:
    cppcheck --enable=all $(shell find $(PRJ_SOURCE_CODE) -name "*.cpp" -o -name "*.hpp")
```

By running `make analyze`, the `Cppcheck` tool will be invoked to analyze all the C++ source files within your project.

These are just a few examples of how you can customize the Makefile to incorporate additional functionality into your build process. You can explore other tools and utilities, such as unit testing frameworks, static analysis tools, or benchmarking tools, and integrate them into your Makefile based on your project's specific needs. Remember to update the dependencies and targets accordingly to ensure that these customizations are executed when necessary.

## Finalizing the Makefile

Finally, we achieved a Makefile which deals with many of problems that you might encounter while setting up the automated build process of your project. Below is the template file I use before customizing it according to my project specific requirements.

```Makefile
ifeq ($(MAKECMDGOALS),)
    $(error No targets given!)
endif

PRJ_NAME := BasicProject

### Paths ###
CURRENT_DIR     := $(notdir $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST))))))
PRJ_ROOT        := $(CURRENT_DIR)
PRJ_SOURCE_CODE := $(PRJ_ROOT)/SourceCode
PRJ_OUTPUT_DIR  := $(PRJ_ROOT)/Outputs
PRJ_OBJECTS_DIR := $(PRJ_OUTPUT_DIR)/Objects
EXECUTABLE      := $(PRJ_OUTPUT_DIR)/$(PRJ_NAME).elf

ifneq ($(MAKECMDGOALS),clean)
    $(info Compiling the project '$(PRJ_NAME)')

    ### Architecture Selection ###
    # Default architecture
    ARCH_TYPE ?= arm

    ifeq ($(ARCH_TYPE), x86_64)
        $(info x86 64-bit architecture choosen!)

        CC := g++
        SZ := size

        SYSTEM_ROOT_PATH := /
    else ifeq ($(ARCH_TYPE), aarch64)
        $(info ARM 64-bit architecture choosen!)

        CC := aarch64-linux-gnu-g++
        SZ := aarch64-linux-gnu-size

        SYSTEM_ROOT_PATH := /path/to/aarch64-rootfs
    else ifeq ($(ARCH_TYPE), arm)
        $(info ARM 32-bit architecture choosen!)

        CC := arm-linux-gnueabihf-g++
        SZ := arm-linux-gnueabihf-size

        SYSTEM_ROOT_PATH := /path/to/arm-rootfs
    else
        $(error Architecture type not recognized! ARCH_TYPE: '$(ARCH_TYPE)')
    endif

    # Check if the toolchain exist in the environment
    ifeq ($(strip $(shell which $(CC))),)
        $(error Toolchain($(CC)) not found in the path! Check the environment.)
    endif

    ### Symbols ###
    DEFINES += -DPRJ_NAME=$(PRJ_NAME)

    ### Compilation Flags ###
    DEBUG_LEVEL         := 3
    OPTIMIZATION_LEVEL  := 0
    CPP_FLAGS           := -O$(OPTIMIZATION_LEVEL) -g$(DEBUG_LEVEL) -Wall -std=c++17

    ### Sanitizers ###
    ifeq ($(DEBUG_LEVEL), 3)
        ifeq ($(OPTIMIZATION_LEVEL), 0)
            $(warning Enabling the sanitizers..)

            # Detect memory related errors
            SANITIZERS += -fsanitize=address

            # Detect undefined behavior
            SANITIZERS += -fsanitize=undefined

            # Detect memory leaks
            SANITIZERS += -fsanitize=leak

            # Detect thread safety issues
            # SANITIZERS += -fsanitize=thread

            # Detect overflows
            SANITIZERS += -fsanitize=signed-integer-overflow

            # Detect out-of-bound accesses
            SANITIZERS += -fsanitize=bounds
        endif
    endif

    ### External Libraries ###
    EXTERNAL_LIBS := -lpthread -lrt -lcairo

    ### Include paths ###
    INCLUDES += -I$(PRJ_SOURCE_CODE)
    INCLUDES += -I$(SYSTEM_ROOT_PATH)/usr/include/cairo

    ### Derived Variables ###
    # Source files to be compiled (Relative path required to reflect the folder hierarchy in objects folder)
    CPP_SRC_FILES   := $(shell find -L $(PRJ_SOURCE_CODE) -type f -name "*.cpp"  -print)                # All C++ files

    # Object files to be linked after compilation
    CPP_OBJECTS     := $(patsubst $(PRJ_SOURCE_CODE)/%.cpp,$(PRJ_OBJECTS_DIR)/%.o, $(CPP_SRC_FILES))    # All C++ objects

    # Dependency files (For properly detecting modifications in header files)
    DEP_FILES       := $(patsubst $(PRJ_OBJECTS_DIR)/%.o,$(PRJ_OBJECTS_DIR)/%.d, $(CPP_OBJECTS))        # All dependency files

    -include $(DEP_FILES) # Missing targets for dependencies
endif

### Targets ###
# Complete build (Compilation and linking)
all: $(CPP_OBJECTS)
	@echo "Executing target '$@' for project '$(PRJ_NAME)'"

	@$(CC) --sysroot="$(SYSTEM_ROOT_PATH)" -o $(EXECUTABLE) $(CPP_OBJECTS) $(SANITIZERS) $(EXTERNAL_LIBS)

#	Create a backup
	@cp $(EXECUTABLE) $(EXECUTABLE)_`date +%Y%m%d_%H%M%S`

	@echo ""
	$(SZ) $(EXECUTABLE)
	@echo ""

	@echo ""
	@echo "------------ Executable info ------------"
	@printf "%-25s %s\n" "Full Path: " 				"`realpath ${EXECUTABLE}`"
	@printf "%-25s %s\n" "Arch Type:" 				"${ARCH_TYPE}"
	@printf "%-25s %s\n" "Target Machine:" 			"`readelf -h ${EXECUTABLE} | awk '/Machine:/{print $$NF}'`"
	@printf "%-25s %s\n" "Compiler:" 				"${CC} `${CC} -dumpfullversion`"
	@printf "%-25s %s\n" "Debug Level(0-3):" 		"${DEBUG_LEVEL}"
	@printf "%-25s %s\n" "Optimization Level(0-3):" "${OPTIMIZATION_LEVEL}"
	@printf "%-25s %s\n" "Sanitizers:" 				"${SANITIZERS}"
	@printf "%-25s %s\n" "External Libraries:" 		"${EXTERNAL_LIBS}"
	@printf "%-25s %s\n" "System Root:" 			"${SYSTEM_ROOT_PATH}"
	@echo ""

	@echo "Finished executing the target '$@' for project '$(PRJ_NAME)'"

clean:
	@echo "Executing target '$@' for project '$(PRJ_NAME)'"

	rm -rf $(PRJ_OBJECTS_DIR)

	@echo "Finished cleaning the target '$@' for project '$(PRJ_NAME)'"

# Generic C/C++ files build target (Object generation)
$(PRJ_OBJECTS_DIR)/%.o : $(PRJ_SOURCE_CODE)/%.cpp
	@echo "Building '$<' for project '$(PRJ_NAME)'"

	@mkdir -p $(@D)
	@$(CC) $(CPP_FLAGS) $(SANITIZERS) $(INCLUDES) $(DEFINES) -c $< -o $@ -MD -MP

	@echo "Built '$<' for project '$(PRJ_NAME)'"

# Prevent confusion between files and targets
.PHONY: all clean
```

## Are we done?

Congratulations on taking the first step towards creating the ultimate Makefile for your projects! In this article, we've covered the essential elements and best practices to help you build a powerful and customizable Makefile. We discussed important topics like directory structure, variable definition, target dependencies, external libraries, compilation flags, and more. These are the building blocks that will make your Makefile efficient and effective.

To make things even easier for you, I've provided a ready-to-use template Makefile. It serves as a starting point that you can customize to fit the specific needs of your project. Whether you're working on a small personal project or a large-scale application, this template will help you save time and effort.

**But we're not done yet!**

In the next part of this article, we'll dive into the user-defined libraries. We'll explore how you can seamlessly integrate these libraries into the Makefile we created in the first part. This will take your build system to the next level, making it even more flexible and modular. So, stay tuned for the second part, where we'll uncover advanced techniques for working with user-defined libraries and show you how to unlock the full potential of Makefile automation.

Remember, the suggestions I've provided are just the beginning. Feel free to experiment and tailor the Makefile to suit your specific needs. With the right Makefile in place, you'll have a robust and efficient build system that will make your development journey a breeze. Happy coding!
