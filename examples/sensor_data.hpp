#ifndef SENSOR_DATA_HPP
#define SENSOR_DATA_HPP

#include "../dds/data_sample.hpp"
#include <string>

class SensorData : public DataSample
{
public:
    int sensor_id;
    double temperature;
    double humidity;
    std::string location;

    SensorData(int id = 0, double temp = 0.0, double hum = 0.0, const std::string &loc = "");
    std::shared_ptr<DataSample> clone() const override;
    std::string toString() const override;
};

#endif // SENSOR_DATA_HPP