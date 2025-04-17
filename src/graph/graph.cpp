#include <iostream>
#include <cmath>
#include <algorithm>
#include "graph.h"

std::uint32_t kolin::graph::coord_to_index(std::uint32_t x, std::uint32_t y, std::uint8_t int_x, std::uint32_t start_x) const
{
    // Adding 1 to the width is to account for any \n characters
    std::uint32_t index{ y * (get_row_num_width() + 1 + m_width * get_col_width(int_x, start_x)) + x - 1 };

    if (index >= m_body.size()) {
        std::cerr << "Index out of range: " << index << " >= " << m_body.size() << '\n';
        throw out_of_range;
    }

    return index;
}

void kolin::graph::set_coord(std::uint32_t x, std::uint32_t y, char c, std::uint8_t int_x, std::uint32_t start_x)
{
    m_body[coord_to_index(x, y, int_x, start_x)] = c;
}

kolin::graph::dataset kolin::graph::average_dataset(const kolin::graph::dataset& data, 
                                                    std::uint32_t width, 
                                                    std::uint8_t int_x, std::uint8_t int_y, 
                                                    std::uint32_t start_x)
{
    // No need to average anything if the intervals are 1
    if (int_x == 1 && int_y == 1) return data;

    // Where the first is N and the second is the sum
    std::vector<std::pair<std::uint8_t, std::uint32_t>> total(width);

    // Put the points from data into the total vector so that it can be averaged
    for (const auto& point : data) {
        // Coordinates rounded to the nearest interval
        const std::uint32_t rx{ point.first - point.first % int_x };
        const std::uint32_t ry{ point.second - point.second % int_y };

        // X Coordinate Translated to the Columns
        const std::uint32_t i{ rx / int_x - start_x };

        // Out of bounds check
        if (i >= total.size() || i < 0 || i > width) continue;

        // Increment N
        ++total[i].first;

        // Add the point's Y-Position to the total
        total[i].second += ry;
    }

    // Make a new dataset where the averaged points will be put
    dataset avg(width);

    // Average the total vector into the final dataset
    for (std::uint32_t i{}; i < total.size(); ++i) {
        if (total[i].first == 0) continue;
        avg[i].first = i * int_x;
        avg[i].second = total[i].second / total[i].first;
    }

    return avg;
}

std::function<std::uint32_t(std::uint32_t x)> kolin::graph::make_linear(const point& p, const point& q)
{
    // Solve for slope with (y2 - y1) / (x2 - x1)
    const double m{ (static_cast<double>(q.second) - p.second) / (static_cast<double>(q.first) - p.first) };

    // Solve for y-intercept with b = y - mx
    const double b{ p.second - m * p.first };

    // Make the lambda
    return [m, b](std::uint32_t x) {
        return static_cast<std::uint32_t>(std::round(m * x + b));
        };
}

void kolin::graph::draw_lines(std::uint8_t int_x, std::uint8_t int_y, std::uint32_t start_x, std::uint32_t start_y)
{
    // 1 point means no lines
    if (m_data.size() == 1) return;

    using point = kolin::graph::point;

    dataset data{ average_dataset(m_data, m_width, int_x, int_y, start_x) };

    // Sort the data by x-values, least to greatest
    std::sort(data.begin(), data.end(), [](const point& a, const point& b) { return a.first < b.first; });

    for (std::size_t i{}; i < data.size() - 1; ++i) {
        // References for easier typing
        const point& p{ data[i] };
        const point& q{ data[i + 1] };

        // Points relative to the top-left corner of the graph
        const point tp{ point_to_coord(p.first, p.second, int_x, int_y, start_x, start_y) };
        const point tq{ point_to_coord(q.first, q.second, int_x, int_y, start_x, start_y) };

        // This function will return an integer y-value when given a double x-value
        const auto func{ make_linear(tp, tq) };

        // For each character between the two points
        for (std::uint32_t x{ tp.first }; x < tq.first; ++x) {

            std::uint32_t ya{ func(x) };
            std::uint32_t yb{ func(x + 1) };

            constexpr char c{ '*' };

            auto sc = [&x, &c, &int_x, &start_x, this](std::uint32_t ya) {
                set_coord(x, ya, c, int_x, start_x);
                };

            // Below, positive slope
            if (ya > yb) {
                for (ya; ya > yb; --ya) {
                    sc(ya);
                }
            }
            // Above, negative slope
            else if (ya < yb) {
                for (ya; ya < yb; ++ya) {
                    sc(ya);
                }
            }
            // Equal, zero slope
            else {
                sc(ya);
            }
        }
    }
}

