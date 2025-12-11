#ifndef DDS_DOMAIN_PARTICIPANT_HPP
#define DDS_DOMAIN_PARTICIPANT_HPP

#include <memory>
#include <map>
#include <vector>
#include <string>
#include <mutex>
#include "topic.hpp"
#include "publisher.hpp"
#include "subscriber.hpp"
#include "data_writer.hpp"
#include "data_reader.hpp"

class DomainParticipant
{
private:
    int domain_id_;
    std::map<std::string, std::shared_ptr<Topic>> topics_;
    std::vector<std::shared_ptr<Publisher>> publishers_;
    std::vector<std::shared_ptr<Subscriber>> subscribers_;
    std::map<std::string, std::vector<DataWriter *>> topic_writers_;
    std::mutex mutex_;

public:
    DomainParticipant(int domain_id);
    std::shared_ptr<Topic> createTopic(const std::string &name, const std::string &type_name, const QoS &qos = QoS());
    std::shared_ptr<Publisher> createPublisher(const QoS &qos = QoS());
    std::shared_ptr<Subscriber> createSubscriber(const QoS &qos = QoS());
    void matchWriterReader(DataWriter *writer, DataReader *reader);
    int getDomainId() const { return domain_id_; }
};

#endif // DDS_DOMAIN_PARTICIPANT_HPP