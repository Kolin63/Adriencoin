#ifndef INVENTORY_H
#define INVENTORY_H

#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <array>
#include <algorithm>
#include "item.h"

using Inventory = std::array<int, adr::Item::MAX>;

namespace adr 
{
    // Returns a list of all Items in the Inventory that aren't 0
    inline std::string getNonZeroItems(const Inventory& inv) {
        std::string output{};

        std::for_each(inv.begin(), inv.end(), [&output](const int& i)
            {
                if (i != 0)
                    output += dpp::emoji{ adr::Item::names[i], adr::Item::emojiIDs[i] }.get_mention()
                    + ' ' + adr::Item::names[i] + ": " + std::to_string(i) + '\n';
            });

        return output;
    }
}

#endif
