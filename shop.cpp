#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <string>
#include <functional>
#include <array>
#include <dpp/dpp.h>
#include "shop.h"
#include "item.h"
#include "job.h"

extern const std::array<std::string, adr::shop::MAX> adr::shop::names {
    "Gaydrien Coins",
    "Adri-Resources",
    "Jobs",
    "Resource Exchange"
};

const std::array<dpp::embed, adr::shop::MAX> adr::shop::makeEmbedsList() {
    std::array<dpp::embed, adr::shop::MAX> arr
    {
        // Gaydrien
        dpp::embed{}.set_title(adr::shop::names[adr::shop::gaydrien]).set_color(0xffd621).set_description("\nGaydrien coins (" + adr::Item::getEmojiMention(adr::Item::gaydriencoin)
            + ") can be traded for 1000 adrien coins (" + adr::Item::getEmojiMention(adr::Item::adriencoin) + "). "
            + "1000 " + adr::Item::getEmojiMention(adr::Item::adriencoin) + " CANNOT be traded in for 1 " + adr::Item::getEmojiMention(adr::Item::gaydriencoin)),

        // Adri-Resource
        dpp::embed{}.set_title(adr::shop::names[adr::shop::adriresource]).set_color(0xe4b8f5).set_description("1 " + adr::Item::getEmojiMention(adr::Item::adriresource) + " trades to: *(pick ONE)*"),

        // Jobs
        dpp::embed{}.set_title(adr::shop::names[adr::shop::job]).set_color(0x0000FF).set_description(*"*Tier Upgrades only available when the buyer has no active loan*"
            + "\n* Tier 2: 100 " + adr::Item::getEmojiMention(adr::Item::adriencoin) + ", 70 " + adr::Item::getEmojiMention(adr::Item::book)
            + "\n* Tier 3: Coming Soon"
            + "\n* Job Change: 100 " + adr::Item::getEmojiMention(adr::Item::adriencoin) + " (Will revert you back to Tier 1 of your new job)"),

        // Resource Exchange
        dpp::embed{}.set_title(adr::shop::names[adr::shop::resource]).set_color(0x0000FF).set_description("10 " + adr::Item::getEmojiMention(adr::Item::adriencoin) + " trades to: *(pick ONE)*")
    };

    for (std::size_t i{}; i < adr::Job::tierOneJobsSize; ++i) {
        arr[adr::shop::adriresource].set_description(arr[adr::shop::adriresource].description
            + "\n* " + std::to_string(adr::Job::jobs[i].item.amount) + adr::Item::getEmojiMention(adr::Job::jobs[i].item.id));

        arr[adr::shop::resource].set_description(arr[adr::shop::adriresource].description
            + "\n* " + std::to_string(adr::Job::jobs[i].item.amount * 5) + adr::Item::getEmojiMention(adr::Job::jobs[i].item.id));
    }

    return arr;
}

extern const std::array<dpp::embed, adr::shop::MAX> adr::shop::embeds{ adr::shop::makeEmbedsList() };

dpp::message adr::shop::getMessage()
{
    dpp::message msg{ adr::shop::embeds[adr::shop::resource] };
    return msg;
}
