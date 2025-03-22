#ifndef TRADE_H
#define TRADE_H

#include <dpp/dpp.h>
#include "Random.h"
#include "inventory.h"

namespace adr {
    class TradeOffer 
    {
    private:
        const dpp::snowflake& m_giverUUID;
        const dpp::snowflake& m_receiverUUID;
        Inventory m_giverGives{};
        Inventory m_receiverGives{};
        std::string m_seed{};

    public:
        TradeOffer(dpp::snowflake& senderUUID, dpp::snowflake& receiverUUID)
            : m_giverUUID{ senderUUID }
            , m_receiverUUID{ receiverUUID }
        {
        }

        enum invTypes {
            give,
            send
        };

        void setInventory(invTypes invType, std::size_t index, int amount) {
            (invType == give ? m_giverGives : m_receiverGives).at(index) = amount;
        }

        std::string encodeDecimal(int64_t decimal);
        const std::string& generateSeed();
        const std::string& getSeed() { return m_seed; }

        dpp::embed getEmbed();
    };
}

#endif
