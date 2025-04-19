#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <array>
#include <dpp/snowflake.h>
#include "emoji.h"

namespace adr
{
    struct Item
    {
        enum Id
        {
            carrot,
            gemstone,
            wood,
            fish,
            rottenflesh,
            book,
            potion,
            adriencoin,
            gaydriencoin,
            adriresource,
            null,
            stockofstonks,

            jerrys_carrots,
            divans_gems,
            jacks_lumber,
            geralds_fish,
            aatroxs_slaying,
            berts_knowledge,
            the_brewers,
            stonks_unlimited,

            MAX
        };

        static const std::array<std::string, MAX> names;
        static adr::Item::Id getId(const std::string& name);
        static const std::array<dpp::snowflake, MAX> emojiIDs;
        static std::string getEmojiMention(adr::Item::Id id);

        Id id{};
        int amount{};
    };
}

#endif
