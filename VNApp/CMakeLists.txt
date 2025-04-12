project(VNApp)

find_package(SDL2 CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)

file(GLOB_RECURSE sources src/*.cpp)
file(GLOB_RECURSE headers src/*.hpp)

include(CMakePrintHelpers)
cmake_print_variables(sources)

add_executable(${PROJECT_NAME} ${sources})

target_link_libraries(${PROJECT_NAME} PRIVATE VNEngine)

# copy dlls for app
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy -t 
    $<TARGET_FILE_DIR:${PROJECT_NAME}> $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
    COMMAND_EXPAND_LISTS
    )

#copy dlls for VNEngine
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy -t 
    $<TARGET_FILE_DIR:${PROJECT_NAME}> $<TARGET_RUNTIME_DLLS:VNEngine>
    COMMAND_EXPAND_LISTS
    )

add_custom_target(copy_resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/game"
    "${CMAKE_CURRENT_BINARY_DIR}/game"
    COMMENT "copying resources and lua files"
    )
add_dependencies(${PROJECT_NAME} copy_resources)