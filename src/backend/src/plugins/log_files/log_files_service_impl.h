#include <future>
#include "log_files/log_files.grpc.pb.h"
#include "plugins/log_files/log_files.h"

namespace mavsdk {
namespace backend {
template <typename LogFiles = LogFiles>
class LogFilesServiceImpl final : public rpc::log_files::LogFilesService::Service {
public:
    LogFilesServiceImpl(LogFiles& log_files) : _log_files(log_files) {}

    grpc::Status GetEntries(
        grpc::ServerContext* /* context */,
        const rpc::log_files::GetEntriesRequest* /* request */,
        rpc::log_files::GetEntriesResponse* response) override
    {
        std::promise<void> result_promise;
        const auto result_future = result_promise.get_future();


        _log_files.get_entries_async([this, response, &result_promise](
            const mavsdk::LogFiles::Result result,
            const std::vector<mavsdk::LogFiles::Entry> entries) {
            if (response != nullptr) {
                auto rpc_entries_result = generateRPCLogFilesResult(result);
                response->set_allocated_log_files_result(rpc_entries_result);

                for (const auto entry : entries) {
                    auto rpc_entry = response->add_entry();
                    translateEntry(entry, rpc_entry);
                }
            }

            result_promise.set_value();
        });

        result_future.wait();
        return grpc::Status::OK;
    }

    grpc::Status DownloadLogFile(grpc::ServerContext* /* context */,
                                 const rpc::log_files::DownloadLogFileRequest* request,
                                 rpc::log_files::DownloadLogFileResponse* response) override
    {
        if (request != nullptr) {
            const auto requested_id = request->id();
            const auto requested_file_path = request->file_path();

            if (response != nullptr) {
                auto log_file_result = _log_files.download_log_file(requested_id, requested_file_path);
                auto* rpc_log_file_result = new rpc::log_files::LogFilesResult();
                rpc_log_file_result->set_result(static_cast<rpc::log_files::LogFilesResult::Result>(log_file_result));

                response->set_allocated_log_files_result(rpc_log_file_result);
            }
        }

        return grpc::Status::OK;
    }

    static void translateEntry(const mavsdk::LogFiles::Entry entry_item, rpc::log_files::Entry* rpc_entry_item)
    {
        rpc_entry_item->set_id(entry_item.id);
        rpc_entry_item->set_date(entry_item.date);
        rpc_entry_item->set_size_bytes(entry_item.size_bytes);
    }

private:
    rpc::log_files::LogFilesResult * generateRPCLogFilesResult(const mavsdk::LogFiles::Result result) const
    {
        auto rpc_result = static_cast<rpc::log_files::LogFilesResult::Result>(result);

        auto rpc_log_files_result = new rpc::log_files::LogFilesResult();
        rpc_log_files_result->set_result(rpc_result);
        rpc_log_files_result->set_result_str(mavsdk::LogFiles::result_str(result));
    }

    LogFiles& _log_files;
};
}
}