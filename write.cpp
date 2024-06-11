#include "data/MyData.hpp"
#include <TTree.h>
#include <TFile.h>

auto main() -> int
{
    auto my_data = MyData{};

    auto file = std::make_unique<TFile>("data_tree.root", "recreate");

    auto tree = TTree{"my_tree", "tree_title"};
    constexpr auto split_level = 99;
    tree.Branch("myData", &my_data, split_level);

    constexpr auto num_entries = 1000;
    for (int idx{}; idx < num_entries; ++idx)
    {
        my_data.value = idx;
        tree.Fill();
    }
    tree.Write();
    return 0;
}
