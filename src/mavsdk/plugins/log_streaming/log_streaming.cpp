// WARNING: THIS FILE IS AUTOGENERATED! As such, it should not be edited.
// Edits need to be made to the proto files
// (see
// https://github.com/mavlink/MAVSDK-Proto/blob/master/protos/log_streaming/log_streaming.proto)

#include <iomanip>

#include "log_streaming_impl.h"
#include "plugins/log_streaming/log_streaming.h"

namespace mavsdk {

using LogStreamingRaw = LogStreaming::LogStreamingRaw;

LogStreaming::LogStreaming(System& system) :
    PluginBase(),
    _impl{std::make_unique<LogStreamingImpl>(system)}
{}

LogStreaming::LogStreaming(std::shared_ptr<System> system) :
    PluginBase(),
    _impl{std::make_unique<LogStreamingImpl>(system)}
{}

LogStreaming::~LogStreaming() {}

void LogStreaming::start_log_streaming_async(const ResultCallback callback)
{
    _impl->start_log_streaming_async(callback);
}

LogStreaming::Result LogStreaming::start_log_streaming() const
{
    return _impl->start_log_streaming();
}

void LogStreaming::stop_log_streaming_async(const ResultCallback callback)
{
    _impl->stop_log_streaming_async(callback);
}

LogStreaming::Result LogStreaming::stop_log_streaming() const
{
    return _impl->stop_log_streaming();
}

LogStreaming::LogStreamingRawHandle
LogStreaming::subscribe_log_streaming_raw(const LogStreamingRawCallback& callback)
{
    return _impl->subscribe_log_streaming_raw(callback);
}

void LogStreaming::unsubscribe_log_streaming_raw(LogStreamingRawHandle handle)
{
    _impl->unsubscribe_log_streaming_raw(handle);
}

bool operator==(const LogStreaming::LogStreamingRaw& lhs, const LogStreaming::LogStreamingRaw& rhs)
{
    return (rhs.data == lhs.data);
}

std::ostream& operator<<(std::ostream& str, LogStreaming::LogStreamingRaw const& log_streaming_raw)
{
    str << std::setprecision(15);
    str << "log_streaming_raw:" << '\n' << "{\n";
    str << "    data: " << log_streaming_raw.data << '\n';
    str << '}';
    return str;
}

std::ostream& operator<<(std::ostream& str, LogStreaming::Result const& result)
{
    switch (result) {
        case LogStreaming::Result::Success:
            return str << "Success";
        case LogStreaming::Result::NoSystem:
            return str << "No System";
        case LogStreaming::Result::ConnectionError:
            return str << "Connection Error";
        case LogStreaming::Result::Busy:
            return str << "Busy";
        case LogStreaming::Result::CommandDenied:
            return str << "Command Denied";
        case LogStreaming::Result::Timeout:
            return str << "Timeout";
        case LogStreaming::Result::Unsupported:
            return str << "Unsupported";
        case LogStreaming::Result::Unknown:
            return str << "Unknown";
        default:
            return str << "Unknown";
    }
}

} // namespace mavsdk