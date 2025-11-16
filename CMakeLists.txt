cmake_minimum_required(VERSION 3.19)
project("Thula" VERSION 1.0.0 LANGUAGES C CXX)

if(MSVC)
    # Use static runtime for both Debug and Release
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    
    # Or alternatively, set flags directly
    add_compile_options(
        "$<$<CONFIG:Debug>:/MTd>"
        "$<$<CONFIG:Release>:/MT>"
    )
endif()
include(${CMAKE_CURRENT_SOURCE_DIR}/scripts/cmake/LibraryTemplate.cmake)

macro(project_name)
    # Creates the name of the package from the directory name.
    string(REGEX REPLACE ".*/([^/]+)" "\\1" THIS_PACKAGE_NAME "${CMAKE_CURRENT_SOURCE_DIR}")
    project(${THIS_PACKAGE_NAME} VERSION 1.0.0 LANGUAGES C CXX)
endmacro()

enable_testing()
#############################################################
# Catch2 - PURE STATIC LIBRARY
#############################################################

# FORCE STATIC RUNTIME FOR CATCH2
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v2.13.8 # This version is important. 
                           # Newer releases don't work.
)

# FORCE STATIC LIBRARY
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CATCH_BUILD_STATIC_LIBRARY ON CACHE BOOL "" FORCE)
set(CATCH_BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(CATCH_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CATCH_INSTALL_DOCS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(Catch2)

# CREATE TARGETS
if(TARGET Catch2)   
    message(STATUS "Catch2 configured as static library with static runtime")
else()
    message(FATAL_ERROR "Catch2 target not found!")
endif()

#############################################################
# Build the project
#############################################################

set(SHARED_DIRS 
    BinaryCounter
    Counter
    Keyboard
    NCounter
    Quantizer
    ShiftRegister
)

foreach (project_dir ${SHARED_DIRS})
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${project_dir}/CMakeLists.txt")
        message("Generating: ${project_dir}")
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${project_dir})
    endif ()
endforeach ()

set(CMAKE_SUPPRESS_REGENERATION true)
