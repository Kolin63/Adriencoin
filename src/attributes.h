#ifndef KOLIN_ADRIENCOIN_ATTRIBUTES_H
#define KOLIN_ADRIENCOIN_ATTRIBUTES_H

#include <dpp/nlohmann/json.hpp>
#include <string>

namespace adr
{
    ///
    /// attributes
    /// @brief A struct containing attributes for a player, usually related 
    /// to the states of items.
    ///
    struct attributes
    {
        ///
        /// save_json()
        /// @brief Save attributes to a JSON player save file
        /// @param data A reference to the JSON object that is being saved
        ///
        void save_json(nlohmann::json& data) const;

        ///
        /// load_json()
        /// @brief Load attributes from a JSON player save file
        /// @param data A const reference to the JSON object containing only
        /// the attributes. The object should not be the entire player save,
        /// only the attributes object
        ///
        void load_json(const nlohmann::json& data);

        ///
        /// list()
        /// @return A human readable list of all attributes that are not
        /// their default values.
        ///
        std::string list() const;
    };
}

#endif
