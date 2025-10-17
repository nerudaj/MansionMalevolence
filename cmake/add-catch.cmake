macro ( AddCatch CATCH2_VERSION )
    set ( CATCH2_FOLDER "${CMAKE_CURRENT_BINARY_DIR}/_deps/catch2" )
    make_directory( "${CATCH2_FOLDER}/include" )

    file ( DOWNLOAD
        https://github.com/catchorg/Catch2/releases/download/${CATCH2_VERSION}/catch_amalgamated.hpp
        "${CATCH2_FOLDER}/include/catch_amalgamated.hpp"
    )

    file ( DOWNLOAD
        https://github.com/catchorg/Catch2/releases/download/${CATCH2_VERSION}/catch_amalgamated.cpp
        "${CATCH2_FOLDER}/catch_amalgamated.cpp"
    )

    add_library ( catch2 STATIC
        "${CATCH2_FOLDER}/include/catch_amalgamated.hpp"
        "${CATCH2_FOLDER}/catch_amalgamated.cpp"
    )

    target_include_directories( catch2 PUBLIC "${CATCH2_FOLDER}/include" )
endmacro ()