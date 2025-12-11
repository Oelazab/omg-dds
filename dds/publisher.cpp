#include "publisher.hpp"
#include <algorithm>

Publisher::Publisher(const QoS &qos) : default_qos_(qos) {}

std::shared_ptr<DataWriter> Publisher::createDataWriter(std::shared_ptr<Topic> topic, const QoS &qos)
{
    auto writer = std::make_shared<DataWriter>(topic, qos);
    writers_.push_back(writer);
    return writer;
}

void Publisher::deleteDataWriter(std::shared_ptr<DataWriter> writer)
{
    writers_.erase(std::remove(writers_.begin(), writers_.end(), writer), writers_.end());
}