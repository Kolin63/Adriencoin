#pragma warning(disable: 4251) // disables a silly warning from dpp

#include <time.h>
#include "player.h"
#include "item.h"
#include "job.h"

adr::Player::Player(const dpp::snowflake& uuid) 
    : m_uuid{ uuid }
{
    if (exists()) {
        load();
    }
    else {
        m_version = CURRENT_SAVE_VERSION;
        save();
        std::cout << uuid << " savedata does not exist. Creating new one.\n";
    }
}

adr::Player::Player(const dpp::snowflake& uuid, const Inventory& inv) 
    : m_uuid{ uuid }
{
    if (!exists()) {
        m_inv = inv;
        m_version = CURRENT_SAVE_VERSION;
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
    std::filesystem::create_directory("playerdata");

    const std::string filename{ "playerdata/" + std::to_string(m_uuid) + ".bin" };
    std::ofstream fs(filename, std::ios::binary);

    fs.write(reinterpret_cast<const char*>(&m_version), sizeof m_version);
    fs.write(reinterpret_cast<const char*>(&m_job), sizeof m_job);
    fs.write(reinterpret_cast<const char*>(&m_tempJob), sizeof m_tempJob);
    fs.write(reinterpret_cast<const char*>(&m_lastWorked), sizeof m_lastWorked);

    for (const int i : m_inv) {
        fs.write(reinterpret_cast<const char*>(&i), sizeof i);
    }
    fs.close();
}

void adr::Player::load()
{
    const std::string filename{ "playerdata/" + std::to_string(m_uuid) + ".bin" };
    if (!exists()) {
        std::cerr << "Error: " << filename << " does not exist.\n";
        return;
    }

    std::ifstream fs(filename, std::ios::binary);

    fs.read(reinterpret_cast<char*>(&m_version), sizeof m_version);
    fs.read(reinterpret_cast<char*>(&m_job), sizeof m_job);
    fs.read(reinterpret_cast<char*>(&m_tempJob), sizeof m_tempJob);
    fs.read(reinterpret_cast<char*>(&m_lastWorked), sizeof m_lastWorked);
    for (int& i : m_inv) {
        fs.read(reinterpret_cast<char*>(&i), sizeof i);
    }
    fs.close();
}

void adr::Player::print() const
{
    char lastWorkedString[100];
    std::tm tm{};
    if (localtime_s(&tm, &m_lastWorked) != 0) {
        std::cerr << "Error converting time.\n";
        return;
    }

    std::cout << m_uuid << "'s Inventory:\n"
        << ((m_job != adr::Job::MAX) ? adr::Job::jobs[m_job].name : "no job")
        << " last worked: " << (std::strftime(lastWorkedString, sizeof lastWorkedString, "%c", &tm) ? lastWorkedString : "error")
        << '\n';

    for (std::size_t i{}; i < m_inv.size(); ++i) {
        std::cout << adr::Item::names[i] << " (" << i << "):\t" << m_inv[i] << '\n';
    }
    std::cout << '\n';
}

const dpp::embed adr::Player::viewEmbed(dpp::cluster& bot) const
{
    dpp::embed embed{};
    bot.user_get_cached(m_uuid, [&](const dpp::confirmation_callback_t& callback){
        if (callback.is_error()) {
            embed.set_title("Error").set_description("There was an error retrieving that data.").set_color(0xFF0000);
            return;
        }

        dpp::user_identified userIdent{ std::get<dpp::user_identified>(callback.value) };

        embed.set_title(userIdent.username + "'s Inventory")
            .set_thumbnail(userIdent.get_avatar_url())
            .set_color(0x0088FF)
            .set_image("https://raw.githubusercontent.com/Kolin63/Adriencoin/refs/heads/main/art/skew-wide.jpg");

        std::string desc{
            "**Job:** " + ((m_job == adr::Job::MAX) ? "none" : adr::Job::jobs[m_job].name)
            + "\nLast Worked: " + dpp::utility::timestamp(m_lastWorked, dpp::utility::tf_short_datetime)
            + "\nCan Work Next " + nextWorkTimestamp()
            + "\n\n**Inventory:**\n"};

        for (std::size_t i{}; i < m_inv.size(); ++i) {
            desc += dpp::emoji{ 
                adr::Item::names[i], adr::Item::emojiIDs[i] }.get_mention() + ' ' + adr::Item::names[i] + ": " 
                + std::to_string(m_inv[i]) + '\n';
        }

        embed.set_description(desc);
    });

    return embed;
}

bool adr::Player::exists() const
{
    return std::filesystem::exists("playerdata/" + std::to_string(m_uuid) + ".bin");
}

bool adr::Player::exists(const dpp::snowflake& uuid)
{
    return std::filesystem::exists("playerdata/" + std::to_string(uuid) + ".bin");
}

std::string adr::Player::nextWorkTimestamp() const
{ 
    std::time_t t{ std::time(0) + nextWork() };
    return dpp::utility::timestamp(t, dpp::utility::tf_relative_time) + " (" + dpp::utility::timestamp(t, dpp::utility::tf_short_time) + ")";
}

int& adr::Player::operator[](int index)
{
    return m_inv[index];
}

const int& adr::Player::operator[](int index) const
{
    return m_inv[index];
}
