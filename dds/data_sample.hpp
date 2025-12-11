#ifndef DDS_DATA_SAMPLE_HPP
#define DDS_DATA_SAMPLE_HPP

#include <memory>
#include <string>

class DataSample
{
public:
    virtual ~DataSample() = default;
    virtual std::shared_ptr<DataSample> clone() const = 0;
    virtual std::string toString() const = 0;
};

#endif // DDS_DATA_SAMPLE_HPP