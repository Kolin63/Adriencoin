#include <cassert>
#include "graph.h"

std::uint32_t adr::graph::coord_to_index(std::uint16_t x, std::uint16_t y) const
{
    assert(x * y < m_body.size() && "x * y must be less than body size");

    // Adding 1 to the width is to account for any \n characters
    int index{ y * (m_width + 1) + x };

    assert(index < m_body.size() && "index must be less than body size");

    return index;
}

void adr::graph::set_coord(std::uint16_t x, std::uint16_t y, char c)
{
    m_body[coord_to_index(x, y)] = c;
}

std::string adr::graph::make_body(std::uint8_t interval)
{
    return std::to_string(interval); // temp
}

std::uint32_t adr::graph::point_to_index(std::uint16_t x, std::uint16_t y, std::uint8_t interval, std::uint16_t start_x) const
{
    assert(x >= start_x && "x cannot be less than start x");

    const std::uint16_t gx{ static_cast<std::uint16_t>(x - start_x - ( (get_col_width() * (x / interval)) + (get_row_num_width() + 1) )) };
    const std::uint16_t gy{ static_cast<std::uint16_t>(get_height() - y) };

    return coord_to_index(gx, gy);
}

void adr::graph::set_point(std::uint16_t x, std::uint16_t y, char c, std::uint8_t interval, std::uint16_t start_x)
{
    m_body[point_to_index(x, y, interval, start_x)] = c;
}

std::size_t adr::graph::get_col_width() const
{
    // Adding 1 is padding, so the numbers aren't directly next to eachother
    return std::to_string(m_width).size() + 1;
}

std::size_t adr::graph::get_row_num_width() const
{
    // Does not account for the seperation pipe 
    return std::to_string(m_height).size();
}
