#pragma once

#include <Rtypes.h>
#include <map>
#include <string>

struct MyData
{
  public:
    MyData() = default;
    bool is_valid = true;
    int value;
    std::string name = "my data";
    std::map<int, std::string> name_map;

    ClassDefNV(MyData, 1);
};
