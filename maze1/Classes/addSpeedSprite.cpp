class Poker : public CCSprite ,public CCTargetedTouchDelegate  
{  
    PokerState m_state;  
	public:  
  
    Poker(void);  
    CCRect rect();  
    virtual void onEnter();  
    virtual void onExit();  
    virtual ~Poker(void);  
    boolean containTouchPoint(CCTouch* touch);  
    virtual bool ccTouchBegan(CCTouch *touch ,CCEvent *event);  
    virtual void ccTouchMoved(CCTouch *touch ,CCEvent *event);  
    virtual void ccTouchEnded(CCTouch *touch ,CCEvent *event);  
  
};  
