# DDS (Data Distribution Service) Implementation

A lightweight, educational implementation of the OMG Data Distribution Service (DDS) specification in C++. This project demonstrates the core concepts and architecture of DDS for publish-subscribe communication.

## Features

- **Standards-Based Architecture**: Follows OMG DDS specification patterns
- **Quality of Service (QoS) Policies**: Configurable reliability, durability, and history
- **Publisher-Subscriber Pattern**: Decoupled data producers and consumers
- **Topic-Based Communication**: Type-safe data channels
- **Thread-Safe Operations**: Mutex-protected concurrent access
- **Listener Pattern**: Asynchronous data notifications
- **Modular Design**: Clean separation of concerns across multiple files

## Quick Start

### Building with g++

```bash
# Compile all source files
g++ -std=c++14 -pthread -I. \
    dds/*.cpp \
    examples/*.cpp \
    -o dds_example

# Run the example
./dds_example
```

### Building with CMake

```bash
mkdir build
cd build
cmake ..
cmake --build .
./dds_example # .\dds_example for windows
```

## Project Structure

```
.
├── dds/
│   ├── qos.hpp                    # Quality of Service policies
│   ├── sample_info.hpp            # Sample metadata
│   ├── data_sample.hpp            # Base data interface
│   ├── topic.hpp/cpp              # Topic implementation
│   ├── data_reader.hpp/cpp        # Subscriber data reader
│   ├── data_writer.hpp/cpp        # Publisher data writer
│   ├── publisher.hpp/cpp          # Publisher entity
│   ├── subscriber.hpp/cpp         # Subscriber entity
│   └── domain_participant.hpp/cpp # Central DDS manager
├── examples/
│   ├── sensor_data.hpp/cpp        # Example data type
│   └── main.cpp                   # Usage example
├── CMakeLists.txt                 # CMake build configuration
└── README.md                      # This file
```

## Core Components

### DomainParticipant
The central entity that manages all DDS entities within a domain. Creates topics, publishers, and subscribers.

```cpp
auto participant = std::make_shared<DomainParticipant>(0);
```

### Topic
Named data channel with type information that connects publishers and subscribers.

```cpp
auto topic = participant->createTopic("SensorTopic", "SensorData", qos);
```

### Publisher & DataWriter
Sends data samples to interested subscribers.

```cpp
auto publisher = participant->createPublisher();
auto writer = publisher->createDataWriter(topic, qos);
writer->write(data);
```

### Subscriber & DataReader
Receives data samples from publishers.

```cpp
auto subscriber = participant->createSubscriber();
auto reader = subscriber->createDataReader(topic, qos);
reader->take(sample, info);
```

## Usage Example

```cpp
#include "dds/domain_participant.hpp"
#include "examples/sensor_data.hpp"

// Create domain participant
auto participant = std::make_shared<DomainParticipant>(0);

// Configure QoS
QoS qos;
qos.reliability = ReliabilityKind::RELIABLE;
qos.history = HistoryKind::KEEP_LAST;
qos.history_depth = 10;

// Create topic
auto topic = participant->createTopic("SensorTopic", "SensorData", qos);

// Create publisher and writer
auto publisher = participant->createPublisher();
auto writer = publisher->createDataWriter(topic, qos);

// Create subscriber and reader
auto subscriber = participant->createSubscriber();
auto reader = subscriber->createDataReader(topic, qos);

// Match writer and reader
participant->matchWriterReader(writer.get(), reader.get());

// Publish data
auto data = std::make_shared<SensorData>(1, 22.5, 55.0, "Room 1");
writer->write(data);

// Read data
std::shared_ptr<DataSample> sample;
SampleInfo info;
if (reader->take(sample, info)) {
    std::cout << sample->toString() << std::endl;
}
```

## Creating Custom Data Types

To create your own data type, inherit from `DataSample`:

```cpp
class MyData : public DataSample {
public:
    int id;
    std::string message;
    
    MyData(int i, const std::string& msg) : id(i), message(msg) {}
    
    std::shared_ptr<DataSample> clone() const override {
        return std::make_shared<MyData>(*this);
    }
    
    std::string toString() const override {
        return "MyData[id=" + std::to_string(id) + ", msg=" + message + "]";
    }
};
```

## Quality of Service (QoS) Policies

### Reliability
- `BEST_EFFORT`: Fast delivery, may lose samples
- `RELIABLE`: Guaranteed delivery with retransmission

### Durability
- `VOLATILE`: Data not persisted
- `TRANSIENT_LOCAL`: Late joiners receive historical data
- `TRANSIENT`: Data persisted to disk
- `PERSISTENT`: Survives system restarts

### History
- `KEEP_LAST`: Keep only N most recent samples
- `KEEP_ALL`: Keep all samples (up to resource limits)

```cpp
QoS qos;
qos.reliability = ReliabilityKind::RELIABLE;
qos.durability = DurabilityKind::TRANSIENT_LOCAL;
qos.history = HistoryKind::KEEP_LAST;
qos.history_depth = 5;
```

## Using Listeners

Implement `DataReaderListener` for asynchronous notifications:

```cpp
class MyListener : public DataReaderListener {
public:
    DataReader* reader;
    
    MyListener(DataReader* r) : reader(r) {}
    
    void onDataAvailable() override {
        std::shared_ptr<DataSample> sample;
        SampleInfo info;
        while (reader->take(sample, info)) {
            std::cout << "New data: " << sample->toString() << std::endl;
        }
    }
};

// Attach listener to reader
MyListener listener(reader.get());
reader->setListener(&listener);
```

## Running Tests

```bash
# Run the example application
./dds_example

# Expected output:
# === DDS Implementation Example ===
# Publishing sensor data...
# Received: SensorData[id=1, temp=21.000000, humidity=52.000000, location=Room 1]
# Received: SensorData[id=2, temp=22.000000, humidity=54.000000, location=Room 2]
# ...
```

## API Reference

### DataWriter Methods
- `write(sample)` - Publish data sample to topic
- `registerReader(reader)` - Manually register a reader
- `getTopic()` - Get associated topic

### DataReader Methods
- `take(sample, info)` - Remove and return data (destructive read)
- `read(sample, info)` - Return data without removing (non-destructive)
- `setListener(listener)` - Attach data available callback
- `getSampleCount()` - Get number of queued samples

### DomainParticipant Methods
- `createTopic(name, type, qos)` - Create or retrieve topic
- `createPublisher(qos)` - Create publisher entity
- `createSubscriber(qos)` - Create subscriber entity
- `matchWriterReader(writer, reader)` - Connect writer to reader

## CMakeLists.txt

```cmake
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
```

## Educational Purpose

This implementation is designed for learning and understanding DDS concepts. For production use, consider:

- **RTI Connext DDS**: Commercial, high-performance implementation
- **Fast-DDS (eProsima)**: Open-source, RTPS wire protocol
- **OpenDDS**: Open-source, feature-complete implementation
- **CycloneDDS**: Eclipse Foundation project

## Limitations

- No network transport layer (in-process only)
- No RTPS wire protocol implementation
- Simplified discovery mechanism
- No content filtering or querying
- No security features

## References

- [OMG DDS Specification](https://www.omg.org/spec/DDS/)
- [DDS Foundation](https://www.dds-foundation.org/)
- [Real-Time Publish-Subscribe (RTPS) Protocol](https://www.omg.org/spec/DDSI-RTPS/)

## Acknowledgments

Based on the OMG Data Distribution Service specification. Project Created as an educational resource for understanding publish-subscribe middleware patterns.
