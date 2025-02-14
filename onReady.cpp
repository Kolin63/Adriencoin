#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <string>
#include <dpp/dpp.h>
#include "onReady.h"
#include "job.h"
#include "item.h"

static void addSlashCommands(dpp::cluster& bot)
{
    std::cout << "making commands!\n";

    //dpp::slashcommand echo{ "echo", "Have the bot echo your message", bot.me.id };
    //echo.add_option(dpp::command_option{ dpp::co_string, "input", "add input here", true });

    //dpp::slashcommand add{ "add", "Add two numbers", bot.me.id };
    //add.add_option(dpp::command_option{ dpp::co_integer, "x", "first number", true });
    //add.add_option(dpp::command_option{ dpp::co_integer, "y", "second number", true });

    for (const Job::Job& i : Job::jobs)
    {
        std::cout << i.name << ' ' << i.action << '\n';
        dpp::slashcommand slashcommand{ i.action, (i.action + ' ' + Item::names[i.item.id]), bot.me.id};
        bot.global_command_create(slashcommand);
    }

    dpp::slashcommand addRoles{ "addroles", "roles", bot.me.id };
    addRoles.default_member_permissions = dpp::p_administrator;

    bot.global_bulk_command_create({ addRoles });
}

void addRoles(dpp::cluster& bot, const dpp::snowflake& guildID)
{
    std::cout << "making roles!\n";

    auto makeRole = [&bot, &guildID](std::string name, uint32_t color) {
        dpp::role role{};
        role.set_name(name);
        role.set_color(color);
        role.set_guild_id(guildID);
        bot.role_create(role);
    };

    makeRole("farmer", 0xFF9D23);
    makeRole("miner", 0x226bc9);
    makeRole("lumberjack", 0x78412e);
    makeRole("fisherman", 0x37e6b4);
    makeRole("warrior", 0xbd1209);
    makeRole("enchanter", 0xab5cff);
    makeRole("brewer", 0xe81c9d);
}

void onReady(dpp::cluster& bot)
{
    bot.global_bulk_command_delete();
    addSlashCommands(bot);
}