#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <string>
#include <algorithm>
#include <dpp/dpp.h>
#include "onEvent.h"
#include "onReady.h"
#include "job.h"
#include "player.h"
#include "shop.h"
#include "product.h"
#include "cache.h"

void adr::onSlashcommand(dpp::cluster& bot, const dpp::slashcommand_t& event)
{
    const std::string& commandName{ event.command.get_command_name() };
    if (commandName == "buy") {
        std::string resultName{};
        try { resultName = std::get<std::string>(event.get_parameter("result")); }
        catch (const std::bad_variant_access& e) { 
            std::cout << "buy error: " << e.what() << '\n'; 
            resultName = ""; 
        }

        std::int64_t times{ 1 };
        try { times = std::get<std::int64_t>(event.get_parameter("times")); }
        catch (const std::bad_variant_access& e) { 
            std::cout << "buy error: " << e.what() << '\n'; 
            times = 1;
        }

        if (times > 100) {
            event.reply(dpp::message{ "You can not buy something more than 100 times." });
            return;
        }

        event.reply(adr::Product::buy(event.command.usr.id, std::get<std::string>(event.get_parameter("product")), resultName, std::min(static_cast<int>(times), 100)));
    }
    else if (commandName == "view") {
        const adr::Player& player{ adr::cache::getPlayerFromCache(std::get<dpp::snowflake>(event.get_parameter("player"))) };
        player.print();
        event.reply(dpp::message{ player.viewEmbed() });
    }
    else if (commandName == "admin") {
        const std::string subcmd{ std::get<std::string>(event.get_parameter("command")) };
        std::cout << "admin subcommand name: " << subcmd << '\n';
        if (subcmd == "addroles") { 
            adr::addRoles(bot, event.command.guild_id);
            event.reply(dpp::message("Attempted to create required roles").set_flags(dpp::m_ephemeral));
        }
        else if (subcmd == "addemojis") {
            adr::addEmojis(bot, event.command.guild_id);
            event.reply(dpp::message("Attempted to create required emojis " + dpp::emoji{ "adriencoin", 1342319536300621876 }.get_mention()).set_flags(dpp::m_ephemeral));
        }
        else if (subcmd == "shopembed") {
            event.reply(adr::shop::getMessage());
        }
        else if (subcmd == "jobembed") {
            dpp::message msg{ dpp::embed{}.set_title("Choose a Job").set_description("This cannot be reversed")
                .set_image("https://raw.githubusercontent.com/Kolin63/Adriencoin/refs/heads/main/art/jmart.jpg")
                .set_color(0x55ff99) };

            dpp::component select{};
            select.set_type(dpp::cot_selectmenu);
            select.set_placeholder("Choose a Job");
            select.set_id("jobselect");

            for (std::size_t i{}; i < adr::Job::tierOneJobsSize; ++i) {
                select.add_select_option(dpp::select_option{ adr::Job::jobs[i].name, adr::Job::jobs[i].name, (
                    std::to_string(adr::Job::jobs[i].item.amount) + ' ' + adr::Item::names[adr::Job::jobs[i].item.id] + ", " + std::to_string(adr::Job::jobs[i].adriencoin) + " adriencoin") }
                    .set_emoji(adr::Item::names[i], adr::Item::emojiIDs[i]));
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
    }
    else if (commandName == "owner") {
        if (event.command.usr.id != 488335709883727882) {
            event.reply(dpp::message("you cant do that!").set_flags(dpp::m_ephemeral));
            return;
        }
        const std::string subcmd{ std::get<std::string>(event.get_parameter("command")) };
        std::cout << "owner subcommand name: " << subcmd << '\n';

        if (subcmd == "setjob") {
            adr::cache::getPlayerFromCache(std::get<dpp::snowflake>(event.get_parameter("user")))
            .setJob(static_cast<adr::Job::Id>(std::get<std::int64_t>(event.get_parameter("index"))));
            event.reply(dpp::message("Set the job").set_flags(dpp::m_ephemeral));
        }
        else if (subcmd == "setinv") {
            adr::cache::getPlayerFromCache(std::get<dpp::snowflake>(event.get_parameter("user")))
            .setInv(static_cast<adr::Item::Id>(std::get<std::int64_t>(event.get_parameter("index"))), static_cast<int>(std::get<std::int64_t>(event.get_parameter("amount"))));
            event.reply(dpp::message("done").set_flags(dpp::m_ephemeral));
        }
        else if (subcmd == "resetworktimer") {
            adr::Player{ std::get<dpp::snowflake>(event.get_parameter("user")) }
            .setLastWorked(0);
            event.reply(dpp::message("done").set_flags(dpp::m_ephemeral));
        }
        else if (subcmd == "save") {
            adr::cache::saveCache();
            event.reply(dpp::message("done").set_flags(dpp::m_ephemeral));
        }
    }
    else {
        adr::doJob(event);
    }
}

void adr::onSelectClick(const dpp::select_click_t& event)
{
    if (event.custom_id == "jobselect") {
        adr::cache::getPlayerElementFromCache(event.command.usr.id).tempJob = adr::Job::getId(event.values[0]);
        event.reply(dpp::message("Job selected, please confirm").set_flags(dpp::m_ephemeral));
    }
    else if (event.custom_id == "shopselect") {
        event.reply(adr::shop::getMessage(adr::shop::getId(event.values[0])).set_flags(dpp::m_ephemeral));
    }
}

void adr::onButtonClick(const dpp::button_click_t& event)
{
    if (event.custom_id == "jobconfirm") {
        adr::Player& player{ adr::cache::getPlayerFromCache(event.command.usr.id) };
        if (player.job() != adr::Job::MAX) {
            event.reply(dpp::message("You already have a job! (" + adr::Job::jobs[player.job()].name + ")").set_flags(dpp::m_ephemeral));
            return;
        }

        player.setJob(adr::cache::getPlayerElementFromCache(event.command.usr.id).tempJob);
        event.reply(dpp::message("Job confirmed to " + adr::Job::jobs[player.job()].name + ". Run /" + adr::Job::jobs[player.job()].action + " to work.").set_flags(dpp::m_ephemeral));
    }
}

void adr::doJob(const dpp::slashcommand_t& event)
{
    const std::string& commandName{ event.command.get_command_name() };

    // turns true if the player can do a job, and stays false otherwise
    // if it stays false, then the bot will tell them that they couldn't do anything
    bool didAJob{ false };

    adr::Player& player{ adr::cache::getPlayerFromCache(event.command.usr.id) };

    if (player.nextWork() >= 0) {
        event.reply("You can work next " + player.nextWorkTimestamp());
    }

    for (const adr::Job& i : adr::Job::jobs) if (player.job() == i.id && commandName == i.action) {
        didAJob = true;
        player[i.item.id] += i.item.amount;
        player[adr::Item::adriencoin] += i.adriencoin;
        player.updateLastWorked();

        event.reply(i.action + ": +" + std::to_string(i.item.amount) + ' ' + dpp::emoji{ adr::Item::names[i.item.id], adr::Item::emojiIDs[i.item.id] }.get_mention()
            + " and +" + std::to_string(i.adriencoin) + ' ' + dpp::emoji{adr::Item::names[adr::Item::adriencoin], adr::Item::emojiIDs[adr::Item::adriencoin]}.get_mention());
    }

    if (!didAJob) {
        event.reply("You could not do that! If you were trying to do a job, do you have a job assigned?");
    }
}
