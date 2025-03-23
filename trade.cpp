#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <utility>
#include <array>
#include "trade.h"
#include "cache.h"

void adr::TradeOffer::setReceiverUUID(const dpp::snowflake& uuid)
{
    m_receiverUUID = uuid;
    m_active = true;
}

void adr::TradeOffer::clear()
{
    m_receiverUUID = m_giverUUID;
    m_giverGives.fill(0);
    m_receiverGives.fill(0);
    m_active = false;
}

void adr::TradeOffer::setInventory(invTypes invType, std::size_t index, int amount)
{
    (invType == adr::TradeOffer::give ? m_giverGives : m_receiverGives).at(index) = amount;
    m_active = true;
}

bool adr::TradeOffer::isValid() const
{
    if (m_receiverUUID == m_giverUUID || m_active == false) return false;

    auto validInv = [](const uint64_t& uuid, const Inventory& inv) -> bool {
        return adr::cache::getPlayerFromCache(uuid).inv() >= inv;
    };

    return validInv(m_giverUUID, m_giverGives) && validInv(m_receiverUUID, m_receiverGives);
}

void adr::TradeOffer::executeTrade()
{
    adr::cache::getPlayerFromCache(m_giverUUID).changeInv(m_giverGives);
    adr::cache::getPlayerFromCache(m_receiverUUID).changeInv(m_receiverGives);
    m_active = false;
}

std::string adr::TradeOffer::encodeDecimal(int64_t decimal)
{
    constexpr std::pair<char, char> range{ '!', '~' }; // inclusive
    constexpr char size{ range.second - range.first + 1 }; // 94
    static_assert(size == 94);

    if (decimal <= 0) return "!";

    std::string output{};

    while (decimal > 0) {
        output += char{ (decimal % size) + range.first };
        decimal /= size;
    }

    return output;
}

const std::string& adr::TradeOffer::generateSeed()
{
    for (std::size_t i{}; i < m_giverGives.size(); ++i) {
        m_seed += encodeDecimal(m_giverGives[i]) + encodeDecimal(m_receiverGives[i]);
    }
    
    return m_seed;
}

dpp::embed adr::TradeOffer::getEmbed()
{
    dpp::embed embed{};
    const std::string& giverUsername{ adr::cache::getElementFromCache(m_giverUUID).username };
    const std::string& receiverUsername{ 
        (m_receiverUUID != m_giverUUID) ? adr::cache::getElementFromCache(m_receiverUUID).username : "Unspecified"};

    embed.set_title("Trade Offer")
        .set_description("Between " + giverUsername + " and " + receiverUsername
            + "\n\n" + giverUsername + " is offering:\n"
            + adr::getNonZeroItems(m_giverGives)
            + '\n' + receiverUsername + " is offering:\n"
            + adr::getNonZeroItems(m_receiverGives)
            + "\nSeed: ```" + generateSeed() + "```");

    return embed;
}
