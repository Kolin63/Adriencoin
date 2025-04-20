#ifndef KOLIN63_EMOJI_H
#define KOLIN63_EMOJI_H

#include <dpp/dpp.h>
#include <string_view>

namespace adr
{
    enum emoji_id {
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
        e_stockofstonks,
        e_paper,

        e_MAX
    };

    constexpr static std::pair<std::string_view, dpp::snowflake> emojis[] {
        { "adr-carrot",	        1342341829894471811 },
        { "adr-gemstone",	    1342341843181899900 },
        { "adr-wood",	        1342341866464739359 },
        { "adr-fish",	        1342341836949295106 },
        { "adr-rottenflesh",	1342341859984543764 },
        { "adr-book",	        1342341814979657788 },
        { "adr-potion",	        1342341851440615454 },
        { "adr-adriencoin",	    1342319536300621876 },
        { "adr-gaydriencoin",	1342730469640376352 },
        { "adr-adriresource",	1342730457414111273 },
        { "adr-stockofstonks",	1353479698021224488 },
        { "adr-paper",	        1356787387463766046 },
    };

    //
    // get_emoji
    //
    // Gets the emoji mention for the given emoji_id
    //
    inline std::string get_emoji(adr::emoji_id id) {
        return dpp::emoji{ emojis[id].first, emojis[id].second }.get_mention();
    }
}

#endif
