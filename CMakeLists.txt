cmake_minimum_required(VERSION 3.10)
project(DDS_Implementation)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# DDS Library
add_library(dds
    dds/topic.cpp
    dds/data_reader.cpp
    dds/data_writer.cpp
    dds/publisher.cpp
    dds/subscriber.cpp
    dds/domain_participant.cpp
)

target_include_directories(dds PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Example executable
add_executable(dds_example
    examples/sensor_data.cpp
    examples/main.cpp
)

target_link_libraries(dds_example dds pthread)