//
//  SearchView.hpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 13/09/2017.
//
//

#ifndef SearchView_hpp
#define SearchView_hpp

#include <cocos2d.h>
#include <ui/CocosGUI.h>


namespace labyrinth {
namespace ui {
namespace impl
{

    class SearchView
        : public cocos2d::ui::Layout
    {
    public:
        SearchView();

        void SetStatus(const std::string& status);

    private:
        cocos2d::ui::Text *     _statusText;
    };

}}}

#endif /* SearchView_hpp */
