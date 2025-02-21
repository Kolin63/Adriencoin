#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <array>
#include <string>
#include <dpp/dpp.h>
#include "item.h"

const std::array<std::string, adr::Item::MAX> adr::Item::names
{
    "adriencoin",
    "carrot",
    "gemstone",
    "wood",
    "fish",
    "rottenflesh",
    "book",
    "potion"
};

const std::array<dpp::snowflake, adr::Item::MAX> adr::Item::emojiIDs
{
    1342319536300621876,
    1342341829894471811,
    1342341843181899900,
    1342341866464739359,
    1342341836949295106,
    1342341859984543764,
    1342341814979657788,
    1342341851440615454
};
