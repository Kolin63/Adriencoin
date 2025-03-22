#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <iostream>
#include <unordered_map>
#include <dpp/dpp.h>
#include "cache.h"
#include "botToken.h"

std::unordered_map<dpp::snowflake, adr::playerCacheElement> adr::cache::playerCache;

adr::playerCacheElement& adr::cache::getPlayerElementFromCache(const dpp::snowflake& uuid)
{
    std::cout << "Searching cache for Player " << uuid << '\n';
    if (auto search = adr::cache::playerCache.find(uuid); search != adr::cache::playerCache.end()) {
        std::cout << "Returing Player " << uuid << " from Cache\n";
        return search->second;
    }

    std::cout << "Could not find Player " << uuid << " in cache, creating new element\n";
    adr::playerCacheElement& found{ adr::cache::playerCache.emplace(uuid, adr::playerCacheElement{ { uuid } }).first->second };

    dpp::cluster bot{ getBotToken() };
    bot.user_get_cached(uuid, [&found](const dpp::confirmation_callback_t& callback) {
        if (callback.is_error()) {
            return;
        }

        dpp::user_identified userIdent{ std::get<dpp::user_identified>(callback.value) };
        found.username = userIdent.username;
        found.avatarURL = userIdent.get_avatar_url();
    });

    return found;
}

void adr::cache::saveCache([[maybe_unused]] const dpp::timer& timer)
{
    std::cout << "Saving Cache... Do Not Exit Program\n";
    for (const auto& i : adr::cache::playerCache) {
        i.second.player.save();
    }
    std::cout << "Finished Saving Cache!\n";
}
