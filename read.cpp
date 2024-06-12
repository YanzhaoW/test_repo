#include "data/MyData.hpp"
#include <TFile.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <iostream>

auto main() -> int
{
    auto file = std::make_unique<TFile>("data.root", "read");

    auto* tree = file->Get<TTree>("cbmsim");

    auto tree_reader = TTreeReader{};
    tree_reader.SetTree(tree);

    auto data = TTreeReaderValue<MyData>{ tree_reader, "MyData" };

    while (tree_reader.Next())
    {
        std::cout << "event Num: " << data->event_num << "\t" << data->value << "\n";
    }

    return 0;
}
