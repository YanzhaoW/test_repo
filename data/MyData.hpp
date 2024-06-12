#pragma once

#include <TNamed.h>

struct MyData : public TNamed
{
  public:
    MyData() = default;
    int event_num = 0;
    double value = 0.;

    ClassDefOverride(MyData, 1);
};
