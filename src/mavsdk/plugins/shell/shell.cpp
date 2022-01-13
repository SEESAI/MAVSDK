// WARNING: THIS FILE IS AUTOGENERATED! As such, it should not be edited.
// Edits need to be made to the proto files
// (see https://github.com/mavlink/MAVSDK-Proto/blob/master/protos/shell/shell.proto)

#include <iomanip>

#include "shell_impl.h"
#include "plugins/shell/shell.h"

namespace mavsdk {

Shell::Shell(System& system) : PluginBase(), _impl{std::make_unique<ShellImpl>(system)} {}

Shell::Shell(std::shared_ptr<System> system) :
    PluginBase(),
    _impl{std::make_unique<ShellImpl>(system)}
{}

Shell::~Shell() {}

Shell::Result Shell::send(std::string command) const
{
    return _impl->send(command);
}

void Shell::subscribe_receive(ReceiveCallback callback)
{
    _impl->subscribe_receive(callback);
}

std::ostream& operator<<(std::ostream& str, Shell::Result const& result)
{
    switch (result) {
        case Shell::Result::Unknown:
            return str << "Unknown";
        case Shell::Result::Success:
            return str << "Success";
        case Shell::Result::NoSystem:
            return str << "No System";
        case Shell::Result::ConnectionError:
            return str << "Connection Error";
        case Shell::Result::NoResponse:
            return str << "No Response";
        case Shell::Result::Busy:
            return str << "Busy";
        default:
            return str << "Unknown";
    }
}

} // namespace mavsdk