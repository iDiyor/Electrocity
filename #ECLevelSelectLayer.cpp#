#include "ECLevelSelectLayer.h"
#include "ECLevelChapterLayer.h"
#include "ECScrollView.h"
#include "ECDataProviderExt.h"
#include "ECDataProvider.h"

ECLevelSelectLayer::ECLevelSelectLayer() {

}

ECLevelSelectLayer::~ECLevelSelectLayer() {
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("level_select_layer_spritesheet.plist");
}

CCScene* ECLevelSelectLayer::scene()
{
	CCScene* scene = NULL;
	do
	{
		scene = CCScene::create();
		CC_BREAK_IF(!scene);

		ECLevelSelectLayer* layer = ECLevelSelectLayer::create();
		CC_BREAK_IF(!layer);

		scene->addChild(layer);

	}while(0);
	return scene;
}
bool ECLevelSelectLayer::init()
{
	bool isSuccess = false;
	do
	{
		CC_BREAK_IF(!CCLayer::init());

		screen_size_ = CCDirector::sharedDirector()->getWinSize();
		
		CCRect winRect;
		winRect.origin = CCDirector::sharedDirector()->getVisibleOrigin();
		CCPoint winPoint = CCDirector::sharedDirector()->getVisibleOrigin();

		float winBottomY = winRect.origin.y;

		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("level_select_layer_spritesheet.plist");
		CCSpriteBatchNode* levelSelectLayerSpritesheet = CCSpriteBatchNode::create("level_select_layer_spritesheet.pvr.ccz");
		this->addChild(levelSelectLayerSpritesheet);

		// scroll view
		ECLevelChapterLayer* chapterOnePage   = ECLevelChapterLayer::CreateWithBackgroundImage("chapter_1.png", 1);
		ECLevelChapterLayer* chapterTwoPage   = ECLevelChapterLayer::CreateWithBackgroundImage("chapter_2.png", 2);
		ECLevelChapterLayer* chapterThreePage = ECLevelChapterLayer::CreateWithBackgroundImage("chapter_3.png", 3);
		ECLevelChapterLayer* chapterFourPage  = ECLevelChapterLayer::CreateWithBackgroundImage("chapter_4.png", 4);

		ECDataProviderExt* data_provider = new ECDataProviderExt("level_state.xml", "levels");
		// containers
		std::vector<int> v_open_levels;
		std::map<int, int> mp_played_with_stars_buttons;
		// loading data and putting them in containers
		data_provider->LoadDataForLevelSelectButtons(v_open_levels, mp_played_with_stars_buttons);

		chapterOnePage->CreateLevelSelectButtons(3,5,1,v_open_levels, mp_played_with_stars_buttons);
		chapterTwoPage->CreateLevelSelectButtons(3,5,16, v_open_levels, mp_played_with_stars_buttons);
		chapterThreePage->CreateLevelSelectButtons(3,5,31, v_open_levels, mp_played_with_stars_buttons);
		chapterFourPage->CreateLevelSelectButtons(3,5,46, v_open_levels,mp_played_with_stars_buttons);

		delete data_provider;
		data_provider = NULL;

		CCArray* levelChapterArray = new CCArray();
		levelChapterArray->addObject(chapterOnePage);
		levelChapterArray->addObject(chapterTwoPage);
		levelChapterArray->addObject(chapterThreePage);
		levelChapterArray->addObject(chapterFourPage);

		ECScrollLayer* scroller = ECScrollLayer::createWithLayersWithOffset(levelChapterArray, 1);
		scroller->_pagesIndicatorNormalColor = ccc4(14,134,93,255);
		scroller->_pagesIndicatorSelectedColor = ccc4(227,164,45,255);
		scroller->_pagesIndicatorPosition = ccp(screen_size_.width * 0.5f, winBottomY+5);
		scroller->_marginOffSet = 60.0f;
		scroller->selectPage(0);
		this->addChild(scroller);

		//this->scheduleUpdate();
		CCString* total_score_string = CCString::createWithFormat("Score: %i", ECDataProvider::GetGeneralScore());
		CCLabelBMFont* total_score_label = CCLabelBMFont::create(total_score_string->getCString(), "general_font.fnt");
		total_score_label->setPosition(ccp(screen_size_.width - total_score_label->getContentSize().width * 0.6f, 
										   total_score_label->getContentSize().height * 0.6f));
		this->addChild(total_score_label);

		isSuccess = true;
	}while(0);
	return isSuccess;
}
void ECLevelSelectLayer::update(float delta)
{

}

void ECLevelSelectLayer::scrollViewDidScroll(CCScrollView* view)
{

}
void ECLevelSelectLayer::scrollViewDidZoom(CCScrollView* view)
{

}
