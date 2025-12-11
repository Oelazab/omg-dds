#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <atomic>
#include "../dds/domain_participant.hpp"
#include "sensor_data.hpp"

class MySensorListener : public DataReaderListener
{
public:
    DataReader *reader;
    std::atomic<int> received_count{0};

    MySensorListener(DataReader *r) : reader(r) {}

    void onDataAvailable() override
    {
        std::shared_ptr<DataSample> sample;
        SampleInfo info;

        while (reader->take(sample, info))
        {
            if (info.valid_data)
            {
                std::cout << "Received: " << sample->toString() << std::endl;
                std::cout.flush(); // Force flush
                received_count++;
            }
        }
    }
};

int main()
{
    std::cout << "=== DDS Implementation Example ===" << std::endl;
    std::cout.flush();

    auto participant = std::make_shared<DomainParticipant>(0);

    QoS qos;
    qos.reliability = ReliabilityKind::RELIABLE;
    qos.history = HistoryKind::KEEP_LAST;
    qos.history_depth = 10;

    auto topic = participant->createTopic("SensorTopic", "SensorData", qos);
    auto publisher = participant->createPublisher();
    auto writer = publisher->createDataWriter(topic, qos);
    auto subscriber = participant->createSubscriber();
    auto reader = subscriber->createDataReader(topic, qos);

    MySensorListener listener(reader.get());
    reader->setListener(&listener);

    participant->matchWriterReader(writer.get(), reader.get());

    std::cout << "\nPublishing sensor data..." << std::endl;
    std::cout.flush();

    const int num_samples = 5;

    for (int i = 1; i <= num_samples; i++)
    {
        auto data = std::make_shared<SensorData>(i, 20.0 + i, 50.0 + i * 2, "Room " + std::to_string(i));
        std::cout << "Publishing sample " << i << "..." << std::endl;
        std::cout.flush();
        writer->write(data);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    // Give time for all callbacks to complete
    std::cout << "\nWaiting for callbacks to complete..." << std::endl;
    std::cout.flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "\nReceived " << listener.received_count.load() << " samples" << std::endl;
    std::cout << "=== DDS Example Complete ===" << std::endl;
    std::cout.flush();

    return 0;
}