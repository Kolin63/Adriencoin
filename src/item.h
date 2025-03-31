#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <array>
#include <dpp/dpp.h>

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
