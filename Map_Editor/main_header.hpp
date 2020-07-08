
/*---------------------------------------------------------------------------------------------------
    * Copyright © 2020 Dragun Konstantin. All rights reserved.
    * Licensed under the MIT License. See License.txt in the project root for license information.
 ----------------------------------------------------------------------------------------------------*/



/** 
    \file
    \brief The header file which unites any other useful headers.

    * This header contains declarations of all the classes and structures that are used in
    * the program. 


**/



#include <stdlib.h>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


#if !defined( _MAIN_HEADER_H)
#define _MAIN_HEADER_H




/**
    * \brief This class allows you to use textures in the program.
    * \version 1.0
    * \date 2, July, 2020
    * \warning Many operation have asymptotics O (log n) as it uses a std::map.
    * 
    * This class has a enum with possibble names of textures. 
    * It also has a std::map which contains pairs ID-number - unique_ptr to the texture.
**/

class Texture_Manager
{

public:
    
    /// Possible names-identificators of pictures.
    enum ID {
                logo, castle, map,
                grass, forest_1,
                texture_number          ///< Number of different texture names
            };

private:

    /** 
        * \details Texture_Map is a std::map object. It is used to find the necessary texture quite easily.
        * Its key is an enum ID object, its value is a unique pointer to the texture.
        * A unique pointer is used to manage carefully with "heavy" objects - textures.
    **/
    std::map <Texture_Manager::ID, std::unique_ptr <sf::Texture> > Texture_Map; 

public:

    /**
        * Loads an image, so it can be used further in the program.
        * 
        \param [in] id The identifying number of the image. Should be in enum ID.
        \param [in] file_name The name and the path to the image.
        \throw std::runtime_error If there is no file with such name in the folder.
    **/
    void load (Texture_Manager::ID id, const char* file_name);


    /**
        * Gets a reference to the texture with this ID.
        * 
        * \param [in] id The identifying number of the image. Should be in enum ID.
        * \return reference to the texture.
    **/
    sf::Texture& get_texture (Texture_Manager::ID id) const noexcept;
};


/** 
     * This is an object of class Texture_Manager which contains all
     *  the necessary textures for the program.
**/
extern Texture_Manager textures;




/**
 * \brief This class allows to manage music files:
 * play, pause, and stop them.
 * 
 * \warning Supported by SFML formates are: OGG, WAV, and AIFF. 
 * 
 * This class manages only background themes. Sounds are not supported by this class.
 * Music files are not loaded to the program memory. They are opened and played right from 
 * the place, written in the file path.
**/
class Music_Manager
{

public:

/// Possible name identificators of music files.
    enum ID
    {
        main_theme,
        forest
    };

/// The class constructor. It connects ID and file names.
    Music_Manager (void) noexcept;


/**
 * \brief Plays the music file.
 * \param [in] music_ID Identificator of a music file.
 * 
 * \throw If it is not possible to play a music file with
 * such an ID, a std::runtime_error exception is thrown.
 **/
    void play (const Music_Manager::ID music_ID);


/**
 * \brief Stops the music file if it was playing. Otherwise does nothing.
 **/
    void stop (void) noexcept;


/**
 * \brief Change the state of a music track.
 * 
 * \param [in] state New state of the music track. true for play, false for pause.
**/
    void set_paused (bool state) noexcept;

private:

    std::map <ID, std::string> storage;             ///< The map to connect ID with music file names.
    sf::Music music;                                ///< The object of SFML class, that allows to manage music.
    float volume;                                   ///< Volume of a playing music track.

};



class Abstract_Field : public sf::Sprite
{

public:
    Abstract_Field (void) noexcept {};
    Abstract_Field (float x, float y) noexcept { this->setPosition (x, y);}
    Abstract_Field (float x, float y, Texture_Manager::ID id) noexcept : Abstract_Field (x, y)
    {
        this->setTexture (textures.get_texture (id));
    }

    void set_texture (Texture_Manager::ID id) noexcept;

private:
    bool is_transparent = 0;

public:

    void set_transparent (bool transparent) noexcept;
    bool get_transparent (void) const noexcept;

    virtual int handle_Mouse_Pressed (float x, float y) const noexcept;
    virtual bool is_on_Mouse (float x, float y) const noexcept;
    virtual void render (sf::RenderWindow& window) const noexcept;
};






class Manager : public Abstract_Field 
{
public:

    Manager (void) noexcept
    : Abstract_Field () {}

    Manager (int number) noexcept
    : Abstract_Field () {array.reserve (number);}

    Manager (float x, float y) noexcept
    : Abstract_Field (x, y) {}

    Manager (float x, float y, Texture_Manager::ID id) noexcept
    : Abstract_Field (x, y, id) {}

    std::vector <Abstract_Field*> array;

    virtual int handle_Mouse_Pressed (float x, float y) const noexcept override;
    //virtual bool is_on_Mouse (float x, float y) override;
    virtual void render (sf::RenderWindow& window) const noexcept override;
    void insert (Abstract_Field* new_field) noexcept;

    ~Manager () {}
};






class Map_Net : public Abstract_Field
{
public:

    size_t texture_size = 32;
    size_t left_edge    = 30;
    size_t upper_edge   = 30;
    size_t width        = 60;
    size_t height       = 60;

    Abstract_Field** map_net;
    
    Map_Net (void);

    void set_settings (size_t left_edge, size_t upper_edge, size_t width, size_t height) noexcept;

    virtual int handle_Mouse_Pressed (float x, float y) const noexcept override;
    virtual bool is_on_Mouse (float x, float y) const noexcept override;
    virtual void render (sf::RenderWindow& window) const noexcept override;

    ~Map_Net ();

private:

    void allocate_memory (void) noexcept;
    void release_memory  (void) noexcept;
};






class Game
{
public:
    Game (void);
    Game (int width, int height);
    void construct (void) noexcept;

    Map_Net texture_map;

    sf::Vector2f offset;
    sf::Vector2i scale_offset;
    double scale = 1;

    sf::RenderWindow window;
    sf::Vector2f window_size;
    sf::View view;

    Manager manager;
    Music_Manager music;


    void run (void);
    void render (void);
    void update (void);
    void load_map (void);
    void handle_Events (void);
    void load_textures (void);
    void preprocessing (void);
    void make_default_map (void) noexcept;

    /**
     * Handles the map zooming, caused by a mouse wheel or a touchpad.
     * 
     * \param [in] delta The change of mouse state. As rool is equal to 1 or -1,
     * although some accurate mice may have a float value.
     * 
    **/

    void handle_zoom (float delta) noexcept;



    void handle_scrolling (float x, float y) noexcept;
    sf::Vector2i calculate_coordinates (void) const noexcept;

};



#endif                      // _MAIN_HEADER_H