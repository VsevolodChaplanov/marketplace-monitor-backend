module;

#include <concepts>
#include <memory>
#include <utility>

#include <generated/items/item-service.grpc.pb.h>
#include <generated/items/item-service.pb.h>
#include <grpcpp/server_context.h>
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
            items_storage_concept(const items_storage_concept&) = default;
            items_storage_concept(items_storage_concept&&) noexcept = default;
            auto operator=(const items_storage_concept&) -> items_storage_concept& = default;
            auto operator=(items_storage_concept&&) noexcept -> items_storage_concept& = default;

            virtual ~items_storage_concept() = default;
        };

    public:
        template<typename ItemStorage>
            requires(!std::same_as<ItemStorage, class item_service>)
        explicit items_service(ItemStorage&& storage)
            : m_storage([storage = std::forward<ItemStorage>(storage)] {
                  class impl final : items_storage_concept {
                      ItemStorage m_storage;

                  public:
                      explicit impl(ItemStorage storage) noexcept(std::is_nothrow_move_constructible_v<ItemStorage>)
                          : m_storage(std::move(storage)) {}

                      impl(const impl&) = default;
                      impl(impl&&) noexcept = default;
                      auto operator=(const impl&) -> impl& = default;
                      auto operator=(impl&&) noexcept -> impl& = default;

                      ~impl() override = default;
                  };
                  return std::make_unique<impl>(std::forward<ItemStorage>(storage));
              }()) {}

        items_service(const items_service&) = delete;
        items_service(items_service&&) noexcept = default;
        auto operator=(const items_service&) -> items_service& = delete;
        auto operator=(items_service&&) noexcept -> items_service& = default;

        auto AddItem([[maybe_unused]] ::grpc::ServerContext* context,
                     [[maybe_unused]] const ::item_service::AddItemRequest* request,
                     [[maybe_unused]] ::item_service::AddItemResponse* response) -> ::grpc::Status override {
            return ::grpc::Status::OK;
        }
        // Get item info by id
        auto GetItem([[maybe_unused]] ::grpc::ServerContext* context,
                     [[maybe_unused]] const ::item_service::GetItemRequest* request,
                     [[maybe_unused]] ::item_service::GetItemResponse* response) -> ::grpc::Status override {
            return ::grpc::Status::OK;
        }
        // Update price by id or sku
        auto UpdatePrice([[maybe_unused]] ::grpc::ServerContext* context,
                         [[maybe_unused]] const ::item_service::UpdatePriceRequest* request,
                         [[maybe_unused]] ::item_service::UpdatePriceResponse* response) -> ::grpc::Status override {
            return ::grpc::Status::OK;
        }

        ~items_service() override = default;

    private:
        std::unique_ptr<items_storage_concept> m_storage;
    };
} // namespace service::items
