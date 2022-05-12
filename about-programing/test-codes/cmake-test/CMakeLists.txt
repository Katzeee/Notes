project(cmaketest)
aux_source_directory(. SOURCE)
message(STATUS "All sources are" ${SOURCE})

add_executable(test test.cpp test.h)

set_target_properties(test 
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
)


get_target_property(A test RUNTIME_OUTPUT_DIRECTORY)
get_target_property(B test RUNTIME_OUTPUT_DIRECTORY_DEBUG)
get_target_property(C test RUNTIME_OUTPUT_DIRECTORY_RELEASE)
message(${PROJECT_BINARY_DIR})
message(${A})
message(${B})
message(${C})