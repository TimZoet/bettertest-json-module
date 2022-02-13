find_package(nlohmann_json REQUIRED)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/../bettertest")
include(bettertestVersionString)

set(NAME bettertest_json)
set(TYPE module)
set(INCLUDE_DIR "include/bettertest_json")
set(SRC_DIR "src")

set(HEADERS
    ${INCLUDE_DIR}/json_exporter.h
    ${INCLUDE_DIR}/json_importer.h
)
 
set(SOURCES
    ${SRC_DIR}/json_exporter.cpp
    ${SRC_DIR}/json_importer.cpp
)

set(DEPS_PUBLIC
    bettertest
)

set(DEPS_PRIVATE
    nlohmann_json::nlohmann_json
)

make_target(
    NAME ${NAME}
    TYPE ${TYPE}
    VERSION ${BETTERTEST_VERSION}
    WARNINGS WERROR
    HEADERS "${HEADERS}"
    SOURCES "${SOURCES}"
    DEPS_PUBLIC "${DEPS_PUBLIC}"
    DEPS_PRIVATE "${DEPS_PRIVATE}"
)

install_target(
    NAME ${NAME}
    TYPE ${TYPE}
    VERSION ${BETTERTEST_VERSION}
    HEADERS "${HEADERS}"
)
