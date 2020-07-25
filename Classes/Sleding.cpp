#include "Sleding.h"
#include "AppMacros.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;
#define ratio 128
#define SLEDFORNTTAG 100
#define SNOWMANTAG 101
#define HEAERTTAG1 201
#define HEAERTTAG2 202
#define HEAERTTAG3 203
#define HEAERTTAG4 204
#define HEAERTTAG5 205

#include "levels.h"

// start screen begins //////////////////////////////////////////////////////////////////////////////
CCScene* Sled_Start::scene()
{
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
	Sled_Start *layer = Sled_Start::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Sled_Start::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    CCMenuItemImage *pStartItem = CCMenuItemImage::create("start_normal.png", "start_clicked.png", this, menu_selector(Sled_Start::menuStart)); 
	CCMenuItemImage *pSettingItem = CCMenuItemImage::create("setting_normal.png", "setting_clicked.png", this, menu_selector(Sled_Start::menuSetting));
	CCMenuItemImage *pCloseItem = CCMenuItemImage::create("exit_normal.png", "exit_clicked.png", this, menu_selector(Sled_Start::menuCloseCallback));
	pStartItem->setPosition(ccp(winSize.width - 200, winSize.height - 500));
	pSettingItem->setPosition(ccp(winSize.width - 200, winSize.height - 575));
	pCloseItem->setPosition(ccp(winSize.width - 200, winSize.height - 650));

	CCMenu* pMenu = CCMenu::create(pStartItem, pSettingItem, pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

	// this is backgroud image of start screen.
    CCSprite* pBgSprite = CCSprite::create("start_bgd.png");

    // position the sprite on the center of the screen
    pBgSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(pBgSprite, 0);

	// snow effect
	CCParticleSystem* pP_S;
	pP_S = CCParticleSnow::create();
	this->addChild(pP_S, 5);
	pP_S->setGravity(ccp(0, -10));
	pP_S->setSpeed(100.f);
	pP_S->setLife(3.f);
	pP_S->setStartSize(15.f);
	pP_S->setEmissionRate(40.f);
	pP_S->setTexture(CCTextureCache::sharedTextureCache()->addImage("snow.png"));

    current_level = 0;
    return true;
}

void Sled_Start::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

void Sled_Start::menuStart(CCObject* pSender)
{
	CCScene *scene = Leveledit::scene();
	scene->addChild(Leveledit::create(), 0);
	CCScene* pscene = CCTransitionSlideInL::create(0.5f, scene);
	CCDirector::sharedDirector()->replaceScene(pscene);
}

void Sled_Start::menuSetting(CCObject* pSender)
{
}
// level edit screen starts ///////////////////////////////////////////////////////////////////////////
CCScene* Leveledit::scene()
{
	CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
	Leveledit *layer = Leveledit::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool Leveledit::init()
{
	if ( !CCLayer::init() )
    {
        return false;
    }

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCSprite* pBgSprite = CCSprite::create("level_edit.png");
    pBgSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(pBgSprite, 0);

	char level_no[15][15] = {
		"01_open.png",
		"02_open.png",
		"03_open.png",
		"04_open.png",
		"05_open.png",
		"06_open.png",
		"07_open.png",
		"08_open.png",
		"09_open.png",
		"10_open.png",
		"11_open.png",
		"12_open.png",
		"13_open.png",
		"14_open.png",
		"15_open.png"
	};

	CCUserDefault* mydata = CCUserDefault::sharedUserDefault();
	mydata->setIntegerForKey("1", 15);
	CCMenuItemImage** level;
	level = new CCMenuItemImage* [15];

	char chr_score[10];
	char key[10];
	for(int i = 0; i < 15; i ++)
	{
		sprintf(key, "%d", i + 1);
		int score;
		score = mydata->getIntegerForKey(key);
		level[i] = CCMenuItemImage::create(level_no[i], level_no[i], "locked.png", this, menu_selector(Leveledit::gamestart));
		level[i]->setUserData((void*)i);
		if(score >= 3)
			level[i]->setEnabled(true);
		else
			level[i]->setEnabled(false);
	}

	CCMenu* pMenu1 = CCMenu::create(level[0], level[1], level[2], level[3], level[4], NULL);
	pMenu1->alignItemsHorizontallyWithPadding(50.f);

	CCMenu* pMenu2 = CCMenu::create(level[5], level[6], level[7], level[8], level[9], NULL);
	pMenu2->alignItemsHorizontallyWithPadding(50.f);

	CCMenu* pMenu3 = CCMenu::create(level[10], level[11], level[12], level[13], level[14], NULL);
	pMenu3->alignItemsHorizontallyWithPadding(50.f);

	pMenu1->setPosition(650.f, 550.f);
	pMenu2->setPosition(650.f, 350.f);
	pMenu3->setPosition(650.f, 150.f);

	pBgSprite->addChild(pMenu1, 1);
	pBgSprite->addChild(pMenu2, 1);
	pBgSprite->addChild(pMenu3, 1);
	
	// back button
	CCMenuItemImage* pBack = CCMenuItemImage::create("back_normal.png", "back_clicked.png", this, menu_selector(Leveledit::back));
	CCMenu* pMenuback = CCMenu::create(pBack, NULL);
	pMenuback->setPosition(100.f, 650.f);
	pBgSprite->addChild(pMenuback);
}

void Leveledit::gamestart(CCObject* pSender)
{
	int lev;
	lev = (int)((CCNode*)pSender)->getUserData();
	current_level = lev;
	CCScene *scene = Main::scene();
	scene->addChild(Main::create());
	CCDirector::sharedDirector()->replaceScene(scene);
}

void Leveledit::back(CCObject* pSender)
{
	CCScene *scene = Sled_Start::scene();
	scene->addChild(Sled_Start::create(), 0);
	CCScene* pscene = CCTransitionSlideInR::create(0.5f, scene);
	CCDirector::sharedDirector()->replaceScene(pscene);
}
// main screen starts ///////////////////////////////////////////////////////////////////////////////////
CCScene* Main::scene()
{
	CCScene *scene = CCScene::create();
	Main *pLayer = Main::create();

	if(pLayer != NULL)
		CCLog("cclog:%s", "OK");
	scene->addChild(pLayer, 0);
	return scene;
}

b2Vec2 Main::toWorld(CCPoint pt)
{
	return b2Vec2(pt.x / ratio, pt.y / ratio);
}

CCPoint Main::toPixel(b2Vec2 vec)
{
	return CCPoint(vec.x * ratio, vec.y * ratio);
}

bool Main::init()
{
	if ( !CCLayer::init() )
    {
        return false;
    }
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bgsound.ogg");
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.2f);
	snowman_cnt = 0;
	CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	pMainBgdSprite = CCSprite::create("main_bgd.png");
	pMainBgdSprite->setPosition(ccp(winsize.width / 2, winsize.height / 2));
	this->addChild(pMainBgdSprite, 0);
	init_score();
	init_snoweffect();
	create_particle();
	init_physics();
	init_environment();
	init_snowman();
	init_sled();
	bForward = false;
	bPausesel = false;
	goods_score = 0;
	setTouchEnabled(true);
	this->schedule(schedule_selector(Main::updategame), 0.01f);
	init_pause();
	return true;
}

void Main::init_score()
{
	pHeartsprite = CCSpriteBatchNode::create("heart.png");
	if(current_level >= 0 && current_level <= 5)
	{
		CCSprite* heart1 = CCSprite::createWithTexture(pHeartsprite->getTexture());
		CCSprite* heart2 = CCSprite::createWithTexture(pHeartsprite->getTexture());
		heart1->setPosition(ccp(80, 650));
		heart2->setPosition(ccp(130, 650));
		pMainBgdSprite->addChild(heart1, 2, HEAERTTAG1);
		pMainBgdSprite->addChild(heart2, 2, HEAERTTAG2);
	}
	else if(current_level >= 6 && current_level <= 8)
	{
		CCSprite* heart1 = CCSprite::createWithTexture(pHeartsprite->getTexture());
		CCSprite* heart2 = CCSprite::createWithTexture(pHeartsprite->getTexture());
		CCSprite* heart3 = CCSprite::createWithTexture(pHeartsprite->getTexture());
		heart1->setPosition(ccp(80, 650));
		heart2->setPosition(ccp(130, 650));
		heart3->setPosition(ccp(180, 650));
		pMainBgdSprite->addChild(heart1, 2, HEAERTTAG1);
		pMainBgdSprite->addChild(heart2, 2, HEAERTTAG2);
		pMainBgdSprite->addChild(heart3, 2, HEAERTTAG3);
	}
	else if(current_level >= 9 && current_level <= 11)
	{
		CCSprite* heart1 = CCSprite::createWithTexture(pHeartsprite->getTexture());
		CCSprite* heart2 = CCSprite::createWithTexture(pHeartsprite->getTexture());
		CCSprite* heart3 = CCSprite::createWithTexture(pHeartsprite->getTexture());
		CCSprite* heart4 = CCSprite::createWithTexture(pHeartsprite->getTexture());
		heart1->setPosition(ccp(80, 650));
		heart2->setPosition(ccp(130, 650));
		heart3->setPosition(ccp(180, 650));
		heart4->setPosition(ccp(230, 650));
		pMainBgdSprite->addChild(heart1, 2, HEAERTTAG1);
		pMainBgdSprite->addChild(heart2, 2, HEAERTTAG2);
		pMainBgdSprite->addChild(heart3, 2, HEAERTTAG3);
		pMainBgdSprite->addChild(heart4, 2, HEAERTTAG4);
	}
	else if(current_level >= 12 && current_level <= 15)
	{
		CCSprite* heart1 = CCSprite::createWithTexture(pHeartsprite->getTexture());
		CCSprite* heart2 = CCSprite::createWithTexture(pHeartsprite->getTexture());
		CCSprite* heart3 = CCSprite::createWithTexture(pHeartsprite->getTexture());
		CCSprite* heart4 = CCSprite::createWithTexture(pHeartsprite->getTexture());
		CCSprite* heart5 = CCSprite::createWithTexture(pHeartsprite->getTexture());
		heart1->setPosition(ccp(80, 650));
		heart2->setPosition(ccp(130, 650));
		heart3->setPosition(ccp(180, 650));
		heart4->setPosition(ccp(230, 650));
		heart5->setPosition(ccp(280, 650));
		pMainBgdSprite->addChild(heart1, 2, HEAERTTAG1);
		pMainBgdSprite->addChild(heart2, 2, HEAERTTAG2);
		pMainBgdSprite->addChild(heart3, 2, HEAERTTAG3);
		pMainBgdSprite->addChild(heart4, 2, HEAERTTAG4);
		pMainBgdSprite->addChild(heart5, 2, HEAERTTAG5);
	}
}

void Main::init_pause()
{
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	
	CCMenuItemImage* pPause_item = CCMenuItemImage::create("pause_normal.png", "pause_clicked.png", "pause_clicked.png", this, menu_selector(Main::gamepause));
	CCMenu* pMenu = CCMenu::create(pPause_item, NULL);
	pMenu->setPosition(s.width - 50.f, s.height - 50.f);
	pMainBgdSprite->addChild(pMenu, 2, 1001);
}

void Main::gamepause(CCObject* pSender)
{
	CCSprite* pausebgd_sprite = CCSprite::create("pause_bgd.png");
	if(bPausesel == false)
	{
		this->pauseSchedulerAndActions();
		pP_S1->pauseSchedulerAndActions();
		pP_S2->pauseSchedulerAndActions();
		pP_S3->pauseSchedulerAndActions();
		pP_S4->pauseSchedulerAndActions();
		pausebgd_sprite->setPosition(pMainBgdSprite->getPosition());
		this->addChild(pausebgd_sprite, 10, 1);

		CCMenuItemImage* pResumeitem = CCMenuItemImage::create("resume.png", "resume.png", this, menu_selector(Main::resume));
		CCMenuItemImage* pReplayitem = CCMenuItemImage::create("replay.png", "replay.png", this, menu_selector(Main::replay));
		CCMenuItemImage* pTomenuitem = CCMenuItemImage::create("tomenu.png", "tomenu.png", this, menu_selector(Main::tomenu));
		CCMenu* pMenu = CCMenu::create(pResumeitem, pReplayitem, pTomenuitem, NULL);
		pMenu->setPosition(340.f, 140.f);
		pMenu->alignItemsHorizontallyWithPadding(50.f);
		pausebgd_sprite->addChild(pMenu, 10);
		bPausesel = true;
	}
	else if(bPausesel == true)
	{
		this->resumeSchedulerAndActions();
		pP_S1->resumeSchedulerAndActions();
		pP_S2->resumeSchedulerAndActions();
		pP_S3->resumeSchedulerAndActions();
		pP_S4->resumeSchedulerAndActions();
		this->removeChildByTag(1);
		bPausesel = false;
	}
}

void Main::create_particle()
{
	pP_S3 = CCParticleFireworks::create();
	pP_S3->setLife(0.3f);
	pP_S3->setTexture(CCTextureCache::sharedTextureCache()->addImage("snowparticle.png"));
	pP_S3->setStartColor(ccc4f(1.f, 1.f, 1.f, 1.f));
	pP_S3->setStartColorVar(ccc4f(0.f, 0.f, 0.f, 0.f));
	pP_S3->setEndColor(ccc4f(1.f, 1.f, 1.f, 1.f));
	pP_S3->setEndColorVar(ccc4f(0.f, 0.f, 0.f, 0.f));
	pP_S3->setStartSize(3.f);
	pP_S3->setStartSizeVar(1.5f);
	pP_S3->setEndSizeVar(0.1f);
	pP_S3->setSpeed(300.f);
	pP_S3->setEmissionRate(200.f);
	pP_S3->setAngle(150.f);
	addChild(pP_S3, 8);

	pP_S4 = CCParticleFireworks::create();
	pP_S4->setLife(0.3f);
	pP_S4->setTexture(CCTextureCache::sharedTextureCache()->addImage("snowparticle.png"));
	pP_S4->setStartColor(ccc4f(1.f, 1.f, 1.f, 1.f));
	pP_S4->setStartColorVar(ccc4f(0.f, 0.f, 0.f, 0.f));
	pP_S4->setEndColor(ccc4f(1.f, 1.f, 1.f, 1.f));
	pP_S4->setEndColorVar(ccc4f(0.f, 0.f, 0.f, 0.f));
	pP_S4->setStartSize(3.f);
	pP_S4->setStartSizeVar(1.5f);
	pP_S4->setEndSizeVar(0.1f);
	pP_S4->setSpeed(300.f);
	pP_S4->setEmissionRate(200.f);
	pP_S4->setAngle(150.f);
	addChild(pP_S4, 8);
}

void Main::init_snoweffect()
{
	// 눈내리는 효과창조
	pP_S1 = CCParticleSnow::create();
	this->addChild(pP_S1, 8);
	pP_S1->setPositionX(this->getPositionX() + CCDirector::sharedDirector()->getVisibleSize().width / 2 - 200);
	pP_S1->setPositionType(kCCPositionTypeRelative);
	pP_S1->setGravity(ccp(0, -10));
	pP_S1->setSpeed(100.f);
	pP_S1->setLife(5.f);
	pP_S1->setEmissionRate(50.f);
	pP_S1->setStartSize(15.f);
	pP_S1->setTexture(CCTextureCache::sharedTextureCache()->addImage("snow.png"));

	pP_S2 = CCParticleSnow::create();
	this->addChild(pP_S2, 8);
	pP_S2->setPositionType(kCCPositionTypeRelative);
	pP_S2->setPositionX(pP_S1->getPositionX() + CCDirector::sharedDirector()->getVisibleSize().width);
	pP_S2->setGravity(ccp(0, -10));
	pP_S2->setSpeed(100.f);
	pP_S2->setLife(3.f);
	pP_S2->setStartSize(15.f);
	pP_S2->setEmissionRate(50.f);
	pP_S2->setTexture(CCTextureCache::sharedTextureCache()->addImage("snow.png"));
}

void Main::init_physics()
{
	b2Vec2 gravity;
	gravity.Set(0.0f, -9.8f);

	world = new b2World(gravity);
	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);
	world->SetContactListener(this);

	b2BodyDef groundDef;
	groundDef.position.Set(0, 0);
	groundBody = world->CreateBody(&groundDef);
	
	b2EdgeShape groundShape;
	for(int i = 0; i < roadsizes[current_level] - 1; i ++)
	{
		groundShape.Set(toWorld(roads[current_level][i]), toWorld(roads[current_level][i + 1]));
		groundBody->CreateFixture(&groundShape, 0);
	}

	// 도로편집 //////////////////////////////////////////////////////////////////////////
	CCDrawNode* road_shape = CCDrawNode::create();
	addChild(road_shape);
	CCPoint clip_road[4];
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	/*CCClippingNode** clipper;
	clipper = new CCClippingNode* [roadsizes[current_level] - 3];
	CCNode** content;
	content = new CCNode* [roadsizes[current_level] - 3];*/
	CCSprite* road_bgd;
	CCSprite* snow_road;

	for(int i = 1; i < roadsizes[current_level] - 2; i ++)
	{
		clip_road[0] = roads[current_level][i];
		clip_road[1] = roads[current_level][i + 1];
		clip_road[2] = ccp(roads[current_level][i + 1].x, -50);
		clip_road[3] = ccp(roads[current_level][i].x, -50);
		road_shape->drawPolygon(clip_road, 4, ccc4f(0.f, 0.f, 0.f, 0.f), 1, ccc4f(0.19f, 0.64f, 0.75f, 1.f));

		/*clipper[i - 1] = CCClippingNode::create();
		clipper[i - 1]->setAnchorPoint(ccp(0, 0));
		clipper[i - 1]->setPosition(ccp(0, 0));
		content[i - 1] = CCSprite::create("road.png");
		content[i - 1]->setPosition(roads[current_level][i].x, -50);
		clipper[i - 1]->setStencil((CCNode*)road_shape);
		this->addChild(clipper[i - 1], 9);
		clipper[i - 1]->addChild(content[i - 1]);*/

		int width;
		if((roads[current_level][i + 1].x - roads[current_level][i].x) >= 600)
		{
			road_bgd = CCSprite::create("road.png");
			width = 1300;
		}
		if((roads[current_level][i + 1].x - roads[current_level][i].x) < 600 && (roads[current_level][i + 1].x - roads[current_level][i].x) >= 300)
		{
			road_bgd = CCSprite::create("road_mid.png");
			width = 752;
		}
		if((roads[current_level][i + 1].x - roads[current_level][i].x) < 300)
		{
			road_bgd = CCSprite::create("road_min.png");
			width = 385;
		}
		
		road_bgd->setAnchorPoint(ccp(0.f, 1.f));
		road_bgd->setPosition(ccp(roads[current_level][i].x, roads[current_level][i].y));
		road_bgd->setSkewY(CC_RADIANS_TO_DEGREES((float)(roads[current_level][i + 1].y - roads[current_level][i].y) / width));
		road_bgd->setScaleX((roads[current_level][i + 1].x - roads[current_level][i].x) / (width - 2));
		this->addChild(road_bgd, 9);

		snow_road = CCSprite::create("snow_road.png");
		snow_road->setAnchorPoint(ccp(0.f, 1.f));
		snow_road->setPosition(ccp(roads[current_level][i].x, roads[current_level][i].y + 2.f));
		snow_road->setSkewY(CC_RADIANS_TO_DEGREES((float)(roads[current_level][i + 1].y - roads[current_level][i].y) / 500.f));
		snow_road->setScaleX((roads[current_level][i + 1].x - roads[current_level][i].x) / 500);
		this->addChild(snow_road, 10);
	}
	////////////////////////////////////////////////////////////////////////////////////////////

	// 주로의 끝판정 ////////////////////////////////////////////////////////////////////////////
	int n = roadsizes[current_level];
	pEnd_sprite = CCSprite::create("endimage.png");

	b2BodyDef df;
	df.type = b2_staticBody;
	df.position.Set(toWorld(ccp(roads[current_level][n - 1].x - 900, 170)).x, toWorld(ccp(roads[current_level][n - 1].x - 900, 170)).y);
	end_body = world->CreateBody(&df);

	b2PolygonShape shape;
	shape.SetAsBox(0.25f, 1.f);
	end_body->CreateFixture(&shape, 0.f);

	pEnd_sprite->setPosition(ccp(toPixel(end_body->GetPosition()).x - 200, toPixel(end_body->GetPosition()).y));
	this->addChild(pEnd_sprite, 2);
	//////////////////////////////////////////////////////////////////////////////////////////
}

