# CMakeLists to add a static library

# Get library name from current path
get_filename_component(LIBNAME ${CMAKE_CURRENT_LIST_DIR} NAME)
message(STATUS "Building library ${LIBNAME}")

# Find library files and directories
file( GLOB LIBSOURCES ${CMAKE_CURRENT_LIST_DIR}/source/*.cpp)
file( GLOB LIBHEADERS ${CMAKE_CURRENT_LIST_DIR}/include/*.h)
set(LIB_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)

# Create library.
add_library(
        ${LIBNAME}
        STATIC
        ${LIBSOURCES}
)

# Include the library header files, and maybe some others
target_include_directories(${LIBNAME} PUBLIC ${LIB_INCLUDE})

# Set the properties you require, e.g. what C++ standard to use. Here applied to library and main (change as needed).
set_target_properties(
        ${LIBNAME}
        PROPERTIES
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

# Install rules
install(
        TARGETS ${LIBNAME}
        DESTINATION lib
)

install(
        FILES ${LIBHEADERS}
        DESTINATION include
)