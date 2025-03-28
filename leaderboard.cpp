#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <time.h>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <dpp/nlohmann/json.hpp>
#include "player.h"
#include "item.h"
#include "cache.h"
#include "leaderboard.h"

// Return an embed with a list of all players 
dpp::embed adr::leaderboard::getLeaderboardEmbed()
{
    using json = nlohmann::json;

    const std::filesystem::path filepath{ "playerdata" };
    std::filesystem::create_directory(filepath);

    using elem = std::pair<int, dpp::snowflake>;
    std::vector<elem> vector{};

    // Parse every playerdata json file without the cache and put it into a list
    for (auto const& dirEntry : std::filesystem::directory_iterator{ filepath }) {
        std::ifstream fs{ dirEntry.path() };
        json data{ json::parse(fs) };
        uint64_t uuid{ data["uuid"] };
        const adr::Player& player{ adr::cache::getPlayerFromCache(uuid) };

        vector.push_back({ player.inv(adr::Item::adriencoin), player.uuid() });
    }

    // Sort the list, greatest to least using the adriencoin (first) element
    std::sort(vector.begin(), vector.end(), [](const elem& a, const elem& b) {
        return a.first > b.first;
        });

    // Make the embed
    dpp::embed embed{};
    embed.set_title("Leaderboard")
        .set_description(dpp::utility::timestamp(std::time(0), dpp::utility::tf_short_date) + "\n\n")
        .set_color(0xFFD700)
        .set_thumbnail("https://raw.githubusercontent.com/Kolin63/Adriencoin/refs/heads/main/art/item/gaydriencoin.png");

    for (std::size_t i{}; i < vector.size(); ++i) {
        const adr::playerCacheElement& pce{ adr::cache::getElementFromCache(vector[i].second)};
        
        embed.set_description(embed.description + std::to_string(i + 1) + ". "
            + pce.username + ": " + adr::Item::getEmojiMention(adr::Item::adriencoin) + ' '
            + std::to_string(pce.player.inv(adr::Item::adriencoin)) + " Adriencoin" + '\n');
    }

    return embed;
}
