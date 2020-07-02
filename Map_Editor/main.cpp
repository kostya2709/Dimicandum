#include "main_header.hpp"

/**
 * This is a map editor for the game "Dimicandum".
 * Here you can create your own maps.
 **/ 

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