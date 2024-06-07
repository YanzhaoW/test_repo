#include "UcesbApp.hpp"
// #include <ext_h101.h>

#include "ext_h101_raw_nnp_tamex.h"
#include "ext_h101_tpat.h"
#include "ext_h101_unpack.h"


struct Composite
{
    EXT_STR_h101_unpack_t unpack;
    EXT_STR_h101_TPAT_t tpat;
    EXT_STR_h101_raw_nnp_tamex_onion_t raw_nnp;
};

constexpr auto executable =
    std::string_view{ "/u/land/fake_cvmfs/10/cvmfs_fairroot_v18.8.0_fs_nov22p1_extra/upexps/202205_s522/202205_s522" };

auto main() -> int
{
    auto event_struct = Composite{};
    auto app = App{ &event_struct };

    using namespace std::string_literals;
    auto args = std::vector{ "--allow-errors"s,
                             "--input-buffer=600Mi"s,
                             "--ntuple=RAW,time-stitch=4000,STRUCT,-"s,
                             "--max-events=10"s,
                             "/lustre/r3b/202205_s522/lmd/main0143_0014.lmd"s };
    try
    {
        // app.launch_server(land_executable, "--ntuple=RAW,STRUCT,-");
        app.init(
            [](auto status, ext_data_struct_info& struct_info)
            {
                EXT_STR_h101_TPAT_ITEMS_INFO(status,
                                             static_cast<ext_data_structure_info*>(struct_info),
                                             offsetof(Composite, tpat),
                                             EXT_STR_h101_TPAT_t,
                                             0); // NOLINT
            });
        app.init(
            [](auto status, ext_data_struct_info& struct_info)
            {
                EXT_STR_h101_unpack_ITEMS_INFO(status,
                                               static_cast<ext_data_structure_info*>(struct_info),
                                               offsetof(Composite, unpack),
                                               EXT_STR_h101_unpack_t,
                                               0); // NOLINT
            });
        app.init(
            [](auto status, ext_data_struct_info& struct_info)
            {
                EXT_STR_h101_raw_nnp_tamex_ITEMS_INFO(status,
                                                      static_cast<ext_data_structure_info*>(struct_info),
                                                      offsetof(Composite, raw_nnp),
                                                      EXT_STR_h101_raw_nnp_tamex_t,
                                                      0); // NOLINT
            });
        app.launch_server(executable, std::move(args));
        app.setup();
    }
    catch (std::exception& ex)
    {
        std::cerr << std::format("Error: {}", ex.what());
    }
    return 0;
}
