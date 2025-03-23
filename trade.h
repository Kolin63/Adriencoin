#ifndef TRADE_H
#define TRADE_H

#include <dpp/dpp.h>
#include "Random.h"
#include "inventory.h"

namespace adr {
    class TradeOffer 
    {
    private:
        bool m_active{ false };
        const dpp::snowflake& m_giverUUID;
        dpp::snowflake m_receiverUUID{ m_giverUUID };
        Inventory m_giverGives{};
        Inventory m_receiverGives{};
        std::string m_seed{};

    public:
        TradeOffer(const dpp::snowflake& giverUUID)
            : m_giverUUID{ giverUUID }
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

    private:
        static std::string encodeDecimal(int64_t decimal);
    public:
        const std::string& generateSeed();
        const std::string& getSeed() { return m_seed; }

        dpp::embed getEmbed();
    };
}

#endif
