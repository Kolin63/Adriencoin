#include "item.h"
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
                .set_title(product.display)
                .set_description("`/buy " + product.name + "`\n> *" + product.joke + "*\n" + product.desc + (product.desc == "" ? "" : "\n") + '\n')
                .set_thumbnail(product.picURL)
                .set_color(product.color);

            auto listItems = [&embed](bool isCost, const Product::sub& sub, const inventory& inv) {
                embed.set_description(embed.description
                    + "> **" + (isCost ? "Cost" 
                        : (sub.resultType == adr::Product::r_one || sub.resultType == adr::Product::r_customOne ? "Result (Choose One)" : "Result")) 
                    + ":**\n");

                for (std::size_t j{}; j < inv.size(); ++j) {
                    embed.set_description(embed.description 
                        + (inv[j] == 0 ? "" : "> * " + std::to_string(inv[j]) + ' ' + adr::get_emoji(static_cast<adr::item_id>(j)) + '\n'));
                }

                if (!isCost) {
                    for (const std::string& str : sub.customResult) {
                        embed.set_description(embed.description + "> * " + str + '\n');
                    }
                }
            };

            for (const Product::sub& sub : product.subproducts) {
                embed.description += "## " + sub.name + ": \n";
                listItems(true, sub, sub.cost);
                listItems(false, sub, sub.result);
                embed.description += '\n';
            }

            if (name == "everything") msg.add_embed(embed);
            else return dpp::message{}.add_embed(embed);
        }
    }

    if (name == "everything") return msg;
    return dpp::message{ "There was an error doing that" }.set_flags(dpp::m_ephemeral);
}

dpp::message adr::shop::buy(
        const dpp::snowflake& uuid, 
        const std::string& productName, 
        const std::string& subprodName,
        const std::string& resultName, 
        int times
        )
{
    if (times > 100)
        return dpp::message{ "You can not buy something more than 100 times." }.set_flags(dpp::m_ephemeral);

    auto muxInv = [](inventory inv, int times) -> inventory {
        std::for_each(inv.begin(), inv.end(), [&times](int& i) { i *= times; });
        return inv;
        };

    for (adr::Product product : adr::Product::products) {
        if (product.name != productName) continue;
        for (Product::sub sub : product.subproducts) {
            if (sub.name != subprodName) continue;
            std::cout << "product: " << productName << ' ' << resultName 
            << " sub: " << sub.name << '\n';

            adr::Player& player{ adr::cache::getPlayerFromCache(uuid) };

            inventory cost{ muxInv(sub.cost, times) };
            inventory result{ muxInv(sub.result, times) };

            if (!player.canBuy(cost)) 
                return dpp::message{ "You can't afford that!" }.set_flags(dpp::m_ephemeral);

            dpp::message msg{ "**Transaction Complete!**\n\n" };

            auto listItems = [&msg, &sub](bool isCost, const inventory& inv, int customResultIndex = -1) {
                msg.set_content(msg.content + "**" + (isCost ? "Cost" : "Result") + ":**\n");

                for (std::size_t j{}; j < inv.size(); ++j) {
                    if (inv[j] != 0) 
                        msg.set_content(msg.content + "* " + std::to_string(inv[j]) + ' ' 
                            + adr::get_emoji(static_cast<adr::item_id>(j)) + '\n');
                }

                if (!isCost) {
                    if (customResultIndex == -1) {
                        for (const std::string& str : sub.customResult) {
                            msg.set_content(msg.content + "* " + str + '\n');
                        }
                    }
                    else {
                        msg.set_content(msg.content + "* " + sub.customResult[customResultIndex] + '\n');
                    }
                }
            };

            listItems(true, cost);
            
            switch (sub.resultType) {

/* all */   case adr::Product::r_all:
                player.subtractInv(cost);
                player.changeInv(result);
                listItems(false, result);
                return msg;
            
/* one */   case adr::Product::r_one:
            {
                adr::item_id resultIndex{ adr::get_item_id(resultName) };

                if (resultIndex == adr::i_null)
                    return dpp::message{ "Something went wrong with the result" }.set_flags(dpp::m_ephemeral);

                player.subtractInv(cost);
                player.changeInv(resultIndex, result[resultIndex]);

                // Make an Inventory where the only non-zero value is the chosen one
                inventory finalResult{};
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
                            listItems(false, result, player.job());
                            return msg;
                        }
                    return dpp::message{ "There was an error changing your job to that." };
                }
                if (productName == "titles") {
                    player.setTitle(adr::daily::t_baron);
                    player.subtractInv(cost);
                    listItems(false, result);
                    return msg;
                }
            }
            return dpp::message{ "There was an error doing that" }.set_flags(dpp::m_ephemeral);
        }
    }
    return dpp::message{ "There was an error doing that" }.set_flags(dpp::m_ephemeral);
}
