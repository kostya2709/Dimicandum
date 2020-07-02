
#include "main_header.hpp"




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









void Manager::insert (Abstract_Field* new_field) noexcept
{
    this->array.push_back (new_field);
}

int Manager::handle_Mouse_Pressed (float x, float y) const noexcept
{
    int num = this->array.size();
    int found = 0;

    for (int i = 0; i < num; ++i)
        if (this->array[i]->is_on_Mouse (x, y))
        {
            this->array[i]->handle_Mouse_Pressed (x, y);
            ++found;
        }
    
    //if (found == 0)
        //printf ("YOU MISSED!\n\n");

    return found;
}

void Manager::render (sf::RenderWindow& window) const noexcept
{
    int size = this->array.size();
    int counter = 0;

    if (!this->get_transparent())
        window.draw (*this);
    
    for (; counter < size; ++counter)
        this->array[counter]->render(window);
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
            map_net[i][j].render(window);
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

    map_net[texture_y][texture_x].set_texture (Texture_Manager::ID::forest_1);

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
