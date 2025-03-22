#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <string>
#include <string_view>
#include <filesystem>
#include <dpp/dpp.h>
#include "onReady.h"
#include "job.h"
#include "item.h"
#include "shop.h"
#include "product.h"

namespace adr {
    void addSlashCommands(dpp::cluster& bot)
    {
        constexpr std::size_t commandsAmount{ adr::Job::tierOneJobsSize + 4 };

        std::cout << "making commands!\n";

        std::vector<dpp::slashcommand> commandList{};
        commandList.reserve(commandsAmount);
        commandList.resize(0);
        std::cout << "commandList capacity: " << commandList.capacity() << " size: " << commandList.size() << '\n';

        for (std::size_t i{}; i < adr::Job::tierOneJobsSize; ++i)
        {
            dpp::slashcommand slashcommand{ adr::Job::jobs[i].action, (adr::Job::jobs[i].action + ' ' + adr::Item::names[adr::Job::jobs[i].item.id]), bot.me.id};
            std::cout << adr::Job::jobs[i].name << ' ' << adr::Job::jobs[i].action << '\n';
            commandList.push_back(slashcommand);
        }

        dpp::slashcommand buy{ "buy", "Buy something", bot.me.id };
        dpp::command_option buyCommandOption{ dpp::co_string, "product", "the product / trade / deal", true };
        for (const adr::Product& product : adr::Product::products) {
            buyCommandOption.add_choice(dpp::command_option_choice{ product.name, product.name });
        }
        dpp::command_option buyResultOption{ dpp::co_string, "result", "what you are getting, if applicable", false };
        for (const std::string& name : adr::Item::names) {
            buyResultOption.add_choice(dpp::command_option_choice{ name, name });
        }
        for (std::size_t i{}; i < adr::Job::tierOneJobsSize; ++i) {
            buyResultOption.add_choice(dpp::command_option_choice{ adr::Job::jobs[i].name, adr::Job::jobs[i].name });
        }
        buy.add_option(buyCommandOption);
        buy.add_option(buyResultOption);
        buy.add_option(dpp::command_option{ dpp::co_integer, "times", "how many times to buy this (max 100)", false });
        commandList.push_back(buy);

        dpp::slashcommand view{ "view", "view a player's inventory and stats", bot.me.id };
        view.add_option({ dpp::co_user, "player", "the player to view", true });
        commandList.push_back(view);

        dpp::slashcommand admin{ "admin", "various admin tools in one command", bot.me.id };
        admin.default_member_permissions = dpp::p_administrator;
        admin.add_option(
            dpp::command_option{ dpp::co_string, "command", "the command to run", true }
            .add_choice(dpp::command_option_choice{ "addroles", "addroles" })
            .add_choice(dpp::command_option_choice{ "jobembed", "jobembed" })
            .add_choice(dpp::command_option_choice{ "shopembed", "shopembed" })
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
            .add_choice(dpp::command_option_choice{ "setjob", "setjob" })
            .add_choice(dpp::command_option_choice{ "save", "save" })
            .add_choice(dpp::command_option_choice{ "resetworktimer", "resetworktimer" }));
        owner.add_option(dpp::command_option{ dpp::co_user, "user", "the user to affect", false });
        owner.add_option(dpp::command_option{ dpp::co_integer, "index", "job / item index", false });
        owner.add_option(dpp::command_option{ dpp::co_integer, "amount", "amount of item", false });
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
        auto addEmoji = [&bot, &guildID](const std::string& emojiName, const dpp::snowflake& emojiID) {
            dpp::emoji emoji{ emojiName, emojiID };
            std::filesystem::path path{ std::filesystem::current_path() / "art" / "item" / (emojiName + ".png")};
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

        for (std::size_t i {}; i < adr::Item::emojiIDs.size(); ++i)
        {
            addEmoji(adr::Item::names[i], adr::Item::emojiIDs[i]);
        }
    }

    void onReady(dpp::cluster& bot)
    {
        addSlashCommands(bot);
    }
}