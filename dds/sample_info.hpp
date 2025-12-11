#ifndef DDS_SAMPLE_INFO_HPP
#define DDS_SAMPLE_INFO_HPP

#include <chrono>

struct SampleInfo
{
    bool valid_data = true;
    std::chrono::system_clock::time_point source_timestamp;
    int instance_handle = 0;
};

#endif // DDS_SAMPLE_INFO_HPP