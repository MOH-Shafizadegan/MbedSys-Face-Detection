# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name and language
project(DB CXX)

find_package(OpenCV REQUIRED)

set(MYSQL_INCLUDE_DIR /usr/include/mysql)
set(SRC src/Database.cpp src/face_detector.cpp)
set(EXE database)
set(OUTDIR bin)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS})
include_directories( ${MYSQL_INCLUDE_DIR})


# Add an executable target with the source file
add_executable(${EXE} ${SRC})
set_target_properties(${EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTDIR})

# Link the executable target with the MySQL library
target_link_libraries(${EXE} mysqlclient ${OpenCV_LIBS})

# Add the MySQL include directory to the target
target_include_directories(${EXE} PUBLIC ${MySQL_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})

