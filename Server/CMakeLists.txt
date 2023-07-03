# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name and language
project(Server CXX)

find_package(OpenCV REQUIRED)

set(MYSQL_INCLUDE_DIR /usr/include/mysql)
set(SRC src/server.cpp)
set(EXE server)
set(OUTDIR bin)

find_package( OpenCV REQUIRED )
find_package(Boost 1.74 REQUIRED COMPONENTS system thread)
include_directories( ${OpenCV_INCLUDE_DIRS})
include_directories( ${MYSQL_INCLUDE_DIR})
include_directories( ${BOOST_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR})

# Add an executable target with the source file
add_executable(${EXE} ${SRC})
set_target_properties(${EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTDIR})

# Link the executable target with the MySQL library
target_link_libraries(${EXE} ${LIBRARY_NAME} ${OpenCV_LIBS} ${Boost_LIBRARIES} pthread mysqlclient)

# Add the MySQL include directory to the target
target_include_directories(${EXE} PUBLIC ${MySQL_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})

