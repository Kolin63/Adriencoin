#ifndef CACHE_H
#define CACHE_H

#include <unordered_map>
#include <string>
#include <array>
#include <dpp/dpp.h>
#include "player.h"
#include "job.h"

namespace adr 
{
    struct playerCacheElement {
        adr::Player player;
        std::string username{};
        std::string avatarURL{};
        adr::Job::Id tempJob{ adr::Job::MAX };

        static constexpr int MAX_TRADE_OFFERS{ 3 };
        std::array<Inventory, MAX_TRADE_OFFERS> tradeOffers{};
    };

    class cache
    {
    private:
        static std::unordered_map<dpp::snowflake, playerCacheElement> playerCache;


    public:
        static playerCacheElement& getPlayerElementFromCache(const dpp::snowflake& uuid);
        static Player& getPlayerFromCache(const dpp::snowflake& uuid) { return getPlayerElementFromCache(uuid).player; }

        static constexpr std::uint64_t SAVE_FREQUENCY_SECONDS{ 60 * 10 }; // 10 Minutes
        static void saveCache(const dpp::timer& = {});
    };
} 

#endif
