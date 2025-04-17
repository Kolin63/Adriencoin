#ifndef BOTTOKEN_H
#define BOTTOKEN_H

#include <string_view>
#include <fstream>
#include <filesystem>
#include <iostream>

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

#endif
