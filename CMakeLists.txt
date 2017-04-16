# General Purpose CMakeLists.txt

# cmake build script
cmake_minimum_required(VERSION 2.8)

# set compiler
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# project
project(final-project)

# modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

# binary directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# options
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(extern/glfw glfw)

# add include directory
include_directories(src)

# add OPENGL
find_package(OPENGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

set(PROJECT_SOURCE
	src/libraries/soil/image_DXT.c
	src/libraries/soil/image_DXT.h
	src/libraries/soil/image_helper.c
	src/libraries/soil/image_helper.h
	src/libraries/soil/soil.c
	src/libraries/soil/soil.h
	src/libraries/soil/stb_image_aug.c
	src/libraries/soil/stb_image_aug.h
	src/libraries/soil/stbi_DDS_aug_c.h
	src/libraries/soil/stbi_DDS_aug.h

	src/libraries/biscuit.h

	src/libraries/mat4.h
	src/libraries/matn.h
	src/libraries/matn.inl

	src/libraries/noise.h

	src/libraries/perlinnoise.cpp
	src/libraries/perlinnoise.h

	src/libraries/vec3.h
	src/libraries/vec2.h
	src/libraries/vecn.h
	src/libraries/vecn.inl

	src/matty/application.h
	src/matty/bufferedgeometry.h
	src/matty/planebufferedgeometry.cc
	src/matty/planebufferedgeometry.h
	src/matty/platform.h
	src/matty/shader.cc
	src/matty/shader.h
	src/matty/terraingeometry.cc
	src/matty/terraingeometry.h
	src/matty/texturemanager.cc
	src/matty/texturemanager.h
	src/matty/vertex.h
	src/matty/camera.h
	src/opengl/gl3w.c
	src/opengl/gl3w.h
	src/opengl/glcorearb.h

	src/main.cc
	src/skybox.cc
	src/skybox.h

)

# build executable
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE})

if(OPENGL_FOUND)
  message(STATUS "opengl found")
  target_link_libraries(${PROJECT_NAME} ${OPENGL_glu_LIBRARY} ${OPENGL_gl_LIBRARY})
endif()

if(OPENGL_FOUND)
	message(STATUS "opengl found")
	target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})

	if(OPENGL_GLU_FOUND)
		message(STATUS "glu found")
		target_link_libraries(${PROJECT_NAME} ${OPENGL_glu_LIBRARY})
	endif()
endif()

target_link_libraries(${PROJECT_NAME} glfw)

if(APPLE)
  set_target_properties(${PROJECT_NAME} PROPERTIES OSX_ARCHITECTURES "i386;x86_64;")
endif()

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/files
  $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

INSTALL(
  TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION ${EXECUTABLE_OUTPUT_PATH}
)
