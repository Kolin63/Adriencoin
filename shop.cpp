#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <functional>
#include <algorithm>
#include "shop.h"
#include "product.h"
#include "cache.h"

dpp::message adr::shop::getMessage(const std::string name)
{
    dpp::message msg{};

    for (const adr::Product& i : adr::Product::products) {
        if (name == i.name || name == "everything") {
            dpp::embed embed{};

            embed
                .set_title(i.name)
                .set_description("> *" + i.joke + "*\n" + i.desc + (i.desc == "" ? "" : "\n") + '\n')
                .set_thumbnail(i.picURL)
                .set_color(i.color);

            auto listItems = [&embed](bool isCost, const Inventory& inv) {
                embed.set_description(embed.description
                    + "**" + (isCost ? "Cost" : "Result") + ":**\n");

                for (std::size_t j{}; j < inv.size(); ++j) {
                    if (inv[j] != 0) 
                        embed.set_description(embed.description + "* " + std::to_string(inv[j]) + ' ' 
                            + adr::Item::getEmojiMention(static_cast<adr::Item::Id>(j)) + '\n');
                }
            };

            listItems(true, i.cost);
            listItems(false, i.result);

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
        return dpp::message{ "You can not buy something more than 100 times." };

    for (const adr::Product& product : adr::Product::products) {
        if (product.name == productName) {
            adr::Player& player{ adr::cache::getPlayerFromCache(uuid) };

            if (player.inv() < product.cost) 
                return dpp::message{ "You can't afford that!" }.set_flags(dpp::m_ephemeral);

            dpp::message msg{ "**Transaction Complete!**\nCost:\n" };

            auto listItems = [&msg](bool isCost, const Inventory& inv) {
                msg.set_content(msg.content + "**" + (isCost ? "Cost" : "Result") + ":**\n");

                for (std::size_t j{}; j < inv.size(); ++j) {
                    if (inv[j] != 0) 
                        msg.set_content(msg.content + "* " + std::to_string(inv[j]) + ' ' 
                            + adr::Item::getEmojiMention(static_cast<adr::Item::Id>(j)) + '\n');
                }
            };

            listItems(true, product.cost);
            
            switch (product.resultType) {

/* all */   case adr::Product::r_all:
                player.subtractInv(product.cost);
                player.changeInv(product.result);
                listItems(false, product.result);
                return msg;
            
/* one */   case adr::Product::r_one:
            {
                adr::Item::Id resultIndex{ adr::Item::getId(resultName) };

                if (resultIndex == adr::Item::null)
                    return dpp::message{ "Something went wrong with the result" }.set_flags(dpp::m_ephemeral);

                player.subtractInv(product.cost);
                player.changeInv(resultIndex, product.result[resultIndex]);

                // Make an Inventory where the only non-zero value is the chosen one
                Inventory finalResult{};
                finalResult[resultIndex] = product.result[resultIndex];

                listItems(false, finalResult);
                return msg;
            }

/* none */  case adr::Product::r_none:
                player.subtractInv(product.cost);
                return msg;

/* cstm */  case adr::Product::r_customAll:
            case adr::Product::r_customOne: 
                if (productName == "tierupgrade") {
                    player.setJob(static_cast<adr::Job::Id>(player.job() + adr::Job::tierOneJobsSize));
                    return dpp::message{ "Upgraded job to " + adr::Job::jobs[player.job()].name };
                }
                if (productName == "jobchange") {
                    for (const adr::Job& job : adr::Job::jobs)
                        if (job.name == resultName) {
                            player.setJob(job.id);
                            return dpp::message{ "Changed job to " + job.name };
                        }
                    return dpp::message{ "There was an error changing your job to that." };
                }
            }
        }
    }
    return dpp::message{ "There was an error doing that" }.set_flags(dpp::m_ephemeral);
}
