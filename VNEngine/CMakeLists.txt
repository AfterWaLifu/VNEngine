project(VNEngine)

file(GLOB_RECURSE sources src/*.cpp)
file(GLOB_RECURSE headers include/*.hpp)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(SndFile CONFIG REQUIRED)
find_package(Lua REQUIRED)
find_package(tinyxml2 CONFIG REQUIRED)

# luabridge is headers-only
find_path(LUABRIDGE_INCLUDE_DIRS "LuaBridge/Array.h")

add_library(${PROJECT_NAME} SHARED ${sources})

target_compile_definitions(${PROJECT_NAME} PRIVATE BUILDING_LIB)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_include_directories(${PROJECT_NAME} PRIVATE 
        ${LUA_INCLUDE_DIR}
        ${LUABRIDGE_INCLUDE_DIRS}
    )

target_link_libraries(${PROJECT_NAME}
        # well known SDL feature
        PUBLIC
        SDL2::SDL2main
        SDL2::SDL2
        
        PRIVATE
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf

        OpenAL::OpenAL
        SndFile::sndfile
        ${LUA_LIBRARIES}
        tinyxml2::tinyxml2
    )