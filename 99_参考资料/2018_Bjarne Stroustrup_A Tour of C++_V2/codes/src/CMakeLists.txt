add_executable(
	chapter-1
	"chapter_1/chapter_1.cpp"
)

add_executable(
	chapter-2
	"chapter_2/chapter_2.cpp"
)

add_subdirectory("chapter_3")
add_executable(
	chapter-3
	"chapter_3/chapter_3.cpp"
)
target_link_libraries(
	chapter-3
	vector
	vector_module
)

add_subdirectory("chapter_4")
add_executable(
	chapter-4
	"chapter_4/chapter_4.cpp"
)
target_link_libraries(
	chapter-4
	complex
	vector
	shape
)

add_executable(
	chapter-5
	"chapter_5/chapter_5.cpp"
)
target_link_libraries(
	chapter-5
	vector
)

add_executable(
	chapter-6
	"chapter_6/chapter_6.cpp"
)
target_link_libraries(
	chapter-6
	vector
)

add_executable(
	chapter-7
	"chapter_7/chapter_7.cpp"
)

add_executable(
	chapter-8
	"chapter_8/chapter_8.cpp"
)

add_executable(
	chapter-9
	"chapter_9/chapter_9.cpp"
)

add_executable(
	chapter-10
	"chapter_10/chapter_10.cpp"
)

add_executable(
	cmake-default 
	"cmake_default/cmake.cpp" 
	"cmake_default/cmake.hpp"
)
