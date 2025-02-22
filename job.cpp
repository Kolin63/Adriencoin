#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <array>
#include <dpp/dpp.h>
#include "item.h"
#include "job.h"
#include "player.h"

namespace adr {
    const std::array<adr::Job, 14> Job::jobs{ {
        { adr::Job::farmer, "farmer", "farm", adr::Item{adr::Item::carrot, 20}, 1, 0xFF9D23 },
        { adr::Job::miner, "miner", "mine", adr::Item{adr::Item::gemstone, 5}, 1, 0x226BC9 },
        { adr::Job::lumberjack, "lumberjack", "chop", adr::Item{adr::Item::wood, 30}, 1, 0x78412E },
        { adr::Job::fisherman, "fisherman", "fish", adr::Item{adr::Item::fish, 30}, 1, 0x37E6B4 },
        { adr::Job::warrior, "warrior", "fight", adr::Item{adr::Item::rottenflesh, 60}, 1, 0xBD1209 },
        { adr::Job::enchanter, "enchanter", "enchant", adr::Item{adr::Item::book, 2}, 1, 0xAB5CFF },
        { adr::Job::brewer, "brewer", "brew", adr::Item{adr::Item::potion, 1}, 1, 0xE81C9D },

        { adr::Job::agriculturalist, "agriculturalist", "farm", adr::Item{adr::Item::carrot, 200}, 10, 0xFF9D23 },
        { adr::Job::collier, "collier", "mine", adr::Item{adr::Item::gemstone, 50}, 10, 0x226BC9 },
        { adr::Job::logger, "logger", "chop", adr::Item{adr::Item::wood, 300}, 10, 0x78412E },
        { adr::Job::piscator, "piscator", "fish", adr::Item{adr::Item::fish, 300}, 10, 0x37E6B4 },
        { adr::Job::knight, "knight", "fight", adr::Item{adr::Item::rottenflesh, 600}, 10, 0xBD1209 },
        { adr::Job::thaumaturgist, "thaumaturgist", "enchant", adr::Item{adr::Item::book, 20}, 10, 0xAB5CFF },
        { adr::Job::alchemist, "alchemist", "brew", adr::Item{adr::Item::potion, 10}, 10, 0xE81C9D },
    } };

    adr::Job::Id Job::getId(std::string_view jobName)
    {
        for (const adr::Job& i : adr::Job::jobs) {
            if (i.name == jobName) return i.id;
        }
        return adr::Job::MAX;
    }
}
