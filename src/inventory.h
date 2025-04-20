#ifndef INVENTORY_H
#define INVENTORY_H

#include <array>
#include <algorithm>
#include "item.h"

using inventory = std::array<int, adr::i_MAX>;

namespace adr 
{
    // Returns a list of all Items in the Inventory that aren't 0
    inline std::string getNonZeroItems(const inventory& inv) {
        std::string output{};

        for (std::size_t i{}; i < inv.size(); ++i)
        {
            if (inv[i] != 0)
                output += get_emoji(static_cast<adr::item_id>(i))
                + ' ' + std::string{ adr::item_names[i] } + ": " 
                + std::to_string(inv[i]) + '\n';
        }

        return output;
    }
}

#endif
