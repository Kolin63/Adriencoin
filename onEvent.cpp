#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <string>
#include <algorithm>
#include <utility>
#include <dpp/dpp.h>
#include "onEvent.h"
#include "onReady.h"
#include "job.h"
#include "player.h"
#include "shop.h"
#include "product.h"
#include "cache.h"

template <typename T>
std::optional<T> getOptionalParam(const std::string& name, const dpp::slashcommand_t& event) {
    T x{};
    try { x = std::get<T>(event.get_parameter(name)); }
    catch (const std::bad_variant_access& e) { 
        std::cout << "error getting optional parameter '" << name << "' error: " << e.what() << '\n'; 
        return {};
    }
    return x;
}

void adr::onSlashcommand(dpp::cluster& bot, const dpp::slashcommand_t& event)
{
    const std::string& commandName{ event.command.get_command_name() };
    if (commandName == "buy") {
        std::string resultName{ getOptionalParam<std::string>("result", event).value_or("")};

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
    else if (commandName == "trade") {
        std::string action{ std::get<std::string>(event.get_parameter("action")) };
        std::optional<dpp::snowflake> receiverUUID{ getOptionalParam<dpp::snowflake>("player", event) };
        std::size_t slot{ std::min(static_cast<std::size_t>( getOptionalParam<int64_t>("slot", event).value_or(1) ) - 1, adr::playerCacheElement::MAX_TRADE_OFFERS - 1)};
        adr::playerCacheElement& pce{ adr::cache::getElementFromCache(event.command.usr.id) };

        if (receiverUUID.has_value() && action != "view")
            pce.tradeOffers[slot].setReceiverUUID(receiverUUID.value());

        if (action == "make") {
            pce.tradeOffers[slot].clear();
            pce.tradeOffers[slot].activate();

            event.reply(dpp::message{ "Created a new trade offer in slot " + std::to_string(slot) }.set_flags(dpp::m_ephemeral));
            return;
        }
        else if (action == "give" || action == "receive") {
            if (!getOptionalParam<std::string>("item", event).has_value() && !getOptionalParam<int64_t>("amount", event).has_value()) 
            {
                event.reply(dpp::message{ "You need to set an item and an amount" }.set_flags(dpp::m_ephemeral));
                return;
            }

            adr::Item::Id index{ adr::Item::getId(getOptionalParam<std::string>("item", event).value()) };
            int amount{ static_cast<int>(getOptionalParam<int64_t>("amount", event).value()) };

            pce.tradeOffers[slot].setInventory((action == "give" ? adr::TradeOffer::give : adr::TradeOffer::receive), index, amount);
            event.reply(dpp::message{ pce.tradeOffers[slot].getEmbed() }.set_flags(dpp::m_ephemeral));
            return;
        }
        else if (action == "propose") {
            if (pce.tradeOffers[slot].getReceiverUUID() == event.command.usr.id) {
                event.reply(dpp::message{ "You need to define a player to receive the trade." }.set_flags(dpp::m_ephemeral));
                return;
            }
            if (!pce.tradeOffers[slot].isValid()) {
                event.reply(dpp::message{ "That trade is invalid. Did you specify the correct slot?" }.set_flags(dpp::m_ephemeral));
                return;
            }

            event.reply(dpp::message{ pce.tradeOffers[slot].getEmbed() });
        }
        else if (action == "accept") {
            if (!receiverUUID.has_value()) {
                event.reply(dpp::message{ "You need to define the player that you are trading with." }.set_flags(dpp::m_ephemeral));
                return;
            }

            const dpp::snowflake& tradeReceiverUUID{ event.command.usr.id };
            const dpp::snowflake& tradeGiverUUID{ receiverUUID.value() };
            adr::playerCacheElement& giverPCE{ adr::cache::getElementFromCache(tradeGiverUUID) };

            if (tradeReceiverUUID != giverPCE.tradeOffers[slot].getReceiverUUID()) {
                event.reply(dpp::message{ "You are not a part of that trade. Did you specify the correct slot?" }.set_flags(dpp::m_ephemeral));
                return;
            }
                
            if (tradeGiverUUID != giverPCE.tradeOffers[slot].getGiverUUID()) {
                event.reply(dpp::message{ "Something went wrong. Did you specify the correct player?" }.set_flags(dpp::m_ephemeral));
                std::cout << "tradeGiverUUID: " << tradeGiverUUID << ' ' << giverPCE.tradeOffers[slot].getGiverUUID() << ' ' << giverPCE.player.uuid() << '\n';
                return;
            }
                
            if (!giverPCE.tradeOffers[slot].isValid()) {
                event.reply(dpp::message{ "That trade is invalid. Did you specify the correct slot?" }.set_flags(dpp::m_ephemeral));
                return;
            }
            
            event.reply(dpp::message{ giverPCE.tradeOffers[slot].getEmbed().set_title("Trade Complete!") });
            giverPCE.tradeOffers[slot].executeTrade();
        }
        else if (action == "view") {
            event.reply(dpp::message{ adr::cache::getElementFromCache(receiverUUID.value_or(event.command.usr.id)).tradeOffers[slot].getEmbed()}.set_flags(dpp::m_ephemeral));
            return;
        }
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
        else if (subcmd == "clearcache") {
            adr::cache::clear();
            event.reply(dpp::message("done").set_flags(dpp::m_ephemeral));
        }
        else if (subcmd == "getindices") {
            std::string body{ "items:\n" };
            for (std::size_t i{}; i < adr::Item::names.size(); ++i) {
                body += std::to_string(i) + ": " + adr::Item::names[i] + '\n';
            }

            body += "\njobs:\n";
            for (std::size_t i{}; i < adr::Job::jobs.size(); ++i) {
                body += std::to_string(i) + ": " + adr::Job::jobs[i].name + '\n';
            }

            event.reply(dpp::message(body).set_flags(dpp::m_ephemeral));
        }
    }
    else {
        adr::doJob(event);
    }
}

void adr::onSelectClick(const dpp::select_click_t& event)
{
    if (event.custom_id == "jobselect") {
        adr::cache::getElementFromCache(event.command.usr.id).tempJob = adr::Job::getId(event.values[0]);
        event.reply(dpp::message("Job selected, please confirm").set_flags(dpp::m_ephemeral));
    }
    else if (event.custom_id == "shopselect") {
        event.reply(adr::shop::getMessage(adr::shop::getId(event.values[0])).set_flags(dpp::m_ephemeral));
    }
}

void adr::onButtonClick(const dpp::button_click_t& event)
{
    if (event.custom_id == "jobconfirm") {
        adr::playerCacheElement& pce{ adr::cache::getElementFromCache(event.command.usr.id) };
        if (pce.player.job() != adr::Job::MAX) {
            event.reply(dpp::message("You already have a job! (" + adr::Job::jobs[pce.player.job()].name + ")").set_flags(dpp::m_ephemeral));
            return;
        }
        if (pce.tempJob == adr::Job::MAX) {
            event.reply(dpp::message("You need to select a job first.").set_flags(dpp::m_ephemeral));
            return;
        }

        pce.player.setJob(adr::cache::getElementFromCache(event.command.usr.id).tempJob);
        event.reply(dpp::message("Job confirmed to " + adr::Job::jobs[pce.player.job()].name + ". Run /" + adr::Job::jobs[pce.player.job()].action + " to work.").set_flags(dpp::m_ephemeral));
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
