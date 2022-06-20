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
    "${INCLUDE_PATH}/generator/err/err_fmt.hpp"
    "${INCLUDE_PATH}/generator/err/err.hpp"
    "${INCLUDE_PATH}/generator/primitives/bezier_patch.hpp"
    "${INCLUDE_PATH}/generator/primitives/box.hpp"
    "${INCLUDE_PATH}/generator/primitives/cone.hpp"
    "${INCLUDE_PATH}/generator/primitives/plane.hpp"
    "${INCLUDE_PATH}/generator/primitives/sphere.hpp"
    "${INCLUDE_PATH}/generator/config.hpp"
    "${INCLUDE_PATH}/generator/module.hpp"
    "${INCLUDE_PATH}/util/coord_conv.hpp"
    "${INCLUDE_PATH}/util/number.hpp"
    "${INCLUDE_PATH}/util/overload.hpp"
    "${INCLUDE_PATH}/util/parse_number.hpp"
    "${INCLUDE_PATH}/util/try.hpp"
)

list(
    APPEND GENERATOR_SOURCES
    "${SRC_PATH}/generator/primitives/bezier_patch.cpp"
    "${SRC_PATH}/generator/primitives/box.cpp"
    "${SRC_PATH}/generator/primitives/cone.cpp"
    "${SRC_PATH}/generator/primitives/plane.cpp"
    "${SRC_PATH}/generator/primitives/sphere.cpp"
    "${SRC_PATH}/generator/main.cpp"
    "${SRC_PATH}/util/coord_conv.cpp"
)

add_executable(generator ${GENERATOR_HEADERS} ${GENERATOR_SOURCES})

target_include_directories(generator PRIVATE ${INCLUDE_PATH})

# Engine
list(
    APPEND ENGINE_HEADERS
    "${INCLUDE_PATH}/engine/parse/xml/camera/camera.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/camera/projection.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/group/model/model.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/group/model/model_list.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/group/transform/rotate.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/group/transform/transform_list.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/group/transform/transform.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/group/transform/translate.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/group/group.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/util/number_attr.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/util/xyz.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/err/err_fmt.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/err/err.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/module.hpp"
    "${INCLUDE_PATH}/engine/parse/xml/xml.hpp"
    "${INCLUDE_PATH}/engine/parse/module.hpp"
    "${INCLUDE_PATH}/engine/render/layout/world/group/transform/rotate.hpp"
    "${INCLUDE_PATH}/engine/render/layout/world/group/transform/scale.hpp"
    "${INCLUDE_PATH}/engine/render/layout/world/group/transform/transform.hpp"
    "${INCLUDE_PATH}/engine/render/layout/world/group/transform/translate.hpp"
    "${INCLUDE_PATH}/engine/render/layout/world/group/group.hpp"
    "${INCLUDE_PATH}/engine/render/layout/world/group/model.hpp"
    "${INCLUDE_PATH}/engine/render/layout/world/camera.hpp"
    "${INCLUDE_PATH}/engine/render/layout/world/world.hpp"
    "${INCLUDE_PATH}/engine/render/camera.hpp"
    "${INCLUDE_PATH}/engine/render/io_events.hpp"
    "${INCLUDE_PATH}/engine/render/keyboard.hpp"
    "${INCLUDE_PATH}/engine/render/module.hpp"
    "${INCLUDE_PATH}/engine/render/render.hpp"
    "${INCLUDE_PATH}/engine/render/renderer.hpp"
    "${INCLUDE_PATH}/engine/render/state.hpp"
    "${INCLUDE_PATH}/engine/config.hpp"
    "${INCLUDE_PATH}/engine/module.hpp"
    "${INCLUDE_PATH}/generator/primitives/box.hpp"
    "${INCLUDE_PATH}/generator/primitives/cone.hpp"
    "${INCLUDE_PATH}/generator/primitives/plane.hpp"
    "${INCLUDE_PATH}/generator/primitives/sphere.hpp"
    "${INCLUDE_PATH}/util/coord_conv.hpp"
    "${INCLUDE_PATH}/util/number.hpp"
    "${INCLUDE_PATH}/util/overload.hpp"
    "${INCLUDE_PATH}/util/parse_number.hpp"
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
    "${SRC_PATH}/engine/parse/xml/group/transform/translate.cpp"
    "${SRC_PATH}/engine/parse/xml/group/group.cpp"
    "${SRC_PATH}/engine/parse/xml/util/xyz.cpp"
    "${SRC_PATH}/engine/parse/xml/xml.cpp"
    "${SRC_PATH}/engine/render/camera.cpp"
    "${SRC_PATH}/engine/render/io_events.cpp"
    "${SRC_PATH}/engine/render/keyboard.cpp"
    "${SRC_PATH}/engine/render/render.cpp"
    "${SRC_PATH}/engine/render/renderer.cpp"
    "${SRC_PATH}/engine/render/state.cpp"
    "${SRC_PATH}/engine/config.cpp"
    "${SRC_PATH}/engine/main.cpp"
    "${SRC_PATH}/generator/primitives/box.cpp"
    "${SRC_PATH}/generator/primitives/cone.cpp"
    "${SRC_PATH}/generator/primitives/plane.cpp"
    "${SRC_PATH}/generator/primitives/sphere.cpp"
    "${SRC_PATH}/util/coord_conv.cpp"
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

# Glew
find_package(GLEW REQUIRED)

# glm
set(GLM_PATH "${LIB_PATH}/glm")
add_subdirectory(${GLM_PATH})

# GLUT
find_package(GLUT REQUIRED)

# intrinsics
set(INTRINSICS_PATH "${LIB_PATH}/intrinsics")

# nonnull_ptr
set(NONNULL_PTR_PATH "${LIB_PATH}/nonnull_ptr")

# OpenGL
find_package(OpenGL REQUIRED)

# rapidxml
set(RAPIDXML_PATH "${LIB_PATH}/rapidxml")

# result
set(RESULT_PATH "${LIB_PATH}/result")

# spdlog
set(SPDLOG_PATH "${LIB_PATH}/spdlog")
add_subdirectory(${SPDLOG_PATH})

# tinyobjloader
set(TINY_OBJ_LOADER_PATH "${LIB_PATH}/tiny_obj_loader")

list(
    APPEND
    LIB_INCLUDE_PATHS
    ${BRIEF_INT_PATH}
    ${FMT_PATH}
    ${GLEW_INCLUDE_DIRS}
    ${GLM_PATH}
    ${GLUT_INCLUDE_DIRS}
    ${INTRINSICS_PATH}
    ${NONNULL_PTR_PATH}
    ${OPENGL_INCLUDE_DIRS}
    ${RAPIDXML_PATH}
    ${RESULT_PATH}
    ${SPDLOG_PATH}
    ${TINY_OBJ_LOADER_PATH}
)

foreach(TARGET generator engine)
    target_include_directories(
        ${TARGET}
        SYSTEM
        PRIVATE ${LIB_PATH} ${LIB_INCLUDE_PATHS}
    )
endforeach()

# Generator linked libraries
target_link_libraries(generator fmt::fmt spdlog::spdlog)

# Engine linked libraries
target_link_libraries(
    engine
    fmt::fmt
    GLEW::GLEW
    glm::glm
    GLUT::GLUT
    ${GLUT_LIBRARY}     # overspecify libraries just in case.
    OpenGL::GL
    ${OPENGL_LIBRARIES}
    spdlog::spdlog
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
