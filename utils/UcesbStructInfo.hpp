#pragma once

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

#include "UcesbMappingFlag.hpp"
#include <ext_data_struct_info.hh>

struct ext_data_structure_item;
class UcesbSource;
class UcesbStructInfo
{
  public:
    UcesbStructInfo() = default;
    auto Get() -> ext_data_struct_info* { return &struct_info_; }
    void CheckStructMapping();

  private:
    void terminate();
    ext_data_struct_info struct_info_;
    UcesbMap map_success_conditions_ = UcesbMap::ok;
};
