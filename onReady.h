#ifndef ONREADY_H
#define ONREADY_H

// To be called once per bot startup
// Adds commands, roles, etc
void onReady(dpp::cluster& bot);

void addRoles(dpp::cluster& bot, const dpp::snowflake& guildID);

#endif
