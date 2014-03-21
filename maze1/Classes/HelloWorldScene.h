#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "/usr/mobile-game-engine/cocos2d-x/external/Box2D/Box2D.h"
#include "SimpleAudioEngine.h"
#include "cocoa/CCGeometry.h" 
#include "cocos-ext.h"

USING_NS_CC;
using namespace extension;
#define PTM_RATIO 32
class HelloWorld : public cocos2d::CCLayerColor{
public:
    ~HelloWorld();
    HelloWorld();
    // returns a Scene that contains the HelloWorld as the only child
    static cocos2d::CCScene* scene();

//    virtual void draw();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
    void didAccelerate(cocos2d::CCAcceleration* pAccelerationValue);    
	//
    virtual void update(float dt);    
	void addMaze();
	void addMazeEdge(int edgeType ,float length,CCPoint centerPosition);
	void changeGravityDirection();
	void addExitEdge(int edgeType,float length,CCPoint centerPosition);
	void touchDragExit(CCObject* pSender, CCControlEvent event);  

private:
	float changeSpeed;
    b2World* _world;
    b2Body *_groundBody;
    b2Fixture *_bottomFixture;
    b2Fixture *_ballFixture;
    b2Body *_ballBody;
    b2Fixture *_paddleFixture;
    b2MouseJoint *_mouseJoint;
	CCSprite *_ball;
	float gravityX;
	float gravityY;
	CCPoint _exitPosition;
	b2Body *_exitBody;
	CCSprite *_exit;
//	CCPoint playerVelocity;	
    
    b2ContactListener *_contactListener;
};

#endif // __HELLOWORLD_SCENE_H__
