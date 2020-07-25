#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
USING_NS_CC;

class Sled_Start : public CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
	void menuSetting(CCObject* pSender);
	void menuStart(CCObject* pSender);
    void menuCloseCallback(CCObject* pSender);
	
    // implement the "static node()" method manually
    CREATE_FUNC(Sled_Start);
};

class Leveledit : public CCLayer
{
public:
	virtual bool init();
	static cocos2d::CCScene* scene();
	void gamestart(CCObject* pSender);
	void back(CCObject* pSender);

	CREATE_FUNC(Leveledit);
};

class Main : public CCLayer, public b2ContactListener
{
public:
	b2World*				world;
	b2WheelJoint*			front_spring;
	b2WheelJoint*			back_spring;
	b2WheelJoint*			middle_spring;
	b2Body*					groundBody;
	b2Body*					sled_front;
	b2Body*					ski_front;
	b2Body*					ski_back;
	b2Body*					truck;
	b2Body**				goods;
	b2Body*					end_body;
	b2Body*					tempbody;
	b2Body*					snowman[15];
	b2Fixture*				firstfront;
	b2Fixture*				secondfront;
	b2Fixture*				groundfixture;
	CCSprite*				pMainBgdSprite;
	CCSprite*				pSled_front_sprite;
	CCSprite*				pSki_front_sprite;
	CCSprite*				pTruck_sprite;
	CCSprite*				pSki_back_sprite;
	CCSprite*				uplink_sprite;
	CCSprite*				downlink_sprite;
	CCSprite*				pGoods_sprite;
	CCSprite*				pEnd_sprite;
	CCSpriteBatchNode*		pHeartsprite;
	CCLabelTTF*				pScore_label;
	CCParticleSystem*		pP_S1; // snow1
	CCParticleSystem*		pP_S2; // snow2
	CCParticleSystem*		pP_S3; // sled front particle
	CCParticleSystem*		pP_S4; // truck particle
	bool					bForward;
	bool					bPausesel;
	int						first_collision;
	int						second_collision;
	int						goods_score;

	b2Vec2 toWorld(CCPoint pt);
	CCPoint toPixel(b2Vec2 vec);
	virtual bool init();
	static cocos2d::CCScene* scene();
	void init_score();
	void init_pause();
	void init_physics();
	void init_sled();
	void init_environment();
	void init_snowman();
	void init_snoweffect();
	void create_particle();
	void updategame(float f);
	void updatesledsprite();
	void updategoods();
	void updateeffect();
	void levelcomplete();
	void levelfailed();
	void gamepause(CCObject* pSender);
	void tomenu(CCObject* pSender);
	void resume(CCObject* pSender);
	void replay(CCObject* pSender);
	void next(CCObject* pSender);
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	CREATE_FUNC(Main);
};
#endif // __HELLOWORLD_SCENE_H__
