#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <array>
#include "product.h"
#include "item.h"
#include "cache.h"

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
        adr::Product::r_custom, { adr::Item{ adr::Item::adriencoin, 0 } } },

    // Job Change - Handling Needs to be Done in Code
    adr::Product{ adr::Product::jobchange, "job_change", { adr::Item{ adr::Item::adriencoin, 100 } },
        adr::Product::r_custom, { adr::Item{ adr::Item::adriencoin, 0 } } },

    // Resource Exchange
    adr::Product{ adr::Product::resourceexchange, "resource_exchange", { adr::Item{ adr::Item::adriencoin, 10 } },
        adr::Product::r_one, {
        adr::Item{ adr::Item::carrot, 100 }, adr::Item{ adr::Item::gemstone, 25 }, adr::Item{ adr::Item::wood, 150 }, 
        adr::Item{ adr::Item::fish, 10 }, adr::Item{ adr::Item::rottenflesh, 300 }, adr::Item{ adr::Item::book, 10 },
        adr::Item{ adr::Item::potion, 5 } } }
};

dpp::message adr::Product::buy(const dpp::snowflake& uuid, const std::string& productName, const std::string& resultName, int times)
{
    std::cout << "buy: " << uuid << ' ' << productName << ' ' << resultName << " * " << times << '\n';

    if (times > 100)
        return dpp::message{ "You can not buy something more than 100 times." };

    adr::Player& player{ adr::cache::getPlayerFromCache(uuid) };

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
        if (player.inv(item.id) < item.amount * times)
            return dpp::message{ "You can't afford this!" };

    // Gets the result choice
    int resultChoice{ -1 };
    if (resultType == adr::Product::r_one && resultName != "")
        for (std::size_t i{}; i < adr::Product::products[productId].result.size(); ++i)
            if (adr::Item::names[adr::Product::products[productId].result[i].id] == resultName)
                resultChoice = static_cast<int>(i);

    // If it is something that does not need to be handled in code
    if (adr::Product::products[productId].result[0].amount != 0) { 
        dpp::message msg{ "Bought:\n" };

        if (resultType == adr::Product::r_all) {
            for (const adr::Item& item : adr::Product::products[productId].result) {
                player.changeInv(item.id, item.amount * times);
                msg.set_content(msg.content + "* " + std::to_string(item.amount * times) + ' ' + dpp::emoji{adr::Item::names[item.id], adr::Item::emojiIDs[item.id]}.get_mention() + '\n');
            }
        }
        else if (resultType == adr::Product::r_one) {
            if (resultChoice == -1)
                return dpp::message{ "You need to make a selection of what to buy" };

            adr::Item resultItem{ adr::Product::products[productId].result[resultChoice] };
            player.changeInv(resultItem.id, resultItem.amount * times);
            msg.set_content(msg.content + "* " + std::to_string(resultItem.amount * times) + ' ' + dpp::emoji{adr::Item::names[resultItem.id], adr::Item::emojiIDs[resultItem.id]}.get_mention() + '\n');
        }

        msg.set_content(msg.content + "For:\n");

        for (const adr::Item& item : adr::Product::products[productId].cost) {
            player.changeInv(item.id, -item.amount * times); // Note the negative, so the amount is subtracted
            msg.set_content(msg.content + "* " + std::to_string(item.amount * times) + ' ' + dpp::emoji{adr::Item::names[item.id], adr::Item::emojiIDs[item.id]}.get_mention() + '\n');
        }

        return msg;
    }
    else {
        switch (productId)
        {
        case adr::Product::tierupgrade:
            player.setJob(static_cast<adr::Job::Id>(player.job() + adr::Job::tierOneJobsSize));
            return dpp::message{ "Upgraded job to " + adr::Job::jobs[player.job()].name };
        case adr::Product::jobchange:
            for (const adr::Job& job : adr::Job::jobs) 
                if (job.name == resultName) {
                    player.setJob(job.id);
                    return dpp::message{ "Changed job to " + job.name };
                }
            return dpp::message{ "There was an error changing your job to that." };
        default:
            break;
        }
    }

    return dpp::message{ "There was an error doing that." };
}
