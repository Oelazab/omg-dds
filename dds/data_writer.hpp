#ifndef DDS_DATA_WRITER_HPP
#define DDS_DATA_WRITER_HPP

#include <memory>
#include <vector>
#include <mutex>
#include "topic.hpp"
#include "data_sample.hpp"
#include "qos.hpp"

class DataReader;

class DataWriter
{
private:
    std::shared_ptr<Topic> topic_;
    QoS qos_;
    std::vector<DataReader *> readers_;
    std::mutex mutex_;
    int sequence_number_ = 0;

public:
    DataWriter(std::shared_ptr<Topic> topic, const QoS &qos = QoS());
    void registerReader(DataReader *reader);
    void unregisterReader(DataReader *reader);
    bool write(const std::shared_ptr<DataSample> &sample);
    std::shared_ptr<Topic> getTopic() const { return topic_; }
};

#endif // DDS_DATA_WRITER_HPP