
#include "main_header.hpp"

#include <stdexcept>

Texture_Manager textures;

sf::Texture& Texture_Manager::get_texture (Texture_Manager::ID id) const noexcept
{
    return *(this->Texture_Map.find (id)->second);
}



void Texture_Manager::load (Texture_Manager::ID id, const char* file_name)
{
    std::unique_ptr <sf::Texture> unique_texture (new sf::Texture());

    if (!unique_texture->loadFromFile (file_name))
        throw std::runtime_error ("Failed to load file " + std::string (file_name));

    this->Texture_Map.insert (std::make_pair (id, std::move (unique_texture)));
}