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
    "null",
    "stockofstonks",

    "Jerry's Carrots",
    "Divan's Gems",
    "Jack's Lumber Co",
    "Gerald's Fish",
    "Aatrox's Slaying Co",
    "Bert's Knowledge",
    "The Brewers",
    "Stonks Unlimited",
};

const std::array<dpp::snowflake, adr::Item::MAX> adr::Item::emojiIDs
{
    1342341829894471811, // carrot
    1342341843181899900, // gemstone
    1342341866464739359, // wood
    1342341836949295106, // fish
    1342341859984543764, // rottenflesh
    1342341814979657788, // book
    1342341851440615454, // potion
    1342319536300621876, // adriencoin
    1342730469640376352, // gaydriencoin
    1342730457414111273, // adriresource
    1342341836949295106, // null (fish texture)
    1353479698021224488, // stockofstonks

    1356787387463766046, // paper (stock)
    1356787387463766046, // paper (stock)
    1356787387463766046, // paper (stock)
    1356787387463766046, // paper (stock)
    1356787387463766046, // paper (stock)
    1356787387463766046, // paper (stock)
    1356787387463766046, // paper (stock)
    1356787387463766046, // paper (stock)
};

// Returns the Item ID Based on a String. Fallsback to adr::Item::null
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
