cmake_minimum_required(VERSION 3.22.1)
project("circlor")

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

        mc/client/ClientInstance.cpp
        mc/client/gui/GuiData.cpp
        mc/entity/actor/Actor.cpp
        mc/entity/actor/player/LocalPlayer.cpp
        mc/entity/actor/player/Player.cpp
        mc/GameMode.cpp
        mc/level/BlockPos.cpp
        mc/math/Vec3.cpp

        HookManager.cpp
        global.cpp
        Circlor.cpp
        JNI_mc.cpp
)
target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        log
        dl
        bytehook::bytehook
        shadowhook::shadowhook
        GlossHook
)