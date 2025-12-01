cmake_minimum_required(VERSION 3.15)
project(Bolt12OfferDecoderProject LANGUAGES C)

# Automatically collect sources
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/lib/*.c"
)

add_executable(b12od ${SOURCES})
target_include_directories(b12od
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
)

set_target_properties(b12od PROPERTIES
    COMPILE_FLAGS "-O3 -Wall"
    LINK_FLAGS "-s WASM=1 -s EXPORTED_FUNCTIONS='[\"_bolt12_offer_json_new\",\"_bolt12_json\",\"_bolt12_offer_json_delete\"]' \
                -s EXPORTED_RUNTIME_METHODS='[\"cwrap\",\"getValue\"]' \
		-s MODULARIZE=1 \
		-s EXPORT_NAME=B12OD"
)
