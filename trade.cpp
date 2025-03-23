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
    // that big lambda is to check if any of the values are negative
    if (m_receiverUUID == m_giverUUID || m_active == false || [](const Inventory& a, const Inventory& b) -> bool {
        for (std::size_t i{}; i < a.size(); ++i) 
            if (a[i] < 0 || b[i] < 0) return true; // true: values are negative
        return false; // false: not negative
        }(m_giverGives, m_receiverGives)) return false;

    auto validInv = [](const uint64_t& uuid, const Inventory& inv) -> bool {
        return adr::cache::getPlayerFromCache(uuid).inv() >= inv;
    };

    return validInv(m_giverUUID, m_giverGives) && validInv(m_receiverUUID, m_receiverGives);
}

void adr::TradeOffer::executeTrade()
{
    adr::Player& giver{ adr::cache::getPlayerFromCache(m_giverUUID) };
    adr::Player& receiver{ adr::cache::getPlayerFromCache(m_receiverUUID) };

    giver.subtractInv(m_giverGives);
    giver.changeInv(m_receiverGives);

    receiver.subtractInv(m_receiverGives);
    receiver.changeInv(m_giverGives);

    clear();
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
    m_seed = "https://";
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
            + "\n(Slot " + std::to_string(m_slot) + ")"
            + "\n\n" + giverUsername + " is offering:\n"
            + adr::getNonZeroItems(m_giverGives)
            + '\n' + receiverUsername + " will give:\n"
            + adr::getNonZeroItems(m_receiverGives)
            + "\nSeed: " + generateSeed())
        .set_thumbnail("https://raw.githubusercontent.com/Kolin63/Adriencoin/refs/heads/main/art/item/emerald.webp")
        .set_color(0x74f793);

    return embed;
}
