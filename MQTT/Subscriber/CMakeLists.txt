# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name and language
project(Subscriber CXX)

set(SRC src/Subscriber.cpp)
set(EXE subscriber)
set(OUTDIR bin)

# Add an executable target with the source file
add_executable(${EXE} ${SRC})
set_target_properties(${EXE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTDIR})

# Link the executable target with the MySQL library
target_link_libraries(${EXE} paho-mqtt3c)


