set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")

set(INCLUDE_DIR "${ROOT_DIR}/include")
set(HEADER_FILES
        "${INCLUDE_DIR}/sharlibs/dylib.hpp")

add_library(
        "${PROJECT_NAME}" STATIC
        ${HEADER_FILES}
        ${SOURCE_FILES}
        "${ROOT_DIR}/.clang-format"
        "${ROOT_DIR}/.clang-tidy")

add_library("sharlibs::sharlibs" ALIAS "${PROJECT_NAME}")

set_target_properties("${PROJECT_NAME}" PROPERTIES DEBUG_POSTFIX "d")
set_target_properties("${PROJECT_NAME}" PROPERTIES LINKER_LANGUAGE CXX)

target_compile_features("${PROJECT_NAME}" PUBLIC cxx_std_20)

target_compile_options(
        "${PROJECT_NAME}"
        PRIVATE "$<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-fvisibility=hidden>"
        "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

target_compile_options("${PROJECT_NAME}"
        PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>")

target_include_directories(
        "${PROJECT_NAME}" SYSTEM
        PUBLIC "$<BUILD_INTERFACE:${INCLUDE_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
        PRIVATE "$<BUILD_INTERFACE:${INCLUDE_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

install(TARGETS "${PROJECT_NAME}" EXPORT "${PROJECT_NAME}-targets")

install(
        EXPORT "${PROJECT_NAME}-targets"
        NAMESPACE "sharlibs::"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

configure_file("${ROOT_DIR}/cmake/config.cmake.in"
        "${PROJECT_NAME}Config.cmake" @ONLY)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        COMPATIBILITY SameMajorVersion)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

install(DIRECTORY "${INCLUDE_DIR}/sharlibs"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
