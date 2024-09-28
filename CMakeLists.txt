cmake_minimum_required(VERSION 3.0.0)
project(pg-template VERSION 0.1.0 LANGUAGES C CXX)

# set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE IMGUI_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/3rdparty/imgui/*.cpp)
set(IMGUI_INCLUDE_DIR_PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/3rdparty/imgui)
add_library(imgui STATIC ${IMGUI_FILES})
target_include_directories(imgui PRIVATE ${IMGUI_INCLUDE_DIR_PRIVATE})

set(3RDPARTY_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src/3rdparty/)

file(GLOB PG_TEMPLATE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM PG_TEMPLATE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
set(PG_TEMPLATE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=address -Wall -Wextra -pedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -fsanitize=address")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Wall -Wextra -pedantic")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")

add_library(pg-template-lib STATIC ${PG_TEMPLATE_FILES})
target_include_directories(pg-template-lib PRIVATE
    ${PG_TEMPLATE_INCLUDE_DIRS}
    ${3RDPARTY_INCLUDE_DIRS}
    ${IMGUI_INCLUDE_DIR_PRIVATE}
)
# target_compile_features(pg-template-lib PUBLIC cxx_std_20)

add_executable(pg-template src/main.cpp)
target_include_directories(pg-template PUBLIC ${PG_TEMPLATE_INCLUDE_DIRS})
target_link_libraries(pg-template pg-template-lib imgui)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(embree 3 REQUIRED)
# find_package(assimp REQUIRED)
find_package(SDL2 REQUIRED)
# find_package(SDL2_image REQUIRED)
# find_package(glm REQUIRED)

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
# PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

target_link_libraries(pg-template
    ${SDL2_LIBRARIES}
    # ${SDL2IMAGE_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    # ${ASSIMP_LIBRARIES}
    # ${GLM_LIBRARIES}
    ${EMBREE_LIBRARIES}
    freeimage   # this is bit dirty but i couldn't find with find_package
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
