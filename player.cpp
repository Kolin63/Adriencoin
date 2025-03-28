#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <time.h>
#include <dpp/nlohmann/json.hpp>
#include "player.h"
#include "item.h"
#include "job.h"
#include "cache.h"

adr::Player::Player(const dpp::snowflake& uuid) 
    : m_uuid{ uuid }
{
    if (exists()) {
        load();
    }
    else {
        save();
        std::cout << uuid << " savedata does not exist. Creating new one.\n";
    }
}

adr::Player::Player(const dpp::snowflake& uuid, const Inventory& inv) 
    : m_uuid{ uuid }
{
    if (!exists()) {
        m_inv = inv;
        std::cout << uuid << " savedata does not exist. Creating new one.\n";
    }
    save();
}

adr::Player::~Player()
{
    save();
}

void adr::Player::save() const
{
    using json = nlohmann::json;
    std::cout << "Saving player " << m_uuid << '\n';

    std::filesystem::create_directory("playerdata");
    const std::string filename{ "playerdata/" + std::to_string(m_uuid) + ".json" };

    json data;

    data["uuid"] = static_cast<uint64_t>(m_uuid);
    data["job"] = m_job;
    data["lastWorked"] = m_lastWorked;
    data["inv"] = m_inv;

    std::ofstream fs(filename);
    fs << std::setw(4) << data << std::endl;

    fs.close();
}

void adr::Player::load()
{
    using json = nlohmann::json;
    std::cout << "Loading player " << m_uuid << '\n';
    std::string filename{ "playerdata/" + std::to_string(m_uuid) + ".json" };

    if (!exists()) {
        std::cerr << "Error: " << filename << " does not exist.\n";
        return;
    }

    std::ifstream fs{ filename };
    json data;

    try {
        data = json::parse(fs);
    }
    catch (const json::parse_error& e) {
        std::cerr << "adr::Player::load() json parse error: " << e.what() << '\n';
        fs.close();
        return;
    }

    if (data["uuid"] != static_cast<uint64_t>(m_uuid)) {
        std::cerr << "Error: " << filename << ' ' << data["uuid"] << " does not match " << m_uuid << '\n';
        fs.close();
        return;
    }

    m_job = data["job"];
    m_lastWorked = data["lastWorked"];
    m_inv = data["inv"];

    fs.close();
}

void adr::Player::print() const
{
    std::cout << m_uuid << "'s Inventory:\n"
        << ((m_job != adr::Job::MAX) ? adr::Job::jobs[m_job].name : "no job")
        << " last worked: " << m_lastWorked << '\n'
        << "next work: " << nextWork() << '\n'
        << "current time: " << std::time(0) << '\n';

    for (std::size_t i{}; i < m_inv.size(); ++i) {
        std::cout << adr::Item::names[i] << " (" << i << "):\t" << m_inv[i] << '\n';
    }
    std::cout << '\n';
}

const dpp::embed adr::Player::viewEmbed() const
{
    const adr::playerCacheElement& playerCache{ adr::cache::getElementFromCache(m_uuid) };
    dpp::embed embed{};
    embed.set_title(playerCache.username + "'s Inventory")
        .set_thumbnail(playerCache.avatarURL)
        .set_color(0x0088FF)
        .set_image("https://raw.githubusercontent.com/Kolin63/Adriencoin/refs/heads/main/art/skew-wide.jpg");

    std::string desc{
        '*' + std::to_string(m_uuid) + "*\n\n" 
        + "**Job:** " + ((m_job == adr::Job::MAX) ? "none" : adr::Job::jobs[m_job].name)
        + "\nLast Worked: " + dpp::utility::timestamp(m_lastWorked, dpp::utility::tf_short_datetime)
        + "\nCan Work Next " + nextWorkTimestamp()
        + "\n\n**Inventory:**\n"};

    desc += adr::getNonZeroItems(m_inv);

    embed.set_description(desc);

    return embed;
}

bool adr::Player::exists() const
{
    return std::filesystem::exists("playerdata/" + std::to_string(m_uuid) + ".json");
}

bool adr::Player::exists(const dpp::snowflake& uuid)
{
    return std::filesystem::exists("playerdata/" + std::to_string(uuid) + ".json");
}

void adr::Player::updateLastWorked() 
{ 
    m_lastWorked = std::time(nullptr); 
    std::cout << m_uuid << " updateLastWorked(), m_lastWorked = " << m_lastWorked 
        << " next work: " << nextWork() << '\n'
        << "current: " << std::time(0) << '\n';
}

std::time_t adr::Player::nextWork() const 
{ 
    // relative time, not since epoch
    return m_lastWorked - std::time(nullptr) + workCooldownSeconds; 
} 

std::string adr::Player::nextWorkTimestamp() const
{ 
    std::time_t t{ std::time(nullptr) + nextWork() };
    return dpp::utility::timestamp(t, dpp::utility::tf_relative_time) + " (" + dpp::utility::timestamp(t, dpp::utility::tf_short_time) + ")";
}

void adr::Player::changeInv(const Inventory& difference)
{
    for (std::size_t i{}; i < m_inv.size(); ++i) {
        m_inv[i] += difference[i];
    }
}

void adr::Player::subtractInv(const Inventory& difference)
{
    for (std::size_t i{}; i < m_inv.size(); ++i) {
        m_inv[i] -= difference[i];
    }
}

int& adr::Player::operator[](int index)
{
    return m_inv[index];
}

const int& adr::Player::operator[](int index) const
{
    return m_inv[index];
}
