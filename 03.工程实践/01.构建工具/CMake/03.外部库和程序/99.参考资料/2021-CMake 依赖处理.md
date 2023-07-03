- Notes

对于依赖，有两种处理方式

1. 下载源码，构建依赖

- 如果依赖项目过大，其构建时间可能非常长

2. 下载预编译好的二进制

- 可能遇到 ABI 兼容性问题；不同平台，编译器，标准库的实现，构建类型
  （debug vs. release），moon phase 以及其它大量其它因素都可能导致兼
  容性问题；你需要预编译的 binary 的构建配置和你系统中完全一致
- 如果预编译的 binary 使用系统的 package manager 拉取的，这种方式则
  问题不大，因为系统中的所有库都使用统一的编译器和一样的标准库编译得
  到.

混合 1, 2.

1. 寻找 pre-compiled binary
   - CMake 的 ~find_package()~, 它会查找系统中的 package，一个 package
     可以认为是一个 CMake 文件，它定义了构建目标，你可以就像使用自己写
     的 ~CMakeLists.txt~ 一样去使用它。
   - 不过如果依赖的项目不支持 ~find_package()~ 使用 =findXXX()= 这一族命令
     手工查找，如
     #+begin_src cmake
     # 在 /usr/lib 和 /usr/local/lib 中查找 foo 库
     find_library(FOO_LIBRARY "foo" "/usr/lib" "/usr/local/lib")
     # 可以对诸如 FOO_LIBRARY 的变量做判断去针对处理，比如提示用户库不存在
     #+end_src
2. 未能寻找到 pre-compiled binary
   1. 对于 header-only 的库
      - 下载源码 (可以使用 git submodule 管理，或者 ~execute_process~ 调
        用其它下载命令)
      - ~set include directory~
      - ~install~
      - setup a target for convenience
      - export target
      - 直接使用给定 target 作为我们的依赖编译自己的代码
        #+begin_src cmake
        target_link_libraries(my_target PUBLIC foo)
        #+end_src
   2. 添加需要使用 CMake 编译的库
      - 下载源码 (可以使用 git submodule 管理，或者 ~execute_process~ 调
        用其它下载命令)
      - 该库使用了 CMake，因此可以直接 ~add_subdirectory()~，编译后，所
        有的 target 可以直接使用
        - 如果依赖项目的 target 配置得当，可以直接使用 =target\under{}link_libraries()=
        - 否则，需要做一些修改
          - 一般在 =add_subdirectory= 之后做修改
          - 一个限制（没看懂）https://github.com/commonmark/cmark/pull/137
   3. 依赖的库需要通过其它构建系统构建
      - 下载源码
      - 使用一些 CMake 命令构建使用
        - 配置期: ~execute_process~
        - 构建期: =add\under{}custom\under{}command()= 和 =add\under{}custom\under{}target()=.
      - 依赖构建结果
   4. 目标库编译极其耗时
      - 使用（满足你 OS 和编译器的）预编译的 binary
      - 要求用户自行安装相关依赖

除了使用 git submodule 管理依赖源码，目前还可以使用 FetchContent[fn:1].

- Footnotes

[fn:1] https://www.foonathan.net/2022/06/cmake-fetchcontent/
