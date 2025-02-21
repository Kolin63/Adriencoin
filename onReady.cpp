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

        dpp::slashcommand setJob{ "setjob", "sets the job of a user", bot.me.id };
        setJob.default_member_permissions = dpp::p_administrator;
        dpp::command_option setJobOption{ dpp::co_integer, "job", "the job to assign", true };

        for (const adr::Job& i : adr::Job::jobs)
        {
            dpp::slashcommand slashcommand{ i.action, (i.action + ' ' + adr::Item::names[i.item.id]), bot.me.id };
            std::cout << i.name << ' ' << i.action << '\n';
            bot.global_command_create(slashcommand);

            setJobOption.add_choice(dpp::command_option_choice{ i.name, i.id });
        }

        setJob.add_option(setJobOption);

        dpp::slashcommand addRoles{ "addroles", "roles", bot.me.id };
        addRoles.default_member_permissions = dpp::p_administrator;

        dpp::slashcommand addCommands{ "addcommands", "re-register the commands if theyre not loading", bot.me.id };
        addCommands.default_member_permissions = dpp::p_administrator;

        dpp::slashcommand printUserInv{ "printuserinv", "print the inventory of a user", bot.me.id };
        printUserInv.default_member_permissions = dpp::p_administrator;

        bot.global_bulk_command_create({ addRoles, addCommands, printUserInv, setJob });
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
            for (const adr::Job& i : adr::Job::jobs) {
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