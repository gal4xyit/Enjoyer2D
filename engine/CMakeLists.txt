add_library(enjoyer2d_engine INTERFACE
        include/Enjoyer2D/Body.hpp
        include/Enjoyer2D/World.hpp
        include/Enjoyer2D/Shape.hpp
        include/Enjoyer2D/AABB.hpp
        include/Enjoyer2D/Collision.hpp
        include/Enjoyer2D/Contact.hpp
        include/Enjoyer2D/WorldStats.hpp)

target_include_directories(enjoyer2d_engine
        INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(enjoyer2d_engine
        INTERFACE
        glm::glm
)