void Main::init_environment()
{
	int seltree;
	for(int i = 2; i < roadsizes[current_level] - 2; i += 2)
	{
		seltree = CCRANDOM_0_1() * 3;
		CCSprite* tree_sprite1;
		CCSprite* tree_sprite2;
		CCSprite* tree_sprite3;
		tree_sprite1 = CCSprite::create("tree.png");
		tree_sprite2 = CCSprite::create("tree.png");
		tree_sprite3 = CCSprite::create("tree.png");
		
		tree_sprite1->setAnchorPoint(ccp(0.5f, 0.02f));
		tree_sprite1->setScaleX(CCRANDOM_0_1() * 0.4f + 0.8f);
		tree_sprite1->setScaleY(CCRANDOM_0_1() * 0.4f + 0.8f);
		tree_sprite1->setPosition(ccp(roads[current_level][i].x, roads[current_level][i].y - CCRANDOM_0_1() * 50));

		tree_sprite2->setAnchorPoint(ccp(0.5f, 0.02f));
		tree_sprite2->setScaleX(CCRANDOM_0_1() * 0.4f + 0.8f);
		tree_sprite2->setScaleY(CCRANDOM_0_1() * 0.4f + 0.8f);
		tree_sprite2->setPosition(ccp(roads[current_level][i].x + 70 + 10 * CCRANDOM_0_1(), roads[current_level][i].y - CCRANDOM_0_1() * 50));

		tree_sprite3->setAnchorPoint(ccp(0.5f, 0.02f));
		tree_sprite3->setScaleX(CCRANDOM_0_1() * 0.4f + 0.8f);
		tree_sprite3->setScaleY(CCRANDOM_0_1() * 0.4f + 0.8f);
		tree_sprite3->setPosition(ccp(roads[current_level][i].x + 120 + 10 * CCRANDOM_0_1(), roads[current_level][i].y - CCRANDOM_0_1() * 50));

		switch (seltree)
		{
		case 0:
			this->addChild(tree_sprite1, 1);
			break;
		case 1:
			this->addChild(tree_sprite1, 1);
			this->addChild(tree_sprite2, 1);
			break;
		case 2:
			this->addChild(tree_sprite1, 1);
			this->addChild(tree_sprite2, 1);
			this->addChild(tree_sprite3, 1);
			break;
		default:
			break;
		}
	}
}

