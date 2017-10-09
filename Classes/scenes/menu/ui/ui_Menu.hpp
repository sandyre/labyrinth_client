//
//  ui_Menu.hpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 14/09/2017.
//
//

#ifndef ui_Menu_hpp
#define ui_Menu_hpp

#include "scenes/menu/ui/LoginView.hpp"
#include "scenes/menu/ui/MenuView.hpp"
#include "scenes/menu/ui/WelcomeView.hpp"

#include <cocos2d.h>
#include <ui/CocosGUI.h>


namespace labyrinth {
namespace ui
{

    class Menu
        : public cocos2d::ui::Layout
    {
    public:
        enum class View
        {
            Welcome,
            Login,
            Main
        };

    public:
        Menu();

        virtual bool init() override;

        void SwitchTo(View view);

        impl::WelcomeView * GetWelcomeView()
        { return _welcomeView; }

        impl::LoginView * GetLoginView()
        { return _loginView; }

        impl::MenuView * GetMenuView()
        { return _menuView; }

    private:
        View                    _currentView;

        impl::WelcomeView *     _welcomeView;
        impl::LoginView *       _loginView;
        impl::MenuView *        _menuView;
    };

}}

#endif /* ui_Menu_hpp */
