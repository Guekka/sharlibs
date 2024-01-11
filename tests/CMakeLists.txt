set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")

set(INCLUDE_DIR "${ROOT_DIR}/tests")

set(SOURCE_DIR "${ROOT_DIR}/tests")
set(SOURCE_FILES
        "${SOURCE_DIR}/dynamic_library.cpp"
        "${SOURCE_DIR}/metaprogramming.cpp")

add_executable(tests ${SOURCE_FILES})

target_compile_options(
        tests PRIVATE "$<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-fvisibility=hidden>"
        "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

target_include_directories(tests PRIVATE "${INCLUDE_DIR}" "${SOURCE_DIR}")

target_link_libraries(tests PRIVATE ${PROJECT_NAME} Catch2::Catch2WithMain)

target_compile_options(tests
        PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>")

find_package(Catch2 CONFIG REQUIRED)
include(Catch)
catch_discover_tests(tests)

macro(acquire_test NAME)
    file(ARCHIVE_EXTRACT INPUT "${ROOT_DIR}/data/${NAME}.7z" DESTINATION
            "${CMAKE_CURRENT_BINARY_DIR}/${NAME}")
endmacro()
