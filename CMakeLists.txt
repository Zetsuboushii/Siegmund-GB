cmake_minimum_required(VERSION 3.28)
project(siegmund_gb C)

set(CMAKE_C_STANDARD 99)

include_directories(${PROJECT_SOURCE_DIR}/common/include)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/common/src)
include_directories(${PROJECT_SOURCE_DIR}/common/lib)
include_directories(${PROJECT_SOURCE_DIR}/gbdk/include)

add_library(common STATIC ${PROJECT_SOURCE_DIR}/common/src/ZGBMain_Init.c
        src/StateGame.c
        src/ZGBMain.c
        src/SpritePlayer.c
        src/SpriteAttack.c
        src/SpriteSkeleton.c)

add_executable(siegmund_gb src/ZGBMain.c)
target_link_libraries(siegmund_gb common)
