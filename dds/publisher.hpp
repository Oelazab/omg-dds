#ifndef DDS_PUBLISHER_HPP
#define DDS_PUBLISHER_HPP

#include <memory>
#include <vector>
#include "data_writer.hpp"
#include "qos.hpp"

class Publisher
{
private:
    std::vector<std::shared_ptr<DataWriter>> writers_;
    QoS default_qos_;

public:
    Publisher(const QoS &qos = QoS());
    std::shared_ptr<DataWriter> createDataWriter(std::shared_ptr<Topic> topic, const QoS &qos = QoS());
    void deleteDataWriter(std::shared_ptr<DataWriter> writer);
};

#endif // DDS_PUBLISHER_HPP