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

#include "UcesbStructInfo.hpp"
#include "UcesbDecl.hpp"

void UcesbStructInfo::CheckStructMapping()
{
    std::cout << std::format("Checking struct mapping.\n");
    auto* struct_info = static_cast<ext_data_structure_info*>(struct_info_);
    auto is_checking_ok = true;
    for (auto* item = struct_info->_items; item != nullptr; item = item->_next_off_item)
    {
        std::cout << std::format("Ucesb struct item name using _items :{} \n", item->_var_name);
    }

    if (not is_checking_ok)
    {
        terminate();
    }
}

void UcesbStructInfo::terminate()
{
    std::cout << std::format("ext_data_clnt::setup() failed to map all items:\n");

    ext_data_struct_info_print_map_success(static_cast<ext_data_structure_info*>(struct_info_),
                                           stderr,
                                           static_cast<UcesbMapUType>(map_success_conditions_));
    throw std::runtime_error("ext_data_clnt::setup() mapping failure may cause unexpected analysis results "
                             "due to missing data members. Unpacker needs fixing.\n\n\n");
}
