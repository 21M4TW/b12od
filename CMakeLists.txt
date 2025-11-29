cmake_minimum_required(VERSION 3.15)
project(Bolt12OfferDecoderProject LANGUAGES C)

include(CMakePackageConfigHelpers)
include(CTest)

option(BUILD_SHARED_LIBS "Build shared libraries instead of static ones" OFF)

# Automatically collect sources
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/lib/*.c"
)

add_library(bolt12_offer_decode STATIC ${SOURCES})
target_include_directories(bolt12_offer_decode
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/lib>
        $<INSTALL_INTERFACE:include>
)

add_library(bolt12_offer_decode_obj_pic OBJECT ${SOURCES})
set_target_properties(bolt12_offer_decode_obj_pic PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(bolt12_offer_decode_pic STATIC $<TARGET_OBJECTS:bolt12_offer_decode_obj_pic>)
target_include_directories(bolt12_offer_decode_pic
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/lib>
        $<INSTALL_INTERFACE:include>
)

add_library(bolt12_offer_decode_shared SHARED $<TARGET_OBJECTS:bolt12_offer_decode_obj_pic>)
target_include_directories(bolt12_offer_decode_shared
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/lib>
        $<INSTALL_INTERFACE:include>
)
set_target_properties(bolt12_offer_decode_shared PROPERTIES OUTPUT_NAME "bolt12_offer_decode")

install(FILES
    DESTINATION lib/cmake/bolt12_offer_decode
    RENAME bolt12_offer_decodeConfig.cmake
)

install(
    TARGETS bolt12_offer_decode
    EXPORT bolt12_offer_decodeTargets
)

install(
    EXPORT bolt12_offer_decodeTargets
    FILE bolt12_offer_decodeTargets.cmake
    NAMESPACE bolt12_offer_decode::
    DESTINATION lib/cmake/bolt12_offer_decode
)

file(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/lib/*.h")
install(FILES ${HEADERS} DESTINATION include)

# 🔧 Add executable
file(GLOB_RECURSE EXE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bin/*.c"
)

add_executable(test_bolt12_offer ${EXE_SOURCES})
add_test(test_bolt12_offer test_bolt12_offer lno1qfqpge38tqmzyrdjj3x2qkdr5y80dlfw56ztq6yd9sme995g3gsxqqm0u2xq4dh3kdevrf4zg6hx8a60jv0gxe0ptgyfc6xkryqqqqqqqq9qc4r9wd6zqan9vd6x7unnzcss9mk8y3wkklfvevcrszlmu23kfrxh49px20665dqwmn4p72pksese)

target_include_directories(test_bolt12_offer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/lib)
target_link_libraries(test_bolt12_offer PRIVATE bolt12_offer_decode)

# Optional: install the executable
install(TARGETS test_bolt12_offer
    RUNTIME DESTINATION bin
)
