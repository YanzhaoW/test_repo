#include "data/MyData.hpp"
#include <TFile.h>
#include <TTree.h>
#include <iostream>

template <typename T>
struct RootBranchData
{
  public:
    explicit RootBranchData(std::unique_ptr<T> data)
        : data_{ std::move(data) }
        , data_ptr_{ data_.get() }
    {
    }

    auto get_ptr_address() -> T** { return &data_ptr_; }
    auto get() -> T* { return data_ptr_; }

  private:
    std::unique_ptr<T> data_ = nullptr;
    T* data_ptr_ = nullptr;
};

auto main() -> int
{
    auto file = std::make_unique<TFile>("data_tree.root", "read");

    auto* tree = file->Get<TTree>("my_tree");

    auto my_data = RootBranchData{ std::make_unique<MyData>() };

    tree->SetBranchAddress<MyData>("myData", my_data.get_ptr_address());

    for (int entry{}; tree->LoadTree(entry) >= 0; ++entry)
    {
        tree->GetEntry(entry);
        std::cout << my_data.get()->value << "\n";
        std::cout << my_data.get()->name << "\n";
    }

    return 0;
}
