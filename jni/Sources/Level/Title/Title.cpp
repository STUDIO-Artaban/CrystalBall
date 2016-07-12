#include "Title.h"
#include "TitleCoords.h"
#include "CrystalBall.h"

#include <libeng/Tools/Tools.h>

#define TEX_BALL_WIDTH          1024.f // 'BALL' texture width
#define TEX_BALL_HEIGHT         256.f // 'BALL' texture height

// Texture IDs
#define TEXTURE_ID_TOPA         2
#define TEXTURE_ID_TOPB         3
#define TEXTURE_ID_COUNTRY1     4
#define TEXTURE_ID_COUNTRY2     5
#define TEXTURE_ID_COUNTRY3     6
#define TEXTURE_ID_QUERY        7

// 'TEXTURE_ID_TOPB' texture size
#define TEX_TOPB_WIDTH          256.f
#define TEX_TOPB_HEIGHT         1024.f

//////
Title::Title() : mScreen(NULL), mShader(NULL), mShader2D(NULL), mTexCountry(TEXTURE_ID_COUNTRY1), mScale(1.f) {

    LOGV(LOG_LEVEL_TITLE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);

    mBackground = NULL;
    mCrystal = NULL;
    mQuery = NULL;
    mC = NULL;
    mR = NULL;
    mY = NULL;
    mS = NULL;
    mT = NULL;
    mA = NULL;
    mL = NULL;
    mBall = NULL;

#ifdef DEBUG
    mLog = NULL;
#endif
}
Title::~Title() {

    LOGV(LOG_LEVEL_TITLE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);

    if (mBackground) delete mBackground;
    if (mCrystal) delete mCrystal;
    if (mQuery) delete mQuery;
    if (mC) delete mC;
    if (mR) delete mR;
    if (mY) delete mY;
    if (mS) delete mS;
    if (mT) delete mT;
    if (mA) delete mA;
    if (mL) delete mL;
    if (mBall) delete mBall;
}

