#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <array>
#include "product.h"
#include "item.h"

const std::array<adr::Product, adr::Product::MAX> adr::Product::products{
    // Gay to Coin
    adr::Product{ adr::Product::gaytocoin, "gay_to_coin", { adr::Item{ adr::Item::gaydriencoin, 1 } }, adr::Product::r_all, { adr::Item{ adr::Item::adriencoin, 1000 } } },

    // Adriresource Trade
    adr::Product{ adr::Product::adriresourcetrade, "adriresource_trade", { adr::Item{ adr::Item::adriresource, 1 } }, adr::Product::r_one, {
        adr::Item{ adr::Item::carrot, 20 }, adr::Item{ adr::Item::gemstone, 5 }, adr::Item{ adr::Item::wood, 30 },
        adr::Item{ adr::Item::fish, 30 }, adr::Item{ adr::Item::rottenflesh, 60 }, adr::Item{ adr::Item::book, 2 },
        adr::Item{ adr::Item::potion, 1 } } },

    // Tier Upgrade - Handling Needs to be Done in Code
    adr::Product{ adr::Product::tierupgrade, "tier_upgrade", {
        adr::Item{ adr::Item::adriencoin, 100 }, adr::Item{adr::Item::book, 70}},
        adr::Product::r_all, { adr::Item{ adr::Item::adriencoin, 0 } } },

    // Job Change - Handling Needs to be Done in Code
    adr::Product{ adr::Product::jobchange, "job_change", { adr::Item{ adr::Item::adriencoin, 100 } },
        adr::Product::r_all, { adr::Item{ adr::Item::adriencoin, 0 } } },

    // Resource Exchange
    adr::Product{ adr::Product::resourceexchange, "resource_exchange", { adr::Item{ adr::Item::adriencoin, 10 } },
        adr::Product::r_one, {
        adr::Item{ adr::Item::carrot, 100 }, adr::Item{ adr::Item::gemstone, 25 }, adr::Item{ adr::Item::wood, 150 }, 
        adr::Item{ adr::Item::fish, 10 }, adr::Item{ adr::Item::rottenflesh, 300 }, adr::Item{ adr::Item::book, 10 },
        adr::Item{ adr::Item::potion, 5 } } }
};

dpp::message adr::Product::buy(const dpp::snowflake& uuid, const std::string& productName, const std::string& resultName)
{
    std::cout << "buy: " << uuid << ' ' << productName << ' ' << resultName << '\n';

    adr::Player player{ uuid };

    // Gets the product ID and Result Type
    adr::Product::Id productId{ adr::Product::MAX };
    adr::Product::ResultType resultType{ adr::Product::r_none };
    for (const adr::Product& product : adr::Product::products) {
        if (product.name == productName) {
            productId = product.id;
            resultType = product.resultType;
            break;
        }
    }
    if (productId == adr::Product::MAX)
        return dpp::message{ "There was an error getting the product id." }.set_flags(dpp::m_ephemeral);
    if (resultType == adr::Product::r_none)
        return dpp::message{ "There was an error getting the result type." }.set_flags(dpp::m_ephemeral);

    // Returns from the function if the player can't afford this
    for (const adr::Item& item : adr::Product::products[productId].cost) 
        if (player.inv(item.id) < item.amount)
            return dpp::message{ "You can't afford this!" };

    // Gets the result choice
    int resultChoice{ -1 };
    if (resultType == adr::Product::r_one && resultName != "")
        for (int i{}; i < adr::Product::products[productId].result.size(); ++i)
            if (adr::Item::names[adr::Product::products[productId].result[i].id] == resultName)
                resultChoice = i;

    // If it is something that does not need to be handled in code
    if (adr::Product::products[productId].result[0].amount != 0) { 
        for (const adr::Item& item : adr::Product::products[productId].cost) 
            player.changeInv(item.id, -item.amount); // Note the negative, so the amount is subtracted

        if (resultType == adr::Product::r_all) {
            for (const adr::Item& item : adr::Product::products[productId].result)
                player.changeInv(item.id, item.amount);
        }
        else if (resultType == adr::Product::r_one) {
            if (resultChoice == -1)
                return dpp::message{ "You need to make a selection of what to buy" };

            player.changeInv(adr::Product::products[productId].result[resultChoice].id, adr::Product::products[productId].result[resultChoice].amount);
        }
    }

    return dpp::message{ "bought it (temp msg)" };
}
