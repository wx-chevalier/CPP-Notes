add_library(
	vector STATIC
	"vector/vector.cpp"
	"vector/vector.hpp"
)
target_include_directories(
	vector PUBLIC
	"vector"
)

add_library(vector_module)
target_sources(
	vector_module PUBLIC
	FILE_SET cxx_modules TYPE CXX_MODULES
	FILES
	"vector/vector.ixx"
)
