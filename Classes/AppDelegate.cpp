#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "StateLoading.h"
#include "AndroidInterface.h"

using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate()
{

}
	
AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(640,1136,kResolutionShowAll);

    // turn on display FPS
   // pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

	CCScene* pScene = StateLoading::scene();
    // run
    pDirector->runWithScene(pScene);

	pDirector->getKeypadDispatcher()->addDelegate(KeyDelegate::getInstance());

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}





void KeyDelegate::keyBackClicked()
{
	exit_game();
}

void KeyDelegate::keyMenuClicked()
{
	
}

KeyDelegate* KeyDelegate::getInstance()
{
	static KeyDelegate instance;
	return &instance;
}

KeyDelegate::KeyDelegate()
{

}

KeyDelegate::~KeyDelegate()
{

}