void Title::load(const Game* game, unsigned char objectId) {

    LOGV(LOG_LEVEL_TITLE, 0, LOG_FORMAT(" - g:%x; o:%d"), __PRETTY_FUNCTION__, __LINE__, game, objectId);
    assert(mScreen->height > mScreen->width); // Portrait
    assert(mScreen);
    assert(mShader);
    assert(mShader2D);

    static float screenRatio = 1.f - (1.f / mScreen->top);
    static short charHeight = static_cast<short>((mScreen->height >> 1) * screenRatio);
    static short minCharH = mScreen->width * (TEX_BALL_HEIGHT / TEX_BALL_WIDTH);

    switch (objectId) {
        case OBJ_ID_BACKGROUND: {

            if (!mBackground) {

                mBackground = new Back2D;
#ifdef DEBUG
                mBackground->initialize(mScreen, mShader, mShader2D, mLog);
#else
                mBackground->initialize(mScreen, mShader, mShader2D);
#endif
                unsigned char* countryImg = static_cast<unsigned char*>(game->getData(DATA_CHAR, DATA_KEY_COUNTRYIMG));
                switch (*countryImg) {

                    case 1: mTexCountry = TEXTURE_ID_COUNTRY1; break;
                    case 2: mTexCountry = TEXTURE_ID_COUNTRY2; break;
                    case 3: mTexCountry = TEXTURE_ID_COUNTRY3; break;
#ifdef DEBUG
                    default: {

                        LOGE(LOG_FORMAT(" - Unexpected country image ID"), __PRETTY_FUNCTION__, __LINE__);
                        assert(NULL);
                        break;
                    }
#endif
                }
                ++(*countryImg);
                if ((*countryImg) > 3)
                    *countryImg = 1;

                mBackground->start(mTexCountry);

                float texCoords[8] = {0};
                texCoords[0] = screenRatio / 2.f;
                texCoords[2] = texCoords[0];
                texCoords[3] = 1.f;
                texCoords[4] = 1.f - texCoords[0];
                texCoords[5] = 1.f;
                texCoords[6] = texCoords[4];
                mBackground->setTexCoords(texCoords);
            }
            else
                mBackground->resume(mTexCountry);

            mBackground->setAlpha(1.f);
            break;
        }
        case OBJ_ID_CRYSTAL: {

            if (!mCrystal) {

                mCrystal = new Sphere(false);
#ifdef DEBUG
                mCrystal->initialize(mScreen, mShader, mShader2D, mLog);
#else
                mCrystal->initialize(mScreen, mShader, mShader2D);
#endif
                mCrystal->start(mBackground->getTextureIdx());

                mCrystal->rotate(127.5f * PI_F / 180.f);
                if (charHeight < minCharH) {
	
                    mScale = (mScreen->height - (minCharH << 1)) / static_cast<float>((mScreen->height - (charHeight << 1)));
                    charHeight = minCharH;
                }
            }
            else
                mCrystal->resume(mBackground->getTextureIdx());

            mCrystal->scale(ALPHA_VELOCITY, ALPHA_VELOCITY);
            mCrystal->setAlpha(0.f);
            break;
        }
        case OBJ_ID_BALL: {

            if (!mBall) {

                mBall = new Static2D;
#ifdef DEBUG
                mBall->initialize(mScreen, mShader, mShader2D, mLog);
#else
                mBall->initialize(mScreen, mShader, mShader2D);
#endif
                mBall->start(TEXTURE_ID_CRYSTAL);

                float texCoords[8] = {0};
                texCoords[1] = TC_TOP_BALL / TEX_CRYSTAL_SIZE;
                texCoords[3] = 1.f;
                texCoords[4] = TC_RIGHT_BALL / TEX_CRYSTAL_SIZE;
                texCoords[5] = 1.f;
                texCoords[6] = texCoords[4];
                texCoords[7] = texCoords[1];
                mBall->setTexCoords(texCoords);
                mBall->setVertices(0, charHeight, mScreen->width, 0);
            }
            else
                mBall->resume(TEXTURE_ID_CRYSTAL);

            mBall->setAlpha(0.f);
            break;
        }
        case OBJ_ID_QUERY: {

            if (!mQuery) {

                mQuery = new Element2D;
#ifdef DEBUG
                mQuery->initialize(mScreen, mShader, mShader2D, mLog);
#else
                mQuery->initialize(mScreen, mShader, mShader2D);
#endif
                mQuery->start(TEXTURE_ID_QUERY);
                mQuery->setTexCoords(FULL_TEXCOORD_BUFFER);

                mQuery->setVertices(static_cast<short>(mScreen->width * 0.1f),
                        static_cast<short>(mScreen->height * 0.9f) - static_cast<short>(charHeight * 0.8f),
                        static_cast<short>(mScreen->width * 0.9f),
                        static_cast<short>(mScreen->height * 0.1f) + static_cast<short>(charHeight * 0.8f));
            }
            else
                mQuery->resume(TEXTURE_ID_QUERY);

            mQuery->scale(ALPHA_VELOCITY, ALPHA_VELOCITY);
            mQuery->setAlpha(0.f);
            break;
        }
        case OBJ_ID_C: {

            if (!mC) {

                mC = new Static2D;
#ifdef DEBUG
                mC->initialize(mScreen, mShader, mShader2D, mLog);
#else
                mC->initialize(mScreen, mShader, mShader2D);
#endif
                mC->start(TEXTURE_ID_TOPA);
                mC->setTexCoords(FULL_TEXCOORD_BUFFER);

                charHeight = mScreen->height - charHeight;
                mC->setVertices(0, mScreen->height, mScreen->width / 7, charHeight);
            }
            else
                mC->resume(TEXTURE_ID_TOPA);

            mC->setAlpha(0.f);
            break;
        }
        case OBJ_ID_R: {

            if (!mR) {

                mR = new Static2D;
#ifdef DEBUG
                mR->initialize(mScreen, mShader, mShader2D, mLog);
#else
                mR->initialize(mScreen, mShader, mShader2D);
#endif
                mR->start(TEXTURE_ID_CRYSTAL);

                float texCoords[8] = {0};
                texCoords[0] = TC_LEFT_R / TEX_CRYSTAL_SIZE;
                texCoords[2] = texCoords[0];
                texCoords[3] = TC_BOTTOM_R / TEX_CRYSTAL_SIZE;
                texCoords[4] = 1.f;
                texCoords[5] = texCoords[3];
                texCoords[6] = 1.f;
                mR->setTexCoords(texCoords);
                mR->setVertices(mScreen->width / 7, mScreen->height, (mScreen->width << 1) / 7,
                        charHeight);
            }
            else
                mR->resume(TEXTURE_ID_CRYSTAL);

            mR->setAlpha(0.f);
            break;
        }
        case OBJ_ID_Y: {

            if (!mY) {

                mY = new Static2D;
#ifdef DEBUG
                mY->initialize(mScreen, mShader, mShader2D, mLog);
#else
                mY->initialize(mScreen, mShader, mShader2D);
#endif
                mY->start(TEXTURE_ID_TOPB);

                float texCoords[8] = {0};
                texCoords[3] = TC_BOTTOM_Y / TEX_TOPB_HEIGHT;
                texCoords[4] = 1.f;
                texCoords[5] = texCoords[3];
                texCoords[6] = 1.f;
                mY->setTexCoords(texCoords);
                mY->setVertices((mScreen->width << 1) / 7, mScreen->height,
                        (mScreen->width * 3) / 7, charHeight);
            }
            else
                mY->resume(TEXTURE_ID_TOPB);

            mY->setAlpha(0.f);
            break;
        }
        case OBJ_ID_S: {

            if (!mS) {

                mS = new Static2D;
#ifdef DEBUG
                mS->initialize(mScreen, mShader, mShader2D, mLog);
#else
                mS->initialize(mScreen, mShader, mShader2D);
#endif
                mS->start(TEXTURE_ID_TOPB);

                float texCoords[8] = {0};
                texCoords[1] = TC_TOP_S / TEX_TOPB_HEIGHT;
                texCoords[3] = TC_BOTTOM_S / TEX_TOPB_HEIGHT;
                texCoords[4] = 1.f;
                texCoords[5] = texCoords[3];
                texCoords[6] = 1.f;
                texCoords[7] = texCoords[1];
                mS->setTexCoords(texCoords);
                mS->setVertices((mScreen->width * 3) / 7, mScreen->height,
                        (mScreen->width << 2) / 7, charHeight);
            }
            else
                mS->resume(TEXTURE_ID_TOPB);

            mS->setAlpha(0.f);
            break;
        }
        case OBJ_ID_T: {

            if(!mT) {

                mT = new Static2D;
#ifdef DEBUG
                mT->initialize(mScreen, mShader, mShader2D, mLog);
#else
                mT->initialize(mScreen, mShader, mShader2D);
#endif
                mT->start(TEXTURE_ID_TOPB);

                float texCoords[8] = {0};
                texCoords[1] = TC_TOP_T / TEX_TOPB_HEIGHT;
                texCoords[3] = TC_BOTTOM_T / TEX_TOPB_HEIGHT;
                texCoords[4] = 1.f;
                texCoords[5] = texCoords[3];
                texCoords[6] = 1.f;
                texCoords[7] = texCoords[1];
                mT->setTexCoords(texCoords);
                mT->setVertices((mScreen->width << 2) / 7, mScreen->height,
                        (mScreen->width * 5) / 7, charHeight);
            }
            else
                mT->resume(TEXTURE_ID_TOPB);

            mT->setAlpha(0.f);
            break;
        }
        case OBJ_ID_A: {

            if (!mA) {

                mA = new Static2D;
#ifdef DEBUG
                mA->initialize(mScreen, mShader, mShader2D, mLog);
#else
                mA->initialize(mScreen, mShader, mShader2D);
#endif
                mA->start(TEXTURE_ID_CRYSTAL);

                float texCoords[8] = {0};
                texCoords[0] = TC_LEFT_A / TEX_CRYSTAL_SIZE;
                texCoords[1] = TC_TOP_A / TEX_CRYSTAL_SIZE;
                texCoords[2] = texCoords[0];
                texCoords[3] = TC_BOTTOM_A / TEX_CRYSTAL_SIZE;
                texCoords[4] = 1.f;
                texCoords[5] = texCoords[3];
                texCoords[6] = 1.f;
                texCoords[7] = texCoords[1];
                mA->setTexCoords(texCoords);
                mA->setVertices((mScreen->width * 5) / 7, mScreen->height,
                        (mScreen->width * 6) / 7, charHeight);
            }
            else
                mA->resume(TEXTURE_ID_CRYSTAL);

            mA->setAlpha(0.f);
            break;
        }
        case OBJ_ID_L: {

            if (!mL) {

                mL = new Static2D;
#ifdef DEBUG
                mL->initialize(mScreen, mShader, mShader2D, mLog);
#else
                mL->initialize(mScreen, mShader, mShader2D);
#endif
                mL->start(TEXTURE_ID_CRYSTAL);

                float texCoords[8] = {0};
                texCoords[0] = TC_LEFT_L / TEX_CRYSTAL_SIZE;
                texCoords[1] = TC_TOP_L / TEX_CRYSTAL_SIZE;
                texCoords[2] = texCoords[0];
                texCoords[3] = TC_BOTTOM_L / TEX_CRYSTAL_SIZE;
                texCoords[4] = 1.f;
                texCoords[5] = texCoords[3];
                texCoords[6] = 1.f;
                texCoords[7] = texCoords[1];
                mL->setTexCoords(texCoords);
                mL->setVertices((mScreen->width * 6) / 7, mScreen->height, mScreen->width,
                        charHeight);
            }
            else
                mL->resume(TEXTURE_ID_CRYSTAL);

            mL->setAlpha(0.f);
            break;
        }
#ifdef DEBUG
        default: {

            LOGD(LOG_LEVEL_TITLE, 0, LOG_FORMAT(" - Unknown item ID: %d"), __PRETTY_FUNCTION__, __LINE__, objectId);
            assert(NULL);
            break;
        }
#endif
    }
}

