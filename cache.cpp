#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <iostream>
#include <unordered_map>
#include <dpp/dpp.h>
#include "cache.h"
#include "botToken.h"

std::unordered_map<dpp::snowflake, adr::playerCacheElement> adr::cache::playerCache;

void adr::cache::cacheUsernameAndAvatar(adr::playerCacheElement& elem)
{
    std::cout << "Caching Username and Avatar for Player " << elem.player.uuid() << '\n';
    dpp::cluster bot{ getBotToken(), dpp::i_default_intents | dpp::i_guild_members };

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
