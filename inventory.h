#pragma once

#include <array>
#include "Item.h"

using Inventory = std::array<int, adr::Item::MAX>;

namespace adr 
{
    // Returns a list of all Items in the Inventory that aren't 0
    std::string getNonZeroItems(const Inventory& inv) {
        std::string output{};

        for (std::size_t i{}; i < inv.size(); ++i) {
            if (inv[i] != 0)
                output += dpp::emoji{ adr::Item::names[i], adr::Item::emojiIDs[i] }.get_mention() 
                + ' ' + adr::Item::names[i] + ": " + std::to_string(inv[i]) + '\n';
        }
    };
}
