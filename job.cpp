#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <array>
#include <dpp/dpp.h>
#include "item.h"
#include "job.h"

namespace adr {
    const std::array<adr::Job, 14> jobs{ {
        { "farmer", "farm", adr::Item{adr::Item::carrot, 20}, 0xFF9D23 },
        { "miner", "mine", adr::Item{adr::Item::gemstone, 5}, 0x226BC9 },
        { "lumberjack", "chop", adr::Item{adr::Item::wood, 30}, 0x78412E },
        { "fisherman", "fish", adr::Item{adr::Item::fish, 30}, 0x37E6B4 },
        { "warrior", "fight", adr::Item{adr::Item::rotten_flesh, 60}, 0xBD1209 },
        { "enchanter", "enchant", adr::Item{adr::Item::book, 2}, 0xAB5CFF },
        { "brewer", "brew", adr::Item{adr::Item::potion, 1}, 0xE81C9D },

        { "agriculturalist", "farm", adr::Item{adr::Item::carrot, 200}, 0xFF9D23 },
        { "collier", "mine", adr::Item{adr::Item::gemstone, 50}, 0x226BC9 },
        { "logger", "chop", adr::Item{adr::Item::wood, 300}, 0x78412E },
        { "piscator", "fish", adr::Item{adr::Item::fish, 300}, 0x37E6B4 },
        { "knight", "fight", adr::Item{adr::Item::rotten_flesh, 600}, 0xBD1209 },
        { "thaumaturgist", "enchant", adr::Item{adr::Item::book, 20}, 0xAB5CFF },
        { "alchemist", "brew", adr::Item{adr::Item::potion, 10}, 0xE81C9D }
    } };

    void doJob([[maybe_unused]] const dpp::cluster& bot, const dpp::slashcommand_t& event)
    {
        const std::string& commandName{ event.command.get_command_name() };

        for (adr::Job i : adr::jobs) if (commandName == i.action){
            event.reply(i.action);
        }
    }
}
