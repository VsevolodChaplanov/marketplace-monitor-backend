#ifndef MARKETPLACEMONITOR_REPOSITORY_HPP
#define MARKETPLACEMONITOR_REPOSITORY_HPP

#include <cassert>
#include <condition_variable>
#include <memory>
#include <monitor/core/exports/exports.hpp>
#include <mutex>
#include <pqxx/connection>
#include <vector>

namespace mo::core {

    struct MONITOR_CORE_EXPORT connection_pool final : public std::enable_shared_from_this<connection_pool> {
        using connection = pqxx::connection;
        using connection_ptr = std::shared_ptr<connection>;

        struct connection_wrapper {

            connection_wrapper(const connection_wrapper&) noexcept = default;
            connection_wrapper& operator=(const connection_wrapper&) noexcept = default;

            connection_wrapper(connection_wrapper&&) noexcept = default;
            connection_wrapper& operator=(connection_wrapper&&) noexcept = default;

            ~connection_wrapper() {
                if (auto pool = pool_.lock()) {
                    pool->return_connection(std::move(connection_));
                }
            }

            pqxx::connection& operator()() & {
                assert(connection_);
                return *connection_;
            }
            pqxx::connection* operator->() & {
                assert(connection_);
                return connection_.get();
            }

            const pqxx::connection& operator()() const & {
                assert(connection_);
                return *connection_;
            }
            const pqxx::connection* operator->() const & {
                assert(connection_);
                return connection_.get();
            }

        private:
            friend connection_pool;

            std::shared_ptr<connection> connection_;
            std::weak_ptr<connection_pool> pool_;

            connection_wrapper(connection_ptr connection, std::weak_ptr<connection_pool> pool)
                : connection_(std::move(connection)), pool_(std::move(pool)) {
                assert(connection_);
            }
        };

        connection_wrapper get() {
            std::unique_lock lock(mutex_);
            cv_.wait(lock, [this] { return used_connections_ < pool_.size(); });
            return {std::move(pool_[used_connections_++]), weak_from_this()};
        }

        connection_pool(const connection_pool&) = delete;
        connection_pool& operator=(const connection_pool&) = delete;

        connection_pool(connection_pool&&) = delete;
        connection_pool& operator=(connection_pool&&) = delete;

        ~connection_pool() = default;

    private:
        static constexpr struct private_tag_t {
        } private_tag{};

        std::vector<std::shared_ptr<connection>> pool_;
        std::size_t used_connections_{pool_.size() - 1};
        std::condition_variable cv_;
        std::mutex mutex_;

        void return_connection(connection_ptr&& connection) {
            {
                const std::scoped_lock lock{mutex_};
                assert(used_connections_ != 0);
                pool_[--used_connections_] = std::move(connection);
            }
            cv_.notify_one();
        }
    };
} // namespace mo::core

#endif // MARKETPLACEMONITOR_REPOSITORY_HPP