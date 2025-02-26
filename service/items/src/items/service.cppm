module;

#include <agrpc/grpc_context.hpp>
#include <agrpc/grpc_executor.hpp>
#include <agrpc/register_awaitable_rpc_handler.hpp>
#include <agrpc/register_callback_rpc_handler.hpp>
#include <agrpc/rpc_type.hpp>
#include <agrpc/server_rpc.hpp>
#include <agrpc/server_rpc_ptr.hpp>
#include <boost/asio/awaitable.hpp>
#include <grpcpp/support/status.h>
#include <items/generated/exports/exports.hpp>

export module service.items;
export import :grpc;

namespace service::items {

    using context_t = agrpc::GrpcContext;

    static void register_add_item_rpc(context_t& grpc_context, async_service_t& service);

    export void SERVICE_ITEMS_EXPORT build_items_service( // NOLINT misc-use-internal-linkage
        context_t& grpc_context, async_service_t& service) {
        register_add_item_rpc(grpc_context, service);
    }
} // namespace service::items

module :private;

namespace service::items {
    void register_add_item_rpc(context_t& grpc_context, async_service_t& service) {
        using add_item_rpc = agrpc::ServerRPC<&async_service_t::RequestAddItem>;

        agrpc::register_callback_rpc_handler<add_item_rpc>(
            grpc_context, service,
            [](auto ptr, add_item_request& request) {
                add_item_response response;
                ptr->finish(std::move(response), grpc::Status::OK, [](bool) {

                });
            },
            [](std::exception_ptr exception, auto&&...) {
                if (exception) {
                    std::rethrow_exception(std::move(exception));
                }
            });
    }
} // namespace service::items
