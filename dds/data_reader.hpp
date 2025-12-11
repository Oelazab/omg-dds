#ifndef DDS_DATA_READER_HPP
#define DDS_DATA_READER_HPP

#include <memory>
#include <queue>
#include <mutex>
#include "topic.hpp"
#include "data_sample.hpp"
#include "sample_info.hpp"
#include "qos.hpp"

class DataReaderListener
{
public:
    virtual ~DataReaderListener() = default;
    virtual void onDataAvailable() = 0;
};

class DataReader
{
private:
    std::shared_ptr<Topic> topic_;
    QoS qos_;
    std::queue<std::pair<std::shared_ptr<DataSample>, SampleInfo>> samples_;
    std::mutex mutex_;
    DataReaderListener *listener_ = nullptr;

public:
    DataReader(std::shared_ptr<Topic> topic, const QoS &qos = QoS());
    void setListener(DataReaderListener *listener);
    void pushSample(std::shared_ptr<DataSample> sample, const SampleInfo &info);
    bool take(std::shared_ptr<DataSample> &sample, SampleInfo &info);
    bool read(std::shared_ptr<DataSample> &sample, SampleInfo &info);
    int getSampleCount() const;
    std::shared_ptr<Topic> getTopic() const { return topic_; }
};

#endif // DDS_DATA_READER_HPP