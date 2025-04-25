function(set_target_info)
    if(WIN32)
        set(TARGET_OS "windows" PARENT_SCOPE)
        set(TARGET_ARCH "x64" PARENT_SCOPE)
        set(VCPKG_TARGET_TRIPLET "x64-windows" PARENT_SCOPE)
    elseif(APPLE)
        set(TARGET_OS "osx" PARENT_SCOPE)
        set(TARGET_ARCH "arm64" PARENT_SCOPE)
        set(VCPKG_TARGET_TRIPLET "arm64-osx" PARENT_SCOPE)
    elseif(UNIX AND NOT APPLE)
        set(TARGET_OS "linux" PARENT_SCOPE)
        set(TARGET_ARCH "x64" PARENT_SCOPE)
        set(VCPKG_TARGET_TRIPLET "x64-linux" PARENT_SCOPE)
    endif()
endfunction()

cmake_minimum_required(VERSION 3.15)


# Set the toolchain file using the environment variable
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
else()
    message(FATAL_ERROR "VCPKG_ROOT environment variable is not set")
endif()

set_target_info()
set(VCPKG_TARGET_TRIPLET "${TARGET_ARCH}-${TARGET_OS}" CACHE STRING "")
set(VCPKG_MANIFEST_DIR ${CMAKE_SOURCE_DIR})

project(sfsim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(sfsim ${SOURCES})

add_compile_options(-Wall -Werror)

find_package(SFML COMPONENTS Network Graphics Window Audio System CONFIG REQUIRED)
target_link_libraries(sfsim PRIVATE SFML::Network SFML::Graphics SFML::Window SFML::Audio SFML::System)
