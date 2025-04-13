#ifndef ADRIENCOIN_GRAPH_H
#define ADRIENCOIN_GRAPH_H

#include <string>
#include <string_view>
#include <vector>

namespace adr
{
    class graph
    {
    private:
        // The body of the graph, which can be printed to display it
        std::string m_body{};

        // The data of the graph, where the first of the pair is the x-coordinate 
        // and the second of the pair is the y-coordinate
        std::vector<std::pair<std::uint16_t, std::uint16_t>> data{};

        // How wide the graph is in characters
        // Does not include \n or \0
        std::uint16_t m_width{}; 
        // How tall the graph is in characters
        std::uint16_t m_height{};

        // The max character width of columns along the x axis when represented in decimal
        std::size_t m_col_width{};
        // The max character width of the row label numbers along the y axis when represented in decimal
        std::size_t m_row_num_width{};

        // Returns a one-dimensional index given a two-dimensional coordinate which can be used to index the body string
        // Note: (0, 0) is the top-left corner
        std::uint32_t coord_to_index(std::uint16_t x, std::uint16_t y) const;
        // Sets a character in the body given a two-dimensional coordinate
        // Note: (0, 0) is the top-left corner
        void set_coord(std::uint16_t x, std::uint16_t y, char c);

    public:
        // Returns the body of the graph
        std::string get_body() const { return m_body; }

        // Makes the body of the graph with the given interval and returns it
        std::string make_body(std::uint8_t interval = 1);

        // Returns a one-dimensional index given a two-dimensional coordinate which can be used to index the body string
        // Note: (0, 0) is the bottom-left corner, or the origin, of the graph
        std::uint32_t point_to_index(std::uint16_t x, std::uint16_t y) const;
        // Sets a character in the body given a two-dimensional coordinate
        // Note: (0, 0) is the bottom-left corner, or the origin, of the graph
        void set_point(std::uint16_t x, std::uint16_t y, char c);

        // How wide the graph is in characters
        // Does not include \n or \0
        void set_width(std::uint16_t width) { m_width = width; }
        // How wide the graph is in characters
        // Does not include \n or \0
        std::uint16_t get_width() const { return m_width; }
        // Returns without re-calculating the value
        std::size_t get_col_width_const() const { return m_col_width; }
        // Re-calculates the value and stores it before returning
        std::size_t get_col_width();
        // Re-calculates the value but does not store it
        std::size_t get_col_width() const;

        // How tall the graph is in characters
        void set_height(std::uint16_t height) { m_height = height; }
        // How tall the graph is in characters
        std::uint16_t get_height() const { return m_height; }
        // Returns without re-calculating the value
        std::size_t get_row_num_width_const() const { return m_row_num_width; }
        // Re-calculates the value and stores it before returning
        std::size_t get_row_num_width();
        // Re-calculates the value but does not store it
        std::size_t get_row_num_width() const;
    };
}

#endif
