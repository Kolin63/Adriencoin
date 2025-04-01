#include <filesystem>
#include "stock.h"

std::filesystem::path path{ "data/stock.json" };

namespace adr
{
    unsigned int adr::Stock::day{ 0 };
    std::array<adr::Stock, adr::Stock::MAX> Stock::stocks{ adr::Stock::parseJSON() };
}

adr::Stock& adr::Stock::getStock(Id id)
{
    return adr::Stock::stocks[id];
}

std::array<adr::Stock, adr::Stock::MAX> adr::Stock::parseJSON()
{
}

void adr::Stock::newDay()
{
    ++adr::Stock::day;
}

unsigned int adr::Stock::getDay()
{
    return adr::Stock::day;
}
