#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <array>
#include <dpp/dpp.h>

namespace adr
{
    struct Item
    {
        enum Id
        {
            adriencoin,
            carrot,
            gemstone,
            wood,
            fish,
            rottenflesh,
            book,
            potion,

            MAX
        };

        static const std::array<std::string, MAX> names;
        static const std::array<dpp::snowflake, MAX> emojiIDs;

        Id id{};
        int amount{};
    };
}

#endif
