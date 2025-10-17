cmake_minimum_required ( VERSION 3.26 )

include ( "cmake/vars.cmake" )
include ( "cmake/cpp-init.cmake" )

cpp_init()
get_version_from_git (
    PROJECT_VERSION_VARIABLE GIT_PROJECT_VERSION
    FULL_VERSION_VARIABLE FULL_PROJECT_VERSION
)

if ( "${CMAKE_SYSTEM_NAME}" STREQUAL "Android" )
    set ( BUILDING_ANDROID ON )
else ()
    set ( BUILDING_ANDROID OFF )
endif ()

project ( ${THE_PROJECT_NAME} VERSION ${GIT_PROJECT_VERSION} )

option ( BUILD_TESTS "Build unit testing target" ON )
option ( USE_NSIS "Use NSIS for packaging" OFF )

set ( OUTPUT_FILE_NAME "${THE_PROJECT_NAME}-v${CMAKE_PROJECT_VERSION}" )

if ( ${BUILDING_ANDROID} AND ${USE_NSIS} )
    message ( FATAL_ERROR "Cannot use NSIS for Android build! Exiting." )
endif ()

if ( ${BUILDING_ANDROID} )
    message ( "Configuring for Android build" )

    set ( CURRENT_USER_NAME $ENV{USERNAME} )

    message ( "Detected current user: ${CURRENT_USER_NAME}" )

    make_directory ( "${CMAKE_CURRENT_BINARY_DIR}/app/src/main" )

    file (
        COPY
            "${CMAKE_CURRENT_SOURCE_DIR}/android/gradle/gradle"
            "${CMAKE_CURRENT_SOURCE_DIR}/android/gradle/build.gradle.kts"
            "${CMAKE_CURRENT_SOURCE_DIR}/android/gradle/gradle.properties"
            "${CMAKE_CURRENT_SOURCE_DIR}/android/gradle/gradlew"
            "${CMAKE_CURRENT_SOURCE_DIR}/android/gradle/gradlew.bat"
            "${CMAKE_CURRENT_SOURCE_DIR}/android/gradle/settings.gradle.kts"
        DESTINATION
            ${CMAKE_CURRENT_BINARY_DIR}
    )
    
    file (
        COPY
            "${CMAKE_CURRENT_SOURCE_DIR}/android/res"
        DESTINATION
            "${CMAKE_CURRENT_BINARY_DIR}/app/src/main"
    )

    configure_file (
        "${CMAKE_CURRENT_SOURCE_DIR}/android/infiles/local.properties.in"
        "${CMAKE_CURRENT_BINARY_DIR}/local.properties"
    )

    configure_file (
        "${CMAKE_CURRENT_SOURCE_DIR}/android/infiles/build.gradle.kts.in"
        "${CMAKE_CURRENT_BINARY_DIR}/app/build.gradle.kts"
    )
    
    configure_file (
        "${CMAKE_CURRENT_SOURCE_DIR}/android/infiles/proguard-rules.pro.in"
        "${CMAKE_CURRENT_BINARY_DIR}/app/proguard-rules.pro"
    )

    configure_file (
        "${CMAKE_CURRENT_SOURCE_DIR}/android/infiles/AndroidManifest.xml.in"
        "${CMAKE_CURRENT_BINARY_DIR}/app/src/main/AndroidManifest.xml"
    )
    
    install (
        FILES
            "${CMAKE_CURRENT_BINARY_DIR}/app/build/outputs/apk/$<CONFIG>/${OUTPUT_FILE_NAME}.apk"
            "${CMAKE_CURRENT_BINARY_DIR}/app/build/outputs/apk/$<CONFIG>/output-metadata.json"
        DESTINATION
            "."
    )
    
    message ("Use the following command to build the APK: gradlew build" )
else ()
    message ("Configuring for Windows build")

    set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Compiled" )

    include ( "cmake/dependencies.cmake" )

    add_subdirectory ( "lib" )
    add_subdirectory ( "bin-windows" )

    if ( ${BUILD_TESTS} )
        enable_testing()
        add_subdirectory ( "tests" )
    endif ()
    
    install (
        DIRECTORY   "assets"
        DESTINATION "."
    )
endif ()

if ( ${USE_NSIS} )
    include ( InstallRequiredSystemLibraries )
    set ( CPACK_GENERATOR "NSIS" )
    cmake_path ( CONVERT "C:/Games" TO_NATIVE_PATH_LIST CPACK_NSIS_INSTALL_ROOT )
    set ( CPACK_NSIS_MODIFY_PATH "OFF")
	set ( CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL "ON" )
	set ( CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/assets-private/appicon.ico" )
	set ( CPACK_NSIS_URL_INFO_ABOUT "${INSTALLER_ABOUT_URL}" )
    
    set ( CPACK_NSIS_DISPLAY_NAME "${THE_PROJECT_NAME}" )
    set ( CPACK_NSIS_PACKAGE_NAME "${THE_PROJECT_NAME}" )
    set ( CPACK_PACKAGE_INSTALL_DIRECTORY "${THE_PROJECT_NAME}" )
    set ( CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${THE_PROJECT_NAME}" )
    
    # Create desktop shortcut pointing to correct location
    list ( APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "  SetOutPath '$INSTDIR\\bin'" )
    list ( APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "  CreateShortcut '$DESKTOP\\${THE_PROJECT_NAME}.lnk' '$INSTDIR\\bin\\${THE_PROJECT_NAME}.exe'" )
    list ( APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "  CreateShortcut '$SMPROGRAMS\\$STARTMENU_FOLDER\\${THE_PROJECT_NAME}.lnk' '$INSTDIR\\bin\\${THE_PROJECT_NAME}.exe'" )
    string (REPLACE ";" "\n" CPACK_NSIS_EXTRA_INSTALL_COMMANDS "${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}")
else ()
    set ( CPACK_GENERATOR "ZIP" )
endif ()
set ( CPACK_PACKAGE_FILE_NAME "${OUTPUT_FILE_NAME}" )

include ( CPack )
