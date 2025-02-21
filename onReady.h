#ifndef ONREADY_H
#define ONREADY_H

namespace adr {
    // To be called once per bot startup
    // Adds commands, roles, etc
    void onReady(dpp::cluster& bot);

    void addRoles(dpp::cluster& bot, const dpp::snowflake& guildID);

    void addEmojis(dpp::cluster& bot, const dpp::snowflake& guildID);

    void addSlashCommands(dpp::cluster& bot);
}

#endif
