
// Copyright © 2020 Dragun Konstantin. All rights reserved.

#include "main_header.hpp"


int main()
{
    Game game;
    game.run();

    Abstract_Field test (100, 100, Texture_Manager::logo);
   //test.setTexture (textures.get_texture (Texture_Manager::logo));




    //Button_Overflow btn4 (400, 400, 200, 200, sf::Color::Magenta);
    game.manager.array.push_back (&test);


    return 0;
}