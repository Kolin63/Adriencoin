#ifndef CACHE_H
#define CACHE_H

#include <unordered_map>
#include <string>
#include <array>
#include <dpp/dpp.h>
#include "player.h"
#include "job.h"
#include "trade.h"

namespace adr 
{
    struct playerCacheElement {
        adr::Player player;
        std::string username{ "Unknown" };
        std::string avatarURL{ "https://cdn.discordapp.com/avatars/1339761558019903528/598edcf7dfc76c72a88b77c91d5e8b3e.png" };
        adr::Job::Id tempJob{ adr::Job::MAX };

        static constexpr std::size_t MAX_TRADE_OFFERS{ 3 };
        std::array<adr::TradeOffer, MAX_TRADE_OFFERS> tradeOffers{ player.uuid(), player.uuid(), player.uuid() };
    };

    class cache
    {
    private:
        static std::unordered_map<dpp::snowflake, playerCacheElement> playerCache;

        static void cacheUsernameAndAvatar(playerCacheElement&);

    public:
        static playerCacheElement& getElementFromCache(const dpp::snowflake& uuid);
        static Player& getPlayerFromCache(const dpp::snowflake& uuid) { return getElementFromCache(uuid).player; }

        static constexpr std::uint64_t SAVE_FREQUENCY_SECONDS{ 60 * 10 }; // 10 Minutes
        static void saveCache(const dpp::timer& = {});
    };
} 

#endif
