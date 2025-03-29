#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <functional>
#include <algorithm>
#include "shop.h"
#include "product.h"
#include "cache.h"

dpp::message adr::shop::getMessage(const std::string name)
{
    dpp::message msg{};

    for (const adr::Product& product : adr::Product::products) {
        if (name == product.name || name == "everything") {
            dpp::embed embed{};

            embed
                .set_title(product.name)
                .set_description("> *" + product.joke + "*\n" + product.desc + (product.desc == "" ? "" : "\n") + '\n')
                .set_thumbnail(product.picURL)
                .set_color(product.color);

            auto listItems = [&embed, &product](bool isCost, const Inventory& inv) {
                embed.set_description(embed.description
                    + "**" + (isCost ? "Cost" 
                        : (product.resultType == adr::Product::r_one || product.resultType == adr::Product::r_customOne ? "Result (Choose One)" : "Result")) 
                    + ":**\n");

                for (std::size_t j{}; j < inv.size(); ++j) {
                    embed.set_description(embed.description 
                        + (inv[j] == 0 ? "" : "* " + std::to_string(inv[j]) + ' ' + adr::Item::getEmojiMention(static_cast<adr::Item::Id>(j)) + '\n'));
                }

                if (!isCost) {
                    for (const std::string& str : product.customResult) {
                        embed.set_description(embed.description + "* " + str + '\n');
                    }
                }
            };

            listItems(true, product.cost);
            listItems(false, product.result);

            if (name == "everything") msg.add_embed(embed);
            else return dpp::message{ embed };
        }
    }

    if (name == "everything") return msg;
    return dpp::message{ "There was an error doing that" }.set_flags(dpp::m_ephemeral);
}

dpp::message adr::shop::buy(const dpp::snowflake& uuid, const std::string& productName, const std::string& resultName, int times)
{
    if (times > 100)
        return dpp::message{ "You can not buy something more than 100 times." }.set_flags(dpp::m_ephemeral);

    auto muxInv = [](Inventory inv, int times) -> Inventory {
        std::for_each(inv.begin(), inv.end(), [&times](int& i) { i *= times; });
        return inv;
        };

    for (adr::Product product : adr::Product::products) {
        if (product.name == productName) {
            std::cout << "product: " << productName << ' ' << resultName << '\n';

            adr::Player& player{ adr::cache::getPlayerFromCache(uuid) };

            Inventory cost{ muxInv(product.cost, times) };
            Inventory result{ muxInv(product.result, times) };

            if (!player.canBuy(cost)) 
                return dpp::message{ "You can't afford that!" }.set_flags(dpp::m_ephemeral);

            dpp::message msg{ "**Transaction Complete!**\n\n" };

            auto listItems = [&msg, &product](bool isCost, const Inventory& inv) {
                msg.set_content(msg.content + "**" + (isCost ? "Cost" : "Result") + ":**\n");

                for (std::size_t j{}; j < inv.size(); ++j) {
                    if (inv[j] != 0) 
                        msg.set_content(msg.content + "* " + std::to_string(inv[j]) + ' ' 
                            + adr::Item::getEmojiMention(static_cast<adr::Item::Id>(j)) + '\n');
                }

                if (!isCost) {
                    for (const std::string& str : product.customResult) {
                        msg.set_content(msg.content + "* " + str + '\n');
                    }
                }
            };

            listItems(true, cost);
            
            switch (product.resultType) {

/* all */   case adr::Product::r_all:
                player.subtractInv(cost);
                player.changeInv(result);
                listItems(false, result);
                return msg;
            
/* one */   case adr::Product::r_one:
            {
                adr::Item::Id resultIndex{ adr::Item::getId(resultName) };

                if (resultIndex == adr::Item::null)
                    return dpp::message{ "Something went wrong with the result" }.set_flags(dpp::m_ephemeral);

                player.subtractInv(cost);
                player.changeInv(resultIndex, result[resultIndex]);

                // Make an Inventory where the only non-zero value is the chosen one
                Inventory finalResult{};
                finalResult[resultIndex] = result[resultIndex];

                listItems(false, finalResult);
                return msg;
            }

/* none */  case adr::Product::r_none:
                player.subtractInv(cost);
                return msg;

/* cstm */  case adr::Product::r_customAll:
            case adr::Product::r_customOne: 
                if (productName == "tier_upgrade") {
                    player.setJob(static_cast<adr::Job::Id>(player.job() + adr::Job::tierOneJobsSize));
                    player.subtractInv(cost);

                    return dpp::message{ "Upgraded job to " + adr::Job::jobs[player.job()].name };
                }
                if (productName == "job_change") {
                    for (const adr::Job& job : adr::Job::jobs)
                        if (job.name == resultName) {
                            player.setJob(job.id);
                            player.subtractInv(cost);

                            listItems(false, result);
                            return msg;
                        }
                    return dpp::message{ "There was an error changing your job to that." };
                }
            }
            return dpp::message{ "There was an error doing that" }.set_flags(dpp::m_ephemeral);
        }
    }
    return dpp::message{ "There was an error doing that" }.set_flags(dpp::m_ephemeral);
}
