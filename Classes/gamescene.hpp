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
#include "gamelogic/player.hpp"
#include "gamelogic/monster.hpp"
#include "gamelogic/item.hpp"
#include "gamelogic/construction.hpp"
#include "gamehud.hpp"
#include "swamp_combo.hpp"
#include "duel_mode.hpp"

#include "gsnet_generated.h"

class GameScene : public cocos2d::Layer
{
public:
    enum State
    {
        WALKING, // walking state
        DUEL,   // battle
        DROWNING
    };
public:
    GameScene();
    ~GameScene();
    virtual void onExit(); // destructor
    
    virtual bool init();
    virtual void update(float);
    
        // for builder (pregamescene)
    void    GenerateMap(const GameMap::Configuration& conf);
    std::vector<Player*>&       GetPlayersList();
    
protected:
    Player *                    GetPlayerByUID(uint32_t);
    Item *                      GetItemByUID(uint16_t);
    Monster *                   GetMonsterByUID(uint16_t);
    
    void    SendEventAndClear();
private:
    GameScene::State            m_eState = State::WALKING;
    GameMap                     m_oGameMap;
    GameHUD *                   m_pGameHUD;
    cocos2d::Layer *            m_pPlayersLayer;
    cocos2d::Layer *            m_pItemsLayer;
    
    Player *                    m_pLocalPlayer;
    std::vector<Player*>        m_aPlayers;
    std::vector<Item*>          m_aItems;
    std::vector<Construction*>  m_aConstrs;
    std::vector<Monster*>       m_aMonsters;
    
    SwampCombo *                m_pSwampCombo;
    DuelMode *                  m_pDuelMode;
        // used to send events
    flatbuffers::FlatBufferBuilder builder;
    uint16_t nSuccCount = 0;
};

#endif /* gamescene_hpp */
