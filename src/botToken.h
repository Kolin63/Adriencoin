#ifndef BOTTOKEN_H
#define BOTTOKEN_H

#include <dpp/intents.h>
#include <string_view>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <dpp/dpp.h>

inline std::string getBotToken()
{
    std::filesystem::path filename{};

#ifdef _WIN32
    filename = "C:/discordtokens/kolintestbot.txt";
    std::cout << "Windows detected, loading " << filename << '\n';
#endif

#ifdef linux
    filename = "./../token.txt";
    std::cout << "Linux detected, loading " << filename << '\n';
#endif

    std::ifstream fs{ filename };

    std::string token{};
    fs >> token;

    std::cout << "Succesfully retrieved token " << token <<  '\n';

    return token;
}

inline dpp::cluster bot { getBotToken()
  , dpp::i_default_intents 
    | dpp::i_guild_members 
    | dpp::i_auto_moderation_configuration
    | dpp::i_auto_moderation_execution
    | dpp::i_guilds
};

#endif
