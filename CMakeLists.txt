cmake_minimum_required(VERSION 3.19)
cmake_policy(SET CMP0110 NEW)
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
add_executable(b12od_exe ${CMAKE_CURRENT_SOURCE_DIR}/src/bin/b12od.c)
target_include_directories(b12od_exe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(b12od_exe PRIVATE b12od)
set_target_properties(b12od_exe PROPERTIES OUTPUT_NAME "b12od")

add_executable(b12od_cmp ${CMAKE_CURRENT_SOURCE_DIR}/src/bin/b12od_cmp.c)
target_include_directories(b12od_cmp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(b12od_cmp PRIVATE b12od)

add_executable(b12od_test ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/src/bin/b12od_cmp.c)
target_include_directories(b12od_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(b12od_test PRIVATE -g -D_MINIMUM_ALLOC_TESTING_)

# Optional: install the executable
install(TARGETS b12od
    RUNTIME DESTINATION bin
)

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/tests/offer_tests.txt" testfile_content)
string(REGEX REPLACE "\n$" "" testfile_content "${testfile_content}")
string(REPLACE "\n" ";" test_lines "${testfile_content}")

foreach(line IN LISTS test_lines)
  string(REPLACE "|" ";" fields "${line}")
  list(GET fields 0 test_name)
  list(GET fields 1 test_string)
  list(GET fields 3 test_expected_result)
  add_test(NAME ${test_name} COMMAND $<TARGET_FILE:b12od_cmp> ${test_string} ${test_expected_result})
endforeach()

foreach(line IN LISTS test_lines)
  string(REPLACE "|" ";" fields "${line}")
  list(GET fields 0 test_name)
  list(GET fields 1 test_string)
  list(GET fields 3 test_expected_result)
  add_test(NAME "[memory] ${test_name}" COMMAND valgrind --track-origins=yes --leak-check=full $<TARGET_FILE:b12od_test> ${test_string} ${test_expected_result})
endforeach()
