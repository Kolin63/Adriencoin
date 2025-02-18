#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <array>

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
            rotten_flesh,
            book,
            potion,

            MAX
        };

        static const std::array<std::string, MAX> names;

        Id id{};
        int amount{};
    };
}

#endif