void Main::init_snowman()
{
	for(int i = 0; i < 15; i ++)
	{
		CCSprite* snowman_sprite = CCSprite::create("snowman.png");

		b2BodyDef df;
		df.type = b2_staticBody;
		df.userData = snowman_sprite;
		df.position.Set(toWorld(snowmans[current_level][i]).x, toWorld(snowmans[current_level][i]).y);
		snowman[i] = world->CreateBody(&df);

		b2PolygonShape shape;
		shape.SetAsBox(0.2f, 0.2f);
		snowman[i]->CreateFixture(&shape, 0.f);

		snowman_sprite->setPosition(snowmans[current_level][i]);
		snowman_sprite->setAnchorPoint(ccp(1, 0));
		this->addChild(snowman_sprite, 6, SNOWMANTAG);
	}
}

void Main::init_sled()
{
	// link
	{
		uplink_sprite = CCSprite::create("uplink.png");
		addChild(uplink_sprite, 7);
		downlink_sprite = CCSprite::create("downlink.png");
		addChild(downlink_sprite, 7);
	}
	// sled front
	{
		pSled_front_sprite = CCSprite::create("sled_front.png");
		pSled_front_sprite->setAnchorPoint(ccp(0.5f, 0.35f));
		addChild(pSled_front_sprite, 7, SLEDFORNTTAG);
		b2PolygonShape sledfront_shape;

		b2BodyDef sled_front_bodydef;
		sled_front_bodydef.type = b2_dynamicBody;
		sled_front_bodydef.position.Set(2.5f, 1.1f);
		sled_front_bodydef.awake = true;
		sled_front_bodydef.userData = pSled_front_sprite;
		sled_front = world->CreateBody(&sled_front_bodydef);

		sledfront_shape.SetAsBox(0.605f, 0.15625f);
		sled_front->CreateFixture(&sledfront_shape, 0.8f);
		sledfront_shape.SetAsBox(0.429f, 0.2f, b2Vec2(-0.1753f, 0.32f), 0.f);
		sled_front->CreateFixture(&sledfront_shape, 0.5f);
	}
	// front ski
	{
		pSki_front_sprite = CCSprite::create("ski_front.png");
		addChild(pSki_front_sprite, 7);
		pSki_front_sprite->setAnchorPoint(ccp(0.5f, 0.13f));


		b2BodyDef skifront_bodydef;
		skifront_bodydef.type = b2_dynamicBody;
		skifront_bodydef.position.Set(2.5f, 0.81f);
		skifront_bodydef.awake = true;
		skifront_bodydef.userData = pSki_front_sprite;
		ski_front = world->CreateBody(&skifront_bodydef);

		b2PolygonShape skifront_shape;
		skifront_shape.SetAsBox(0.59f, 0.02f);
		ski_front->CreateFixture(&skifront_shape, 1.f);
		skifront_shape.SetAsBox(0.11f, 0.02f, b2Vec2(0.65f, 0.05f), 0.6f);
		firstfront = ski_front->CreateFixture(&skifront_shape, 1.f);

		b2FixtureDef fd;
		fd.shape = &skifront_shape;
		fd.density = 1.0f;
		fd.friction = 0.f;
		fd.restitution = 0.1f;
		ski_front->CreateFixture(&fd);
	}
	// sledfront jion skifront
	{
		b2WheelJointDef jd;
		b2Vec2 axis(0.f, 1.f);
		jd.Initialize(sled_front, ski_front, b2Vec2(3.f, 0.9f), axis);
		jd.motorSpeed = 0;
		jd.enableMotor = false;
		jd.dampingRatio = 0.7f;
		jd.frequencyHz = 15.f;
		jd.collideConnected = true;
		front_spring = (b2WheelJoint*)world->CreateJoint(&jd);

		jd.Initialize(sled_front, ski_front, b2Vec2(2.f, 0.9f), axis);
		jd.motorSpeed = 0;
		jd.enableMotor = false;
		jd.dampingRatio = 0.f;
		jd.frequencyHz = 35.f;
		jd.collideConnected = true;
		front_spring = (b2WheelJoint*)world->CreateJoint(&jd);
	}
	// truck
	{
		pTruck_sprite = CCSprite::create("truck.png");
		addChild(pTruck_sprite, 7);
		pTruck_sprite->setAnchorPoint(ccp(0.5f, 0.23f));

		b2BodyDef truck_bodydef;
		truck_bodydef.type = b2_dynamicBody;
		truck_bodydef.position.Set(0.8f, 0.95f);
		truck_bodydef.awake = true;
		truck_bodydef.userData = pTruck_sprite;
		truck = world->CreateBody(&truck_bodydef);

		b2PolygonShape truck_shape;
		truck_shape.SetAsBox(0.86f, 0.02f);
		truck->CreateFixture(&truck_shape, 0.3f); // bottom
		truck_shape.SetAsBox(0.02f, 0.273f, b2Vec2(0.83f, 0.253f), 0.f);
		truck->CreateFixture(&truck_shape, 0.2f); // right
		truck_shape.SetAsBox(0.02f, 0.273f, b2Vec2(-0.83f, 0.253f), 0.f);
		truck->CreateFixture(&truck_shape, 0.5f); // left
	}
	// back ski
	{
		pSki_back_sprite = CCSprite::create("ski_back.png");
		addChild(pSki_back_sprite, 7);
		pSki_back_sprite->setAnchorPoint(ccp(0.49f, 0.13f));

		b2BodyDef skiback_bodydef;
		skiback_bodydef.type = b2_dynamicBody;
		skiback_bodydef.position.Set(0.8f, 0.81f);
		skiback_bodydef.awake = true;
		skiback_bodydef.userData = pSki_back_sprite;
		ski_back = world->CreateBody(&skiback_bodydef);

		b2PolygonShape skiback_shape;
		skiback_shape.SetAsBox(0.859f, 0.02f);
		ski_back->CreateFixture(&skiback_shape, 0.3f);
		skiback_shape.SetAsBox(0.11f, 0.02f, b2Vec2(0.93f, 0.05f), 0.6f);
		secondfront = ski_back->CreateFixture(&skiback_shape, 0.1f);

		b2FixtureDef fd;
		fd.shape = &skiback_shape;
		fd.density = 1.0f;
		fd.friction = 0.f;
		fd.restitution = 0.1f;
		ski_back->CreateFixture(&fd);
	}
	// truck jion backski
	{
		b2WheelJointDef jd;
		b2Vec2 axis(0.f, 1.f);
		jd.Initialize(truck, ski_back, b2Vec2(0.2f, 0.87f), axis);
		jd.motorSpeed = 0;
		jd.enableMotor = false;
		jd.dampingRatio = 0.9f;
		jd.frequencyHz = 15.f;
		jd.collideConnected = true;
		back_spring = (b2WheelJoint*)world->CreateJoint(&jd);

		jd.Initialize(truck, ski_back, b2Vec2(1.35f, 0.87f), axis);
		jd.motorSpeed = 0;
		jd.enableMotor = false;
		jd.dampingRatio = 1.9f;
		jd.frequencyHz = 15.f;
		jd.collideConnected = true;
		back_spring = (b2WheelJoint*)world->CreateJoint(&jd);
	}
	// truck join sled_front
	{
		b2WheelJointDef jd;
			
		b2Vec2 axis(1.f, 0.f); // bottom
		jd.Initialize(sled_front, truck, b2Vec2(1.8f, 1.1f), axis);
		jd.motorSpeed = 0;
		jd.enableMotor = false;
		jd.dampingRatio = 2.f;
		jd.frequencyHz = 40.f;
		jd.collideConnected = true;
		middle_spring = (b2WheelJoint*)world->CreateJoint(&jd);
	}
	// goods
	{
		int type;
		b2BodyDef df;
		df.type = b2_dynamicBody;

		b2PolygonShape shape;
		b2FixtureDef fd;
		fd.friction = 0.9f;
		fd.restitution = 0.1f;

		goods = new b2Body* [goods_cnt[current_level]];
		for(int i = 0; i < goods_cnt[current_level]; i ++)
		{
			type = CCRANDOM_0_1() * 4;
			switch (type)
			{
			case 0:
				pGoods_sprite = CCSprite::create("goods_01.png");
				df.position.Set(0.4f + CCRANDOM_0_1() * 0.3f, 1.f + CCRANDOM_0_1() * 0.5f);
				break;
			case 1:
				pGoods_sprite = CCSprite::create("goods_04.png");
				df.position.Set(0.5f + CCRANDOM_0_1() * 0.4f, 1.f + CCRANDOM_0_1() * 0.5f);
				break;
			case 2:
				pGoods_sprite = CCSprite::create("goods_03.png");
				df.position.Set(0.9f + CCRANDOM_0_1() * 0.5f, 1.3f + CCRANDOM_0_1() * 0.5f);
				break;
			case 3:
				pGoods_sprite = CCSprite::create("goods_02.png");
				df.position.Set(1.4f + CCRANDOM_0_1() * 0.1f, 1.2f + CCRANDOM_0_1() * 0.5f);
				break;
			default:
				break;
			}
			addChild(pGoods_sprite, 2);
			df.userData = pGoods_sprite;
			goods[i] = world->CreateBody(&df);

			switch (type)
			{
			case 0:
				shape.SetAsBox(0.1f, 0.1f);
				break;
			case 1:
				shape.SetAsBox(0.12f, 0.12f);
				break;
			case 2:
				shape.SetAsBox(0.11f, 0.11f);
				break;
			case 3:
				shape.SetAsBox(0.11f, 0.11f);
				break;
			default:
				break;
			}

			goods[i]->CreateFixture(&shape, 0.1f);

			fd.shape = &shape;
			goods[i]->CreateFixture(&fd);
		}
	}
}

