#include "domain_participant.hpp"

DomainParticipant::DomainParticipant(int domain_id) : domain_id_(domain_id) {}

std::shared_ptr<Topic> DomainParticipant::createTopic(const std::string &name, const std::string &type_name, const QoS &qos)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (topics_.find(name) != topics_.end())
    {
        return topics_[name];
    }
    auto topic = std::make_shared<Topic>(name, type_name, qos);
    topics_[name] = topic;
    return topic;
}

std::shared_ptr<Publisher> DomainParticipant::createPublisher(const QoS &qos)
{
    auto publisher = std::make_shared<Publisher>(qos);
    publishers_.push_back(publisher);
    return publisher;
}

std::shared_ptr<Subscriber> DomainParticipant::createSubscriber(const QoS &qos)
{
    auto subscriber = std::make_shared<Subscriber>(qos);
    subscribers_.push_back(subscriber);
    return subscriber;
}

void DomainParticipant::matchWriterReader(DataWriter *writer, DataReader *reader)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (writer->getTopic()->getName() == reader->getTopic()->getName())
    {
        writer->registerReader(reader);
        topic_writers_[writer->getTopic()->getName()].push_back(writer);
    }
}