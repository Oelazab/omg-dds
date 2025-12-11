#ifndef DDS_QOS_HPP
#define DDS_QOS_HPP

enum class ReliabilityKind
{
    BEST_EFFORT,
    RELIABLE
};
enum class DurabilityKind
{
    VOLATILE,
    TRANSIENT_LOCAL,
    TRANSIENT,
    PERSISTENT
};
enum class HistoryKind
{
    KEEP_LAST,
    KEEP_ALL
};

struct QoS
{
    ReliabilityKind reliability = ReliabilityKind::RELIABLE;
    DurabilityKind durability = DurabilityKind::VOLATILE;
    HistoryKind history = HistoryKind::KEEP_LAST;
    int history_depth = 1;
    int max_samples = 100;
};

#endif // DDS_QOS_HPP