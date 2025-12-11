#ifndef DDS_TOPIC_HPP
#define DDS_TOPIC_HPP

#include <string>
#include "qos.hpp"

class Topic
{
private:
    std::string name_;
    std::string type_name_;
    QoS qos_;

public:
    Topic(const std::string &name, const std::string &type_name, const QoS &qos = QoS());
    std::string getName() const { return name_; }
    std::string getTypeName() const { return type_name_; }
    QoS getQoS() const { return qos_; }
};

#endif // DDS_TOPIC_HPP