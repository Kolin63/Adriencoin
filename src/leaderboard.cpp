#include "emoji.h"
#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <vector>
#include <algorithm>
#include <dpp/nlohmann/json.hpp>
#include "player.h"
#include "item.h"
#include "cache.h"
#include "leaderboard.h"

//
// get_leaderboard_embed
//
// Returns a DPP embed that contains a list of all players sorted by
// amount of Adriencoin 
//
dpp::embed adr::leaderboard::get_leaderboard_embed()
{
    std::vector<dpp::snowflake> vector{ adr::cache::cacheAll() };

    // Sort the list, greatest to least using the adriencoin (first) element
    std::sort(vector.begin(), vector.end(), 
        [](const dpp::snowflake& a, const dpp::snowflake& b) {
            const adr::Player x{ adr::cache::getPlayerFromCache(a) };
            const adr::Player y{ adr::cache::getPlayerFromCache(b) };
            return x.inv(adr::i_adriencoin) > y.inv(adr::i_adriencoin);
        });

    // Make the embed
    dpp::embed embed{};
    embed.set_title("Leaderboard")
        .set_description(dpp::utility::timestamp(std::time(0), dpp::utility::tf_short_date) + "\n\n")
        .set_color(0xFFD700)
        .set_thumbnail("https://raw.githubusercontent.com/Kolin63/Adriencoin/refs/heads/main/art/item/gaydriencoin.png");

    for (std::size_t i{}; i < vector.size(); ++i) {
        const adr::playerCacheElement& pce{ adr::cache::getElementFromCache(vector[i])};
        
        embed.set_description(embed.description + std::to_string(i + 1) + ". "
            + pce.username + ": " + adr::get_emoji(e_adriencoin) + ' '
            + std::to_string(pce.player.inv(adr::i_adriencoin)) 
            + " Adriencoin" + '\n');
    }

    return embed;
}