std::string kolin::graph::make_body(std::uint8_t int_x, std::uint8_t int_y, std::uint32_t start_x, std::uint32_t start_y)
{
    // Interval may not be 0
    if (int_x == 0 || int_y == 0) return "Error: Intervals may not be equal to 0";

    // Y-Axis Numbers and Dividers and Rows
    for (std::int64_t i{ static_cast<std::int64_t>(m_height) - 1}; i >= 0; --i) {
        std::string num{ std::to_string(i * int_y + start_y) };
        m_body += std::string(get_row_num_width() - 1 - num.size(), ' ') + num + '|' + std::string(m_width * get_col_width(int_x, start_x), ' ') + '\n';
    }

    // X-Axis Divider
    m_body += std::string(get_row_num_width(), ' ') + std::string(get_col_width(int_x, start_x) * m_width, '-') + '\n';

    // X-Axis Numbers
    m_body += std::string(get_col_width(int_x, start_x), ' ');

    for (std::uint32_t i{}; i < m_width; ++i) {
        std::string num{ std::to_string(i * int_x + start_x) };
        m_body += std::string(get_col_width(int_x, start_x) - num.size(), ' ') + num;
    }

    // Draw Lines
    try {
        draw_lines(int_x, int_y, start_x, start_y);
    }
    catch (kolin::graph::error) {}

    // Sets the data points
    for (const auto& point : average_dataset(m_data, m_width, int_x, int_y, start_x)) {
        try {
            set_point(point.first, point.second, '#', int_x, int_y, start_x, start_y);
        }
        // if there is an error we simply do not set that point
        catch (kolin::graph::error) {}
    }

    return m_body; 
}

kolin::graph::point kolin::graph::point_to_coord(std::uint32_t x, std::uint32_t y, 
                                                 std::uint8_t int_x, std::uint8_t int_y, 
                                                 std::uint32_t start_x, std::uint32_t start_y) const
{
    if (x < start_x) throw kolin::graph::out_of_range;
    if (y < start_y) throw kolin::graph::out_of_range;

    if (x >= start_x + int_x * m_width) throw kolin::graph::out_of_range;
    if (y >= start_y + int_y * m_height) throw kolin::graph::out_of_range;

    const std::uint32_t gx{ get_col_width(int_x, start_x) * ((x / int_x) - start_x + 1) + (get_row_num_width()) };
    const std::uint32_t gy{ get_height() - (y / int_y) - 1 + (start_y / int_y) };

    return { gx, gy };
}

std::uint32_t kolin::graph::point_to_index(std::uint32_t x, std::uint32_t y, 
                                           std::uint8_t int_x, std::uint8_t int_y, 
                                           std::uint32_t start_x, std::uint32_t start_y) const
{
    const kolin::graph::point gp{ point_to_coord(x, y, int_x, int_y, start_x, start_y) };

    return coord_to_index(gp.first, gp.second, int_x, start_x);
}

void kolin::graph::set_point(std::uint32_t x, std::uint32_t y, char c, 
                             std::uint8_t int_x, std::uint8_t int_y, 
                             std::uint32_t start_x, std::uint32_t start_y)
{
    m_body[point_to_index(x, y, int_x, int_y, start_x, start_y)] = c;
}

std::uint32_t kolin::graph::get_col_width(std::uint8_t int_x, std::uint32_t start_x) const
{
    // Adding 1 is padding, so the numbers aren't directly next to eachother
    return static_cast<std::uint32_t>(std::to_string((m_width + start_x - 1) * int_x).size() + 1);
}

std::uint32_t kolin::graph::get_row_num_width() const
{
    // Does not account for the seperation pipe 
    return static_cast<std::uint32_t>(std::to_string(m_height).size() + 1);
}
