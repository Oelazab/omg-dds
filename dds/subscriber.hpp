#ifndef DDS_SUBSCRIBER_HPP
#define DDS_SUBSCRIBER_HPP

#include <memory>
#include <vector>
#include "data_reader.hpp"
#include "qos.hpp"

class Subscriber
{
private:
    std::vector<std::shared_ptr<DataReader>> readers_;
    QoS default_qos_;

public:
    Subscriber(const QoS &qos = QoS());
    std::shared_ptr<DataReader> createDataReader(std::shared_ptr<Topic> topic, const QoS &qos = QoS());
    void deleteDataReader(std::shared_ptr<DataReader> reader);
};

#endif // DDS_SUBSCRIBER_HPP