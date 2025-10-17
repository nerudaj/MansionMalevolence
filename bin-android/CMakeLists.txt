cmake_minimum_required ( VERSION 3.28 )

include ( ../cmake/vars.cmake )
include ( ../cmake/cpp-init.cmake )

bootstrap_cpm ()

set ( USE_PREBUILT_LIBRARIES OFF )
include ( ../cmake/dependencies.cmake )

project ( ${ANDROID_NATIVE_LIB_NAME} )

set ( CMAKE_CXX_STANDARD 23 )
set ( CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -u \
    Java_com_google_androidgamesdk_GameActivity_initializeNativeCode")
    
add_subdirectory ( ../lib "${CMAKE_CURRENT_BINARY_DIR}/lib" )
    
add_library(${PROJECT_NAME} SHARED
    Main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIB_TARGET_NAME} )

# The ANativeActivity_onCreate function from SFML::Main needs to be exposed in
# our libsfml-example.so file so that is can be loaded at runtime.
target_link_libraries(${PROJECT_NAME} PUBLIC
        -Wl,--whole-archive
        SFML::Main
        -Wl,--no-whole-archive
)