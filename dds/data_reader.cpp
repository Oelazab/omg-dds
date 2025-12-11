#include "data_reader.hpp"

DataReader::DataReader(std::shared_ptr<Topic> topic, const QoS &qos)
    : topic_(topic), qos_(qos) {}

void DataReader::setListener(DataReaderListener *listener)
{
    listener_ = listener;
}

void DataReader::pushSample(std::shared_ptr<DataSample> sample, const SampleInfo &info)
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        samples_.push({sample, info});

        if (qos_.history == HistoryKind::KEEP_LAST &&
            samples_.size() > static_cast<size_t>(qos_.history_depth))
        {
            samples_.pop();
        }
    }

    // Call listener outside of lock to avoid deadlock
    if (listener_)
    {
        listener_->onDataAvailable();
    }
}

bool DataReader::take(std::shared_ptr<DataSample> &sample, SampleInfo &info)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (samples_.empty())
        return false;

    auto &pair = samples_.front();
    sample = pair.first;
    info = pair.second;
    samples_.pop();
    return true;
}

bool DataReader::read(std::shared_ptr<DataSample> &sample, SampleInfo &info)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (samples_.empty())
        return false;

    auto &pair = samples_.front();
    sample = pair.first;
    info = pair.second;
    return true;
}

int DataReader::getSampleCount() const
{
    return samples_.size();
}