void Main::updatesledsprite()
{
	float updis, downdis;
	float scale;
	float angle;

	pSled_front_sprite = (CCSprite*)sled_front->GetUserData();
	angle = sled_front->GetAngle();
	pSled_front_sprite->setPosition(toPixel(sled_front->GetPosition()));
	pSled_front_sprite->setRotation(-angle * (180 / 3.14159265358979));

	updis = (sled_front->GetWorldPoint(b2Vec2(-0.605f, 0.1f)).x - truck->GetWorldPoint(b2Vec2(0.86f, 0.25f)).x);
	scale = updis / 0.24f;
	uplink_sprite->setScaleX(scale);
	uplink_sprite->setRotation(-angle * (180 / 3.14159265358979));
	uplink_sprite->setPosition(ccp((truck->GetWorldPoint(b2Vec2(0.85f, 0.28f)).x + (updis / 2)) * ratio, (truck->GetWorldPoint(b2Vec2(0.85f, 0.28f)).y) * 128));

	downdis = (sled_front->GetWorldPoint(b2Vec2(-0.605f, -0.03f)).x - truck->GetWorldPoint(b2Vec2(0.86f, 0.12f)).x);
	scale = downdis / 0.24f;
	downlink_sprite->setScaleX(scale);
	downlink_sprite->setRotation(-angle * (180 / 3.14159265358979));
	downlink_sprite->setPosition(ccp((truck->GetWorldPoint(b2Vec2(0.85f, 0.12f)).x + (downdis / 2)) * ratio, (truck->GetWorldPoint(b2Vec2(0.85f, 0.12f)).y) * 128));

	pSki_front_sprite = (CCSprite*)ski_front->GetUserData();
	angle = ski_front->GetAngle();
	pSki_front_sprite->setPosition(toPixel(ski_front->GetPosition()));
	pSki_front_sprite->setRotation(-angle * (180 / 3.14159265358979));

	pTruck_sprite = (CCSprite*)truck->GetUserData();
	angle = truck->GetAngle();
	pTruck_sprite->setPosition(toPixel(truck->GetPosition()));
	pTruck_sprite->setRotation(-angle * (180 / 3.14159265358979));

	pSki_back_sprite = (CCSprite*)ski_back->GetUserData();
	angle = ski_back->GetAngle();
	pSki_back_sprite->setPosition(toPixel(ski_back->GetPosition()));
	pSki_back_sprite->setRotation(-angle * (180 / 3.14159265358979));
}

