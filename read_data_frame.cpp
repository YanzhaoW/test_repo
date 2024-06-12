#include "data/MyData.hpp"
#include <ROOT/RDataFrame.hxx>
#include <TEntryList.h>
#include <TFile.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <iostream>

auto main() -> int
{
    // auto dataframe = ROOT::RDataFrame("my_tree", "data_tree.root", { "myData" });

    // dataframe.Foreach(
    //     [](const MyData& data)
    //     {
    //         std::cout << data.value << "\n";
    //         return 0;
    //     });

    auto file = std::make_unique<TFile>("data_tree.root", "read");

    auto entry_list = TEntryList{ "entry_list", "entry_list" };
    entry_list.EnterRange(200, 230);
    entry_list.EnterRange(190, 195);

    auto tree = TTreeReader{ "my_tree", file.get(), &entry_list };
    // auto tree = TTreeReader{ "my_tree", file.get() };

    auto data = TTreeReaderValue<MyData>{ tree, "myData" };
    std::cout << "size: " << sizeof(data) << "\n";

    // while (tree.Next())
    // {
    //     std::cout << data->value << "\t" << data->name << "\n";
    // }

    tree.Restart();
    std::cout << "entry number: " << tree.GetEntries() << "\n";
    tree.SetEntry(16);
    std::cout << data->value << "\t" << data->name << "\n";

    // auto* list = tree.GetEntryList();
    // list->Print();

    return 0;
}
