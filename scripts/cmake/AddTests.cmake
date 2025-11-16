#############################################################
# AUTOMATIC TEST REGISTRATION WITH CTEST
#############################################################

enable_testing()

if(DEFINED PROJECT_LIBRARIES)
    foreach(PL ${PROJECT_LIBRARIES})
        message("-- Linking Project Libraries: ${PROJECT_LIBRARIES}")
        target_link_libraries(${PROJECT_NAME}_test ${PL})
    endforeach()
else()
    message("THERE ARE NO PROJECT LIBRARIES DEFINED!")
endif()

if(DEFINED PROJECT_LIBRARIES_SHARED)
    message("-- Linking Shared Project Libraries: ${PROJECT_LIBRARIES_SHARED}")
    target_link_libraries(${PROJECT_NAME}_test ${PROJECT_LIBRARIES_SHARED})
else()
    message("THERE ARE NO SHARED PROJECT LIBRARIES DEFINED!")
endif()

#############################################################
# TEST SETUP
#############################################################

add_test(
    NAME ${PROJECT_NAME}_test
    COMMAND $<TARGET_FILE:${PROJECT_NAME}_test>
)

set_tests_properties(${PROJECT_NAME}_test 
    PROPERTIES 
        TIMEOUT 30 
        LABELS "unit;shared"
)

message(STATUS "${PROJECT_NAME} test target created and registered with CTest")
