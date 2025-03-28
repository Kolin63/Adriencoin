#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <dpp/dpp.h>
#include "botToken.h"
#include "onReady.h"
#include "player.h"
#include "onEvent.h"
#include "cache.h"

int main()
{
    dpp::cluster bot{ getBotToken(), dpp::i_default_intents | dpp::i_guild_members };

    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event)
        {
            adr::onSlashcommand(bot, event);
        });
    bot.on_select_click([&bot](const dpp::select_click_t& event)
        {
            adr::onSelectClick(event);
        });
    bot.on_button_click([&bot](const dpp::button_click_t& event)
        {
            adr::onButtonClick(event);
        });


    bot.on_ready([&bot]([[maybe_unused]] const dpp::ready_t& event)
    {
        if (dpp::run_once<struct register_bot_commands>())
        {
            adr::onReady(bot);

            bot.start_timer(adr::cache::saveCache, adr::cache::SAVE_FREQUENCY_SECONDS, adr::cache::saveCache);
        }
    });

    bot.start(dpp::st_wait);
}