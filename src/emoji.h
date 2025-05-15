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

        e_kismet_feather,

        e_MAX
    };

// DEBUG should be defined on the Test Bot, and not on the Release Bot
#ifndef DEBUG
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
        { "adr_loving_scarf",   1368791550355574804 },
        { "adr_spirit_scepter", 1368791532244832348 },
        { "adr_livid_dagger",   1368791513294704781 },
        { "adr_giant_sword",    1368791492788883566 },
        { "adr_wither_shield",  1368791465047752796 },
        { "adr_shadow_warp",    1368791443371458602 },
        { "adr_implosion",      1368791422068723836 },
        { "adr_hyperion",       1368791391441911838 },

        { "adr_kismet_feather", 1371656767611601008 },
    };
#else
    constexpr static std::pair<std::string_view, dpp::snowflake> emojis[] {
        { "adr_carrot",	        1366636287313842268 },
        { "adr_gemstone",	    1366636272830906408 },
        { "adr_wood",	        1366636255487721472 },
        { "adr_fish",	        1366636235103146024 },
        { "adr_rottenflesh",	1366636215323070474 },
        { "adr_book",	        1366636193470615622 },
        { "adr_potion",	        1366636162395013241 },
        { "adr_adriencoin",	    1366636135077511178 },
        { "adr_gaydriencoin",	1366636056723853383 },
        { "adr_adriresource",	1366636032350752811 },
        { "adr_stockofstonks",	1366635996602568735 },
        { "adr_paper",	        1366635951446822973 },

        { "adr_bonzo_mask",     1366635930185764955 },
        { "adr_loving_scarf",   1368790626187083848 },
        { "adr_spirit_scepter", 1368790613394198659 },
        { "adr_livid_dagger",   1368790596956983386 },
        { "adr_giant_sword",    1368790581563625592 },
        { "adr_wither_shield",  1368790420259082331 },
        { "adr_shadow_warp",    1368790400613089350 },
        { "adr_implosion",      1368790386297802893 },
        { "adr_hyperion",       1368790219305783366 },

        { "adr_kismet_feather", 1371656542159503492 },
    };
#endif

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