void Main::updategoods()
{
	float angle;
	CCSprite* goodsprite;
	for(int i = 0; i < goods_cnt[current_level]; i ++)
	{
		if(goods[i] != NULL)
		{
			goodsprite = (CCSprite*)goods[i]->GetUserData();
			angle = goods[i]->GetAngle();
			goodsprite->setPosition(toPixel(goods[i]->GetPosition()));
			goodsprite->setRotation(-angle * (180 / 3.14159265358979));
		}
	}
}

void Main::updateeffect()
{
	// 눈내리는 효과
	if(pP_S1->getPositionX() + CCDirector::sharedDirector()->getVisibleSize().width < pMainBgdSprite->getPositionX())
	{
		pP_S1->setPositionX(pP_S2->getPositionX() + CCDirector::sharedDirector()->getVisibleSize().width);
		CCParticleSystem* tmp;
		tmp = pP_S1;
		pP_S1 = pP_S2;
		pP_S2 = tmp;
	}
	pP_S1->setPositionY(pMainBgdSprite->getPositionY() + 400);
	pP_S2->setPositionY(pMainBgdSprite->getPositionY() + 400);
	// 썰매앞부분에서 눈보라효과
	pP_S3->setPosition(pSki_front_sprite->getPosition().x + 85, pSki_front_sprite->getPosition().y - 5);
	pP_S3->setEmissionRate(sled_front->GetLinearVelocity().Length() * 17.f * first_collision);
	// 썰매뒤부분에서 눈보라효과
	pP_S4->setPosition(pSki_back_sprite->getPosition().x + 100, pSki_back_sprite->getPosition().y - 5);
	pP_S4->setEmissionRate(truck->GetLinearVelocity().Length() * 12.f * second_collision);
	// 눈보라의 각도설정
	pP_S3->setAngle(180 - pSki_front_sprite->getRotation() - 5);
	pP_S4->setAngle(180 - pSki_front_sprite->getRotation() - 5);
}

