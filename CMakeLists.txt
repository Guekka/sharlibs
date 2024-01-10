cmake_minimum_required(VERSION 3.19)

project(
        sharlibs
        VERSION 0.1.0
        LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

option(SHARLIBS_BUILD_SRC "whether we should build the library itself" ON)
if("${SHARLIBS_BUILD_SRC}")
    add_subdirectory(src)
endif()

option(SHARLIBS_BUILD_EXAMPLES "whether we should build the examples" OFF)
if("${SHARLIBS_BUILD_EXAMPLES}")
    add_subdirectory(examples)
endif()

if("${BUILD_TESTING}")
    include(CTest)
    add_subdirectory(tests)
endif()

option(SHARLIBS_BUILD_DOCS "whether we should build documentation" OFF)
if("${SHARLIBS_BUILD_DOCS}")
    add_subdirectory(docs)
endif()
