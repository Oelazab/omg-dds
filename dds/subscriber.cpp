#include "subscriber.hpp"
#include <algorithm>

Subscriber::Subscriber(const QoS &qos) : default_qos_(qos) {}

std::shared_ptr<DataReader> Subscriber::createDataReader(std::shared_ptr<Topic> topic, const QoS &qos)
{
    auto reader = std::make_shared<DataReader>(topic, qos);
    readers_.push_back(reader);
    return reader;
}

void Subscriber::deleteDataReader(std::shared_ptr<DataReader> reader)
{
    readers_.erase(std::remove(readers_.begin(), readers_.end(), reader), readers_.end());
}