void Main::updategame(float f)
{
	float updis, downdis;
	float scale;
	float timestep = 0.01f;
	float angle;
	int32 velocityiterations = 16;
	int32 positioniterations = 1; 
	first_collision = 0;
	second_collision = 0;
	tempbody = NULL;
	world->Step(timestep, velocityiterations, positioniterations);
	updatesledsprite();
	updategoods();
	if(bForward)
		ski_front->ApplyForce(b2Vec2(10.f, 0.f), b2Vec2(ski_front->GetWorldCenter().x, ski_front->GetWorldCenter().y + 0.3f));

	this->setPositionX(-((pSled_front_sprite->getPositionX()) - 600));
	this->setPositionY(-((pSled_front_sprite->getPositionY()) - 200));
	pMainBgdSprite->setPositionX(pSled_front_sprite->getPositionX() + 40);
	pMainBgdSprite->setPositionY(pSled_front_sprite->getPositionY() + 150);
	updateeffect();
}

void Main::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	b2Body* a = contact->GetFixtureA()->GetBody();
	b2Body* b = contact->GetFixtureB()->GetBody();

	b2Fixture* af = contact->GetFixtureA();
	b2Fixture* bf = contact->GetFixtureB();

	CCSprite* endsprite = (CCSprite*)sled_front->GetUserData();

	if(b == ski_front && a == end_body) 
	{
		this->unscheduleAllSelectors();
		pP_S3->stopSystem();
		pP_S4->stopSystem();
		levelcomplete();
	}

	if(bf == firstfront && af->GetBody() == groundBody)
		first_collision = 1;

	if(bf == secondfront && af->GetBody() == groundBody)
		second_collision = 1;
	
	for(int i = 0; i < goods_cnt[current_level]; i ++)
	{
		if((b == goods[i] && a == groundBody) || (b == goods[i] && a == end_body))
		{
			goods[i] = NULL;
			goods_score ++;
			SimpleAudioEngine::sharedEngine()->playEffect("ptfall.wav");
			SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1.0f);
			switch (goods_score)
			{
			case 1:
				pMainBgdSprite->removeChildByTag(HEAERTTAG1);
				break;
			case 2:
				pMainBgdSprite->removeChildByTag(HEAERTTAG2);
				break;
			case 3:
				pMainBgdSprite->removeChildByTag(HEAERTTAG3);
				break;
			case 4:
				pMainBgdSprite->removeChildByTag(HEAERTTAG4);
				break;
			case 5:
				pMainBgdSprite->removeChildByTag(HEAERTTAG5);
				break;
			default:
				break;
			}
			
			if(current_level >= 0 && current_level <= 5)
			{
				if(goods_score == 2)
				{
					this->unscheduleAllSelectors();
					pP_S3->stopSystem();
					pP_S4->stopSystem();
					levelfailed();
				}
			}
			else if(current_level >= 6 && current_level <= 8)
			{
				if(goods_score == 3)
				{
					this->unscheduleAllSelectors();
					pP_S3->stopSystem();
					pP_S4->stopSystem();
					levelfailed();
				}
			}
			else if(current_level >= 9 && current_level <= 11)
			{
				if(goods_score == 4)
				{
					this->unscheduleAllSelectors();
					pP_S3->stopSystem();
					pP_S4->stopSystem();
					levelfailed();
				}
			}
			else if(current_level >= 12 && current_level <= 15)
			{
				if(goods_score == 5)
				{
					this->unscheduleAllSelectors();
					pP_S3->stopSystem();
					pP_S4->stopSystem();
					levelfailed();
				}
			}
			break;
		}
	}

	CCSprite* sledbodysprite = (CCSprite*)b->GetUserData();
	CCSprite* snowmansprite = (CCSprite*)a->GetUserData();

	//flg = false;
	if(sledbodysprite!= NULL && snowmansprite!=NULL)
	{
		if(snowmansprite->getTag() == SNOWMANTAG)
		{
			contact->SetEnabled(false);
			if(snowmansprite->isVisible())
			{
				snowman_cnt ++;
				SimpleAudioEngine::sharedEngine()->playEffect("eat.wav");
				SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1.0f);
			}
			snowmansprite->setVisible(false);
		}
	}
}

