//
//  MenuScene.hpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 14/09/2017.
//
//

#ifndef MenuScene_hpp
#define MenuScene_hpp

#include "scenes/menu/ui/ui_Menu.hpp"

#include "toolkit/UdpSocket.hpp"


namespace labyrinth
{

    class MenuScene
        : public cocos2d::Scene
    {
        using SignalConnection = boost::signals2::scoped_connection;

    public:
        virtual bool init() override;

    private:
        virtual void update(float delta) override;

        void StartClickHandler();

        void LoginClickHandler();

        void RegisterClickHandler() { }

        void SearchClickHandler();

        void CollectionClickHandler() { }

        void ShopClickHandler() { }

        void SettingsClickHandler() { }

    private:
        ui::Menu *          _ui;

        SignalConnection    _onStartClickConnection;
        SignalConnection    _onLoginClickConnection;
        SignalConnection    _onRegisterClickConnection;

        SignalConnection    _onSearchClickConnection;
        SignalConnection    _onCollectionClickConnection;
        SignalConnection    _onShopClickConnection;
        SignalConnection    _onSettingsClickConnection;
    };

}

#endif /* MenuScene_hpp */
