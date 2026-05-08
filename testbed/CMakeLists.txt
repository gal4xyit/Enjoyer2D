set(ENJOYER2D_TESTBED_MAIN "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

if(EXISTS "${ENJOYER2D_TESTBED_MAIN}")
    add_executable(enjoyer2d_testbed
            "${ENJOYER2D_TESTBED_MAIN}"
            src/app/TestbedState.hpp
            src/rendering/View.hpp
            src/rendering/Renderer.cpp
            src/rendering/Renderer.hpp
            src/scenes/Scenes.cpp
            src/scenes/Scenes.hpp
            src/app/InputState.hpp
            src/rendering/DebugRenderSettings.hpp
            src/rendering/DebugState.hpp
            src/scenes/SceneState.hpp
            src/editor/Editor.cpp
            src/editor/Editor.hpp
            src/app/TestbedActions.cpp
            src/app/TestbedActions.hpp
            src/editor/TestbedUi.cpp
            src/editor/TestbedUi.hpp
            src/app/InputHandler.cpp
            src/app/InputHandler.hpp
            src/app/TestbedUpdate.cpp
            src/app/TestbedUpdate.hpp
            src/app/TestbedApp.cpp
            src/app/TestbedApp.hpp
    )

    target_include_directories(enjoyer2d_testbed
        PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/src"
    )

    target_link_libraries(enjoyer2d_testbed
        PRIVATE
            enjoyer2d_engine
            enjoyer2d_imgui
            SDL3::SDL3
            glm::glm
    )

    if(WIN32)
        add_custom_command(TARGET enjoyer2d_testbed POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:enjoyer2d_testbed>
                $<TARGET_FILE_DIR:enjoyer2d_testbed>
            COMMAND_EXPAND_LISTS
        )
    endif()
else()
    message(STATUS "Skipping enjoyer2d_testbed: testbed/src/main.cpp does not exist yet.")
endif()
