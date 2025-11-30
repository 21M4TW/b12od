cmake_minimum_required(VERSION 3.15)
project(Bolt12OfferDecoderProject LANGUAGES C)

include(CMakePackageConfigHelpers)
include(CTest)

option(BUILD_SHARED_LIBS "Build shared libraries instead of static ones" OFF)

add_compile_options(-O3 -march=native -Wall)

# Automatically collect sources
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/lib/*.c"
)

add_library(b12od STATIC ${SOURCES})
target_include_directories(b12od
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

add_library(b12od_obj_pic OBJECT ${SOURCES})
target_include_directories(b12od_obj_pic
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
set_target_properties(b12od_obj_pic PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(b12od_pic STATIC $<TARGET_OBJECTS:b12od_obj_pic>)
target_include_directories(b12od_pic
    PUBLIC
        $<INSTALL_INTERFACE:include>
)

add_library(b12od_shared SHARED $<TARGET_OBJECTS:b12od_obj_pic>)
target_include_directories(b12od_shared
    PUBLIC
        $<INSTALL_INTERFACE:include>
)
set_target_properties(b12od_shared PROPERTIES OUTPUT_NAME "b12od")

install(FILES
    DESTINATION lib/cmake/b12od
    RENAME b12odConfig.cmake
)

install(
    TARGETS b12od
    EXPORT b12odTargets
)

install(
    EXPORT b12odTargets
    FILE b12odTargets.cmake
    NAMESPACE b12od::
    DESTINATION lib/cmake/b12od
)

file(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/lib/*.h")
install(DIRECTORY include/b12od DESTINATION include)

# 🔧 Add executable
file(GLOB_RECURSE EXE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bin/*.c"
)

add_executable(b12od_exe ${EXE_SOURCES})
target_include_directories(b12od_exe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(b12od_exe PRIVATE b12od)
set_target_properties(b12od_exe PROPERTIES OUTPUT_NAME "b12od")

add_executable(b12od_test ${SOURCES} ${EXE_SOURCES})
target_include_directories(b12od_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(b12od_test PRIVATE -D_MINIMUM_ALLOC_TESTING_)

add_test(NAME memory_single COMMAND valgrind --tool=memcheck $<TARGET_FILE:b12od_test> lno1qfqpge38tqmzyrdjj3x2qkdr5y80dlfw56ztq6yd9sme995g3gsxqqm0u2xq4dh3kdevrf4zg6hx8a60jv0gxe0ptgyfc6xkryqqqqqqqq9qc4r9wd6zqan9vd6x7unnzcss9mk8y3wkklfvevcrszlmu23kfrxh49px20665dqwmn4p72pksese)
add_test(NAME decode_single COMMAND $<TARGET_FILE:b12od_exe> lno1qfqpge38tqmzyrdjj3x2qkdr5y80dlfw56ztq6yd9sme995g3gsxqqm0u2xq4dh3kdevrf4zg6hx8a60jv0gxe0ptgyfc6xkryqqqqqqqq9qc4r9wd6zqan9vd6x7unnzcss9mk8y3wkklfvevcrszlmu23kfrxh49px20665dqwmn4p72pksese)

# Optional: install the executable
install(TARGETS b12od
    RUNTIME DESTINATION bin
)
