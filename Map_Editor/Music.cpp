
/*---------------------------------------------------------------------------------------------------
    * Copyright © 2020 Dragun Konstantin. All rights reserved.
    * Licensed under the MIT License. See License.txt in the project root for license information.
 ----------------------------------------------------------------------------------------------------*/

#include "main_header.hpp"






Music_Manager::Music_Manager (void) noexcept : volume (100.f)
{
    storage [main_theme] = "../Music/Time.ogg";                                             ///< Add a new ID.
    storage [forest]     = "../Music/Forest.ogg";                                           ///< Add a new ID.
}



void Music_Manager::play (const Music_Manager::ID music_ID)
{
    std::string file_name = storage [music_ID];                                             ///< Find file name according to ID.

    if (!music.openFromFile (file_name))                                                    ///< Try to open the file.
        throw std::runtime_error ("Failed to load a music file \"" + file_name + "\"!\n");  ///< Throw an exception if it is impossible to open the file.
    
    music.setVolume (volume);                                                               ///< Set volume to the current volume.
    music.setLoop (true);                                                                   ///< Make the music play again after it finishes.
    music.play();                                                                           ///< Make the music play.
}

void Music_Manager::stop (void) noexcept
{
    music.stop();                                                                           ///< Stop the music.
}

void Music_Manager::set_paused (bool state) noexcept
{
    if (state)                                                                              ///< If state is true,
        music.pause();                                                                      ///< stop the music.
    else                                                                                    ///< Otherwise,
        music.play();                                                                       ///< play the music.
}