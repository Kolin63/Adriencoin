#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <string>
#include <dpp/dpp.h>
#include "onSlashcommand.h"
#include "onReady.h"
#include "job.h"
#include "player.h"

void adr::onSlashcommand(dpp::cluster& bot, const dpp::slashcommand_t& event)
{
    const std::string& commandName{ event.command.get_command_name() };
    if (commandName == "addroles") { 
        adr::addRoles(bot, event.command.guild_id);
        event.reply(dpp::message("Attempted to create required roles").set_flags(dpp::m_ephemeral));
    }
    else if (commandName == "addcommands") {
        adr::addSlashCommands(bot);
        event.reply(dpp::message("Attempted to register commands").set_flags(dpp::m_ephemeral));
    }
    else if (commandName == "printuserinv") {
        adr::Player{ event.command.usr.id }.print();
        event.reply(dpp::message("Data printed to console").set_flags(dpp::m_ephemeral));
    }
    else if (commandName == "setjob") {
        adr::Job::Id job = static_cast<adr::Job::Id>(std::get<int64_t>(event.get_parameter("job")));
        adr::Player{ event.command.usr.id }.setJob(job);
        event.reply(dpp::message("Set the job").set_flags(dpp::m_ephemeral));
    }
    else {
        adr::doJob(bot, event);
    }
}

void adr::doJob([[maybe_unused]] const dpp::cluster& bot, const dpp::slashcommand_t& event)
{
    const std::string& commandName{ event.command.get_command_name() };

    // turns true if the player can do a job, and stays false otherwise
    // if it stays false, then the bot will tell them that they couldn't do anything
    bool didAJob{ false };

    adr::Player player{ event.command.usr.id };

    for (const adr::Job& i : adr::Job::jobs) if (player.job() == i.id && commandName == i.action) {
        didAJob = true;
        player[i.item.id] += i.item.amount;
        player[adr::Item::adriencoin] += i.adriencoin;

        event.reply(i.action + ": +" + std::to_string(i.item.amount) + ' ' + adr::Item::names[i.item.id] + " and +" + std::to_string(i.adriencoin) + " Adriencoin.");
    }

    if (!didAJob) {
        event.reply("You could not do that! If you were trying to do a job, do you have a job assigned?");
    }
}
