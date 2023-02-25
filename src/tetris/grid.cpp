#include "tetris/grid.hpp"


ttr::Grid::Grid(){
}

ttr::Grid::Grid(int width, int height, int line_thickness, int square_size, sf::Color grid_color, sf::RenderWindow* window){
    this->width = width;
    this->height = height;
    this->line_thickness = line_thickness;
    this->square_size = square_size;
    this->grid_color = grid_color;
    this->window = window;
}

void ttr::Grid::set_position(const sf::Vector2f& new_position){
    position = new_position;
}

void ttr::Grid::build(){
    grid_horizontal_lines = std::vector<sf::RectangleShape>(height + 1);
    grid_vertical_lines = std::vector<sf::RectangleShape>(width + 1);

    for(int i = 0; i <= height; i++){
        auto line = &grid_horizontal_lines[i];
        line->setFillColor(grid_color);
        line->setSize(sf::Vector2f(width * square_size, line_thickness));
        line->setPosition(position + sf::Vector2f(0, i * square_size));
    }
    for(int i = 0; i <= width; i++){
        auto line = &grid_vertical_lines[i];
        line->setFillColor(grid_color);
        line->setSize(sf::Vector2f(line_thickness, height * square_size + line_thickness));
        line->setPosition(position + sf::Vector2f(i * square_size, 0));
    }
}

void ttr::Grid::draw(){
    for(sf::RectangleShape& line : grid_horizontal_lines){
        window->draw(line);
    }
    for(sf::RectangleShape& line : grid_vertical_lines){
        window->draw(line);
    }
}

sf::Vector2f ttr::Grid::get_position() const{
    return position;
}

int ttr::Grid::get_width() const{
    return width;
}

int ttr::Grid::get_height() const{
    return height;
}

int ttr::Grid::get_square_size() const{
    return square_size;
}


ttr::Grid ttr::load_grid(const YAML::Node& config, sf::RenderWindow* window){
    auto grid_cfg = config["Grid"];
    sf::Color grid_color(grid_cfg["color"][0].as<int>(), grid_cfg["color"][1].as<int>(), grid_cfg["color"][2].as<int>());
    
    ttr::Grid grid(grid_cfg["width"].as<int>(), grid_cfg["height"].as<int>(),
                     grid_cfg["line_thickness"].as<int>(), grid_cfg["square_size"].as<int>(), grid_color, window);
    
    grid.set_position(sf::Vector2f(grid_cfg["position"][0].as<int>(), grid_cfg["position"][1].as<int>()));
    grid.build();

    return grid;
}


