//
//  ui_GameSearch.hpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 13/09/2017.
//
//

#ifndef ui_GameSearch_hpp
#define ui_GameSearch_hpp

#include "scenes/gamesearch/ui/LoadingView.hpp"
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
        enum class View
        {
            Search,
            Lobby,
            Loading
        };

    public:
        GameSearch();

        virtual bool init() override;

        void SwitchTo(View view);

        impl::SearchView * GetSearchView() { return _searchView; }
        impl::LobbyView * GetLobbyView() { return _lobbyView; }
        impl::LoadingView * GetLoadingView() { return _loadingView; }

    private:
        View                  _currentView;

        impl::SearchView *    _searchView;
        impl::LobbyView *     _lobbyView;
        impl::LoadingView *   _loadingView;
    };

}}

#endif /* ui_GameSearch_hpp */
