#
# COMP 308
# If you add new source files, you must list them here
#

# TODO list your header files (.hpp) here
SET(headers
	# Stephen
	"mainheaders.hpp"
	"imageLoader.hpp"
	"shaderLoader.hpp"
	"geometry.hpp"
	"octree.hpp"
	"scenemanager.hpp"
	"boidsystem.hpp"
	"boid.hpp"
	"boidstrategy.hpp"
	"preystrategy.hpp"
	"predatorstrategy.hpp"
	
	# Cullum
	"projection.hpp"
	"voronoi.hpp"
	
	# Callum
	"coral.hpp"
	"branch.hpp"
	"shape.hpp"
	"SpaceCoral.hpp"
)


# TODO list your source files (.cpp) here
SET(sources
	# Stephen
	"main.cpp"
	"geometry.cpp"
	"octree.cpp"
	"scenemanager.cpp"
	"boidsystem.cpp"
	"boid.cpp"
	"preystrategy.cpp"
	"predatorstrategy.cpp"
	
	# Cullum
	"projection.cpp"
	"voronoi.cpp"
	
	# Callum
	"coral.cpp"
	"branch.cpp"
	"shape.cpp"
	"SpaceCoral.cpp"
)

# Add executable target and link libraries
# You do not need to touch this
add_executable(${COMP308_ASSIGNMENT} ${headers} ${sources})
target_link_libraries(${COMP308_ASSIGNMENT} PRIVATE glfw ${GLFW_LIBRARIES} glew glm stb)

