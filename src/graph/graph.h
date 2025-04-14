#ifndef KOLIN63_GRAPH_H
#define KOLIN63_GRAPH_H

#include <string>
#include <vector>
#include <functional>

namespace kolin
{
    class graph
    {
    private:
        // The body of the graph, which can be printed to display it
        std::string m_body{};

    public:
        // Represents a point on the graph, where teh first of the pair is the 
        // x-coordinate and the second of the pair is the y-coordinate
        using point = std::pair<std::uint32_t, std::uint32_t>;
        // The data of the graph, where the first of the pair is the x-coordinate 
        // and the second of the pair is the y-coordinate
        using dataset = std::vector<point>;
    private:
        // The data of the graph, where the first of the pair is the x-coordinate 
        // and the second of the pair is the y-coordinate
        dataset m_data{};

        // How wide the graph is in characters
        // Does not include \n or \0
        std::uint32_t m_width{}; 
        // How tall the graph is in characters
        std::uint32_t m_height{};

        // Returns a one-dimensional index given a two-dimensional coordinate which can be used to index the body string
        // Note: (0, 0) is the top-left corner
        std::uint32_t coord_to_index(std::uint32_t x, std::uint32_t y, std::uint8_t int_x, std::uint32_t start_x) const;
        // Sets a character in the body given a two-dimensional coordinate
        // Note: (0, 0) is the top-left corner
        void set_coord(std::uint32_t x, std::uint32_t y, char c, std::uint8_t int_x, std::uint32_t start_x);

        // Takes a dataset of kolin::graph::points and averages it 
        // into a dataset based on the interval and width of the graph 
        static dataset average_dataset(const dataset& data, std::uint32_t width, std::uint8_t int_x, std::uint8_t int_y, std::uint32_t start_x);

        // Takes two points and returns a linear equation that passes
        // through the two points
        static std::function<std::uint32_t(std::uint32_t x)> make_linear(const point& a, const point& b);

        // Draws lines between points given a dataset
        void draw_lines(std::uint8_t int_x, std::uint8_t int_y, std::uint32_t start_x, std::uint32_t start_y);

    public:
        enum error {
            out_of_range,
        };

        graph(std::uint32_t width, std::uint32_t height, const dataset& data)
            : m_width{ width }, m_height{ height }, m_data{ data }
        { }

        // Returns the body of the graph
        std::string get_body() const { return m_body; }

        // Makes the body of the graph with the given interval and returns it
        std::string make_body(std::uint8_t int_x = 1, std::uint8_t int_y = 1, std::uint32_t start_x = 0, std::uint32_t start_y = 0);

        // Returns a two-dimensional coordinate relative to the top-left corner of the graph given a 
        // coordinate relative to the bottom-let corner of the graph
        point point_to_coord(std::uint32_t x, std::uint32_t y, std::uint8_t int_x = 1, std::uint8_t int_y = 1, std::uint32_t start_x = 1, std::uint32_t start_y = 1) const;
        // Returns a one-dimensional index given a two-dimensional coordinate which can be used to index the body string
        // Note: (0, 0) is the bottom-left corner, or the origin, of the graph
        std::uint32_t point_to_index(std::uint32_t x, std::uint32_t y, std::uint8_t int_x = 1, std::uint8_t int_y = 1, std::uint32_t start_x = 1, std::uint32_t start_y = 1) const;
        // Sets a character in the body given a two-dimensional coordinate
        // Note: (0, 0) is the bottom-left corner, or the origin, of the graph
        void set_point(std::uint32_t x, std::uint32_t y, char c, std::uint8_t int_x = 1, std::uint8_t int_y = 1, std::uint32_t start_x = 1, std::uint32_t start_y = 1);

        // How wide the graph is in characters
        // Does not include \n or \0
        void set_width(std::uint32_t width) { m_width = width; }
        // How wide the graph is in characters
        // Does not include \n or \0
        std::uint32_t get_width() const { return m_width; }
        // Re-calculates the value but does not store it
        std::uint32_t get_col_width(std::uint8_t int_x, std::uint32_t start_x) const;

        // How tall the graph is in characters
        void set_height(std::uint32_t height) { m_height = height; }
        // How tall the graph is in characters
        std::uint32_t get_height() const { return m_height; }
        // Re-calculates the value but does not store it
        std::uint32_t get_row_num_width() const;
    };
}

#endif
