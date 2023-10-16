# CMakeList.txt : CMake project for OpenLightShadow, include source and define
# project specific logic here.
#

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE Classes
     "*.h"
     "*.cpp"
)

file(GLOB_RECURSE Shaders
     "*.glsl"
)

# stbload

include_directories(stb_load)

# glew
include_directories(../libs/GLEW/include)
link_directories(../libs/GLEW/lib/Release/WIN32)

# glfw
include_directories(../libs/GLFW/include)     
link_directories(../libs/GLFW/lib-static-ucrt )

#glm
include_directories(../libs/GLM)     
link_directories(../libs/GLM/build/glm/Debug)


include_directories(${PROJECT_SOURCE_DIR})
add_executable (LOPGL ${Classes})


foreach(shader_file ${Shaders})
    add_custom_command(
        TARGET ${CMAKE_PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${shader_file}
                $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/${shader_name})
endforeach()


target_link_libraries(LOPGL ${OPENGL_gl_LIBRARY} imgui glfw3dll glew32s opengl32)

