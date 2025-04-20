#ifndef TRADE_H
#define TRADE_H

#include <dpp/dpp.h>
#include "inventory.h"

namespace adr {
    class TradeOffer 
    {
    private:
        bool m_active{ false };
        const dpp::snowflake m_giverUUID;
        dpp::snowflake m_receiverUUID{ m_giverUUID };
        inventory m_giverGives{};
        inventory m_receiverGives{};
        std::string m_seed{};
        std::size_t m_slot{};

    public:
        TradeOffer(const dpp::snowflake& giverUUID, std::size_t slot)
            : m_giverUUID{ giverUUID }
            , m_slot{ slot }
        {
        }

        void setReceiverUUID(const dpp::snowflake& uuid);
        const dpp::snowflake& getReceiverUUID() const { return m_receiverUUID; }
        const dpp::snowflake& getGiverUUID() const { return m_giverUUID; }

        bool isActive() const { return m_active; }
        void activate() { m_active = true; }
        void clear();

        enum invTypes {
            give,
            receive
        };

        void setInventory(invTypes invType, std::size_t index, int amount);
        bool isValid() const;
        // make sure to check isValid() beforehand
        void executeTrade();

        const std::string& generateSeed();
        const std::string& getSeed() { return m_seed; }
        void clearSeed() { m_seed = ""; }

        dpp::embed getEmbed();
    };
}

#endif
