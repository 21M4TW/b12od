cmake_minimum_required(VERSION 3.15)
project(Bolt12OfferDecoderProject LANGUAGES C)

include(CMakePackageConfigHelpers)

option(BUILD_SHARED_LIBS "Build shared libraries instead of static ones" ON)

# Automatically collect sources
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/lib/*.c"
)

add_library(bolt12_offer_decode ${SOURCES})

target_include_directories(bolt12_offer_decode
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/lib>
        $<INSTALL_INTERFACE:include>
)

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

set_target_properties(bolt12_offer_decode PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin"
)

file(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/lib/*.h")
install(FILES ${HEADERS} DESTINATION include)

# 🔧 Add executable
file(GLOB_RECURSE EXE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bin/*.c"
)

add_executable(test_bolt12_offer ${EXE_SOURCES})

target_include_directories(test_bolt12_offer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/lib)
target_link_libraries(test_bolt12_offer PRIVATE bolt12_offer_decode)

# Optional: install the executable
install(TARGETS test_bolt12_offer
    RUNTIME DESTINATION bin
)
