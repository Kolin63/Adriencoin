#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <string>
#include <string_view>
#include <filesystem>
#include <dpp/dpp.h>
#include "onReady.h"
#include "job.h"
#include "item.h"
#include "shop.h"

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
            bot.global_bulk_command_create({ slashcommand });

            setJobOption.add_choice(dpp::command_option_choice{ i.name, i.id });
        }
        setJobOption.add_choice(dpp::command_option_choice{ "MAX", adr::Job::MAX });

        setJob.add_option(setJobOption);

        dpp::slashcommand buy{ "buy", "Buy something", bot.me.id };
        dpp::command_option buyCommandOption{ dpp::co_string, "id", "what you are buying", true };
        for (const std::string& name : adr::shop::product::names) {
            buyCommandOption.add_choice(dpp::command_option_choice{ name, name });
        }
        buy.add_option(buyCommandOption);

        dpp::slashcommand view{ "view", "view a player's inventory and stats", bot.me.id };
        view.add_option({ dpp::co_user, "player", "the player to view", true });

        dpp::slashcommand setInv{ "setinv", "set the amount of an item a player has", bot.me.id };
        setInv.default_member_permissions = dpp::p_administrator;
        setInv.add_option({ dpp::co_user, "player", "the player to modify", true });
        setInv.add_option({ dpp::co_integer, "itemid", "the item to modify", true });
        setInv.add_option({ dpp::co_integer, "amount", "the amount of item to set to", true });

        dpp::slashcommand addRoles{ "addroles", "roles", bot.me.id };
        addRoles.default_member_permissions = dpp::p_administrator;

        dpp::slashcommand addCommands{ "addcommands", "re-register the commands if theyre not loading", bot.me.id };
        addCommands.default_member_permissions = dpp::p_administrator;

        dpp::slashcommand printUserInv{ "printuserinv", "print the inventory of a user", bot.me.id };
        printUserInv.default_member_permissions = dpp::p_administrator;

        dpp::slashcommand jobEmbed{ "jobembed", "send an embed for choosing a job", bot.me.id };
        jobEmbed.default_member_permissions = dpp::p_administrator;

        dpp::slashcommand shopEmbed{ "shopembed", "shop embed", bot.me.id };
        jobEmbed.default_member_permissions = dpp::p_administrator;

        dpp::slashcommand addEmojis{ "addemojis", "add the emojis", bot.me.id };
        addEmojis.default_member_permissions = dpp::p_administrator;

        bot.global_bulk_command_create({ buy, view, jobEmbed, shopEmbed, setInv, addRoles, addEmojis, addCommands, setJob });
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