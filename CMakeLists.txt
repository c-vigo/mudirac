cmake_minimum_required(VERSION 3.0)
project(MuDirac VERSION 1.0.1)

# The standard is C++ 17
set(CMAKE_CXX_STANDARD 17)

# Define the core files as static libraries
add_subdirectory(lib)

# Main executables
add_subdirectory(src bin)

# Now, test executables
enable_testing()
add_subdirectory(test)