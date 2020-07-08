

/*---------------------------------------------------------------------------------------------------
    * Copyright © 2020 Dragun Konstantin. All rights reserved.
    * Licensed under the MIT License. See License.txt in the project root for license information.
 ----------------------------------------------------------------------------------------------------*/


#include "main_header.hpp"
#include <stdexcept>

Texture_Manager textures;

sf::Texture& Texture_Manager::get_texture (Texture_Manager::ID id) const noexcept
{
    return *(this->Texture_Map.find (id)->second);
}



void Texture_Manager::load (Texture_Manager::ID id, const char* file_name) try
{
    std::unique_ptr <sf::Texture> unique_texture (new sf::Texture());

    if (!unique_texture->loadFromFile (file_name))
        throw std::runtime_error ("Failed to load file " + std::string (file_name));

    this->Texture_Map.insert (std::make_pair (id, std::move (unique_texture)));
}
catch (std::runtime_error error)
{
    printf ("Attention!!!\n%s.\nCheck whether the file is in the folder!\n\n\n", error.what());
}