#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <utility>
#include <array>
#include "trade.h"
#include "cache.h"

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
    const std::string& giverUsername{ adr::cache::getPlayerElementFromCache(m_giverUUID).username };
    const std::string& receiverUsername{ adr::cache::getPlayerElementFromCache(m_receiverUUID).username };

    embed.set_title("Trade Offer")
        .set_description("Between " + giverUsername + " and " + receiverUsername
            + "\n\n" + giverUsername + " is offering:\n"
            + adr::getNonZeroItems(m_giverGives)
            + '\n' + receiverUsername + " is offering:\n"
            + adr::getNonZeroItems(m_receiverGives)
            + "\nSeed: ```" + generateSeed() + "```");

    return embed;
}
