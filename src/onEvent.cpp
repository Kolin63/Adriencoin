#include "dungeon.h"
#include "emoji.h"
#include "item.h"
#include <dpp/message.h>
#include <string>
#include <algorithm>
#include <dpp/dpp.h>
#include <variant>
#include "onEvent.h"
#include "onReady.h"
#include "job.h"
#include "player.h"
#include "shop.h"
#include "cache.h"
#include "stock.h"
#include "leaderboard.h"

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
        std::string resultName{};
        try {
            resultName = std::get<std::string>(event.get_parameter("result"));
        } catch(std::bad_variant_access&) {}

        std::string subprodName{};
        try {
            subprodName = event.command.get_command_interaction()
                .options[0].options[0].name;
        } catch (...) {}

        std::int64_t times{ getOptionalParam<int64_t>("times", event).value_or(1) };

        if (times > 100) {
            event.reply(dpp::message{ "You can not buy something more than 100 times." }.set_flags(dpp::m_ephemeral));
            return;
        }

        //event.reply(adr::shop::buy(event.command.usr.id, std::get<std::string>(event.get_parameter("product")), resultName, std::min(static_cast<int>(times), 100)));
        dpp::message tmp = adr::shop::buy(
                event.command.usr.id, 
                event.command.get_command_interaction().options[0].name, 
                subprodName,
                resultName, 
                std::min(static_cast<int>(times), 100
                ));
        event.reply(tmp);
    }
    else if (commandName == "dungeon") {
        adr::dungeon::handle_slash_command(bot, event);
    }
    else if (commandName == "lsitem") {
        dpp::message msg{ "Due to restrictions in Discord's API, there can "
        "only be 25 items in a slashcommand choice. Therefore, you will need "
        "to manually type in the item names for commands like `/trade`\n\n" };

        for (std::size_t i{}; i < adr::i_MAX; ++i) {
            msg.content += get_emoji(static_cast<item_id>(i)) + ' '
                + std::to_string(i) + ". "
                + static_cast<std::string>(item_names[i]) + '\n';
        }

        msg.set_flags(dpp::m_ephemeral);

        event.reply(msg);
        return;
    }
    else if (commandName == "use") {
        const std::string item{ std::get<std::string>(event.get_parameter("item")) };
        adr::Player& player{ adr::cache::getPlayerFromCache(event.command.usr.id) };

        if (item == "loving_scarf") {
            if (player.inv(i_loving_scarf) <= 0) {
                event.reply(dpp::message{ "You don't have any Loving Scarfs!" }
                    .set_flags(dpp::m_ephemeral));
                return;
            }
            if (player.inv(i_bonzo_mask) <= 0) {
                event.reply(dpp::message{ "You don't have any Bonzo Masks to use that on!" }
                    .set_flags(dpp::m_ephemeral));
                return;
            }
            if (player.m_atr.bonzo_love.val) {
                event.reply(dpp::message{ "You have already used that item! (You should consider trading it)" } 
                    .set_flags(dpp::m_ephemeral));
                return;
            }
            player.changeInv(i_loving_scarf, -1);
            player.m_atr.bonzo_love.val = true;
            event.reply("Used " + get_emoji(e_loving_scarf) + " Loving Scarf on " + get_emoji(e_bonzo_mask) + " Bonzo Mask");
        }
        else if (item == "wither_shield") {
            if (player.inv(i_wither_shield) <= 0) {
                event.reply(dpp::message{ "You don't have a Wither Shield scroll!" }
                    .set_flags(dpp::m_ephemeral));
                return;
            }
            if (player.inv(i_hyperion) <= 0) {
                event.reply(dpp::message{ "You don't have a Hyperion to use that on!" }
                    .set_flags(dpp::m_ephemeral));
                return;
            }
            if (player.m_atr.wither_shield.val) {
                event.reply(dpp::message{ "You have already used that item! (You should consider trading it)" } 
                    .set_flags(dpp::m_ephemeral));
                return;
            }
            player.changeInv(i_wither_shield, -1);
            player.m_atr.wither_shield.val = true;
            event.reply("Used " + get_emoji(e_wither_shield) + " Wither Shield on " + get_emoji(e_hyperion) + " Hyperion");
        }
        else if (item == "shadow_warp") {
            if (player.inv(i_shadow_warp) <= 0) {
                event.reply(dpp::message{ "You don't have a Shadow Warp scroll!" }
                    .set_flags(dpp::m_ephemeral));
                return;
            }
            if (player.inv(i_hyperion) <= 0) {
                event.reply(dpp::message{ "You don't have a Hyperion to use that on!" }
                    .set_flags(dpp::m_ephemeral));
                return;
            }
            if (player.m_atr.shadow_warp.val) {
                event.reply(dpp::message{ "You have already used that item! (You should consider trading it)" } 
                    .set_flags(dpp::m_ephemeral));
                return;
            }
            player.changeInv(i_shadow_warp, -1);
            player.m_atr.shadow_warp.val = true;
            event.reply("Used " + get_emoji(e_shadow_warp) + " Shadow Warp on " + get_emoji(e_hyperion) + " Hyperion");
        }
        else if (item == "implosion") {
            if (player.inv(i_implosion) <= 0) {
                event.reply(dpp::message{ "You don't have an Implosion scroll!" }
                    .set_flags(dpp::m_ephemeral));
                return;
            }
            if (player.inv(i_hyperion) <= 0) {
                event.reply(dpp::message{ "You don't have a Hyperion to use that on!" }
                    .set_flags(dpp::m_ephemeral));
                return;
            }
            if (player.m_atr.implosion.val) {
                event.reply(dpp::message{ "You have already used that item! (You should consider trading it)" } 
                    .set_flags(dpp::m_ephemeral));
                return;
            }
            player.changeInv(i_implosion, -1);
            player.m_atr.implosion.val = true;
            event.reply("Used " + get_emoji(e_implosion) + " Implosion on " + get_emoji(e_hyperion) + " Hyperion");
        }
        else {
            event.reply(dpp::message{ "Sorry, you can't use that item" });
            return;
        }
    }
    else if (commandName == "stock") {
        const std::string action{ event.command.get_command_interaction().options[0].name };

        adr::Player& player{ adr::cache::getPlayerFromCache(event.command.usr.id) };

        if (action == "view") {
            const std::string stockName{ std::get<std::string>(event.get_parameter("stock")) };
            event.reply(adr::Stock::getEmbed(stockName));
            return;
        }
        else if (action == "compact") {
            event.reply(adr::Stock::getEmbed("compact"));
            return;
        }
        else if (action == "graph") {
            const std::string stockName{ std::get<std::string>(event.get_parameter("stock")) };
            std::int64_t historyLength{ getOptionalParam<std::int64_t>("history", event).value_or(10) };

            if (historyLength < 8) historyLength = 8;

            if (historyLength > 100) historyLength = 100;

            event.reply(adr::Stock::getGraph(stockName, static_cast<std::uint32_t>(historyLength)));
            return;
        }
        else {
            const std::string stockName{ std::get<std::string>(event.get_parameter("stock")) };
            const int amount{ std::clamp(static_cast<int>(getOptionalParam<int64_t>("amount", event).value_or(1)), 1, 100) };
            adr::Stock& stock{ adr::Stock::getStock(stockName) };

            std::cout << "Stock transaction, expected " << stockName
                << ", got " << stock.getName() << '\n';

            const bool buying{ action == "buy" };

            // If the player is buying, they are spending adriencoin
            // If the player is selling, they are spending the stock they are selling
            // Branchless programming just for funzies
            const adr::item_id itemSpending{ static_cast<adr::item_id>(buying * adr::i_adriencoin + !buying * adr::get_item_id(stockName)) };
            const int spendingAmount{ buying * amount * stock.getValue() + !buying * amount };
            // Vice Versa for Item Receiving
            const adr::item_id itemReceiving{ static_cast<adr::item_id>(!buying * adr::i_adriencoin + buying * adr::get_item_id(stockName)) };
            const int receivingAmount{ !buying * amount * stock.getValue() + buying * amount };

            if (player.inv(itemSpending) < spendingAmount) {
                event.reply(dpp::message{ "You cannot afford that!" }.set_flags(dpp::m_ephemeral));
                return;
            }

            if (buying && stock.getUnissued() < receivingAmount) {
                event.reply(dpp::message{ "There are not enough unissued stocks!" }.set_flags(dpp::m_ephemeral));
                return;
            }

            player.changeInv(itemSpending, -spendingAmount);
            player.changeInv(itemReceiving, receivingAmount);

            stock.changeOutstanding(buying * receivingAmount + !buying * -spendingAmount);

            event.reply(dpp::message{ "**Stock Transaction Complete!**\n**Spent:** " + std::to_string(spendingAmount) + ' ' + adr::get_emoji(itemSpending) + ' ' + std::string{ adr::item_names[itemSpending] }
                + "\n**Received:** " + std::to_string(receivingAmount) + ' ' + adr::get_emoji(itemReceiving) + ' ' + std::string{ adr::item_names[itemReceiving] } });

            return;
        }
    }
    else if (commandName == "view") {
        const adr::Player& player{ adr::cache::getPlayerFromCache(std::get<dpp::snowflake>(event.get_parameter("player"))) };
        player.print();
        event.reply(dpp::message{ player.viewEmbed() });
    }
    else if (commandName == "trade") {
        dpp::command_data_option subcommand{ event.command.get_command_interaction().options[0] };
        std::size_t slot{ static_cast<std::size_t>( getOptionalParam<int64_t>("slot", event).value_or(0) ) };
        adr::playerCacheElement& pce{ adr::cache::getElementFromCache(event.command.usr.id) };

        if (subcommand.name == "item") {
            pce.tradeOffers[slot].clearSeed();

            pce.tradeOffers[slot].setInventory(
                (std::get<std::string>(event.get_parameter("type")) == "give" ? adr::TradeOffer::give : adr::TradeOffer::receive), 
                adr::get_item_id(std::get<std::string>(event.get_parameter("item"))),
                static_cast<int>(std::get<int64_t>(event.get_parameter("amount")))
            );

            event.reply(dpp::message{ pce.tradeOffers[slot].getEmbed() }.set_flags(dpp::m_ephemeral));
            return;
        }
        else if (subcommand.name == "propose") {
            pce.tradeOffers[slot].generateSeed();

            pce.tradeOffers[slot].setReceiverUUID(std::get<dpp::snowflake>(event.get_parameter("player")));

            event.reply(dpp::message{ pce.tradeOffers[slot].getEmbed()
                .set_image("https://raw.githubusercontent.com/Kolin63/Adriencoin/refs/heads/main/art/highfive-wide.jpg") });
        }
        else if (subcommand.name == "accept") {
            const dpp::snowflake& tradeReceiverUUID{ event.command.usr.id };
            const dpp::snowflake tradeGiverUUID{ std::get<dpp::snowflake>(event.get_parameter("player")) };
            adr::playerCacheElement& giverPCE{ adr::cache::getElementFromCache(tradeGiverUUID) };

            if (tradeReceiverUUID != giverPCE.tradeOffers[slot].getReceiverUUID()) {
                event.reply(dpp::message{ "You are not a part of that trade. Did you specify the correct slot?" }.set_flags(dpp::m_ephemeral));
                return;
            }
                
            if (tradeGiverUUID != giverPCE.tradeOffers[slot].getGiverUUID()) {
                event.reply(dpp::message{ "Something went wrong. Did you specify the correct player?" }.set_flags(dpp::m_ephemeral));
                return;
            }

            if (!giverPCE.tradeOffers[slot].isValid()) {
                event.reply(dpp::message{ "That trade is invalid. Check that both players can afford the proposed trade and that the correct slot is specified." }.set_flags(dpp::m_ephemeral));
                return;
            }

            if (std::get<std::string>(event.get_parameter("seed")) != giverPCE.tradeOffers[slot].getSeed()) {
                event.reply(dpp::message{ "That seed was incorrect. Has the trade been updated since you copied it?" }.set_flags(dpp::m_ephemeral));
                return;
            }
            
            event.reply(dpp::message{ giverPCE.tradeOffers[slot].getEmbed().set_title("Trade Complete!")
                .set_image("https://raw.githubusercontent.com/Kolin63/Adriencoin/refs/heads/main/art/hello-wide.jpg") });
            giverPCE.tradeOffers[slot].executeTrade();
        }
        else if (subcommand.name == "view") {
            event.reply(dpp::message{ adr::cache::getElementFromCache(getOptionalParam<dpp::snowflake>("player", event).value_or(event.command.usr.id))
                .tradeOffers[slot].getEmbed()}.set_flags(dpp::m_ephemeral));
            return;
        }
        else if (subcommand.name == "cancel") {
            pce.tradeOffers[slot].clear();
            event.reply(dpp::message{ "Canceled trade in slot " + std::to_string(slot) }.set_flags(dpp::m_ephemeral));
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
        else if (subcmd == "leaderboardembed") {
            event.reply(adr::leaderboard::get_leaderboard_embed());
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
                    std::to_string(adr::Job::jobs[i].item.amount) + ' ' + std::string{ adr::item_names[adr::Job::jobs[i].item.id] } + ", " + std::to_string(adr::Job::jobs[i].adriencoin) + " adriencoin") }
                    .set_emoji(adr::emojis[i].first, adr::emojis[i].second));
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
            .setInv(get_item_id(std::get<std::string>(event.get_parameter("string"))), static_cast<int>(std::get<std::int64_t>(event.get_parameter("amount"))));
            event.reply(dpp::message("done").set_flags(dpp::m_ephemeral));
        }
        else if (subcmd == "changeinv") {
            adr::cache::getPlayerFromCache(std::get<dpp::snowflake>(event.get_parameter("user")))
            .changeInv(get_item_id(std::get<std::string>(event.get_parameter("string"))), static_cast<int>(std::get<std::int64_t>(event.get_parameter("amount"))));
            event.reply(dpp::message("done").set_flags(dpp::m_ephemeral));
        }
        else if (subcmd == "resetworktimer") {
            adr::Player& player{ adr::cache::getPlayerFromCache(std::get<dpp::snowflake>(event.get_parameter("user"))) };
            player.setLastWorked(0);
            player.setLastFought(0);
            event.reply(dpp::message("done").set_flags(dpp::m_ephemeral));
        }
        else if (subcmd == "save") {
            adr::cache::saveCache();
            adr::Stock::saveJSON();
            event.reply(dpp::message("done").set_flags(dpp::m_ephemeral));
        }
        else if (subcmd == "clearcache") {
            adr::cache::clear();
            event.reply(dpp::message("done").set_flags(dpp::m_ephemeral));
        }
        else if (subcmd == "reloadstocks") {
            adr::Stock::parseJSON();
            event.reply(dpp::message("done").set_flags(dpp::m_ephemeral));
            return;
        }
        else if (subcmd == "getindices") {
            std::string body{ "items:\n" };
            for (std::size_t i{}; i < adr::i_MAX; ++i) {
                body += adr::get_emoji(static_cast<adr::item_id>(i)) + ' ' 
                    + std::to_string(i) + ": " 
                    + std::string{ adr::item_names[i] } + '\n';
            }

            body += "\njobs:\n";
            for (std::size_t i{}; i < adr::Job::jobs.size(); ++i) {
                body += std::to_string(i) + ": " + adr::Job::jobs[i].name + '\n';
            }

            body += "\nstocks:\n";
            for (std::size_t i{}; i < adr::Stock::getStockSize(); ++i) {
                body += std::to_string(i) + ": " + adr::Stock::getStock(static_cast<adr::Stock::Id>(i)).getName() + '\n';
            }

            event.reply(dpp::message(body).set_flags(dpp::m_ephemeral));
        }
        else if (subcmd == "dailies") {
            adr::daily::doDailyTasks(bot);
            event.reply(dpp::message("done").set_flags(dpp::m_ephemeral));
        }
        else if (subcmd == "setstock") {
                adr::Stock::getStock(static_cast<adr::Stock::Id>(std::get<std::int64_t>(event.get_parameter("index"))))
                        .setValue(static_cast<int>(std::get<std::int64_t>(event.get_parameter("amount"))));
            event.reply(dpp::message("done").set_flags(dpp::m_ephemeral));
        }
        else if (subcmd == "percentstock") {
                adr::Stock::getStock(static_cast<adr::Stock::Id>(std::get<std::int64_t>(event.get_parameter("index"))))
                        .changeValue(static_cast<int>(std::get<std::int64_t>(event.get_parameter("amount"))));
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
        adr::cache::getElementFromCache(event.command.usr.id).tempJob = adr::Job::getId(event.values[0]);
        event.reply(dpp::message("Job selected, please confirm").set_flags(dpp::m_ephemeral));
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
        event.reply(dpp::message{ "You can work next " + player.nextWorkTimestamp() }.set_flags(dpp::m_ephemeral));
        return;
    }

    for (const adr::Job& i : adr::Job::jobs) if (player.job() == i.id && commandName == i.action) {
        didAJob = true;
        player[i.item.id] += i.item.amount;
        player[adr::i_adriencoin] += i.adriencoin;
        player.updateLastWorked();

        adr::Stock::jobWorked(i.id);

        event.reply(i.action + ": +" + std::to_string(i.item.amount) + ' ' + get_emoji(i.item.id)
            + " and +" + std::to_string(i.adriencoin) + ' ' + get_emoji(e_adriencoin));
    }

    if (!didAJob) {
        if (player.job() == adr::Job::MAX) {
            event.reply("You don't have a job!");
        }
        else {
            event.reply("That is not your job! Your job is " + adr::Job::jobs[player.job()].name);
        }
    }
}
