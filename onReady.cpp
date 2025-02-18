#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <unordered_map>
#include <algorithm>
#include <string>
#include <dpp/dpp.h>
#include "onReady.h"
#include "job.h"
#include "item.h"

namespace adr {
    void addSlashCommands(dpp::cluster& bot)
    {
        std::cout << "making commands!\n";

        //dpp::slashcommand echo{ "echo", "Have the bot echo your message", bot.me.id };
        //echo.add_option(dpp::command_option{ dpp::co_string, "input", "add input here", true });

        //dpp::slashcommand add{ "add", "Add two numbers", bot.me.id };
        //add.add_option(dpp::command_option{ dpp::co_integer, "x", "first number", true });
        //add.add_option(dpp::command_option{ dpp::co_integer, "y", "second number", true });

        for (const adr::Job& i : adr::jobs)
        {
            dpp::slashcommand slashcommand{ i.action, (i.action + ' ' + adr::Item::names[i.item.id]), bot.me.id };
            std::cout << i.name << ' ' << i.action << '\n';
            bot.global_command_create(slashcommand);
        }

        dpp::slashcommand addRoles{ "addroles", "roles", bot.me.id };
        addRoles.default_member_permissions = dpp::p_administrator;

        dpp::slashcommand addCommands{ "addcommands", "re-register the commands if theyre not loading", bot.me.id };
        addCommands.default_member_permissions = dpp::p_administrator;

        bot.global_bulk_command_create({ addRoles, addCommands });
    }

    void addRoles(dpp::cluster& bot, const dpp::snowflake& guildID)
    {
        std::cout << "checking and creating roles!\n";

        bot.roles_get(guildID, [&bot, &guildID](const dpp::confirmation_callback_t& event) {
            if (event.is_error()) {
                std::cerr << "Failed to get roles: " << event.get_error().message << '\n';
                return;
            }

            const auto& roles = std::get<dpp::role_map>(event.value);
            for (const adr::Job& i : adr::jobs) {
                auto it = std::find_if(roles.begin(), roles.end(), [&i](const auto& pair) {
                    return pair.second.name == i.name;
                    });

                if (it == roles.end()) {
                    dpp::role role{};
                    role.set_name(i.name);
                    role.set_color(i.color);
                    role.set_guild_id(guildID);

                    bot.role_create(role);
                }
            }
            });
    }

    void onReady(dpp::cluster& bot)
    {
        addSlashCommands(bot);
    }
}