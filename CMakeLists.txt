
# NOTE: Requires MSVC C++ compiler via Build Tools for Visual Studio 2019
# cmake .. -G "Visual Studio 16 2019" -A x64
# cmake --build . --config Release

cmake_minimum_required(VERSION 3.16)
project(imascs LANGUAGES CXX)

# sources
file(GLOB SOURCES src/*.cc)
add_executable(${PROJECT_NAME} ui.manifest ui.rc ${SOURCES})

# includes
target_include_directories(${PROJECT_NAME}
    PRIVATE src
    PRIVATE res)

# subdirectories
add_subdirectory(src/core)
add_subdirectory(src/capture)

# compiler options
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} PRIVATE imascs.core imascs.capture windowsapp D3D11)

# UAC requireAdministrator configured via ui.manifest
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS
    "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\" /SUBSYSTEM:WINDOWS")

# version
set(UI_VERSION_MAJOR "0")
set(UI_VERSION_MINOR "0")
set(UI_VERSION_PATCH "1")
set(UI_VERSION "${UI_VERSION_MAJOR}.${UI_VERSION_MINOR}.${UI_VERSION_PATCH}")

# manifest authoring
configure_file(res/ui.manifest.in ${PROJECT_BINARY_DIR}/ui.manifest)
# binary versioning
configure_file(res/ui.rc.in ${PROJECT_BINARY_DIR}/ui.rc)
configure_file(res/icon.ico ${PROJECT_BINARY_DIR}/icon.ico COPYONLY)
configure_file(res/resource.h ${PROJECT_BINARY_DIR}/resource.h COPYONLY)

# check Windows 10 SDK compatiblity
string(REPLACE "." ";" WINAPI_VER "${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}")
list(GET WINAPI_VER 0 WINAPI_VER_MAJOR)
list(GET WINAPI_VER 1 WINAPI_VER_MINOR)
list(GET WINAPI_VER 2 WINAPI_VER_BUILD)
if(WINAPI_VER_MAJOR EQUAL 10)
    if(WINAPI_VER_MINOR EQUAL 0)
        if(WINAPI_VER_BUILD GREATER_EQUAL 19041)
            set(WINAPI_COMPATIBLE TRUE)
        endif()
    else()
        set(WINAPI_COMPATIBLE TRUE)
    endif()
elseif(WINAPI_VER_MAJOR GREATER 10)
    set(WINAPI_COMPATIBLE TRUE)
endif()
if(NOT WINAPI_COMPATIBLE)
    message(FATAL_ERROR "Requires Windows 10 SDK 10.0.19041.0 or above")
endif()
