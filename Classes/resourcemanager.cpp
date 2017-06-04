//
//  resourcemanager.cpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 25.02.17.
//
//

#include "resourcemanager.hpp"

#include "resources.hpp"

#include <SimpleAudioEngine.h>

ResourceManager::ResourceManager()
{
    
}

ResourceManager::~ResourceManager()
{
    
}

void
ResourceManager::PreloadTextures()
{
    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/graphics/graphics.plist",
                                                                      "res/graphics/graphics.png");
}

void
ResourceManager::UnloadTextures()
{
    
}

void
ResourceManager::PreloadSounds()
{
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    for(auto& sound : SoundEffects)
    {
        audio->preloadEffect(sound.c_str());
    }
    for(auto& music : SoundMusic)
    {
        audio->preloadBackgroundMusic(music.c_str());
    }
}

void
ResourceManager::UnloadSounds()
{
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    for(auto& sound : SoundEffects)
    {
        audio->unloadEffect(sound.c_str());
    }
    for(auto& sound : SoundMusic)
    {
        audio->unloadEffect(sound.c_str());
    }
}
