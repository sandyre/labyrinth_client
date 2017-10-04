//
//  ui_GameSearch.hpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 13/09/2017.
//
//

#ifndef ui_GameSearch_hpp
#define ui_GameSearch_hpp

#include "scenes/gamesearch/ui/LobbyView.hpp"
#include "scenes/gamesearch/ui/SearchView.hpp"

#include <cocos2d.h>
#include <ui/CocosGUI.h>


namespace labyrinth {
namespace ui
{

    class GameSearch
        : public cocos2d::ui::Layout
    {
    public:
        enum View
        {
            Search,
            Lobby
        };

    public:
        GameSearch();

        void SwitchTo(View view);

        impl::SearchView * GetSearchView()
        { return _searchView; }

        impl::LobbyView * GetLobbyView()
        { return _lobbyView; }

    private:
        View                  _currentView;

        impl::SearchView *    _searchView;
        impl::LobbyView *     _lobbyView;
    };

}}

#endif /* ui_GameSearch_hpp */
