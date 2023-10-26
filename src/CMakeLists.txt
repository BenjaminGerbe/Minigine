# CMakeList.txt : CMake project for OpenLightShadow, include source and define
# project specific logic here.
#

set(CMAKE_CXX_STANDARD 17)

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

# yaml-cpp
include_directories(../libs/yaml-cpp/include)     
link_directories(../libs/yaml-cpp/build/Debug)


#glm
include_directories(../libs/GLM)     

#TinyXML2
include_directories(../libs/tinyxml2)



set(ICON_FILE "${CMAKE_SOURCE_DIR}/Minigine.ico")
configure_file(${ICON_FILE} ${CMAKE_BINARY_DIR}/Minigine.ico COPYONLY)
set(CMAKE_RC_COMPILER_INIT "windres")
enable_language(RC)

include_directories(${PROJECT_SOURCE_DIR})
add_executable ( ${CMAKE_PROJECT_NAME} ${Classes} "${CMAKE_SOURCE_DIR}/resources.rc")

foreach(shader_file ${Shaders})
    add_custom_command(
        TARGET ${CMAKE_PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${shader_file}
                $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/${shader_name})
endforeach()

set_source_files_properties("Headers/data.h" PROPERTIES COMPILE_FLAGS "-w")

target_link_libraries( ${CMAKE_PROJECT_NAME} ${OPENGL_gl_LIBRARY} yaml-cppd.lib imgui glfw3dll glew32s opengl32)

