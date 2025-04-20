#include <array>
#include <dpp/dpp.h>
#include "item.h"
#include "job.h"

namespace adr {
    const std::array<adr::Job, 14> Job::jobs{ {
        { adr::Job::farmer, "farmer", "farm", adr::item{adr::i_carrot, 20}, 1, 0xFF9D23 },
        { adr::Job::miner, "miner", "mine", adr::item{adr::i_gemstone, 5}, 1, 0x226BC9 },
        { adr::Job::lumberjack, "lumberjack", "chop", adr::item{adr::i_wood, 30}, 1, 0x78412E },
        { adr::Job::fisherman, "fisherman", "fish", adr::item{adr::i_fish, 30}, 1, 0x37E6B4 },
        { adr::Job::warrior, "warrior", "fight", adr::item{adr::i_rottenflesh, 60}, 1, 0xBD1209 },
        { adr::Job::enchanter, "enchanter", "enchant", adr::item{adr::i_book, 2}, 1, 0xAB5CFF },
        { adr::Job::brewer, "brewer", "brew", adr::item{adr::i_potion, 1}, 1, 0xE81C9D },

        { adr::Job::agriculturalist, "agriculturalist", "farm", adr::item{adr::i_carrot, 200}, 10, 0xFF9D23 },
        { adr::Job::collier, "collier", "mine", adr::item{adr::i_gemstone, 50}, 10, 0x226BC9 },
        { adr::Job::logger, "logger", "chop", adr::item{adr::i_wood, 300}, 10, 0x78412E },
        { adr::Job::piscator, "piscator", "fish", adr::item{adr::i_fish, 300}, 10, 0x37E6B4 },
        { adr::Job::knight, "knight", "fight", adr::item{adr::i_rottenflesh, 600}, 10, 0xBD1209 },
        { adr::Job::thaumaturgist, "thaumaturgist", "enchant", adr::item{adr::i_book, 20}, 10, 0xAB5CFF },
        { adr::Job::alchemist, "alchemist", "brew", adr::item{adr::i_potion, 10}, 10, 0xE81C9D },
    } };

    adr::Job::Id Job::getId(std::string_view jobName)
    {
        for (const adr::Job& i : adr::Job::jobs) {
            if (i.name == jobName) return i.id;
        }
        return adr::Job::MAX;
    }
}
