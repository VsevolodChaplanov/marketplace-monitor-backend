module;

#include <memory>
#include <utility>

#include <generated/items/item-service.grpc.pb.h>
#include <generated/items/item-service.pb.h>
#include <grpcpp/support/status.h>

export module service:items;

export namespace service::items {
    using add_item_request = item_service::AddItemRequest;
    using add_item_response = item_service::AddItemResponse;

    using get_item_request = item_service::GetItemRequest;
    using get_item_response = item_service::GetItemResponse;

    using update_item_price_request = item_service::UpdatePriceRequest;
    using update_item_price_response = item_service::UpdatePriceResponse;

    class items_service final : public item_service::ItemService::Service {
        struct items_storage_concept {
            virtual ~items_storage_concept() = default;
        };

    public:
        template<typename ItemStorage>
        explicit items_service(ItemStorage&& storage)
            : m_storage([storage = std::forward<ItemStorage>(storage)] {
                  class Impl final : items_storage_concept {
                      ItemStorage storage;

                  public:
                      ~Impl() override = default;
                  };
                  return std::make_unique<Impl>(std::forward<ItemStorage>(storage));
              }()) {}

        items_service(const items_service&) = delete;
        items_service(items_service&&) noexcept = default;
        items_service& operator=(const items_service&) = delete;
        items_service& operator=(items_service&&) noexcept = default;

        ::grpc::Status AddItem(::grpc::ServerContext* context, const ::item_service::AddItemRequest* request,
                               ::item_service::AddItemResponse* response) override {
            return ::grpc::Status::OK;
        }
        // Get item info by id
        ::grpc::Status GetItem(::grpc::ServerContext* context, const ::item_service::GetItemRequest* request,
                               ::item_service::GetItemResponse* response) override {
            return ::grpc::Status::OK;
        }
        // Update price by id or sku
        ::grpc::Status UpdatePrice(::grpc::ServerContext* context, const ::item_service::UpdatePriceRequest* request,
                                   ::item_service::UpdatePriceResponse* response) override {
            return ::grpc::Status::OK;
        }

        ~items_service() override = default;

    private:
        std::unique_ptr<items_storage_concept> m_storage;
    };
} // namespace service::items
