#include "data_writer.hpp"
#include "data_reader.hpp"
#include <algorithm>

DataWriter::DataWriter(std::shared_ptr<Topic> topic, const QoS &qos)
    : topic_(topic), qos_(qos) {}

void DataWriter::registerReader(DataReader *reader)
{
    std::lock_guard<std::mutex> lock(mutex_);
    readers_.push_back(reader);
}

void DataWriter::unregisterReader(DataReader *reader)
{
    std::lock_guard<std::mutex> lock(mutex_);
    readers_.erase(std::remove(readers_.begin(), readers_.end(), reader), readers_.end());
}

bool DataWriter::write(const std::shared_ptr<DataSample> &sample)
{
    std::lock_guard<std::mutex> lock(mutex_);

    SampleInfo info;
    info.valid_data = true;
    info.source_timestamp = std::chrono::system_clock::now();
    info.instance_handle = sequence_number_++;

    for (auto *reader : readers_)
    {
        reader->pushSample(sample->clone(), info);
    }

    return true;
}