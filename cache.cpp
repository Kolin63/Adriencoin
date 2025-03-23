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
    dpp::cluster bot{ getBotToken() };

    std::promise<void> promise{};
    std::future<void> future{ promise.get_future() };

    std::function<void(const dpp::confirmation_callback_t&)> func{ [&elem, &promise](const dpp::confirmation_callback_t& callback) {
        if (callback.is_error()) {
            std::cout << "Error getting username and avatar url for the cache\n";
            promise.set_value();
            return;
        }

        dpp::user_identified userIdent{ std::get<dpp::user_identified>(callback.value) };
        elem.username = userIdent.username;
        elem.avatarURL = userIdent.get_avatar_url();
        std::cout << "found uuid and username and avatarURL: " << userIdent.id << ' ' << elem.username << ' ' << elem.avatarURL << '\n';

        promise.set_value();
    } };

    bot.user_get_cached(elem.player.uuid(), func);

    future.wait();

    std::cout << elem.player.uuid() << ' ' << elem.username << ' ' << elem.avatarURL << ": " 
        << (elem.username == "" ? "Failure\n" : "Success! " + elem.username + '\n');
}

adr::playerCacheElement& adr::cache::getElementFromCache(const dpp::snowflake& uuid)
{
    std::cout << "Searching cache for Player " << uuid << '\n';
    if (auto search = adr::cache::playerCache.find(uuid); search != adr::cache::playerCache.end()) {
        std::cout << "Returning Player " << uuid << " from Cache\n";
        return search->second;
    }

    std::cout << "Could not find Player " << uuid << " in cache, creating new element\n";
    adr::playerCacheElement elem{ { uuid }, "", "", adr::Job::MAX, {{ { uuid }, { uuid }, { uuid } }} };

    cacheUsernameAndAvatar(elem);

    adr::cache::playerCache.emplace(uuid, elem);
    std::cout << "Done creating player " << uuid << " in cache.\n";

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
