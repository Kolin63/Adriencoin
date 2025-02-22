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
    "Resource Exchange",
    "Everything"
};

adr::shop::Id adr::shop::getId(const std::string_view str) {
    for (std::size_t i{}; i < adr::shop::MAX; ++i) {
        if (adr::shop::names[i] == str) 
            return static_cast<adr::shop::Id>(i);
    }
    return adr::shop::MAX;
}

extern const std::array<adr::Item::Id, adr::shop::MAX> adr::shop::itemIcons{
    adr::Item::gaydriencoin,
    adr::Item::adriresource,
    adr::Item::carrot,
    adr::Item::gemstone,
    adr::Item::adriencoin
};

extern const std::array<std::string, adr::shop::MAX> adr::shop::jokes{
    "If Adrien gets a B, is he Bdrien?",
    "If you get a resource for the first time, is it just a source?",
    "If there is a jaw-b, is there a chin-b?",
    "If there is an Exchange, why is there no Ychange?",
    "If there is Eve-rything, is there day of-rything?"
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
        dpp::embed{}.set_title(adr::shop::names[adr::shop::job]).set_color(0xff804a).set_description("*Tier Upgrades only available when the buyer has no active loan* \n* Tier 2: 100 "
            + adr::Item::getEmojiMention(adr::Item::adriencoin) + ", 70 " + adr::Item::getEmojiMention(adr::Item::book)
            + "\n* Tier 3: Coming Soon"
            + "\n* Job Change: 100 " + adr::Item::getEmojiMention(adr::Item::adriencoin) + " (Will revert you back to Tier 1 of your new job)"),

        // Resource Exchange
        dpp::embed{}.set_title(adr::shop::names[adr::shop::resource]).set_color(0xff534a).set_description("10 " + adr::Item::getEmojiMention(adr::Item::adriencoin) + " trades to: *(pick ONE)*"),

        // Everything
        dpp::embed{}.set_title(adr::shop::names[adr::shop::everything]).set_color(0x6998ff)
    };

    for (std::size_t i{}; i < adr::Job::tierOneJobsSize; ++i) {
        arr[adr::shop::adriresource].set_description(arr[adr::shop::adriresource].description
            + "\n* " + std::to_string(adr::Job::jobs[i].item.amount) + adr::Item::getEmojiMention(adr::Job::jobs[i].item.id));

        arr[adr::shop::resource].set_description(arr[adr::shop::resource].description
            + "\n* " + std::to_string(adr::Job::jobs[i].item.amount * 5) + adr::Item::getEmojiMention(adr::Job::jobs[i].item.id));
    }

    for (std::size_t i{}; i < adr::shop::MAX; ++i) {
        arr[i].set_thumbnail(dpp::emoji{ adr::Item::names[adr::shop::itemIcons[i]], adr::Item::emojiIDs[adr::shop::itemIcons[i]] }.get_url());
    }

    return arr;
}

extern const std::array<dpp::embed, adr::shop::MAX> adr::shop::embeds{ adr::shop::makeEmbedsList() };

dpp::message adr::shop::getMessage(adr::shop::Id id)
{
    if (id == adr::shop::MAX)
        return dpp::message{}.add_embed(dpp::embed{}.set_title("Error").set_description("There was an error getting that").set_color(0xFF0000));

    dpp::message msg{};

    if (id != adr::shop::everything) 
        msg.add_embed(adr::shop::embeds[id]);
    else 
        for (std::size_t i{}; i < adr::shop::everything; ++i)
            msg.add_embed(adr::shop::embeds[i]);

    dpp::component select{};
    select.set_type(dpp::cot_selectmenu)
        .set_placeholder("Choose")
        .set_id("shopselect");

    for (std::size_t i{}; i < adr::shop::MAX; ++i) {
        select.add_select_option(dpp::select_option{ adr::shop::names[i], adr::shop::names[i], adr::shop::jokes[i]}
            .set_emoji(adr::Item::names[adr::shop::itemIcons[i]], adr::Item::emojiIDs[adr::shop::itemIcons[i]]));
    }

    select.set_default_value(adr::shop::names[id]);

    msg.add_component(dpp::component{}.add_component(select));

    return msg;
}
