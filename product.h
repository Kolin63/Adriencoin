#ifndef PRODUCT_H
#define PRODUCT_H

#include <array>
#include <vector>
#include <string_view>
#include "item.h"
#include "player.h"

namespace adr 
{
    struct Product 
    {
        enum Id {
            gaytocoin,
            adriresourcetrade,
            tierupgrade,
            jobchange,
            resourceexchange,

            MAX
        };

        enum ResultType {
            r_all,
            r_one,
            r_none
        };

        const Id id{ adr::Product::MAX };
        const std::string& name{};
        const std::vector<adr::Item> cost{};
        const ResultType resultType{ adr::Product::r_all };
        const std::vector<adr::Item> result{};

        const static std::array<adr::Product, adr::Product::MAX> products;

        static dpp::message buy(const dpp::snowflake& uuid, const std::string& productName, const std::string& resultName = NULL);
    };
}

#endif
