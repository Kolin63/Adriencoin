#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <string>
#include <dpp/dpp.h>
#include "onEvent.h"
#include "onReady.h"
#include "job.h"
#include "player.h"

void adr::onSlashcommand(dpp::cluster& bot, const dpp::slashcommand_t& event)
{
    const std::string& commandName{ event.command.get_command_name() };
    if (commandName == "view") {
        adr::Player player{ std::get<dpp::snowflake>(event.get_parameter("player")) };
        player.print();
        event.reply("printed to console.");
    }
    else if (commandName == "addroles") { 
        adr::addRoles(bot, event.command.guild_id);
        event.reply(dpp::message("Attempted to create required roles").set_flags(dpp::m_ephemeral));
    }
    else if (commandName == "addemojis") {
        adr::addEmojis(bot, event.command.guild_id);
        event.reply(dpp::message("Attempted to create required emojis " + dpp::emoji{ "adriencoin", 1342319536300621876 }.get_mention()).set_flags(dpp::m_ephemeral));
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
    else if (commandName == "jobembed") {
        dpp::message msg{ dpp::embed{}.set_title("Choose a Job").set_description("This cannot be reversed").
            set_image("https://raw.githubusercontent.com/Kolin63/Adriencoin/refs/heads/main/art/jmart.jpg?token=GHSAT0AAAAAAC6CA7OU2LUHS2CDSSTJ4MUSZ5YBC5Q")};

        dpp::component select{};
        select.set_type(dpp::cot_selectmenu);
        select.set_placeholder("Choose a Job");
        select.set_id("jobselect");

        for (std::size_t i{}; i < adr::Job::tierOneJobsSize; ++i) {
            select.add_select_option(dpp::select_option{ adr::Job::jobs[i].name, adr::Job::jobs[i].name, (
                std::to_string(adr::Job::jobs[i].item.amount) + ' ' + adr::Item::names[adr::Job::jobs[i].item.id] + ", " + std::to_string(adr::Job::jobs[i].adriencoin) + " adriencoin") }
                .set_emoji(adr::Item::names[i+1], adr::Item::emojiIDs[i+1]));
        }

        dpp::component confirm{};
        confirm.set_type(dpp::cot_button);
        confirm.set_label("Confirm");
        confirm.set_emoji("\xE2\x9C\x85");
        confirm.set_style(dpp::cos_success);
        confirm.set_id("jobconfirm");

        msg.add_component(dpp::component{}.add_component(select));
        msg.add_component(dpp::component{}.add_component(confirm));

        event.reply(msg);
    }
    else {
        adr::doJob(bot, event);
    }
}

void adr::onSelectClick(const dpp::select_click_t& event)
{
    if (event.custom_id == "jobselect") {
        adr::Player player{ event.command.usr.id };
        player.setTempJob(adr::Job::getId(event.values[0]));
        event.reply(dpp::message("Job selected, please confirm").set_flags(dpp::m_ephemeral));
    }
}

void adr::onButtonClick(const dpp::button_click_t& event)
{
    if (event.custom_id == "jobconfirm") {
        Player player{ event.command.usr.id };
        if (player.job() != adr::Job::MAX) {
            event.reply(dpp::message("You already have a job! (" + adr::Job::jobs[player.job()].name + ")").set_flags(dpp::m_ephemeral));
            return;
        }

        player.setJob(player.tempJob());
        event.reply(dpp::message("Job confirmed to " + adr::Job::jobs[player.job()].name + ". Run /" + adr::Job::jobs[player.job()].action + " to work.").set_flags(dpp::m_ephemeral));
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
