# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name and language
project(MySQLTest CXX)

set(MYSQL_INCLUDE_DIR /usr/include/mysql)
set(SRC src/init_dB.cpp)
set(EXE init_dB)
set(OUTDIR bin)

# Add an executable target with the source file
add_executable(${EXE} ${SRC})
set_target_properties(${EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTDIR})

# Link the executable target with the MySQL library
target_link_libraries(${EXE} mysqlclient)

# Add the MySQL include directory to the target
target_include_directories(${EXE} PUBLIC ${MYSQL_INCLUDE_DIR})

