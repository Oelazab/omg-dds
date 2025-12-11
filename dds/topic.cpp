#include "topic.hpp"

Topic::Topic(const std::string &name, const std::string &type_name, const QoS &qos)
    : name_(name), type_name_(type_name), qos_(qos) {}