void Title::pause() {

    LOGV(LOG_LEVEL_TITLE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);

    if (mBackground) mBackground->pause();
    if (mCrystal) mCrystal->pause();
    if (mQuery) mQuery->pause();
    if (mC) mC->pause();
    if (mR) mR->pause();
    if (mY) mY->pause();
    if (mS) mS->pause();
    if (mT) mT->pause();
    if (mA) mA->pause();
    if (mL) mL->pause();
    if (mBall) mBall->pause();
}

void Title::render(bool resetUniform) const {

#ifdef DEBUG
    LOGV(LOG_LEVEL_TITLE, (*mLog % 100), LOG_FORMAT(" - r:%s"), __PRETTY_FUNCTION__, __LINE__,
            (resetUniform)? "true":"false");
    assert(mBackground);
    assert(mCrystal);
    assert(mQuery);
    assert(mC);
    assert(mR);
    assert(mY);
    assert(mS);
    assert(mT);
    assert(mA);
    assert(mL);
    assert(mBall);
#endif

    mBackground->render(resetUniform);

    mCrystal->render(true);
    mQuery->render(true);

    mC->render(true);
    mR->render(false);
    mY->render(false);
    mS->render(false);
    mT->render(false);
    mA->render(false);
    mL->render(false);
    mBall->render(false);
}
