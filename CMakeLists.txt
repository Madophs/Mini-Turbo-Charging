cmake_minimum_required(VERSION 3.10)

# Abbreviated MTC
project(Mini_Turbo_Charging VERSION 1.0)

# C++11
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add EPM directory
add_subdirectory(EPM)
add_subdirectory(DB)
add_subdirectory(test)
add_subdirectory(test/googletest)
add_executable(Mini_Turbo_Charging main.cpp)
add_library(Mini_Turbo_Charging_lib main.cpp)

target_compile_options(Mini_Turbo_Charging PRIVATE -g)
target_link_libraries(Mini_Turbo_Charging PUBLIC EventThreads pthread DB  pqxx pq EPM_Conf EPM_Events ACE)

configure_file(MTC_Version.h.in MTC_Version.h)

target_include_directories(Mini_Turbo_Charging PUBLIC
							"${PROJECT_BINARY_DIR}"
							"${PROJECT_BINARY_DIR}/EPM"
							"${PROJECT_BINARY_DIR}/DB"
							"${PROJECT_BINARY_DIR}/test"
							)

