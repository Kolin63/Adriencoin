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

        e_bonzo_mask,
        e_loving_scarf,
        e_spirit_sceptre,
        e_livid_dagger,
        e_giant_sword,
        e_wither_shield,
        e_shadow_warp,
        e_implosion,
        e_hyperion,

        e_MAX
    };

    constexpr static std::pair<std::string_view, dpp::snowflake> emojis[] {
        { "adr_carrot",	        1342341829894471811 },
        { "adr_gemstone",	    1342341843181899900 },
        { "adr_wood",	        1342341866464739359 },
        { "adr_fish",	        1342341836949295106 },
        { "adr_rottenflesh",	1342341859984543764 },
        { "adr_book",	        1342341814979657788 },
        { "adr_potion",	        1342341851440615454 },
        { "adr_adriencoin",	    1342319536300621876 },
        { "adr_gaydriencoin",	1342730469640376352 },
        { "adr_adriresource",	1342730457414111273 },
        { "adr_stockofstonks",	1353479698021224488 },
        { "adr_paper",	        1356787387463766046 },

        { "adr_bonzo_mask",     1366630925148098611 },
        { "adr_loving_scarf",   1366631057696358450 },
        { "adr_spirit_scepter", 1366631149564203050 },
        { "adr_livid_dagger",   1366631402719674408 },
        { "adr_giant_sword",    1366631433027981443 },
        { "adr_wither_shield",  1366631457531101215 },
        { "adr_shadow_warp",    1366631581250355253 },
        { "adr_implosion",      1366631617941995570 },
        { "adr_hyperion",       1366631645863481384 },

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
