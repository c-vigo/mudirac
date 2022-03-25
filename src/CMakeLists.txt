configure_file(mudirac.in.hpp ${CMAKE_CURRENT_SOURCE_DIR}/mudirac.hpp)
add_executable(mudirac mudirac.cpp)
target_link_libraries(mudirac mudiraclib)

# Install rules
install(
        TARGETS mudirac
        DESTINATION bin
)

install(
        FILES mudirac.hpp
        DESTINATION include
)

