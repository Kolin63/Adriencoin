#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <dpp/dpp.h>
#include "botToken.h"
#include "onReady.h"
#include "player.h"
#include "onSlashcommand.h"

int main()
{
    dpp::cluster bot(getBotToken());

    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event)
        {
            adr::onSlashcommand(bot, event);
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