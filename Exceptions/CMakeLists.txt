# project("Exceptions")

# add_library(${PROJECT_NAME}
#     ${PROJECT_NAME}.hpp
#     ${PROJECT_NAME}.cpp
# )

# # TODO: Find a new variable name.
# set(XXX ${SHARED_DIR}/${PROJECT_NAME})

# # Set include directories
# target_include_directories(${PROJECT_NAME} PUBLIC 
#     $<BUILD_INTERFACE:${XXX}>
#     $<INSTALL_INTERFACE:include>
# )

# # Create .lib file for Windows.
# if(WIN32)
#     set_target_properties(${PROJECT_NAME} PROPERTIES
#         OUTPUT_NAME ${PROJECT_NAME}
#     )
# endif()
