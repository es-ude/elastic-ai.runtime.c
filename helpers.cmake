include(FetchContent)

function(add_unity)
    FetchContent_Declare(
            unity
            GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
            GIT_TAG v2.5.2
    )
    FetchContent_Populate(unity)

    add_subdirectory(${unity_SOURCE_DIR})
endfunction()
