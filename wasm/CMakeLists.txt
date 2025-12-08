cmake_minimum_required(VERSION 3.19)
cmake_policy(SET CMP0110 NEW)
project(Bolt12OfferDecoderProject LANGUAGES C)

include(CMakePackageConfigHelpers)

# Automatically collect sources
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/b12od-core/src/lib/*.c"
)

add_executable(wasm_binding ${SOURCES})
target_include_directories(wasm_binding
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/b12od-core/include>
)

set_target_properties(wasm_binding PROPERTIES
    COMPILE_FLAGS "-O3 -Wall"
    LINK_FLAGS "-s WASM=1 -s EXPORTED_FUNCTIONS='[\"_bolt12_offer_json_new\",\"_bolt12_json\",\"_bolt12_offer_json_delete\",\"_bolt12_offer_json_shrink_to_fit\",\"_bolt12_offer_json_get_size\"]' \
    		-s EXPORTED_RUNTIME_METHODS='[\"cwrap\",\"getValue\",\"HEAPU8\"]' \
		-s MODULARIZE=1 \
		-s EXPORT_NAME=B12OD"
)
set_target_properties(wasm_binding PROPERTIES OUTPUT_NAME "b12od")

install(TARGETS wasm_binding DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/b12od.wasm DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
