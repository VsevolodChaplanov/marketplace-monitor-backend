/**
 * @brief wraps generated headers in module to speed up compilation
 */
module;

#include <items/generated/grpc/item-service.grpc.pb.h> // IWYU pragma: keep
#include <items/generated/grpc/item-service.pb.h>      // IWYU pragma: keep

export module service.items:grpc;

namespace service::items {
    export using async_service_t = typename item_service::ItemService::AsyncService;
    export using add_item_request = item_service::AddItemRequest;
    export using add_item_response = item_service::AddItemResponse;
} // namespace service::items
