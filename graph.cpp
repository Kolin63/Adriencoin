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

std::string adr::graph::make_body(std::uint8_t int_x, std::uint8_t int_y)
{
    m_body = "";
}

std::uint32_t adr::graph::point_to_index(std::uint16_t x, std::uint16_t y, std::uint8_t int_x = 1, std::uint8_t int_y = 1, std::uint16_t start_x = 0) const
{
    assert(x >= start_x && "x cannot be less than start x");

    const std::uint16_t gx{ static_cast<std::uint16_t>(x - start_x - ( (get_col_width() * (x / int_x)) + (get_row_num_width() + 1) )) };
    const std::uint16_t gy{ static_cast<std::uint16_t>(get_height() - (y / int_y)) };

    return coord_to_index(gx, gy);
}

void adr::graph::set_point(std::uint16_t x, std::uint16_t y, char c, std::uint8_t int_x = 1, std::uint8_t int_y = 1, std::uint16_t start_x = 0)
{
    m_body[point_to_index(x, y, int_x, int_y, start_x)] = c;
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
