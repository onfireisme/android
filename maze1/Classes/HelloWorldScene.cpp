#include "HelloWorldScene.h"
#define ArrayLength 1000
#define XLINE 0
#define YLINE 1
#define LINEHEIGHT 4 
#define LINEWIDTH 90
#define HEIGHTSCALE 0.6
HelloWorld::~HelloWorld()
{
    CC_SAFE_DELETE(_world);
    _groundBody = NULL;
//    delete _contactListener;
}
HelloWorld::HelloWorld()
{
	changeSpeed=6.0f;
	gravityX=1;
	gravityY=1;
    setTouchEnabled( true );
    
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
	// Define the gravity vector.
	b2Vec2 gravity;
	gravity.Set(0.0f, -0.0f);
	
	// Construct a world object, which will hold and simulate the rigid bodies.
	_world = new b2World(gravity);
	//add the bottom
	CCPoint bottomeCenterPointer(winSize.height*0.5f,LINEHEIGHT*0.5f);
	addMazeEdge(XLINE,winSize.height,bottomeCenterPointer);
	//add the top
	CCPoint topCenterPointer(winSize.height*0.5f,(winSize.height)-LINEHEIGHT*0.5f);
	addMazeEdge(XLINE,winSize.height,topCenterPointer);
	//add the left
	CCPoint leftCenterPointer(LINEHEIGHT*0.5f,winSize.height*0.5f);
	addMazeEdge(YLINE,winSize.height,leftCenterPointer);
	//add the right
	CCPoint rightCenterPointer((winSize.height)-LINEHEIGHT*0.5f,winSize.height*0.4f);
	addMazeEdge(YLINE,winSize.height*0.8f,rightCenterPointer);
	//add maze 
	addMaze();
	_exitPosition.x=winSize.height-20*0.5f;
	_exitPosition.y=winSize.height*0.9f;
	addExitEdge(YLINE,winSize.height*0.2f,_exitPosition);
	/*
	 *the hero part
	 */
    // Create sprite and add it to the layer
	_ball= CCSprite::create("ball.png", 
			CCRectMake(0, 0, 150, 150) );
	_ball->setScale(0.15f);
    _ball->setPosition(ccp(26.0f, 26.0f));
//    _ball->setTag(1);
    this->addChild(_ball);
	_ball->retain();
    
    // ball defination
    b2BodyDef ballBodyDef;
    ballBodyDef.type = b2_dynamicBody;
    ballBodyDef.position.Set(26.0f/PTM_RATIO, 26.0f/PTM_RATIO);
    ballBodyDef.userData = _ball;
    
    _ballBody = _world->CreateBody(&ballBodyDef);
    
    // Create circle shape
	b2PolygonShape ballBox;
	ballBox.SetAsBox(26.0f*0.31f/PTM_RATIO,26.0f*0.3f/PTM_RATIO);
    b2CircleShape circle;
    circle.m_radius = 16.0f/PTM_RATIO;
    
    // Create shape definition and add body
    b2FixtureDef ballShapeDef;
    ballShapeDef.shape = &circle;
    ballShapeDef.density = 4.0f;
    ballShapeDef.friction = 0.9f;
    ballShapeDef.restitution = 0.3f;
    _ballBody->CreateFixture(&ballShapeDef);
//    _ballFixture = _ballBody->CreateFixture(&ballShapeDef);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(
			"background-music-aac.caf", true);
}
CCScene* HelloWorld::scene()
{
    CCScene *scene = NULL;
    // 'scene' is an autorelease object
    scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init(){
	//1. super init first
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	if ( !CCLayerColor::initWithColor( ccc4(255,255,255,255) ) ){
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
	//open the sensitive of the gravity
	setAccelerometerEnabled(true);
	//se
    this->schedule(schedule_selector(HelloWorld::update));
	this->schedule( schedule_selector(HelloWorld::changeGravityDirection), changeSpeed );
//	_world->SetContactListener(_contactListener);


    /////////////////////////////
    // 3. add your codes below...
	/*
	_ball= CCSprite::create("Ball.jpg", 
			CCRectMake(0, 0, 52, 52) );
    _ball->setPosition(ccp(winSize.width*0.5f, winSize.height*0.5f));
//    _ball->setTag(1);
    this->addChild(_ball);
	_ball->retain();
	*/
	//add some buttons
    CCLabelTTF * label = CCLabelTTF::create("speedUp", "MarkerFelt",25);
    CCControlButton * speedUpButton = CCControlButton ::create(label,CCScale9Sprite::create("speedUp.png"));
//    speedUpButton->setScale(0.3f);  
	speedUpButton->setPosition(ccp(winSize.width-70.0f,winSize.height-30.0f));
//    speedUpButton->setPreferredSize(CCSize(50,30));  
	this->addChild(speedUpButton);
    speedUpButton->addTargetWithActionForControlEvents(this,
			cccontrol_selector(HelloWorld::touchDragExit), CCControlEventTouchDragExit);  


	/*
    CCLabelTTF * downLabel = CCLabelTTF::create("speedDown", "MarkerFelt",25);
    CCControlButton * speedDownButton = CCControlButton ::create(downLabel,CCScale9Sprite::create("speedDown.png"));
//    speedDownButton->setScale(0.3f);  
	speedDownButton->setPosition(ccp(winSize.width-70.0f,winSize.height-80.0f));
//    speedDownButton->setPreferredSize(CCSize(40,30 ));  
	this->addChild(speedDownButton);
	*/

	
    return true;
}
void HelloWorld::touchDragExit(CCObject* pSender, CCControlEvent event){
	if(changeSpeed==3){
		return ;
	}
	changeSpeed-=0.5f;
}
void HelloWorld::menuCloseCallback(CCObject* pSender){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}
void HelloWorld::changeGravityDirection(){
	int randomNumber=rand()%3+1;	
	switch(randomNumber){
		case 1:
			gravityX=-1.0;
			gravityY=1.0;
			break;
		case 2:
			gravityY=-1.0;
			gravityX=1.0;
			break;
		case 3:
			gravityX=-1.0;
			gravityY=-1.0;
			break;
		case 4:
			gravityX=1.0;
			gravityY=1.0;
		default:
			break;
	}
}
void HelloWorld::didAccelerate(cocos2d::CCAcceleration* pAccelerationValue){
	//we set the hero with mass ,then what we need to do is just set the 
	//direction of the gravity,the direction of the gravity will move the hero
	int count=0;
	b2Vec2 gravity(pAccelerationValue->x*9.8f*gravityX,pAccelerationValue->y*9.8f*gravityY);
	_world->SetGravity(gravity);
	/*
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();  
	CCDirector *directorPosition=CCDirector::sharedDirector();
	CCSize ballSize=_ball->getContentSize();
	CCPoint ballCurrentPosition=_ball->getPosition();
	CCPoint ballCurrentUIPosition=directorPosition->convertToUI(ballCurrentPosition);
	CCPoint ballNewUIPoistion;
	ballNewUIPoistion.x=ballCurrentUIPosition.x+pAccelerationValue->x*9.0f;
	ballNewUIPoistion.y=ballCurrentUIPosition.y+pAccelerationValue->y*9.0f;
	CCPoint ballNewGLPosition=directorPosition->convertToGL(ballNewUIPoistion);
	*/
	/*
    //越界判断  
    if (ballNewGLPosition.x >winSize.width-ballSize.width) {  
       ballNewGLPosition.x = winSize.width-ballSize.width;  
    }else if(ballNewGLPosition.x < ballSize.width*0.5f)  
    {  
       ballNewGLPosition.x = ballSize.width*0.5f;  
    }
    if (ballNewGLPosition.y >winSize.height-ballSize.height) {  
       ballNewGLPosition.y = winSize.height-ballSize.height;  
    }else if(ballNewGLPosition.y < ballSize.height*0.5f)  
    {  
       ballNewGLPosition.y = ballSize.height*0.5f;  
    }
	_ball->setPosition(ballNewGLPosition);
	*/
	//b2Vec2 gravity(pAccelerationValue->x * 15, pAccelerationValue->y * 15);//重力感应方向的改变，表示向量力的变化  
    //_world->SetGravity(gravity);//这两句表示判断重力的感应方向的转变
}
void HelloWorld::update(float dt){
	// we did not need to worried about the over the screen event,why?because the box2d has done it
	// for you
	int velocityIterations = 8;    
    int positionIterations = 1;   
    _world->Step(dt, velocityIterations, positionIterations);    
	//we only want to update the situation of the ball,so we dont need to go through all the body
	CCSprite *ballData = (CCSprite*) _ballBody->GetUserData();    
	ballData->setPosition(ccp(_ballBody->GetPosition().x * PTM_RATIO, _ballBody->GetPosition().y * PTM_RATIO));    
	ballData->setRotation(CC_RADIANS_TO_DEGREES(_ballBody->GetAngle()));    
	float maxCollisionDistance=40.0f;
	float distance=ccpDistance(ballData->getPosition(),_exitPosition);
	if(distance<maxCollisionDistance){
		this->removeChild(_exit,true);
	}
}
void HelloWorld::addMaze(){
	//we create a simple maze,this maze has 8 lines 
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	float unitLength=winSize.height*0.2f;
	CCPoint pointArray[8]={
		ccp(unitLength,unitLength*3.0f*0.5f),
		ccp(unitLength,unitLength*4.0f),
		ccp(unitLength*2.0f,unitLength*2.5f),
		ccp(unitLength*2.5f,unitLength),
		ccp(unitLength*4.0f,unitLength*1.5f),
		ccp(unitLength*3.5f,unitLength*3.0f),
		ccp(unitLength*3.0f,unitLength*3.5f),
		ccp(unitLength*4.0f,unitLength*4.0f)
	};
	float length[8]={3.0,2.0,3.0,1.0,3.0,1.0,1.0,2.0};
	int lineType[8]={YLINE,XLINE,YLINE,XLINE,YLINE,XLINE,YLINE,XLINE};
	int i=0;
	for(i;i<8;i++){
		addMazeEdge(lineType[i],unitLength*length[i],pointArray[i]);
	}
}
void HelloWorld::addExitEdge(int edgeType,float length,CCPoint centerPosition){
	float wallScale=length/90.0f;
	/*
	b2BodyDef edgeBodyDef;
	edgeBodyDef.position.Set(centerPosition.x/PTM_RATIO, centerPosition.y/PTM_RATIO); 
    edgeBodyDef.type = b2_dynamicBody;
	*/
	if(edgeType==XLINE){
		//add the edge
		_exit=CCSprite::create("widthWall.jpg",
				CCRectMake(0,0,20,1));
		_exit->setPosition(ccp(centerPosition.x,centerPosition.y));
		_exit->setScaleX(wallScale);
		this->addChild(_exit);
	}
	if(edgeType==YLINE){
		//add the edge
		_exit=CCSprite::create("heightWall.jpg",
				CCRectMake(0,0,20,90));
		_exit->setPosition(ccp(centerPosition.x,centerPosition.y));
		_exit->setScaleY(wallScale);
		this->addChild(_exit);
	}
	/*
	//connect the sprite to the body
	edgeBodyDef.userData=mazeEdge;
	//create the line body
	_exitBody=_world->CreateBody(&edgeBodyDef);
	//create the fixture
	b2PolygonShape lineBoxShape;
	if(edgeType==XLINE){
		lineBoxShape.SetAsBox(length*0.5f/PTM_RATIO,20.0f*0.5f/PTM_RATIO);
	}
	if(edgeType==YLINE){
		lineBoxShape.SetAsBox(20.0f*0.5f/PTM_RATIO,length*0.5f/PTM_RATIO);
	}
	//the fixture def
	b2FixtureDef edgeFixtureDef;
	edgeFixtureDef.shape=&lineBoxShape;
	edgeFixtureDef.friction=0.9f;
	edgeFixtureDef.density=3.0f;
	_exitBody->CreateFixture(&edgeFixtureDef);
	*/
}
void HelloWorld::addMazeEdge(int edgeType ,float length,CCPoint centerPosition){
	float wallScale=length/90.0f;
	b2BodyDef edgeBodyDef;
	edgeBodyDef.position.Set(centerPosition.x/PTM_RATIO, centerPosition.y/PTM_RATIO); 
	CCSprite *mazeEdge;
	if(edgeType==XLINE){
		//add the edge
		mazeEdge=CCSprite::create("widthWall.jpg",
				CCRectMake(0,0,90,4));
		mazeEdge->setPosition(ccp(centerPosition.x,centerPosition.y));
		mazeEdge->setScaleX(wallScale);
		this->addChild(mazeEdge);
	}
	if(edgeType==YLINE){
		//add the edge
		mazeEdge=CCSprite::create("heightWall.jpg",
				CCRectMake(0,0,4,90));
		mazeEdge->setPosition(ccp(centerPosition.x,centerPosition.y));
		mazeEdge->setScaleY(wallScale);
		this->addChild(mazeEdge);
	}
	//connect the sprite to the body
	edgeBodyDef.userData=mazeEdge;
	//create the line body
	b2Body *lineBody=_world->CreateBody(&edgeBodyDef);
	//create the fixture
	b2PolygonShape lineBoxShape;
	if(edgeType==XLINE){
		lineBoxShape.SetAsBox(length*0.5f/PTM_RATIO,LINEHEIGHT*0.5f/PTM_RATIO);
	}
	if(edgeType==YLINE){
		lineBoxShape.SetAsBox(LINEHEIGHT*0.5f/PTM_RATIO,length*0.5f/PTM_RATIO);
	}
	//the fixture def
	b2FixtureDef edgeFixtureDef;
	edgeFixtureDef.shape=&lineBoxShape;
	edgeFixtureDef.friction=0.9f;
	lineBody->CreateFixture(&edgeFixtureDef);
}
