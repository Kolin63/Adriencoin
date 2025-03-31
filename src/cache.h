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
        std::string username{ };
        std::string avatarURL{ };
        adr::Job::Id tempJob{ adr::Job::MAX };

        static constexpr std::size_t MAX_TRADE_OFFERS{ 3 };
        std::array<adr::TradeOffer, MAX_TRADE_OFFERS> tradeOffers;
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
        static void clear() { saveCache(); adr::cache::playerCache.clear(); }
        static std::vector<dpp::snowflake> cacheAll();
    };
} 

#endif
