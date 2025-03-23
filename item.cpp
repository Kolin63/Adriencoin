#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <array>
#include <string>
#include <dpp/dpp.h>
#include "item.h"

const std::array<std::string, adr::Item::MAX> adr::Item::names
{
    "carrot",
    "gemstone",
    "wood",
    "fish",
    "rottenflesh",
    "book",
    "potion",
    "adriencoin",
    "gaydriencoin",
    "adriresource",
    "null"
};

const std::array<dpp::snowflake, adr::Item::MAX> adr::Item::emojiIDs
{
    1342341829894471811,
    1342341843181899900,
    1342341866464739359,
    1342341836949295106,
    1342341859984543764,
    1342341814979657788,
    1342341851440615454,
    1342319536300621876,
    1342730469640376352,
    1342730457414111273,
    1342341836949295106,
};

adr::Item::Id adr::Item::getId(const std::string& name)
{
    for (std::size_t i{}; i < names.size(); ++i) {
        if (names[i] == name) return static_cast<adr::Item::Id>(i);
    }
    return adr::Item::null;
}

std::string adr::Item::getEmojiMention(adr::Item::Id id)
{
    return dpp::emoji{ adr::Item::names[id], adr::Item::emojiIDs[id] }.get_mention();
}
