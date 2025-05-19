#include <iostream>
#include <unordered_map>
#include <dpp/dpp.h>
#include "botToken.h"
#include "cache.h"

std::unordered_map<dpp::snowflake, adr::playerCacheElement> adr::cache::playerCache;

void adr::cache::cacheUsernameAndAvatar(adr::playerCacheElement& elem)
{
    std::cout << "Caching Username and Avatar for Player " << elem.player.uuid() << '\n';

    if (elem.player.uuid() == 0) {
      std::cout << "skipping uuid 0\n";
      elem.username = "ayan desai";
      elem.avatarURL = "https://gvanrossum.github.io/images/guido-headshot-2019.jpg";
      return;
    }

    std::function<void(const dpp::confirmation_callback_t&)> func{ [&elem](const dpp::confirmation_callback_t& callback) {
        if (callback.is_error()) {
            std::cout << "Error getting username and avatar url\n";
            return;
        }

        dpp::user_identified userIdent{ std::get<dpp::user_identified>(callback.value) };
        elem.username = userIdent.username;
        elem.avatarURL = userIdent.get_avatar_url();
        std::cout << "found uuid and username and avatarURL: " << userIdent.id << ' ' << elem.username << ' ' << elem.avatarURL << '\n';
    } };

    // Try to get username and avatar from cache
    bot.user_get_cached(elem.player.uuid(), func);

    std::cout << elem.player.uuid() << ' ' << elem.username << ' ' << elem.avatarURL << ": " 
        << (elem.username == "" ? "Failure\n" : "Success! " + elem.username + '\n');
}

adr::playerCacheElement& adr::cache::getElementFromCache(const dpp::snowflake& uuid)
{
    std::cout << "Searching cache for Player " << uuid << '\n';
    if (auto search = adr::cache::playerCache.find(uuid); search != adr::cache::playerCache.end()) {
        std::cout << "Returning Player " << uuid << " from Cache\n";

        // If the username data is still not cached, try to cache it again
        if (search->second.username == "") cacheUsernameAndAvatar(search->second);

        return search->second;
    }

    std::cout << "Could not find Player " << uuid << " in cache, creating new element\n";
    adr::playerCacheElement elem{ { uuid }, "", "", adr::Job::MAX, {{ { uuid, 0 }, { uuid, 1 }, { uuid, 2 } }} };

    adr::cache::playerCache.emplace(uuid, elem);
    std::cout << "Done creating player " << uuid << " in cache.\n";

    cacheUsernameAndAvatar(adr::cache::getElementFromCache(uuid));

    return adr::cache::getElementFromCache(uuid);
}

void adr::cache::saveCache([[maybe_unused]] const dpp::timer& timer)
{
    std::cout << "Saving Cache... Do Not Exit Program\n";
    for (const auto& i : adr::cache::playerCache) {
        i.second.player.save();
    }
    std::cout << "Finished Saving Cache!\n";
}

// Parse every playerdata file with the cache and return a list of their uuids
std::vector<dpp::snowflake> adr::cache::cacheAll() {
    using json = nlohmann::json;

    const std::filesystem::path filepath{ "./../playerdata" };
    std::filesystem::create_directory(filepath);

    std::vector<dpp::snowflake> vector{};
    vector.reserve(4);

    for (auto const& dirEntry : std::filesystem::directory_iterator{ filepath }) {
        std::ifstream fs{ dirEntry.path() };
        json data;

        try {
            data = json::parse(fs);
        }
        catch (const json::parse_error& e) {
            std::cerr << "adr::leaderboard::getLeaderboardEmbed() json parse error: " << e.what() << '\n';
            fs.close();
            return vector;
        }

        const adr::Player& player{ adr::cache::getPlayerFromCache(data["uuid"]) };

        vector.push_back(player.uuid());
    }

    return vector;
}

