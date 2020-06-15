cmake_minimum_required(VERSION 3.10)

# Abbreviated MTC
project(Mini_Turbo_Charging VERSION 1.0)

# C++11
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add EPM directory
add_subdirectory(EPM)
add_subdirectory(Database)

add_executable(Mini_Turbo_Charging main.cpp)

target_compile_options(Mini_Turbo_Charging PRIVATE -g)
target_link_libraries(Mini_Turbo_Charging PUBLIC EventThreads pthread DB  pqxx pq EPM_Conf EPM_Events ACE)
#target_link_libraries(Mini_Turbo_Charging PUBLIC EPM_Conf)

configure_file(MTC_Version.h.in MTC_Version.h)

target_include_directories(Mini_Turbo_Charging PUBLIC
							"${PROJECT_BINARY_DIR}"
							"${PROJECT_BINARY_DIR}/EPM"
							"${PROJECT_BINARY_DIR}/Database"
							)

