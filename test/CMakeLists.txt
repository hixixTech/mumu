# We need thread support
find_package(Threads REQUIRED)

# Enable ExternalProject CMake module
include(ExternalProject)

# Download and install GoogleTest
ExternalProject_Add(
    gtest
    URL https://github.com/google/googletest/archive/master.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    INSTALL_COMMAND ""
)

# Get GTest source and binary directories from CMake project
ExternalProject_Get_Property(gtest source_dir binary_dir)


link_directories("${binary_dir}/googlemock/gtest/Debug/" "${binary_dir}/googlemock/Debug/")

# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
include_directories("${source_dir}/googletest/include"
                    "${source_dir}/googlemock/include")
file(GLOB SRCS ./src/*.cpp)
file(GLOB Main_SRCS ../src/*)
file(GLOB Test_SRCS ./src/testSrc/*)
source_group("MainSrc" FILES ${Main_SRCS})
source_group("testSrc" FILES ${Test_SRCS})
include_directories(../src/)
set(general_path "${CMAKE_CURRENT_BINARY_DIR}/mumu_generate")
set(GENERAL_SRC)
if (EXISTS ${general_path})
	FILE(GLOB_RECURSE GENERAL_SRC "${general_path}/*.cpp")
	source_group("mumu_generate" FILES ${GENERAL_SRC})
endif()
add_executable(unitTest ${SRCS} ${Main_SRCS} ${Test_SRCS} ${GENERAL_SRC})
target_link_libraries(unitTest
    gtestd
    gmockd
)