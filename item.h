#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <array>

namespace adr
{
    enum Id
    {
        carrot,
        gemstone,
        wood,
        fish,
        rotten_flesh,
        book,
        potion,

        MAX
    };

    std::array<std::string, MAX> names{
        "carrot",
        "gemstone",
        "wood",
        "fish",
        "rotten flesh",
        "book",
        "potion"
    };

    struct Item
    {
        Id id{};
        int amount{};
    };
}

#endif
