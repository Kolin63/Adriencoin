#include "inventory.h"
#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <chrono>
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
    m_seed = "";
}

void adr::TradeOffer::setInventory(invTypes invType, std::size_t index, int amount)
{
    (invType == adr::TradeOffer::give 
        ? m_giverGives 
        : m_receiverGives
    ).at(index) = amount;

    m_active = true;
}

bool adr::TradeOffer::isValid() const
{
    if (m_receiverUUID == m_giverUUID || m_active == false || m_seed == ""

    // that ugly lambda is to check if any of the values are negative
        || [](const inventory& a, const inventory& b) -> bool {
            for (std::size_t i{}; i < a.size(); ++i) 
                // true: values are negative
                if (a[i] < 0 || b[i] < 0) return true; 
            // false: not negative
            return false; 
        } (m_giverGives, m_receiverGives)
    ) 
        return false;

    // check that the players inventory satisfies the trade inventory
    auto validInv = 
    [](const dpp::snowflake& uuid, const inventory& inv) -> bool {
        return adr::cache::getPlayerFromCache(uuid).canBuy(inv);
    };

    return validInv(m_giverUUID, m_giverGives) 
        && validInv(m_receiverUUID, m_receiverGives);
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

const std::string& adr::TradeOffer::generateSeed()
{
    m_seed = "https://a" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) + '/';
    
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
            + (m_seed != "" ? "\nSeed: " + m_seed : "\n"))
        .set_thumbnail("https://raw.githubusercontent.com/Kolin63/Adriencoin/refs/heads/main/art/item/emerald.webp")
        .set_color(0x74f793);

    return embed;
}
