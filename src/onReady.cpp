#include <dpp/appcommand.h>
#include <string>
#include <filesystem>
#include <dpp/dpp.h>
#include "onReady.h"
#include "job.h"
#include "item.h"
#include "product.h"
#include "stock.h"
#include "dungeon.h"

namespace adr {
    void addSlashCommands(dpp::cluster& bot)
    {
        constexpr std::size_t commandsAmount{ adr::Job::tierOneJobsSize + 5 };

        std::cout << "making commands!\n";

        std::vector<dpp::slashcommand> commandList{};
        commandList.reserve(commandsAmount);
        commandList.resize(0);
        std::cout << "commandList capacity: " << commandList.capacity() << " size: " << commandList.size() << '\n';

        for (std::size_t i{}; i < adr::Job::tierOneJobsSize; ++i)
        {
            dpp::slashcommand slashcommand{ adr::Job::jobs[i].action, (adr::Job::jobs[i].action + ' ' + std::string{ adr::item_names[adr::Job::jobs[i].item.id] }), bot.me.id};
            std::cout << adr::Job::jobs[i].name << ' ' << adr::Job::jobs[i].action << '\n';
            commandList.push_back(slashcommand);
        }

        adr::Product::addSlashCommands(bot, commandList);
        
        adr::Stock::addSlashCommands(bot, commandList);

        dpp::slashcommand view{ "view", "view a player's inventory and stats", bot.me.id };
        view.add_option({ dpp::co_user, "player", "the player to view", true });
        commandList.push_back(view);

        dpp::command_option tradeItemOption{ dpp::co_string, "item", "The item that is being traded", true };
        // for (const std::string_view i : adr::item_names) {
        //     std::string str{ i };
        //     tradeItemOption.add_choice(dpp::command_option_choice{ str, str });
        // }

        dpp::slashcommand trade{ "trade", "trade with another player", bot.me.id };
        trade.add_option(
            dpp::command_option{ dpp::co_sub_command, "item", "Edit what items are being given" }
            .add_option(dpp::command_option{ dpp::co_string, "type", "If you are giving or receiving this item", true }
                .add_choice(dpp::command_option_choice{ "give", "give" })
                .add_choice(dpp::command_option_choice{ "receive", "receive" }))
            .add_option(tradeItemOption)
            .add_option(dpp::command_option{ dpp::co_integer, "amount", "The amount of the item that is being traded", true })
            .add_option(dpp::command_option{ dpp::co_integer, "slot", "Specify what trade slot you are using, defaults to 0", false })
        );
        trade.add_option(
            dpp::command_option{ dpp::co_sub_command, "propose", "Finalize and propose the trade" }
            .add_option(dpp::command_option{ dpp::co_user, "player", "Who you are trading with", true })
            .add_option(dpp::command_option{ dpp::co_integer, "slot", "Specify what trade slot you are using, defaults to 0", false })
        );
        trade.add_option(
            dpp::command_option{ dpp::co_sub_command, "accept", "Accept a trade from somebody else" }
            .add_option(dpp::command_option{ dpp::co_user, "player", "Who you are trading with", true })
            .add_option(dpp::command_option{ dpp::co_string, "seed", "The seed of the trade, provided in the embed", true })
            .add_option(dpp::command_option{ dpp::co_integer, "slot", "Specify what trade slot you are using, defaults to 0", false })
        );
        trade.add_option(
            dpp::command_option{ dpp::co_sub_command, "view", "View a trade" }
            .add_option(dpp::command_option{ dpp::co_user, "player", "The player who's trade you are viewing", false })
            .add_option(dpp::command_option{ dpp::co_integer, "slot", "Specify what trade slot you are using, defaults to 0", false })
        );
        trade.add_option(
            dpp::command_option{ dpp::co_sub_command, "cancel", "Cancel one of your own trades" }
            .add_option(dpp::command_option{ dpp::co_integer, "slot", "Specify what trade slot you are using, defaults to 0", false })
        );
        commandList.push_back(trade);

        adr::dungeon::add_slash_commands(bot, commandList);

        dpp::slashcommand use{ "use", "Use an Item", bot.me.id };
        use.add_option(
            dpp::command_option{ dpp::co_string, "item", "The Item to Use", true }
            .add_choice(dpp::command_option_choice{ "loving_scarf", "loving_scarf" })
            .add_choice(dpp::command_option_choice{ "wither_shield", "wither_shield" })
            .add_choice(dpp::command_option_choice{ "shadow_warp", "shadow_warp" })
            .add_choice(dpp::command_option_choice{ "implosion", "implosion" })
        );
        commandList.push_back(use);

        dpp::slashcommand lsitem{ "lsitem", "List all item names", bot.me.id };
        commandList.push_back(lsitem);

        dpp::slashcommand admin{ "admin", "various admin tools in one command", bot.me.id };
        admin.default_member_permissions = dpp::p_administrator;
        admin.add_option(
            dpp::command_option{ dpp::co_string, "command", "the command to run", true }
            .add_choice(dpp::command_option_choice{ "addroles", "addroles" })
            .add_choice(dpp::command_option_choice{ "jobembed", "jobembed" })
            .add_choice(dpp::command_option_choice{ "shopembed", "shopembed" })
            .add_choice(dpp::command_option_choice{ "leaderboardembed", "leaderboardembed" })
            .add_choice(dpp::command_option_choice{ "addemojis", "addemojis" }));
        admin.add_option(dpp::command_option{ dpp::co_user, "user", "the user to affect", false });
        admin.add_option(dpp::command_option{ dpp::co_integer, "index", "job / item index", false });
        admin.add_option(dpp::command_option{ dpp::co_integer, "amount", "amount of item", false });
        commandList.push_back(admin);

        dpp::slashcommand owner{ "owner", "various owner tools in one command, only meant for kolin63", bot.me.id };
        owner.default_member_permissions = dpp::p_administrator;
        owner.add_option(
            dpp::command_option{ dpp::co_string, "command", "the command to run", true }
            .add_choice(dpp::command_option_choice{ "setinv", "setinv" })
            .add_choice(dpp::command_option_choice{ "changeinv", "changeinv" })
            .add_choice(dpp::command_option_choice{ "setjob", "setjob" })
            .add_choice(dpp::command_option_choice{ "save", "save" })
            .add_choice(dpp::command_option_choice{ "clearcache", "clearcache" })
            .add_choice(dpp::command_option_choice{ "reloadstocks", "reloadstocks" })
            .add_choice(dpp::command_option_choice{ "getindices", "getindices" })
            .add_choice(dpp::command_option_choice{ "dailies", "dailies" })
            .add_choice(dpp::command_option_choice{ "setstock", "setstock" })
            .add_choice(dpp::command_option_choice{ "percentstock", "percentstock" })
            .add_choice(dpp::command_option_choice{ "godmode", "godmode" })
            .add_choice(dpp::command_option_choice{ "resetworktimer", "resetworktimer" }));
        owner.add_option(dpp::command_option{ dpp::co_user, "user", "the user to affect", false });
        owner.add_option(dpp::command_option{ dpp::co_integer, "index", "job / item index", false });
        owner.add_option(dpp::command_option{ dpp::co_integer, "amount", "amount of item", false });
        owner.add_option(dpp::command_option{ dpp::co_string, "string", "wow! a string!", false });
        commandList.push_back(owner);

        std::cout << "commandList capacity: " << commandList.capacity() << " size: " << commandList.size() << '\n';
        for (std::size_t i{}; i < commandList.size(); ++i) { std::cout << i << ": " << commandList[i].name << '\n'; }
        bot.global_bulk_command_create(commandList);
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

    void addEmojis(dpp::cluster& bot, const dpp::snowflake& guildID)
    {
        auto addEmoji = [&bot, &guildID](const std::string emojiName, const dpp::snowflake& emojiID) {
            dpp::emoji emoji{ emojiName, emojiID };
            std::filesystem::path path{ "./../art/item/" + emojiName + ".png" };
            std::cout << "PATH: " << path << '\n';

            // Load the image from a file
            std::ifstream file(path, std::ios::binary);
            if (!file) {
                std::cerr << "Failed to open emoji file.\n";
                return;
            }

            std::ostringstream oss;
            oss << file.rdbuf();
            std::string imageData = oss.str();

            emoji.load_image(imageData, dpp::i_png);

            bot.guild_emoji_create(guildID, emoji);
        };

        for (std::size_t i {}; i < adr::e_MAX; ++i)
        {
            addEmoji(std::string{ adr::emojis[i].first }, adr::emojis[i].second);
        }
    }

    void onReady(dpp::cluster& bot)
    {
        addSlashCommands(bot);
    }
}
