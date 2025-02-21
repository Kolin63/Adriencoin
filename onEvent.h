#ifndef ON_EVENT_H
#define ON_EVENT_H

#include <dpp/dpp.h>

namespace adr {
    void onSlashcommand(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void onSelectClick(const dpp::select_click_t& event);
    void onButtonClick(const dpp::button_click_t& event);

    void doJob(const dpp::cluster& bot, const dpp::slashcommand_t& event);
}

#endif
