file(GLOB ENJOYER2D_TEST_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

if(ENJOYER2D_TEST_SOURCES)
    add_executable(enjoyer2d_tests
            ${ENJOYER2D_TEST_SOURCES}
    )

    target_link_libraries(enjoyer2d_tests
            PRIVATE
            enjoyer2d_engine
            Catch2::Catch2WithMain
            glm::glm
    )

    list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")
    include(Catch)
    catch_discover_tests(enjoyer2d_tests)
else()
    message(STATUS "Skipping enjoyer2d_tests: no tests/*.cpp files exist yet.")
endif()
