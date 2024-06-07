/******************************************************************************
 *   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
 *   Copyright (C) 2019-2024 Members of R3B Collaboration                     *
 *                                                                            *
 *             This software is distributed under the terms of the            *
 *                 GNU General Public Licence (GPL) version 3,                *
 *                    copied verbatim in the file "LICENSE".                  *
 *                                                                            *
 * In applying this license GSI does not waive the privileges and immunities  *
 * granted to it by virtue of its status as an Intergovernmental Organization *
 * or submit itself to any jurisdiction.                                      *
 ******************************************************************************/

#include "UcesbLauncher.hpp"
#include <boost/algorithm/string.hpp>
#include <filesystem>
#include <format>
#include <iostream>

#include <ext_data_clnt.hh>

constexpr auto CHILD_CLOSE_WAITING_TIME = std::chrono::seconds(5);

namespace fs = std::filesystem;
namespace bp = boost::process;
namespace
{
    bool Check_exist(std::string_view exe)
    {
        auto exe_path = fs::path{ exe };
        return fs::exists(exe_path) || fs::is_symlink(exe_path);
    }

    // TODO: C++23 insert range
    template <typename T>
    void Append_elements(std::vector<T>& base, std::vector<T> to_append)
    {
        base.reserve(base.size() + to_append.size());
        for (auto& ele : to_append)
        {
            base.emplace_back(std::move(ele));
        }
    }

    // void Append_lmds(std::vector<std::string>& lmds, std::string_view filename_regex)
    // {
    //     // expand filenames on regex
    //     Append_elements(lmds, GetFilesFromRegex(filename_regex));
    // }

    // auto parse_splits(std::vector<std::string> splits) -> UcesbServerLauncher::ResolveResult
    // {
    //     auto result = UcesbServerLauncher::ResolveResult{};

    //     auto option_regex = std::regex{ "^--[0-9A-z,=\\-]+" };
    //     auto lmd_regex = std::regex{ "^.*\\.lmd$" };

    //     for (auto& split_item : splits)
    //     {
    //         if (std::regex_match(split_item, option_regex))
    //         {
    //             result.options.emplace_back(std::move(split_item));
    //         }
    //         else if (std::regex_match(split_item, lmd_regex))
    //         {
    //             Append_lmds(result.lmds, split_item);
    //         }
    //         else if (Check_exist(split_item))
    //         {
    //             // it must be an executable then
    //             if (not result.executable.empty())
    //             {
    //                 //R3BLOG(info, std::format("Ucesb Executable has been set to \"{}\" ", result.executable));
    //                 //R3BLOG(error, std::format("Found another executable \"{}\" but only one is allowed!",
    //                 split_item)); continue;
    //             }
    //             //R3BLOG(info, std::format("Ucesb Executable is set to \"{}\" ", split_item));
    //             result.executable = std::move(split_item);
    //         }
    //         else
    //         {
    //             // In other cases, let ucesb deal with this
    //             result.others.emplace_back(std::move(split_item));
    //         }
    //     }
    //     return result;
    // }

    // void lmd_filenames_sorting(std::vector<std::string>& filenames)
    // {
    //     // simple lexicographically sorting
    //     std::sort(filenames.begin(), filenames.end());
    // }

    // auto resolve_exe_options_lmd(std::string cmd) -> UcesbServerLauncher::ResolveResult
    // {
    //     if (cmd.empty())
    //     {
    //         throw std::logic_error("Ucesb command string is empty!");
    //     }
    //     // R3BLOG(debug, std::format("Resolving string command: {}", cmd));
    //     boost::trim(cmd);
    //     auto splits = std::vector<std::string>{};
    //     boost::split(splits, cmd, boost::is_any_of(" "), boost::token_compress_on);
    //     if (splits.empty())
    //     {
    //         throw runtime_error(std::format("Get 0 element from splitting string {}", cmd));
    //     }
    //     auto results = parse_splits(std::move(splits));
    //     lmd_filenames_sorting(results.lmds);

    //     return results;
    // }
} // namespace

// void UcesbServerLauncher::SetLaunchCmd(const std::string& command_string)
// {
//     launch_strings_ = resolve_exe_options_lmd(command_string);
//     if (launch_strings_.executable.empty())
//     {
//         //R3BLOG(error, std::format("An unpacker executable doesn't exist in options {:?}", command_string));
//     }
//     Append_elements(launch_args, std::move(launch_strings_.options));
//     Append_elements(launch_args, std::move(launch_strings_.lmds));
//     Append_elements(launch_args, std::move(launch_strings_.others));

//     //R3BLOG(debug,
//            std::format("Ucesb command after resolving wildcard filename: \n {} {}",
//                        launch_strings_.executable,
//                        std::join(launch_args, " ")));
// }

void UcesbServerLauncher::launch()
{
    server_pipe_ = boost::process::async_pipe{ ios_ };
    ucesb_server_ = std::make_unique<boost::process::child>(
        exe_string_, boost::process::args(launch_args_), boost::process::std_out > server_pipe_);
    std::cout << std::format("Launching an ucesb server with pid: {}\n", ucesb_server_->id());
    if (auto is_status_ok = client_->connect(server_pipe_.native_source()); not is_status_ok)
    {
        std::cout << std::format("ext_data_clnt::connect() failed\n");
        const auto* msg = (client_->last_error() == nullptr) ? UCESB_NULL_STR_MSG : client_->last_error();
        throw std::runtime_error(std::format("UCESB error: {}", msg));
    }
}

// void UcesbServerLauncher::Setup(ext_data_struct_info& struct_info, size_t event_struct_size) {}

void UcesbServerLauncher::close()
{
    if (auto ret_val = client_->close(); ret_val != 0)
    {
        std::cout << std::format("ext_data_clnt::close() failed\n");
    }
    auto err_code = std::error_code{};
    auto boost_err = boost::system::error_code{};

    server_pipe_.close(boost_err);
    std::cout << std::format("pipe closed: {}\n", boost_err.what());

    if (not ucesb_server_->wait_for(CHILD_CLOSE_WAITING_TIME, err_code))
    {
        std::cout << std::format("Failed to close Ucesb server! Error code: {}\n", err_code.value());
        ucesb_server_->terminate(err_code);
        std::cout << std::format("Killing Ucesb server\n");
    }
    else
    {
        std::cout << std::format("Ucesb server is closed successfully\n");
    }

    auto child_handle = bp::child::child_handle{ ucesb_server_->id() };
    std::cout << std::format("Trying to killing Ucesb server with pid: {}\n", child_handle.id());
    bp::detail::api::terminate(child_handle, err_code);
    // ucesb_server_->terminate(err_code);
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << std::format("Killed: {}\n", err_code.value());
}
