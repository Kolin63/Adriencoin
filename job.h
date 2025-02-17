#ifndef JOB_H
#define JOB_H

#include <string>
#include <array>
#include "item.h"

namespace adr
{
    struct Job
    {
        std::string name{};
        std::string action{};
        adr::Item item{};
        uint32_t color{};
    };

    std::array<Job, 14> jobs{ {
        { "farmer", "farm", adr::Item{adr::carrot, 20}, 0xFF9D23 },
        { "miner", "mine", adr::Item{adr::gemstone, 5}, 0x226BC9 },
        { "lumberjack", "chop", adr::Item{adr::wood, 30}, 0x78412E },
        { "fisherman", "fish", adr::Item{adr::fish, 30}, 0x37E6B4 },
        { "warrior", "fight", adr::Item{adr::rotten_flesh, 60}, 0xBD1209 },
        { "enchanter", "enchant", adr::Item{adr::book, 2}, 0xAB5CFF },
        { "brewer", "brew", adr::Item{adr::potion, 1}, 0xE81C9D },

        { "agriculturalist", "farm", adr::Item{adr::carrot, 200}, 0xFF9D23 },
        { "collier", "mine", adr::Item{adr::gemstone, 50}, 0x226BC9 },
        { "logger", "chop", adr::Item{adr::wood, 300}, 0x78412E },
        { "piscator", "fish", adr::Item{adr::fish, 300}, 0x37E6B4 },
        { "knight", "fight", adr::Item{adr::rotten_flesh, 600}, 0xBD1209 },
        { "thaumaturgist", "enchant", adr::Item{adr::book, 20}, 0xAB5CFF },
        { "alchemist", "brew", adr::Item{adr::potion, 10}, 0xE81C9D }
    } };
}

#endif
