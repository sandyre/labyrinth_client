//
//  gamescene.hpp
//  labyrinth
//
//  Created by Aleksandr Borzikh on 12.01.17.
//
//

#ifndef gamescene_hpp
#define gamescene_hpp

#include "cocos2d.h"
#include "gamelogic/gamemap.hpp"
#include "gamelogic/units/units_inc.hpp"
#include "gamelogic/item.hpp"
#include "gamelogic/construction.hpp"
#include "gamehud.hpp"
#include "swamp_combo.hpp"
#include "duel_mode.hpp"
#include "LightEffect.h"

#include "gsnet_generated.h"

class GameScene : public cocos2d::Layer
{
public:
    GameScene();
    ~GameScene();
    virtual void onExit(); // destructor
    
    virtual bool init();
    virtual void update(float);
    
        // for builder (pregamescene)
    void    GenerateMap(const GameMap::Configuration& conf);
    std::vector<Hero*>&       GetPlayersList();
    
protected:
    Hero *                      GetPlayerByUID(uint32_t);
    Item *                      GetItemByUID(uint16_t);
    Monster *                   GetMonsterByUID(uint16_t);
    
    void    ApplyInputEvents();
    void    SendOutputEvents();
    void    UpdateView(float);
private:
    GameMap                     m_oGameMap;
    GameHUD *                   m_pGameHUD;
    cocos2d::Layer *            m_pPlayersLayer;
    cocos2d::Layer *            m_pItemsLayer;
    
    Hero *                      m_pLocalPlayer;
    std::vector<Hero*>          m_aPlayers;
    std::vector<Item*>          m_aItems;
    std::vector<Construction*>  m_aConstrs;
    std::vector<Monster*>       m_aMonsters;
    
    SwampCombo *                m_pSwampCombo;
    DuelMode *                  m_pDuelMode;
    
        // used to send events
    std::queue<std::vector<char>> m_aOutEvents;
    flatbuffers::FlatBufferBuilder builder;
    uint16_t nSuccCount = 0;
    
        // do be deleted
    LightEffect *               m_pLight;
};

#endif /* gamescene_hpp */
