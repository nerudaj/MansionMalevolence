include ( "${CMAKE_CURRENT_LIST_DIR}/add-catch.cmake" )

set ( CXXOPTS_VERSION "3.3.1" )
set ( NLOHMANN_VERSION "3.12.0" )
set ( SFML_VERSION "3.0.0" )
set ( DGM_LIB_VERSION "main" )
set ( FSM_LIB_VERSION "2.1.1" )
set ( TGUI_VERSION "1.x" )
set ( CATCH2_VERSION "v3.8.0" )
set ( RANGEV3_VERSION "master" )
set ( FMTLIB_VERSION "12.1.0" )

CPMAddPackage("gh:jarro2783/cxxopts#v${CXXOPTS_VERSION}")
CPMAddPackage("gh:nlohmann/json#v${NLOHMANN_VERSION}")

set ( SFML_STATIC_LIBRARIES ${USE_SFML_TGUI_STATIC_LINKAGE} )
#CPMAddPackage("gh:SFML/SFML#${SFML_VERSION}")
CPMAddPackage("gh:nerudaj/SFML#feature-android-joystick-support")

set ( TGUI_BACKEND SFML_GRAPHICS )
set ( TGUI_STATIC_LIBRARIES ${USE_SFML_TGUI_STATIC_LINKAGE} )
CPMAddPackage("gh:texus/TGUI#${TGUI_VERSION}")

CPMAddPackage("gh:nerudaj/dgm-lib#${DGM_LIB_VERSION}")
# CPMAddPackage("gh:nerudaj/fsm-lib#v${FSM_LIB_VERSION}") # Not yet supported on Android
AddCatch( "${CATCH2_VERSION}" )

if ( "${CMAKE_SYSTEM_NAME}" STREQUAL "Android" )
    CPMAddPackage("gh:fmtlib/fmt#${FMTLIB_VERSION}")
    
    CPMAddPackage(
        NAME range-v3
        GITHUB_REPOSITORY ericniebler/range-v3
        GIT_TAG ${RANGEV3_VERSION}
        GIT_SUBMODULES "test"
    )
endif ()
