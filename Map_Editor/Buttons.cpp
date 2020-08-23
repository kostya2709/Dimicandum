
/*---------------------------------------------------------------------------------------------------
    * Copyright © 2020 Dragun Konstantin. All rights reserved.
    * Licensed under the MIT License. See License.txt in the project root for license information.
 ----------------------------------------------------------------------------------------------------*/

#include "main_header.hpp"

Texture_Manager::ID current_style = Texture_Manager::ID::forest_1;

void Abstract_Field::set_transparent (bool transparent) noexcept
{
    this->is_transparent = transparent;
}

bool Abstract_Field::get_transparent (void) const noexcept
{
    return this->is_transparent;
}

void Abstract_Field::set_texture (Texture_Manager::ID id) noexcept
{
    this->setTexture (textures.get_texture (id));
}


bool Abstract_Field::is_on_Mouse (float x, float y) const noexcept
{
    sf::FloatRect bound = this->getGlobalBounds();
    
    if (bound.contains (x, y))
        return 1;

    return 0;
}

void Abstract_Field::render (sf::RenderWindow& window) const noexcept
{
    if (!this->get_transparent())
        window.draw (*this);
}

int Abstract_Field::handle_Mouse_Pressed (float x, float y) const noexcept
{
    printf ("Coordintes %g, %g are inside of the field\n", x, y);
    return 0;
}





Manager::Manager (int number) noexcept : Abstract_Field () 
{
    for (int i = 0; i < count_layer; ++i)
        array [i].reserve (number);
}

void Manager::insert (Abstract_Field* new_field, Manager::layer layer) noexcept
{
    this->array [layer].push_back (new_field);
}

int Manager::handle_Mouse_Pressed (float x, float y) const noexcept
{
    int found_layer = -1;

    for (int layer = count_layer - 1; layer >= 0; --layer)
    {
        int num = this->array [layer].size();

        for (int i = 0; i < num; ++i)
            if (this->array [layer][i]->is_on_Mouse (x, y))
            {
                this->array [layer][i]->handle_Mouse_Pressed (x, y);
                found_layer = layer;
                break;
            }
        if (found_layer != -1)
            break;
    }
    
    //if (found == -1)
        //printf ("YOU MISSED!\n\n");

    return found_layer;
}

void Manager::render (sf::RenderWindow& window) const noexcept
{
    if (!this->get_transparent())
            window.draw (*this);
    
    for (int layer = 0; layer < count_layer; ++layer)
    {
        int size = this->array [layer].size();
        
        for (int counter = 0; counter < size; ++counter)
            this->array [layer][counter]->render(window);
    }
}








void Map_Net::allocate_memory (void) noexcept
{
    map_net = new Abstract_Field* [height];
    for (size_t i = 0; i < height; ++i)
        map_net [i] = new Abstract_Field [width]; 
}


Map_Net::Map_Net (void) 
{
    allocate_memory();
}

void Map_Net::set_settings (size_t left_edge, size_t upper_edge, size_t width, size_t height) noexcept
{
    release_memory();

    this->left_edge = left_edge;
    this->upper_edge = upper_edge;
    this->width = width;
    this->height = height;

    allocate_memory();
}

void Map_Net::render (sf::RenderWindow& window) const noexcept
{
    for (size_t i = 0; i < height; ++i)
        for (size_t j = 0; j < width; ++j)
            map_net[i][j].render (window);
}

bool Map_Net::is_on_Mouse (float x, float y) const noexcept
{
    if (x >= left_edge  && x <= left_edge  + width  * texture_size
     && y >= upper_edge && y <= upper_edge + height * texture_size) 
            return 1;

    return 0;
}

int Map_Net::handle_Mouse_Pressed (float x, float y) const noexcept
{
    size_t texture_x = ((size_t)x - left_edge)  / texture_size;
    size_t texture_y = ((size_t)y - upper_edge) / texture_size;

    map_net[texture_y][texture_x].set_texture (current_style);

    return 0;
}

void Map_Net::release_memory (void) noexcept
{
    for (size_t i = 0; i < height; ++i)
        delete [] map_net[i];

    delete [] map_net;
}

Map_Net::~Map_Net (void)
{
    release_memory();
}
