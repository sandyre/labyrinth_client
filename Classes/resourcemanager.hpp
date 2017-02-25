//
//  resourcemanager.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 25.02.17.
//
//

#ifndef resourcemanager_hpp
#define resourcemanager_hpp

#include <cocos2d.h>

class ResourceManager
{
public:
    static ResourceManager& Instance()
    {
        static ResourceManager rm;
        return rm;
    }
    
    void    PreloadTextures();
    void    UnloadTextures();
    void    PreloadSounds();
    void    UnloadSounds();
private:
    ResourceManager();
    ~ResourceManager();
    
    ResourceManager(ResourceManager const&) = delete;
    ResourceManager& operator=(ResourceManager const&) = delete;
    
private:
    
};

#endif /* resourcemanager_hpp */
