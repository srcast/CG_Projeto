################################################################################
# Header
################################################################################
cmake_minimum_required(VERSION 3.16) # 3.16 required for Link-Time Optimization
project("CG-2021-22" VERSION 1.0 LANGUAGES CXX)
################################################################################


################################################################################
# Options
################################################################################
set(CMAKE_CXX_STANDARD 20) # C++20
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # compile_commands.json for IntelliSense
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
################################################################################


################################################################################
# Paths
################################################################################
set(SRC_PATH "${PROJECT_SOURCE_DIR}/src")
set(INCLUDE_PATH "${PROJECT_SOURCE_DIR}/include")
set(LIB_PATH "${PROJECT_SOURCE_DIR}/lib")
################################################################################


################################################################################
# Targets
################################################################################
# Generator
list(
    APPEND GENERATOR_HEADERS
    "${INCLUDE_PATH}/generator/primitives/module.hpp"
    "${INCLUDE_PATH}/generator/primitives/box.hpp"
    "${INCLUDE_PATH}/generator/primitives/cone.hpp"
    "${INCLUDE_PATH}/generator/primitives/plane.hpp"
    "${INCLUDE_PATH}/generator/primitives/sphere.hpp"
    "${INCLUDE_PATH}/generator/util/coord_conv.hpp"
    "${INCLUDE_PATH}/util/pretty_print.hpp"
    "${INCLUDE_PATH}/util/try.hpp"
)

list(
    APPEND GENERATOR_SOURCES
    "${SRC_PATH}/generator/primitives/box.cpp"
    "${SRC_PATH}/generator/primitives/cone.cpp"
    "${SRC_PATH}/generator/primitives/plane.cpp"
    "${SRC_PATH}/generator/primitives/sphere.cpp"
    "${SRC_PATH}/generator/util/coord_conv.cpp"
    "${SRC_PATH}/generator/main.cpp"
)

add_executable(generator ${GENERATOR_HEADERS} ${GENERATOR_SOURCES})

target_include_directories(generator PRIVATE ${INCLUDE_PATH})


# Engine
list(
    APPEND ENGINE_HEADERS
    "${INCLUDE_PATH}/engine/parse/util/fp.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/camera/camera.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/camera/projection.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/group/model/model.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/group/model/model_list.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/group/transform/rotate.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/group/transform/transform_list.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/group/transform/transform.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/group/group.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/util/fp_attr.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/util/xyz.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/err/err_fmt.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/err/err.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/module.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/world.hpp"
    "${INCLUDE_PATH}/engine/parse/module.hpp"
    "${INCLUDE_PATH}/engine/render/world/group/group.hpp"
    "${INCLUDE_PATH}/engine/render/world/group/model.hpp"
    "${INCLUDE_PATH}/engine/render/world/group/transform.hpp"
    "${INCLUDE_PATH}/engine/render/world/camera.hpp"
    "${INCLUDE_PATH}/engine/render/world/world.hpp"
    "${INCLUDE_PATH}/engine/render/keyboard.hpp"
    "${INCLUDE_PATH}/engine/render/module.hpp"
    "${INCLUDE_PATH}/engine/render/renderer.hpp"
    "${INCLUDE_PATH}/engine/config.hpp"
    "${INCLUDE_PATH}/engine/module.hpp"
    "${INCLUDE_PATH}/util/pretty_print.hpp"
    "${INCLUDE_PATH}/util/try.hpp"
)

list(
    APPEND ENGINE_SOURCES
    "${SRC_PATH}/engine/parse/xml/camera/camera.cpp"
    "${SRC_PATH}/engine/parse/xml/camera/projection.cpp"
    "${SRC_PATH}/engine/parse/xml/group/model/model.cpp"
    "${SRC_PATH}/engine/parse/xml/group/model/model_list.cpp"
    "${SRC_PATH}/engine/parse/xml/group/transform/rotate.cpp"
    "${SRC_PATH}/engine/parse/xml/group/transform/transform_list.cpp"
    "${SRC_PATH}/engine/parse/xml/group/transform/transform.cpp"
    "${SRC_PATH}/engine/parse/xml/group/group.cpp"
    "${SRC_PATH}/engine/parse/xml/world.cpp"
    "${SRC_PATH}/engine/render/renderer.cpp"
    "${SRC_PATH}/engine/main.cpp"
)

add_executable(engine ${ENGINE_HEADERS} ${ENGINE_SOURCES})

target_include_directories(engine PRIVATE ${INCLUDE_PATH})
################################################################################


################################################################################
# 3rd Party Libraries
################################################################################
# brief_int
set(BRIEF_INT_PATH "${LIB_PATH}/brief_int")

# {fmt}
set(FMT_PATH "${LIB_PATH}/fmt")
add_subdirectory(${FMT_PATH})

# glm
set(GLM_PATH "${LIB_PATH}/glm")
add_subdirectory(${GLM_PATH})

# GLUT
find_package(GLUT REQUIRED)

# OpenGL
find_package(OpenGL REQUIRED)

# rapidxml
set(RAPIDXML_PATH "${LIB_PATH}/rapidxml")

# result
set(RESULT_PATH "${LIB_PATH}/result")

list(
    APPEND
    LIB_INCLUDE_PATHS
    ${BRIEF_INT_PATH}
    ${FMT_PATH}
    ${GLM_PATH}
    ${GLUT_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${RAPIDXML_PATH}
    ${RESULT_PATH}
)

foreach(TARGET generator engine)
    target_include_directories(
        ${TARGET}
        SYSTEM
        PRIVATE ${LIB_INCLUDE_PATHS}
    )
endforeach()

# Generator linked libraries
target_link_libraries(generator fmt::fmt)

# Engine linked libraries
target_link_libraries(
    engine
    fmt::fmt
    glm::glm
    GLUT::GLUT
    ${GLUT_LIBRARY}     # overspecify libraries just in case.
    OpenGL::GL
    ${OPENGL_LIBRARIES}
)
################################################################################


################################################################################
# Compiler flags
################################################################################
if(MSVC)
    # Compiling with Microsoft Visual C++.
    list(APPEND WARNING_FLAGS "/W4")
else()
    # Compiling with g++/clang++/etc.
    list(
        APPEND WARNING_FLAGS
        "-Wall" "-Wextra" "-Wdouble-promotion"
        "-Wconversion" "-Wsign-conversion"
    )
endif()

foreach(TARGET generator engine)
    target_compile_options(
        ${TARGET}
        PRIVATE ${WARNING_FLAGS}
    )
endforeach()
################################################################################


################################################################################
# Link-Time-Optimization
################################################################################
include(CheckIPOSupported)

check_ipo_supported(RESULT LTO_SUPPORTED OUTPUT USE_LTO_ERROR)

if(LTO_SUPPORTED)
    message(STATUS "LTO enabled")
    foreach(TARGET generator engine)
        set_property(
            TARGET ${TARGET}
            PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE
    )
    endforeach()
else()
    message(STATUS "LTO not supported: <${USE_LTO_ERROR}>")
endif()
################################################################################