void Main::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent)
{
	bForward = true;
	sled_front->ApplyForce(b2Vec2(10.f, 0.f), b2Vec2(0.9f, 1.2f));
}

void Main::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent)
{
	bForward = false;
}

void Main::ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent)
{
	
}

void Main::levelcomplete()
{
	pMainBgdSprite->removeChildByTag(1001);
	CCSprite* levelsucess = CCSprite::create("levelsuccess.png");
	levelsucess->setPosition(ccp(-this->getPositionX() + CCDirector::sharedDirector()->getVisibleSize().width / 2, -this->getPositionY() + CCDirector::sharedDirector()->getVisibleSize().height / 2));
	this->addChild(levelsucess, 10);

	CCMenuItemImage* pNextitem = CCMenuItemImage::create("next.png", "next.png", "next_disable.png", this, menu_selector(Main::next));
	CCMenuItemImage* pReplayitem = CCMenuItemImage::create("replay.png", "replay.png", this, menu_selector(Main::replay));
	CCMenuItemImage* pTomenu = CCMenuItemImage::create("tomenu.png", "tomenu.png", this, menu_selector(Main::tomenu));

	if(snowman_cnt >= 10)
		pNextitem->setEnabled(true);
	else
		pNextitem->setEnabled(false);

	CCMenu* pMenu = CCMenu::create(pReplayitem, pNextitem, pTomenu, NULL);
	pMenu->setPosition(ccp(670.f, 280.f));
	pMenu->alignItemsHorizontallyWithPadding(50.f);
	levelsucess->addChild(pMenu);
	
	CCUserDefault* mydata = CCUserDefault::sharedUserDefault();

	char level[20];
	sprintf(level, "%d", current_level + 1);
	
	int record;
	record = mydata->getIntegerForKey(level);
	
	if(record < 13)
	{
		int real_cnt;
		if(snowman_cnt % 2 == 0)
			real_cnt = snowman_cnt / 2;
		else
			real_cnt = snowman_cnt / 2 + 1;
		mydata->setIntegerForKey(level, real_cnt);
	}
}

