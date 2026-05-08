set(ENJOYER2D_IMGUI_SOURCES
    "${ENJOYER2D_IMGUI_DIR}/imgui.cpp"
    "${ENJOYER2D_IMGUI_DIR}/imgui_draw.cpp"
    "${ENJOYER2D_IMGUI_DIR}/imgui_tables.cpp"
    "${ENJOYER2D_IMGUI_DIR}/imgui_widgets.cpp"
    "${ENJOYER2D_IMGUI_DIR}/backends/imgui_impl_sdl3.cpp"
    "${ENJOYER2D_IMGUI_DIR}/backends/imgui_impl_sdlrenderer3.cpp"
)

foreach(ENJOYER2D_IMGUI_SOURCE IN LISTS ENJOYER2D_IMGUI_SOURCES)
    if(NOT EXISTS "${ENJOYER2D_IMGUI_SOURCE}")
        message(FATAL_ERROR
            "Dear ImGui source file not found: ${ENJOYER2D_IMGUI_SOURCE}\n"
            "Dear ImGui must be vendored under vendor/imgui."
        )
    endif()
endforeach()

add_library(enjoyer2d_imgui STATIC ${ENJOYER2D_IMGUI_SOURCES})

target_include_directories(enjoyer2d_imgui
    PUBLIC
        "${ENJOYER2D_IMGUI_DIR}"
        "${ENJOYER2D_IMGUI_DIR}/backends"
)

target_link_libraries(enjoyer2d_imgui
    PUBLIC
        SDL3::SDL3
)
