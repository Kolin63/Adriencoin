#ifndef ON_SLASHCOMMAND_H
#define ON_SLASHCOMMAND_H

#include <dpp/dpp.h>

namespace adr {
    void onSlashcommand(dpp::cluster& bot, const dpp::slashcommand_t& event);

    void doJob(const dpp::cluster& bot, const dpp::slashcommand_t& event);
}

#endif
