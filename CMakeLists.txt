cmake_minimum_required(VERSION 3.0)
project (arcballcamera)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "{CMAKE_SOURCE_DIR}/cmake/")
find_package(OpenGL REQUIRED)
add_subdirectory(external)
include_directories(
		${OPENGL_INCLUDE_DIRS}
		external/glew-1.13.0/include/
		external/glfw-3.1.2/include/
		external/glm-0.9.7.1/
		
		
)
set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw3
	GLEW_1130
	
	
	
)
add_executable(arcballcamera main.cpp shader.h shader.cpp) 
target_link_libraries(${PROJECT_NAME}
	${ALL_LIBS}	
	
 )