void Main::levelfailed()
{
	pMainBgdSprite->removeChildByTag(1001);
	CCSprite* failed_sprite = CCSprite::create("levelfailed.png");
	failed_sprite->setPosition(ccp(-this->getPositionX() + CCDirector::sharedDirector()->getVisibleSize().width / 2, -this->getPositionY() + CCDirector::sharedDirector()->getVisibleSize().height / 2));
	this->addChild(failed_sprite, 10);

	CCMenuItemImage* pReplay = CCMenuItemImage::create("replay.png", "replay.png", this, menu_selector(Main::replay));
	CCMenuItemImage* pTomenu = CCMenuItemImage::create("tomenu.png", "tomenu.png", this, menu_selector(Main::tomenu));
	CCMenu* pMenu = CCMenu::create(pReplay, pTomenu, NULL);
	pMenu->alignItemsHorizontallyWithPadding(100.f);
	pMenu->setPosition(650.f, 300.f);
	failed_sprite->addChild(pMenu);
}

void Main::tomenu(CCObject* pSender)
{
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	CCScene *scene = Leveledit::scene();
	scene->addChild(Leveledit::create(), 0);
	CCDirector::sharedDirector()->replaceScene(scene);
}

void Main::resume(CCObject* pSender)
{
	bPausesel = true;
	gamepause(pSender);
}

void Main::replay(CCObject* pSender)
{
	CCScene *scene = Main::scene();
	scene->addChild(Main::create(), 0);
	CCDirector::sharedDirector()->replaceScene(scene);
}

void Main::next(CCObject* pSender)
{
	current_level ++;
	if(current_level == 16)
	{
		CCScene *scene = Leveledit::scene();
		scene->addChild(Leveledit::create(), 0);
		CCDirector::sharedDirector()->replaceScene(scene);
		return;
	}
	CCScene *scene = Main::scene();
	scene->addChild(Main::create(), 0);
	CCDirector::sharedDirector()->replaceScene(scene);
}
