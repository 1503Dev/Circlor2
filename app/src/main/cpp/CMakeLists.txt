cmake_minimum_required(VERSION 3.22.1)
project("circlor2")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
include_directories(include)

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--version-script=${CMAKE_SOURCE_DIR}/exported_symbols.map")

find_package(bytehook REQUIRED CONFIG)
find_package(shadowhook REQUIRED CONFIG)

add_library(GlossHook STATIC IMPORTED)
set_target_properties(GlossHook PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/../jniLibs/arm64-v8a/libGlossHook.a)

add_library(${CMAKE_PROJECT_NAME} SHARED
        main.cpp
        mc/GuiData.cpp
        mc/ClientInstance.cpp
        mc/LocalPlayer.cpp
        mc/Player.cpp
        mc/Actor.cpp
        mc/GameMode.cpp
        global.cpp
        Circlor2.cpp
)
target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        log
        dl
        bytehook::bytehook
        shadowhook::shadowhook
        GlossHook
)