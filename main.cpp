#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <dpp/dpp.h>

const std::string BOT_TOKEN = "";
// invite: { "https://discord.com/oauth2/authorize?client_id=1339021268292337786&scope=bot+applications.commands&permissions=8" };

int main()
{
    dpp::cluster bot(BOT_TOKEN);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([](const dpp::slashcommand_t& event)
        {
            const std::string& commandName{ event.command.get_command_name() };
            if (commandName == "echo")
            {
                std::string input{ std::get<std::string>(event.get_parameter("input")) };
                event.reply("echo: " + input);
            }
            else if (commandName == "add")
            {
                std::int64_t x{ std::get<std::int64_t>(event.get_parameter("x")) };
                std::int64_t y{ std::get<std::int64_t>(event.get_parameter("y")) };

                event.reply(std::to_string(x) + " + " + std::to_string(y) + " = " + std::to_string(x + y));
            }
            else if (commandName == "test")
            {
                event.reply("test");
            }
        });

    bot.on_ready([&bot]([[maybe_unused]] const dpp::ready_t& event)
        {
            if (dpp::run_once<struct register_bot_commands>())
            {
                std::cout << "Registering commands!\n";

                dpp::slashcommand echo{ "echo", "Have the bot echo your message", bot.me.id };
                echo.add_option(dpp::command_option{ dpp::co_string, "input", "add input here", true });

                dpp::slashcommand add{ "add", "Add two numbers", bot.me.id };
                add.add_option(dpp::command_option{ dpp::co_integer, "x", "first number", true });
                add.add_option(dpp::command_option{ dpp::co_integer, "y", "second number", true });

                dpp::slashcommand test{ "test", "test", bot.me.id };

                bot.global_bulk_command_create({ echo, add, test });
            }
        });

    bot.start(dpp::st_wait);
}