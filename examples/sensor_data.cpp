#include "sensor_data.hpp"

SensorData::SensorData(int id, double temp, double hum, const std::string &loc)
    : sensor_id(id), temperature(temp), humidity(hum), location(loc) {}

std::shared_ptr<DataSample> SensorData::clone() const
{
    return std::make_shared<SensorData>(*this);
}

std::string SensorData::toString() const
{
    return "SensorData[id=" + std::to_string(sensor_id) +
           ", temp=" + std::to_string(temperature) +
           ", humidity=" + std::to_string(humidity) +
           ", location=" + location + "]";
}