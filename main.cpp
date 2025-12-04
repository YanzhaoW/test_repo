#include <asio.hpp>
#include <asio/experimental/awaitable_operators.hpp>
#include <asio/experimental/parallel_group.hpp>
#include <print>
#include <random>
#include <vector>

namespace
{
    auto task(int input) -> asio::awaitable<int>
    {
        co_return input % 7 ? 10 * input : throw std::runtime_error("seven is not a lucky number");
    }
    auto generate(asio::thread_pool& pool)
    {
        auto ops = std::vector<decltype(co_spawn(pool, task(1)))>{};

        auto num = 10 + (std::random_device{}() % 5);
        for (unsigned i = 1; i < num; ++i)
        {
            ops.push_back(co_spawn(pool, task(i)));
        }
        return ops;
    }

    void consume(auto ops)
    {
        auto g = asio::experimental::make_parallel_group(std::move(ops));
        std::move(g).async_wait( //
            asio::experimental::wait_for_all(),
            [](auto const& order, auto&& exceptions, auto&& rvals)
            {
                for (size_t i = 0; i < order.size(); ++i)
                {
                    try
                    {
                        std::println("task {:2} finished:", order.at(i));
                        if (exceptions.at(i))
                        {
                            std::rethrow_exception(exceptions.at(i));
                        }

                        std::println("{}", rvals.at(i));
                    }
                    catch (std::exception const& e)
                    {
                        std::println("ERROR: {:?}", e.what());
                    }
                }
            });
    }
} // namespace

auto main() -> int
{
    asio::thread_pool pool{ 4 };

    int i;

    std::println("{}", i);

    auto ops = generate(pool);
    consume(std::move(ops));

    pool.join();
}
