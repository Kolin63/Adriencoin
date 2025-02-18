#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <dpp/dpp.h>
#include "botToken.h"
#include "onReady.h"
#include "job.h"

int main()
{
    dpp::cluster bot(getBotToken());

    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event)
    {
        const std::string& commandName{ event.command.get_command_name() };
        if (commandName == "addroles") { 
            adr::addRoles(bot, event.command.guild_id);
            event.reply(dpp::message("Attempted to create required roles").set_flags(dpp::m_ephemeral));
        }
        else if (commandName == "addcommands") {
            adr::addSlashCommands(bot);
            event.reply(dpp::message("Attempted to register commands").set_flags(dpp::m_ephemeral));
        }
        else {
            adr::doJob(bot, event);
        }
    });

    bot.on_ready([&bot]([[maybe_unused]] const dpp::ready_t& event)
    {
        if (dpp::run_once<struct register_bot_commands>())
        {
            adr::onReady(bot);
        }
    });

    bot.start(dpp::st_wait);
}