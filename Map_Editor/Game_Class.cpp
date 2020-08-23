
/*---------------------------------------------------------------------------------------------------
    * Copyright © 2020 Dragun Konstantin. All rights reserved.
    * Licensed under the MIT License. See License.txt in the project root for license information.
 ----------------------------------------------------------------------------------------------------*/

#include "main_header.hpp"
#include <cmath>


void Game::construct (void) noexcept
{
    manager = *(new Manager ());
    window_size.x = window.getSize().x;
    window_size.y = window.getSize().y;
    //view.setViewport (sf::FloatRect (0.2, 0.2, 0.75, 0.75));
    view.reset (sf::FloatRect (0, 0, window_size.x, window_size.y));

    background.setScale (window_size);
    //background.set_texture (Texture_Manager::ID::forest_1);

    try 
    {
        music.play (Music_Manager::ID::forest);
    }
    catch (std::runtime_error& error)
    {
        printf ("Attention! %s\n\n", error.what());
    }
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

    manager.insert (&texture_map, Manager::middle_layer);

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
    try
    {
        textures.load (Texture_Manager::logo, "Objects/Pictures/logo.png");
        textures.load (Texture_Manager::castle, "Objects/Pictures/Castles/cas1.png");
        textures.load (Texture_Manager::map, "Objects/Pictures/Maps/map1.png");
        textures.load (Texture_Manager::grass, "../Textures/Forest/grass.png");
        textures.load (Texture_Manager::forest_1, "../Textures/Forest/forest_1.png");
        textures.load (Texture_Manager::forest_2, "../Textures/Forest/forest_2.png");
        textures.load (Texture_Manager::forest_3, "../Textures/Forest/forest_3.png");
        textures.load (Texture_Manager::sea, "../Textures/Sea/sea.png");
        textures.load (Texture_Manager::sand, "../Textures/Sea/sand.png");
    }
    catch (std::runtime_error error)
    {
        printf ("Attention!!!\n%s.\nCheck whether the file is in the folder!\n\n\n", error.what());
    }
}

/*
void Game::load_map (void)
{
    Manager* game_map = new Manager (0, 0, Texture_Manager::map);

    Abstract_Field* castle = new Abstract_Field (100, 200, Texture_Manager::castle);

    game_map->insert (castle);

    manager.insert (game_map);

}
*/



void Game::preprocessing (void)
{
    load_textures();
    make_default_map();
    Music_Manager music;
    music.play (Music_Manager::ID::main_theme);
}

void Game::render (void)
{
    window.setView (view);
    this->window.clear (sf::Color (255, 255, 255, 255));

    window.draw (background);

    this->manager.render (this->window);
    
    this->window.display();
}
    
static bool mouse_pressed = false;

void Game::handle_Events (void)
{
    sf::Event event;
    while (this->window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::MouseMoved:
                if (!mouse_pressed)
                    break;

            case sf::Event::MouseButtonPressed:
            {
                mouse_pressed = true;
                sf::Vector2i real_mouse = calculate_coordinates();
                this->manager.handle_Mouse_Pressed (real_mouse.x, real_mouse.y);
            }
            break;

            case sf::Event::MouseButtonReleased:
                mouse_pressed = false;
            break;

            case sf::Event::MouseWheelScrolled:
                handle_zoom (event.mouseWheelScroll.delta);
            break;

            case sf::Event::KeyPressed:
                handle_key (event);
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
    constexpr float min_scale = 0.05;                                               ///< The minimal scale of the map.
    constexpr float max_scale = 2;                                                  ///< The maximal scale of the map.
    constexpr float grade = 0.1;                                                    ///< The "step", shows how fast the map is scaled.

    if ((scale <= min_scale && delta < 0) || (scale >= max_scale && delta > 0))     ///< If the scale is at the limit,
        return;                                                                     ///< do not do anything.

    double zoom_delta = grade * abs (delta);                                        ///< Shows at what value we zoom the map according to the grade and the delta.
    double final_zoom_delta = 1 + zoom_delta;                                       ///< The value to multiply the high and the width of the screen.
    double is_bigger = 1;                                                           ///< The flag that shows whether the picture becomes "bigger".


    if (delta < 0)                                                                  ///< Handle the case when the picture becomes "smaller"
    {    
        final_zoom_delta = 1 / final_zoom_delta;                                    ///< The value to multiply the screen is reversed.
        zoom_delta *= final_zoom_delta;                                             ///< Update the zoom delta.
        is_bigger = -1;                                                             ///< The flag shows that picture becomes "smaller".
    }

    double offset_x = is_bigger * window_size.x * zoom_delta / 2;                   ///< New offsets which are caused by zooming.
    double offset_y = is_bigger * window_size.y * zoom_delta / 2;                   ///< They are equal to the half of the difference between 
                                                                                    ///< the old size and the new one.                                                 
    offset -= sf::Vector2f (offset_x, offset_y);                                    ///< Update the general offset.

    scale *= final_zoom_delta;                                                      ///< Update the general scale.
    
    window_size.x *= final_zoom_delta;                                              ///< Update the variable that reflect the window width.
    window_size.y *= final_zoom_delta;                                              ///< Update the variable that reflect the window height.
    view.zoom (final_zoom_delta);                                                   ///< Zoom the map.
    
    if (delta > 0)                                                                  ///< Handle the case when the picture becomes "bigger".
    {
        sf::Vector2f temp_offset = {};                                              ///< Make a temporary variable for offset.
        
        if (offset.x < 0)                                                           ///< If the field is out of the frame while scaling,
            temp_offset.x = - offset.x;                                             ///< make the offset equal to the current offset, but positive.

        if (offset.y < 0)                                                           ///< If the field is out of the frame while scaling,
            temp_offset.y = - offset.y;                                             ///< make the offset equal to the current offset, but positive.

        view.move (temp_offset);                                                    ///< Move the camera to the right place.
        offset += temp_offset;                                                      ///< Update the general offset.
    }
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

void Game::handle_key (sf::Event& event) noexcept
{
    switch (event.key.code)
    {
        case sf::Keyboard::Num1:
            current_style = Texture_Manager::ID::grass;
        break;

        case sf::Keyboard::Num2:
            current_style = Texture_Manager::ID::forest_1;
        break;

        case sf::Keyboard::Num3:
            current_style = Texture_Manager::ID::forest_2;
        break;

        case sf::Keyboard::Num4:
            current_style = Texture_Manager::ID::forest_3;
        break;

        case sf::Keyboard::Num5:
            current_style = Texture_Manager::ID::sea;
        break;

        case sf::Keyboard::Num6:
            current_style = Texture_Manager::ID::sand;
        break;

        default:
        break;
    }
}