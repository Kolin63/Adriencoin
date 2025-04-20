#ifndef ITEM_H
#define ITEM_H

#include <string_view>
#include <dpp/snowflake.h>
#include "emoji.h"

namespace adr
{
    //
    // item_id
    // An enum containing all item ids
    //
    enum item_id
    {
        i_carrot,
        i_gemstone,
        i_wood,
        i_fish,
        i_rottenflesh,
        i_book,
        i_potion,
        i_adriencoin,
        i_gaydriencoin,
        i_adriresource,
        i_null,
        i_stockofstonks,

        i_jerrys_carrots,
        i_divans_gems,
        i_jacks_lumber,
        i_geralds_fish,
        i_aatroxs_slaying,
        i_berts_knowledge,
        i_the_brewers,
        i_stonks_unlimited,

        i_MAX
    };

    //
    // item_names
    //
    // A constexpr array of C-Style Strings that contain item names,
    // where the index is the item_id and the element is the name
    //
    static constexpr const char* item_names[]
    {
        "carrot",
        "gemstone",
        "wood",
        "fish",
        "rottenflesh",
        "book",
        "potion",
        "adriencoin",
        "gaydriencoin",
        "adriresource",
        "null",
        "stockofstonks",

        "jerrys_carrots",
        "divans_gems",
        "jacks_lumber",
        "geralds_fish",
        "aatroxs_slaying",
        "berts_knowledge",
        "the_brewers",
    };

    // 
    // item_emoji_key
    //
    // An array where the index is the item ID and the element
    // is that item's emoji ID
    //
    // This is so that different items can share the same emoji
    // 
    static constexpr const emoji_id item_emoji_key[]
    {
        e_carrot,
        e_gemstone,
        e_wood,
        e_fish,
        e_rottenflesh,
        e_book,
        e_potion,
        e_adriencoin,
        e_gaydriencoin,
        e_adriresource,
        e_fish,
        e_stockofstonks,

        e_paper,
        e_paper,
        e_paper,
        e_paper,
        e_paper,
        e_paper,
        e_paper,
        e_paper,

        e_fish,
    };

    //
    // get_emoji
    //
    // Gets the emoji mention for the given item_id 
    //
    inline std::string get_emoji(adr::item_id i) {
        return get_emoji(item_emoji_key[i]);
    }

    inline item_id get_item_id(std::string_view name) {
        for (std::size_t i{}; i < adr::i_MAX; ++i) {
            if (item_names[i] == name) {
                return static_cast<item_id>(i);
            }
        }
        return i_null;
    }

    //
    // item
    //
    // A struct that contains an item_id and an amount
    //
    struct item
    {
        const item_id id{ i_null };
        int amount{ 0 };
    };
}

#endif
