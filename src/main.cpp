#include <dpp/dpp.h>
#include "botToken.h"
#include "onReady.h"
#include "onEvent.h"
#include "cache.h"
#include "daily.h"

int main()
{
#ifdef DEBUG 
    std::cout << std::string(80, '=') 
        << "\nDEBUG MODE ENABLED\n" 
        << std::string(80, '=') << '\n';
#endif
    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([](const dpp::slashcommand_t& event)
        {
            adr::onSlashcommand(bot, event);
        });
    bot.on_select_click([](const dpp::select_click_t& event)
        {
            adr::onSelectClick(event);
        });
    bot.on_button_click([](const dpp::button_click_t& event)
        {
            adr::onButtonClick(event);
        });


    bot.on_ready([]([[maybe_unused]] const dpp::ready_t& event)
    {
        if (dpp::run_once<struct register_bot_commands>())
        {
            adr::onReady(bot);

            // Autosave the Cache
            bot.start_timer(adr::cache::saveCache, adr::cache::SAVE_FREQUENCY_SECONDS, adr::cache::saveCache);

            // Do Dailies at midnight
            // First one calculates time to midnight from time now
            // After that one, a persistent timer is started to run every 24 hours
            bot.start_timer([](dpp::timer t) {

                bot.stop_timer(t);
                adr::daily::doDailyTasks(bot);
                bot.start_timer([]([[maybe_unused]] const dpp::timer& t) {
                    adr::daily::doDailyTasks(bot);
                    }, (60 * 60 * 24));

                }, adr::daily::getTimeToMidnight());
        }
    });

    bot.start(dpp::st_wait);
}
