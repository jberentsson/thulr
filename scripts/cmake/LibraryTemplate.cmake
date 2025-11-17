macro(library_template PROJECT_LIBRARIES)
    #############################################################
    # Library Projecte Template
    #############################################################
    
    if(NOT DEFINED PROJECT_NAME)
        message(FATAL_ERROR "PROJECT_NAME must be defined before calling library_template")
    endif()

    #############################################################
    # LIBRARY TARGET
    #############################################################

    add_library(${PROJECT_NAME} STATIC 
        ${PROJECT_NAME}.cpp
        ${PROJECT_NAME}.hpp
    )

    # Force static runtime for MSVC
    if(MSVC)
        set_target_properties(${PROJECT_NAME} PROPERTIES
            MSVC_RUNTIME_LIBRARY "MultiThreaded"
        )
    endif()

    target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    # Link with Counter for dependent libraries
    if(NOT ${PROJECT_NAME} STREQUAL "Counter")
        target_link_libraries(${PROJECT_NAME} Counter)
    endif()

    if(DEFINED PROJECT_LIBRARIES AND PROJECT_LIBRARIES)
        foreach(PL ${PROJECT_LIBRARIES})
            target_link_libraries(${PROJECT_NAME} ${PL})
        endforeach()
    endif()

    set_target_properties(${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        POSITION_INDEPENDENT_CODE ON
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    #############################################################
    # TEST TARGET
    #############################################################

    set(TEST_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}_test.cpp
    )

    add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})

    # Catch2
    target_link_libraries(${PROJECT_NAME}_test PRIVATE Catch2::Catch2WithMain ${PROJECT_NAME})

    target_include_directories(${PROJECT_NAME}_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../
        #${CMAKE_CURRENT_SOURCE_DIR}/../../
        #${CMAKE_CURRENT_SOURCE_DIR}/../extern
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
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/tests
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/tests
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/tests
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/tests
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

    #############################################################
    # Done!
    #############################################################

endmacro()
