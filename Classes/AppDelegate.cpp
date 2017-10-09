#include "AppDelegate.h"

#include "gamescene.hpp"
#include "gameconfig.hpp"
#include "resourcemanager.hpp"
#include "scenes/menu/MenuScene.hpp"

#include "toolkit/make_autorelease.hpp"

USING_NS_CC;


static cocos2d::Size iphone5Resolution = cocos2d::Size(540, 960);
static cocos2d::Size iphone7Resolution = cocos2d::Size(750, 1334);
static cocos2d::Size iphone7PResolution = cocos2d::Size(1242, 2208);

static cocos2d::Size designResolutionSize = cocos2d::Size(576, 1024);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
	auto screensize = cocos2d::Size(405, 720);
//    auto screensize = designResolutionSize;
    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("labyrinth", cocos2d::Rect(0, 0, screensize.width, screensize.height));
#else
        glview = GLViewImpl::create("labyrinth");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::EXACT_FIT);
    
        // Load game configuration
    GameConfiguration::Instance().Load();
    
        // Load game resources
    ResourceManager::Instance().PreloadTextures();
    ResourceManager::Instance().PreloadSounds();
    
    NetSystem::Instance().CreateChannel("masterserver",
                                        Poco::Net::SocketAddress(GameConfiguration::Instance().GetServerAddress()));

    register_all_packages();
    
    // create a scene. it's an autorelease object
    auto scene = make_autorelease<labyrinth::MenuScene>();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be paused
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
