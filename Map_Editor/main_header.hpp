#include <stdlib.h>
#include <stdio.h>
#include <SFML/Graphics.hpp>


#ifndef _MAIN_HEADER_H
#define _MAIN_HEADER_H 1


class Texture_Manager
{

public:
    enum ID {
                logo, castle, map,
                grass, forest_1,
                texture_number
            };

private:

    std::map <Texture_Manager::ID, std::unique_ptr <sf::Texture> > Texture_Map; 

public:

    void load (Texture_Manager::ID id, const char* file_name);
    sf::Texture& get_texture (Texture_Manager::ID id) const noexcept;
};

extern Texture_Manager textures;





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

    virtual int handle_Mouse_Pressed (float x, float y) const noexcept {return 0;};
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

    sf::Vector2i offset;
    sf::Vector2i scale_offset;
    double scale = 1;

    sf::RenderWindow window;
    sf::Vector2f window_size;
    sf::View view;
    Manager manager;


    void run (void);
    void render (void);
    void update (void);
    void load_map (void);
    void handle_Events (void);
    void load_textures (void);
    void preprocessing (void);
    void make_default_map (void) noexcept;
    void handle_zoom (float delta) noexcept;
    void handle_scrolling (float x, float y) noexcept;
    sf::Vector2i calculate_coordinates (void) const noexcept;

};



#endif