//
//  LoadingView.hpp
//  labyrinth
//
//  Created by Alexandr Borzykh on 10/10/2017.
//

#ifndef LoadingView_hpp
#define LoadingView_hpp

#include <cocos2d.h>
#include <ui/CocosGUI.h>


namespace labyrinth {
namespace ui {
namespace impl
{

        class LoadingView
            : public cocos2d::ui::Layout
        {
        public:
            virtual bool init() override;

            void SetStatus(const std::string& status);

        private:
            cocos2d::ui::Text *     _statusText;
        };

}}}

#endif /* LoadingView_hpp */
