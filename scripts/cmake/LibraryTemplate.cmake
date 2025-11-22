macro(library_template PROJECT_LIBRARIES)
    if(NOT DEFINED PROJECT_NAME)
        message(FATAL_ERROR "PROJECT_NAME must be defined before calling library_template")
    endif()

    set(SOURCES 
        ${PROJECT_NAME}.cpp
        ${PROJECT_NAME}.hpp
    )

    #############################################################
    # SINGLE STATIC LIBRARY TARGET
    #############################################################
    
    # Create just ONE static library
    add_library(${PROJECT_NAME}_static STATIC ${SOURCES})

    # Force static runtime for MSVC
    if(MSVC)
        set_target_properties(${PROJECT_NAME}_static PROPERTIES
            MSVC_RUNTIME_LIBRARY "MultiThreaded"
        )
    endif()

    target_include_directories(${PROJECT_NAME}_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    # Link dependencies - use _static suffix for all library dependencies
    #if(NOT ${PROJECT_NAME} STREQUAL "Counter")
    #    target_link_libraries(${PROJECT_NAME}_static PUBLIC Counter_static)
    #endif()

    if(DEFINED PROJECT_LIBRARIES AND PROJECT_LIBRARIES)
        foreach(PL ${PROJECT_LIBRARIES})
            target_link_libraries(${PROJECT_NAME}_static PUBLIC ${PL}_static)
        endforeach()
    endif()

    set_target_properties(${PROJECT_NAME}_static PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        POSITION_INDEPENDENT_CODE ON
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    #############################################################
    # TEST EXECUTABLE
    #############################################################

    set(TEST_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}_test.cpp
    )

    add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})

    # Link test executable with ALL required libraries
    target_link_libraries(${PROJECT_NAME}_test PRIVATE 
        Catch2::Catch2WithMain 
        ${PROJECT_NAME}_static
    )

    # Also link with base libraries that the test might need directly
    if(NOT ${PROJECT_NAME} STREQUAL "Counter")
        target_link_libraries(${PROJECT_NAME}_test PRIVATE Counter_static)
    endif()

    if(DEFINED PROJECT_LIBRARIES AND PROJECT_LIBRARIES)
        foreach(PL ${PROJECT_LIBRARIES})
            target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PL}_static)
        endforeach()
    endif()

    target_include_directories(${PROJECT_NAME}_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../
        ${CMAKE_PROJECT_ROOT}/source
    )

    # Force static runtime for test executable
    if(MSVC)
        set_target_properties(${PROJECT_NAME}_test PROPERTIES
            MSVC_RUNTIME_LIBRARY "MultiThreaded"
        )
    endif()

    set_target_properties(${PROJECT_NAME}_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    #############################################################
    # Unit Tests
    #############################################################

    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../scripts/cmake/AddTests.cmake)
        include(${CMAKE_CURRENT_SOURCE_DIR}/../../scripts/cmake/AddTests.cmake)
    else()
        message("The CMake config for the unit tests is not available.")
    endif()

endmacro()
