#ifndef BOTTOKEN_H
#define BOTTOKEN_H

#include <string_view>
#include <fstream>
#include <iostream>

inline std::string getBotToken()
{
    std::string filename{ "C:/discordtokens/adriencoin.txt" };
    std::ifstream fs{ filename, std::ios::binary };

    std::string token{};
    fs >> token;
    return token;
}

#endif
