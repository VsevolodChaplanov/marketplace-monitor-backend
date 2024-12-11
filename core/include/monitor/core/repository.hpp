#ifndef MARKETPLACEMONITOR_REPOSITORY_HPP
#define MARKETPLACEMONITOR_REPOSITORY_HPP

#include <algorithm>
#include <cassert>
#include <concepts>
#include <condition_variable>
#include <memory>
#include <monitor/core/exports/exports.hpp>
#include <mutex>
#include <pqxx/connection>
#include <vector>

namespace mo::core {

    template<typename T> struct storage_duration_trait {
        using storage_duration = T;
    };

    template<std::movable T> class basic_object_pool final : public std::enable_shared_from_this<basic_object_pool<T>> {
        static constexpr struct private_tag_t {
        } private_tag{};

        using object_storage_duration = typename storage_duration_trait<T>::storage_duration;

    public:
        struct object_wrapper {

            object_wrapper(const object_wrapper&) noexcept = delete;
            object_wrapper& operator=(const object_wrapper&) noexcept = delete;

            object_wrapper(object_wrapper&&) noexcept = default;
            object_wrapper& operator=(object_wrapper&&) noexcept = default;

            ~object_wrapper() {
                if (auto pool = pool_.lock()) {
                    pool->return_object(std::move(object_));
                }
            }

            auto operator*() & -> object_storage_duration& { return object_; }

            auto operator*() const & -> const object_storage_duration& { return object_; }

        private:
            friend basic_object_pool<T>;

            object_storage_duration object_;
            std::weak_ptr<basic_object_pool> pool_;

            object_wrapper(object_storage_duration object, std::weak_ptr<basic_object_pool> pool)
                : object_(std::move(object)), pool_(std::move(pool)) {}
        };

        explicit basic_object_pool(private_tag_t);

        explicit basic_object_pool(std::vector<object_storage_duration>&& pool, private_tag_t)
            : pool_(std::move(pool)) {};

        template<std::regular_invocable object_factory>
        static auto make(object_factory&& factory, std::size_t amount) -> std::shared_ptr<basic_object_pool<T>> {
            std::vector<object_storage_duration> pool;
            pool.reserve(amount);
            for ([[maybe_unused]] auto _ : std::views::iota(0) | std::views::take(amount)) {
                pool.push_back(factory());
            }
            return std::make_shared<basic_object_pool>(std::move(pool), private_tag);
        }

        template<std::size_t n, std::regular_invocable object_factory>
        static auto make(object_factory&& factory) -> std::shared_ptr<basic_object_pool<T>> {
            return make(std::forward<object_factory>(factory), n);
        }

        auto get() -> object_wrapper {
            std::unique_lock lock(mutex_);
            cv_.wait(lock, [this] { return used_objects_ < pool_.size(); });
            return {std::move(pool_[used_objects_++]), this->weak_from_this()};
        }

        basic_object_pool(const basic_object_pool&) = delete;
        auto operator=(const basic_object_pool&) -> basic_object_pool& = delete;

        basic_object_pool(basic_object_pool&&) = delete;
        auto operator=(basic_object_pool&&) -> basic_object_pool& = delete;

        ~basic_object_pool() = default;

    private:
        std::vector<object_storage_duration> pool_;
        std::size_t used_objects_{0};
        std::condition_variable cv_;
        std::mutex mutex_;

        void return_object(object_storage_duration&& object) {
            {
                const std::scoped_lock lock{mutex_};
                assert(used_objects_ != 0);
                pool_[--used_objects_] = std::move(object);
            }
            cv_.notify_one();
        }
    };

    template<>
    struct storage_duration_trait<pqxx::connection> final : storage_duration_trait<std::unique_ptr<pqxx::connection>> {
    };

    using connection_pool = basic_object_pool<pqxx::connection>;
} // namespace mo::core

#endif // MARKETPLACEMONITOR_REPOSITORY_HPP