add_library(
	complex STATIC
	"complex/complex.cpp"
	"complex/complex.hpp"
)
target_include_directories(
	complex PUBLIC
	"complex"
)

add_library(
	shape STATIC
	"shape/shape.hpp"
	"shape/circle.cpp"
	"shape/circle.hpp"
	"shape/smiley.cpp"
	"shape/smiley.hpp"
)
target_include_directories(
	shape PUBLIC
	"shape"
)
