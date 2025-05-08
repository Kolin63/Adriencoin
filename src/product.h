#ifndef PRODUCT_H
#define PRODUCT_H

#include <dpp/dpp.h>
#include <vector>
#include "item.h"
#include "player.h"

namespace adr 
{
    struct Product 
    {
        enum ResultType {
            r_all,
            r_one,
            r_none,
            r_customAll,
            r_customOne,
        };

        const std::string name{};
        const std::string display{};
        const std::string desc{};
        const std::string joke{};
        const std::string picURL{};
        const uint32_t color{};

        struct sub 
        {
            const inventory cost{};
            const ResultType resultType{ adr::Product::r_none };
            const inventory result{};
            const std::vector<std::string> customResult{};
            const bool noTimes{};
        };

        const std::vector<sub> subproducts{};

        inline static std::vector<Product> products;

        static bool parseJson();
        static ResultType getResultType(const std::string& str);
        static void addSlashCommands(dpp::cluster& bot, std::vector<dpp::slashcommand>& commandList);
    };
}

#endif
