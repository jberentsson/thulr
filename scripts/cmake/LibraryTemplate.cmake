macro(library_template PROJECT_LIBRARIES)
    #############################################################
    # Input Validation & Setup
    #############################################################
    if(NOT DEFINED PROJECT_NAME)
        message(FATAL_ERROR "PROJECT_NAME must be defined before calling library_template.")
    endif()

    message(STATUS "Configuring library: ${PROJECT_NAME}")

    set(SOURCES 
        ${PROJECT_NAME}.cpp
        ${PROJECT_NAME}.hpp
    )

    #############################################################
    # SMART STATIC LIBRARY TARGET
    #############################################################
    
    add_library(${PROJECT_NAME}_static STATIC ${SOURCES})

    # Force static runtime for MSVC.
    if(MSVC)
        set_target_properties(${PROJECT_NAME}_static PROPERTIES
            MSVC_RUNTIME_LIBRARY "MultiThreaded"
        )
    endif()

    target_include_directories(${PROJECT_NAME}_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    # IMPROVED DEPENDENCY HANDLING - Handle missing targets gracefully
    if(DEFINED PROJECT_LIBRARIES AND PROJECT_LIBRARIES)
        foreach(PL ${PROJECT_LIBRARIES})
            # Skip if trying to link to itself
            if(PL STREQUAL ${PROJECT_NAME})
                message(WARNING "  Skipping self-link: ${PROJECT_NAME} -> ${PL}")
                continue()
            endif()
            
            # Check if target exists now, or add as interface dependency
            if(TARGET ${PL}_static)
                target_link_libraries(${PROJECT_NAME}_static PUBLIC ${PL}_static)
                message(STATUS "   Linked ${PROJECT_NAME}_static -> ${PL}_static")
            else()
                # Target doesn't exist yet, but might be created later
                # Add it as an interface dependency that will be resolved later
                message(STATUS "   Deferred linking: ${PROJECT_NAME}_static -> ${PL}_static (target not yet available)")
                # This will be resolved during the generation phase
                target_link_libraries(${PROJECT_NAME}_static PUBLIC ${PL}_static)
            endif()
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

    # Only create test executable if test source exists
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}_test.cpp)
        add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})

        # Link with the main library
        target_link_libraries(${PROJECT_NAME}_test
            Catch2::Catch2WithMain 
            ${PROJECT_NAME}_static
        )

        # CRITICAL: Also link test with all the same dependencies as the main library
        if(DEFINED PROJECT_LIBRARIES AND PROJECT_LIBRARIES)
            foreach(PL ${PROJECT_LIBRARIES})
                # Skip self-linking
                if(PL STREQUAL ${PROJECT_NAME})
                    continue()
                endif()
                
                # Link test with the same dependencies
                if(TARGET ${PL}_static)
                    target_link_libraries(${PROJECT_NAME}_test ${PL}_static)
                    message(STATUS "   Linked ${PROJECT_NAME}_test -> ${PL}_static")
                else()
                    # Even if target doesn't exist yet, add the dependency
                    target_link_libraries(${PROJECT_NAME}_test ${PL}_static)
                    message(STATUS "   Deferred test linking: ${PROJECT_NAME}_test -> ${PL}_static")
                endif()
            endforeach()
        endif()

        target_include_directories(${PROJECT_NAME}_test PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/../
            ${CMAKE_SOURCE_DIR}
        )

        if(MSVC)
            # Force static runtime for test executable.
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

        set(ADD_TESTS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../scripts/cmake/AddTests.cmake)
        
        if(EXISTS ${ADD_TESTS_PATH})
            include(${ADD_TESTS_PATH})
        else()
            message("The CMake config for the unit tests is not available.")
        endif()
    else()
        message(STATUS "   No test file found for ${PROJECT_NAME}")
    endif()

    message(STATUS " Successfully configured ${PROJECT_NAME}")

endmacro()
