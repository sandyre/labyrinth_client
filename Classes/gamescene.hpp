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
#include "ui/ui_gamescene.hpp"
#include "duel_mode.hpp"

#include "gsnet_generated.h"

//class BattleInputController
//{
//public:
//    enum InputStatus
//    {
//        NEED_RESET,
//        NEED_NEXT_CHAR,
//        INPUT_DONE,
//        INPUT_FAILED
//    };
//    struct Action
//    {
//        InputStatus status;
//        size_t current_symbol = 0;
//        std::array<InputMove, 5> sequence;
//    };
//public:
//    BattleInputController();
//    
//    void TakeInput(InputMove move)
//    {
//        for(auto& action : m_aActions)
//        {
//            if(action.status == INPUT_DONE)
//                continue;
//            if(action.status == NEED_NEXT_CHAR)
//            {
//                
//            }
//        }
//    }
//    
//    std::vector<Action> m_aActions;
//};

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
    void    UpdateWorld(float); // updates logic!
    void    UpdateHUD(float);
private:
    GameMap                     m_oGameMap;
    UIGameScene *               m_pUI;
    cocos2d::Layer *            m_pPlayersLayer;
    cocos2d::Layer *            m_pItemsLayer;
    
        // local player pointer
    Hero *                      m_pLocalPlayer;
//    BattleInputController       m_oBIC;
    
    std::vector<Hero*>          m_aPlayers;
    std::vector<Item*>          m_aItems;
    std::vector<Construction*>  m_aConstrs;
    std::vector<Monster*>       m_aMonsters;
    
    DuelMode *                  m_pDuelMode;
    
        // used to send events
    std::queue<std::vector<char>> m_aOutEvents;
    flatbuffers::FlatBufferBuilder builder;
    uint16_t nSuccCount = 0;
};

#endif /* gamescene_hpp */
