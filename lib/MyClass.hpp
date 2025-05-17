#pragma once

#include "AsioClass.hpp"
#include <memory>

class MyClass
{
  public:
    MyClass() = default;
    void Print();

  private:
    std::unique_ptr<AsioClass> object_ = nullptr;
};
