#ifndef JOB_H
#define JOB_H

#include <string>
#include <array>
#include "item.h"

namespace Job
{
    struct Job
    {
        std::string name{};
        std::string action{};
        Item::Item item{};
    };

    std::array<Job, 14> jobs{ {
        { "farmer", "farm", Item::Item{Item::carrot, 20} },
        { "miner", "mine", Item::Item{Item::gemstone, 5} },
        { "lumberjack", "chop", Item::Item{Item::wood, 30} },
        { "fisherman", "fish", Item::Item{Item::fish, 30} },
        { "warrior", "fight", Item::Item{Item::rotten_flesh, 60} },
        { "enchanter", "enchant", Item::Item{Item::book, 2} },
        { "brewer", "brew", Item::Item{Item::potion, 1} },

        { "agriculturalist", "farm", Item::Item{Item::carrot, 200} },
        { "collier", "mine", Item::Item{Item::gemstone, 50} },
        { "logger", "chop", Item::Item{Item::wood, 300} },
        { "piscator", "fish", Item::Item{Item::fish, 300} },
        { "knight", "fight", Item::Item{Item::rotten_flesh, 600} },
        { "thaumaturgist", "enchant", Item::Item{Item::book, 20} },
        { "alchemist", "brew", Item::Item{Item::potion, 10} }
    } };
}

#endif
