#############################################################
# AUTOMATIC TEST REGISTRATION WITH CTEST
#############################################################

enable_testing()

if(DEFINED PROJECT_LIBRARIES)
    foreach(PL ${PROJECT_LIBRARIES})
        message("-- Linking Project Libraries: ${PROJECT_LIBRARIES}")
        if(TARGET ${PL}_static)
            target_link_libraries(${PROJECT_NAME}_test ${PL}_static)
        else()
            message(WARNING "Library target ${PL}_static not found for ${PROJECT_NAME}_test")
        endif()
    endforeach()
else()
    message("THERE ARE NO PROJECT LIBRARIES DEFINED!")
endif()

if(DEFINED PROJECT_LIBRARIES_SHARED)
    foreach(PL ${PROJECT_LIBRARIES_SHARED})
        message("-- Linking Shared Project Libraries: ${PROJECT_LIBRARIES_SHARED}")
        if(TARGET ${PL}_static)
            target_link_libraries(${PROJECT_NAME}_test ${PL}_static)
        else()
            message(WARNING "Shared library target ${PL}_static not found for ${PROJECT_NAME}_test")
        endif()
    endforeach()
else()
    message("THERE ARE NO SHARED PROJECT LIBRARIES DEFINED!")
endif()

#############################################################
# TEST SETUP - FIXED FOR tests/ DIRECTORY
#############################################################
include(CTest)

# Get the test executable output path - respect the tests/ directory
get_target_property(TEST_OUTPUT_DIR ${PROJECT_NAME}_test RUNTIME_OUTPUT_DIRECTORY)

# If RUNTIME_OUTPUT_DIRECTORY is not set, use the default tests directory
if(NOT TEST_OUTPUT_DIR)
    set(TEST_OUTPUT_DIR ${CMAKE_BINARY_DIR}/tests)
else()
    # Ensure we use the directory where tests are actually built
    message(STATUS "Test output directory detected: ${TEST_OUTPUT_DIR}")
endif()

# Use the full path to the test executable in the tests/ directory
add_test(
    NAME ${PROJECT_NAME}_test
    COMMAND ${TEST_OUTPUT_DIR}/${PROJECT_NAME}_test
)

# Set test properties with the correct working directory
set_tests_properties(${PROJECT_NAME}_test 
    PROPERTIES 
        TIMEOUT 30 
        LABELS "unit;shared"
        WORKING_DIRECTORY ${TEST_OUTPUT_DIR}
)
