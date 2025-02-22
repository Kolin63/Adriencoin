#ifndef SHOP_H
#define SHOP_H

#include <string>
#include <dpp/dpp.h>

namespace adr {
    namespace shop {
        enum shopScreens {
            gaydrien,
            adriresource,
            job,
            resource,

            MAX
        };

        extern const std::array<std::string, MAX> names;
        const std::array<dpp::embed, adr::shop::MAX> makeEmbedsList();
        extern const std::array<dpp::embed, adr::shop::MAX> embeds;

        dpp::message getMessage();
    }
}

#endif
