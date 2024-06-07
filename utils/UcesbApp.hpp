#pragma once

#include "UcesbLauncher.hpp"
#include "UcesbStructInfo.hpp"
#include <format>

template <typename StructType>
class App
{
  public:
    explicit App(StructType* event_struct)
        : event_struct_{ event_struct }
    {
    }

    ~App() { server_launcher_.close(); }
    App(const App&) = delete;
    App(App&&) = delete;
    auto operator=(const App&) -> App& = delete;
    auto operator=(App&&) -> App& = delete;

    template <typename Fnt>
    void init(Fnt&& fnt)
    {
        auto status_ok = 1;
        fnt(status_ok, *ucesb_client_struct_info_.Get());

        if (status_ok == 0)
        {
            std::cout << std::format("Failed to setup structure information.");
        }
    }

    void launch_server(std::string_view exe, std::vector<std::string> args)
    {
        server_launcher_.set_exe(exe);
        server_launcher_.set_args(std::move(args));
        server_launcher_.launch();
    }

    void setup()
    {
        auto is_struct_map_success = uint32_t{};
        std::cout << std::format("Setting up ucesb client...\n");
        if (ucesb_client_.setup(
                nullptr, 0, ucesb_client_struct_info_.Get(), &is_struct_map_success, sizeof(StructType)) == 0)
        {
            ucesb_client_struct_info_.CheckStructMapping();
        }
        else
        {
            std::cout << std::format("ext_data_clnt::setup() failed\n");
            const auto* msg = (ucesb_client_.last_error() == nullptr) ? UCESB_NULL_STR_MSG : ucesb_client_.last_error();
            throw std::runtime_error(std::format("UCESB error: {}", msg));
        }
    }

  private:
    ext_data_clnt ucesb_client_;
    StructType* event_struct_ = nullptr;
    UcesbServerLauncher server_launcher_{ &ucesb_client_ };
    UcesbStructInfo ucesb_client_struct_info_;
};
