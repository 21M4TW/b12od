cmake_minimum_required(VERSION 3.19)
cmake_policy(SET CMP0110 NEW)
project(Bolt12OfferDecoderProject LANGUAGES C)

include(CMakePackageConfigHelpers)

# Normalize build type for single-config generators
if(CMAKE_BUILD_TYPE)
  string(TOUPPER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_UPPER)
  set(EFFECTIVE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_${BUILD_TYPE_UPPER}}")
else()
  # For multi-config generators, use generator expressions
  set(EFFECTIVE_C_FLAGS
    "$<IF:$<CONFIG:Debug>,${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG},
    $<IF:$<CONFIG:Release>,${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE},
    $<IF:$<CONFIG:RelWithDebInfo>,${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELWITHDEBINFO},
    $<IF:$<CONFIG:MinSizeRel>,${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_MINSIZEREL},
    ${CMAKE_C_FLAGS}> >"
  )
endif()
include(CMakeLists.core.txt)
include(CMakeLists.bindings.txt)
include(CMakeLists.python.txt)
include(CMakeLists.rust.txt)
include(CMakeLists.wasm.txt)

add_custom_target(all_bindings)
add_dependencies(all_bindings python_binding rust_binding wasm_binding)

