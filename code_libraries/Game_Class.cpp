
/*---------------------------------------------------------------------------------------------------
    * Copyright © 2020 Dragun Konstantin. All rights reserved.
    * Licensed under the MIT License. See License.txt in the project root for license information.
 ----------------------------------------------------------------------------------------------------*/

#include "main_header.hpp"
#include <cmath>



void Game::construct (void) noexcept
{
    manager = *(new Manager ());
    view.reset (sf::FloatRect (0, 0, window.getSize().x, window.getSize().y)); 
    window_size.x = window.getSize().x;
    window_size.y = window.getSize().y;
}

Game::Game (void) : window (sf::VideoMode::getFullscreenModes()[0], "Map Editor")
{
    construct();
}

Game::Game (int width, int height) : window (sf::VideoMode (width, height), "Map Editor")
{
    construct();
    manager.setPosition (0, 0);
}

void Game::make_default_map (void) noexcept
{

    manager.insert (&texture_map);

    size_t cur_x  = texture_map.left_edge;
    size_t cur_y  = texture_map.upper_edge;


    for (size_t y = 0; y < texture_map.height; ++y)
    {
        for (size_t x = 0; x < texture_map.width; ++x)
        {
            texture_map.map_net[y][x].set_texture (Texture_Manager::grass);
            texture_map.map_net[y][x].setPosition (cur_x, cur_y);
            cur_x += texture_map.texture_size;
        }

        cur_y += texture_map.texture_size;
        cur_x = texture_map.left_edge;
    }
}

void Game::update (void)
{
    //view.move (1, 0);
    //view.zoom (1.000001);
}

void Game::load_textures (void)
{
    textures.load (Texture_Manager::logo, "Objects/Pictures/logo.png");
    textures.load (Texture_Manager::castle, "Objects/Pictures/Castles/cas1.png");
    textures.load (Texture_Manager::map, "Objects/Pictures/Maps/map1.png");
    textures.load (Texture_Manager::grass, "../Textures/Forest/grass.png");
    textures.load (Texture_Manager::forest_1, "../Textures/Forest/forest_1.png");
}


void Game::load_map (void)
{
    Manager* game_map = new Manager (0, 0, Texture_Manager::map);

    Abstract_Field* castle = new Abstract_Field (100, 200, Texture_Manager::castle);

    game_map->insert (castle);

    manager.insert (game_map);

}

void Game::preprocessing (void)
{
    load_textures();
    make_default_map();
}

void Game::render (void)
{
    window.setView (view);
    this->window.clear (sf::Color (255, 255, 255, 255));
    
    this->manager.render (this->window);
    
    this->window.display();
}

void Game::handle_Events (void)
{
    sf::Event event;
    while (this->window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::MouseButtonPressed:
            {
                sf::Vector2i real_mouse = calculate_coordinates();
                this->manager.handle_Mouse_Pressed (real_mouse.x, real_mouse.y);
            }
            break;

            case sf::Event::MouseWheelScrolled:
                handle_zoom (event.mouseWheelScroll.delta);
            break;
            
            case sf::Event::Closed:
                printf ("Do you really want to leave?\n");
                this->window.close();
            break;

            default:
            break;
        }
    }

    handle_scrolling (sf::Mouse::getPosition (window).x, sf::Mouse::getPosition (window).y);
}


sf::Vector2i Game::calculate_coordinates (void) const noexcept
{
    sf::Vector2i mouse = sf::Mouse::getPosition (window);
    printf ("scale %g, real (%g, %g), +offset (%g, %g), *scale (%g, %g)\n", scale,
    mouse.x, mouse.y, mouse.x + offset.x, mouse.y + offset.y, scale * mouse.x + offset.x, scale * mouse.y + offset.y);

    mouse.x = round (scale * mouse.x + offset.x);
    mouse.y = round (scale * mouse.y + offset.y);
    
    return mouse;
}

void Game::handle_scrolling (float x, float y) noexcept
{
    sf::Vector2f offset (0, 0);
    const int frame = 50;
    const int move  = 1;

    //printf ("x %g, y %g\n", x, y);

    if (x <= frame && this->offset.x > 0) 
        offset.x = - move;
    if (x >= window.getSize().x - frame)
        offset.x = move;
    if (y <= frame  && this->offset.y > 0) 
        offset.y = - move;
    if (y >= window.getSize().y - frame)
        offset.y = move;
     
    view.move (offset.x, offset.y);
    this->offset += offset;
}

void Game::handle_zoom (float delta) noexcept
{
    constexpr float min_scale = 0.05;
    constexpr float max_scale = 2;
    constexpr float grade = 0.1;

    if ((scale <= min_scale && delta < 0) || (scale >= max_scale && delta > 0))
        return;

    double result = grade * abs (delta);
    double final_result = 1 + result;
    double is_bigger = 1; 




    if (delta < 0)
    {    
        final_result = 1 / final_result;
        result *= final_result;
        is_bigger = -1;
    }

    double offset_x = is_bigger * window_size.x * result / 2;
    double offset_y = is_bigger * window_size.y * result / 2;

    offset -= sf::Vector2f (offset_x, offset_y);

    scale *= final_result;
    
    window_size.x *= final_result;
    window_size.y *= final_result;
    view.zoom (final_result);
    
    if (delta > 0)
    {
        printf ("%g\n", offset);
    }

    //view.move (offset_x, offset_y);
}

void Game::run (void)
{
    this->preprocessing();
    //this->load_map();

    while (this->window.isOpen())
    {
        this->handle_Events();
        this->update();
        this->render();
    }
}