#include "CrystalLevel.h"
#include "Global.h"

#include <libeng/Game/2D/Game2D.h>
#ifdef __ANDROID__
#include <boost/math/tr1.hpp>
#else
#include <math.h>
#endif

#define CAM_WIDTH               640
#define CAM_HEIGHT              480

#define MAX_LOAD_STEP           49
#define CRYSTAL_SIZE            690 // Ball diameter in the texture (in pixel)
#define VOLUME_VELOCITY         0.005f // Note that the default sound volume is 0.5 and not 1 (see DEFAULT_VOLUME)
#define MAX_LOAD_PROG           MAX_LOAD_STEP - 17

#define DELTA_REF_CRYSTAL       15880L
#define DELTA_REF_VOLUME        14150L
#define DELTA_REF_BACK          13920L

// Delay(s)
#ifdef DEBUG
#define DDELAY_B4_INTRO         150
#define DDELAY_OF_INTRO         50
#define DDELAY_B4_ANSWER        50
#endif
#define DELAY_B4_INTRO          1323064.f
#define DELAY_OF_INTRO          643669.f
#define DELAY_B4_ANSWER         418536.f

// Sound IDs
#define SOUND_ID_INTRO          1
#define SOUND_ID_TOUCHED        2
#define SOUND_ID_RESPONSE       3
#define SOUND_ID_REPLIED        4

//////
CrystalLevel::CrystalLevel(Game* game) : Level(game), mStep(0) {

    LOGV(LOG_LEVEL_CRYSTALLEVEL, 0, LOG_FORMAT(" - g:%x"), __PRETTY_FUNCTION__, __LINE__, game);
    game->mAccelPrecision = 0.f;
    Inputs::InputUse = USE_INPUT_TOUCH | USE_INPUT_ACCEL;

    mCamera = Camera::getInstance();
    mPlayer = Player::getInstance();
    mMic = Mic::getInstance();

    mProgress = NULL;
    mTitle = new Title;
    mBackground = NULL;
    mResponse = NULL;
    mBall = NULL;
    mCrystal = NULL;
}
CrystalLevel::~CrystalLevel() {

    LOGV(LOG_LEVEL_CRYSTALLEVEL, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    assert(mTextures->getIndex(TEXTURE_ID_COLOR) != TEXTURE_IDX_INVALID);
    mTextures->delTexture(mTextures->getIndex(TEXTURE_ID_COLOR));

    if (mProgress) delete mProgress;
    if (mTitle) delete mTitle;
    if (mBackground) delete mBackground;
    if (mResponse) delete mResponse;
    if (mBall) delete mBall;
    if (mCrystal) delete mCrystal;
}

void CrystalLevel::pause() {

    LOGV(LOG_LEVEL_CRYSTALLEVEL, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    Level::pause();

    // Stop all sounds when pause/resume operation (if needed)
    if ((mPlayer->getIndex(SOUND_ID_INTRO) != SOUND_IDX_INVALID) && (mPlayer->getResumeFlag(0))) mPlayer->stop(0);
    if ((mPlayer->getIndex(SOUND_ID_TOUCHED) != SOUND_IDX_INVALID) && (mPlayer->getResumeFlag(1))) mPlayer->stop(1);
    if ((mPlayer->getIndex(SOUND_ID_RESPONSE) != SOUND_IDX_INVALID) && (mPlayer->getResumeFlag(2))) mPlayer->stop(2);
    if ((mPlayer->getIndex(SOUND_ID_REPLIED) != SOUND_IDX_INVALID) && (mPlayer->getResumeFlag(3))) mPlayer->stop(3);

    if (mProgress) mProgress->pause();
    if (mTitle) mTitle->pause();
    if (mBackground) mBackground->pause();
    if (mResponse) mResponse->stop(); // pause
    if (mBall) mBall->stop(); // pause
    if (mCrystal) mCrystal->pause();
    
#ifndef __ANDROID__
    if ((mStep > CANCELLING_START) && (mStep < DARKEN_BALL))
        mMic->stop();
#endif

    //////
    if (mStep < START_DISPLAYED)
        mStep = 0; // Redo all processus (excepted title displaying)
    else if (mStep < DISPLAYING_ANSWER)
        mStep = CANCELLING_INTRO; // Display start message
    else
        mStep = DISPLAYING_ANSWER; // Display previous answer
}

bool CrystalLevel::loaded(const Game* game) {

    LOGV(LOG_LEVEL_CRYSTALLEVEL, 0, LOG_FORMAT(" - l:%d"), __PRETTY_FUNCTION__, __LINE__, mLoadStep);
#ifdef __ANDROID__
    if ((mTitle) && (mLoadStep > 15))
        mPlayer->resume(); // Restart player (needed for lock/unlock operation)
#endif

    switch (mLoadStep) {
        case 1: {

            assert(mTextures->getIndex(TEXTURE_ID_COLOR) == TEXTURE_IDX_INVALID);
            mTextures->genTexture(mTextures->addTexColor());

            if (!mProgress) {

                mProgress = new Progress(false);
#ifdef DEBUG
                mProgress->initialize(game->getScreen(), game->getShader(), game2DVia(game)->getShader2D(), game->getLog());
#else
                mProgress->initialize(game->getScreen(), game->getShader(), game2DVia(game)->getShader2D());
#endif
                Progress::Info progInfo;
                progInfo.maxPos = (mTitle)? 18:(MAX_LOAD_STEP - 17);

                progInfo.doneRed = 0xff;
                progInfo.doneGreen = 0xff;
                progInfo.doneBlue = 0xff;

                progInfo.todoRed = 0x80;
                progInfo.todoGreen = 0x80;
                progInfo.todoBlue = 0x80;

                mProgress->start(&progInfo);
                mProgress->position(0, 2, game->getScreen()->width, 0);
            }
            else
                mProgress->resume();
            break;
        }
        case 2: {

            if (mTitle)
#ifdef DEBUG
                mTitle->initialize(game->getScreen(), game->getShader(), game2DVia(game)->getShader2D(), game->getLog());
#else
                mTitle->initialize(game->getScreen(), game->getShader(), game2DVia(game)->getShader2D());
#endif
           else {

               mCrystal->resume(TEXTURE_ID_CRYSTAL);
               mCrystal->setAlpha(0.f);

               mLoadStep = 16; // Do not display title
           }
           break;
        }
        case Title::OBJ_ID_BACKGROUND: // 3
        case Title::OBJ_ID_CRYSTAL:
        case Title::OBJ_ID_BALL:
        case Title::OBJ_ID_QUERY:
        case Title::OBJ_ID_C:
        case Title::OBJ_ID_R:
        case Title::OBJ_ID_Y:
        case Title::OBJ_ID_S:
        case Title::OBJ_ID_T:
        case Title::OBJ_ID_A:
        case Title::OBJ_ID_L: {

            mTitle->load(game, mLoadStep);
            break;
        }
        case 14: {

            if (!mCrystal) {

                mCrystal = new Element2D;
#ifdef DEBUG
                mCrystal->initialize(game->getScreen(), game->getShader(), game2DVia(game)->getShader2D(), game->getLog());
#else
                mCrystal->initialize(game->getScreen(), game->getShader(), game2DVia(game)->getShader2D());
#endif
                mCrystal->start(TEXTURE_ID_CRYSTAL);
                mCrystal->setVertices(0, (game->getScreen()->height - game->getScreen()->width) >> 1, game->getScreen()->width,
                        game->getScreen()->height - ((game->getScreen()->height - game->getScreen()->width) >> 1));

                float texCoords[8] = {0};
                texCoords[3] = CRYSTAL_SIZE / TEX_CRYSTAL_SIZE;
                texCoords[4] = texCoords[3];
                texCoords[5] = texCoords[3];
                texCoords[6] = texCoords[3];
                mCrystal->setTexCoords(texCoords);
                mCrystal->rotate(135.f * PI_F / 180.f);
            }
            else
                mCrystal->resume(TEXTURE_ID_CRYSTAL);

            mCrystal->setAlpha(0.f);
#ifndef __ANDROID__
            mCrystal->setRed(1.6f);
            mCrystal->setGreen(1.6f);
            mCrystal->setBlue(1.6f);
#endif
            break;
        }
        case 15: {

            mProgress->setMax(MAX_LOAD_STEP - 16);

            Player::loadSound(SOUND_ID_INTRO);
            mPlayer->resume(); // Start the player thread (usually done when finished to load a level)
            mPlayer->stop(0);
            mPlayer->play(0);
            break;
        }
        case 16: {

            if (mCrystal->getAlpha() < 1.f) {

                mCrystal->setAlpha(mCrystal->getAlpha() + ALPHA_VELOCITY);

                float crystalScale = mTitle->showTitle(mCrystal->getAlpha());
                mCrystal->scale(crystalScale, crystalScale);

                --mLoadStep;
            }
            break;
        }
        case 17: {

            if (!mBackground) {

                mBackground = new Static2D(false);
#ifdef DEBUG
                mBackground->initialize(game->getScreen(), game->getShader(), game2DVia(game)->getShader2D(), game->getLog());
#else
                mBackground->initialize(game->getScreen(), game->getShader(), game2DVia(game)->getShader2D());
#endif
                mCamera->start(CAM_WIDTH, CAM_HEIGHT);
                mBackground->start(mCamera->getCamTexIdx());

                short camRatio = (((game->getScreen()->height * CAM_HEIGHT) / CAM_WIDTH) - game->getScreen()->width) >> 1;
                mBackground->setVertices(-camRatio, game->getScreen()->height, game->getScreen()->width + camRatio, 0);

                float texCoords[8] = {0};
                texCoords[1] = CAM_HEIGHT / static_cast<float>((*mTextures)[mCamera->getCamTexIdx()]->height);
                texCoords[2] = CAM_WIDTH / static_cast<float>((*mTextures)[mCamera->getCamTexIdx()]->width);
                texCoords[3] = texCoords[1];
                texCoords[4] = texCoords[2];
                mBackground->setTexCoords(texCoords);
#ifndef __ANDROID__
                mBackground->setBGRA(true);
#endif
            }
            else
                mBackground->resume(mCamera->getCamTexIdx());

            mBackground->setAlpha(0.f);
            break;
        }
        case 18: {

            if (!mBall) {

                mBall = new Ball;
#ifdef DEBUG
                mBall->initialize(game->getScreen(), game->getShader(), game2DVia(game)->getShader2D(), game->getLog());
#else
                mBall->initialize(game->getScreen(), game->getShader(), game2DVia(game)->getShader2D());
#endif
                mBall->start(game->getShader());
                mBall->scale(MAX_SPHERE_SCALE, MAX_SPHERE_SCALE);
            }
            else
                mBall->resume();

            //mBall->setAlpha(0.f); // Blending disabled
            mBall->setRed(0.f);
            mBall->setGreen(0.f);
            mBall->setBlue(0.f);
            break;
        }
        case 19: {

            if (!mResponse) {

                mResponse = new Response;
#ifdef DEBUG
                mResponse->initialize(game->getScreen(), game->getShader(), game2DVia(game)->getShader2D(), game->getLog());
#else
                mResponse->initialize(game->getScreen(), game->getShader(), game2DVia(game)->getShader2D());
#endif
                mResponse->start();
            }
            if (mStep == DISPLAYING_ANSWER) {

                mResponse->force();
                mResponse->setAlpha(0.f);
            }
            else
                mResponse->reset();
            break;
        }
        case RESPONSE_ID_INTRO: // 20
        case RESPONSE_ID_START:

        case RESPONSE_ID_YES1:
        case RESPONSE_ID_YES2:
        case RESPONSE_ID_YES3:
        case RESPONSE_ID_YES4:
        case RESPONSE_ID_YES5:
        case RESPONSE_ID_YES6:
        case RESPONSE_ID_YES7:
        case RESPONSE_ID_YES8:
        case RESPONSE_ID_YES9:
        case RESPONSE_ID_YES10:
        case RESPONSE_ID_YES11:
        case RESPONSE_ID_MAYBE1:
        case RESPONSE_ID_MAYBE2:
        case RESPONSE_ID_MAYBE3:
        case RESPONSE_ID_MAYBE4:
        case RESPONSE_ID_MAYBE5:
        case RESPONSE_ID_NO1:
        case RESPONSE_ID_NO2: {

            if ((mLoadStep == RESPONSE_ID_INTRO) && (mStep == CANCELLING_INTRO))
                break; // No need to load intro texture

            mResponse->load(mLoadStep);
            break;
        }
        case 40: {

            if (!mTitle)
                break;

            float scaleHide = mTitle->hideTitle();
            if (scaleHide < 1.f) {

                if (mTitle->getScale() < 1.f)
                    mCrystal->scale(scaleHide, scaleHide);
                --mLoadStep;
            }
            else
                mCrystal->scale(1.f, 1.f);
            break;
        }
        case 41: Player::loadSound(SOUND_ID_TOUCHED); break;
        case 42: Player::loadSound(SOUND_ID_RESPONSE); break;
        case 43: Player::loadSound(SOUND_ID_REPLIED); break;

        case (RESPONSE_ID_NO3 + 4): // 4 steps after last response texture load
        case (RESPONSE_ID_NO4 + 4):
        case (RESPONSE_ID_NO5 + 4):
        case (RESPONSE_ID_NO6 + 4):
        case (RESPONSE_ID_NO7 + 4): {

            mResponse->load(mLoadStep - 4);
            break;
        }
        case MAX_LOAD_STEP: {

            delete mProgress;
            mProgress = NULL;

            if (mTitle) {

                mBackground->setAlpha(1.f);
                mBall->setRed(MAX_BALL_COLOR);
                mBall->setGreen(MAX_BALL_COLOR);
                mBall->setBlue(MAX_BALL_COLOR);
            }
            break;
        }
#ifdef DEBUG
        default: {

            LOGW(LOG_FORMAT(" - Unknown load step: %d"), __PRETTY_FUNCTION__, __LINE__, mLoadStep);
            assert(NULL);
            break;
        }
#endif
    }
    if (mLoadStep != MAX_LOAD_STEP) {

        unsigned char pos = mLoadStep;
        if (!mTitle) {
            if (mLoadStep == 1) pos = 0;
            else pos -= 16;
        }
        else if ((mTitle) && (mLoadStep > 14))
            pos -= 15;

        mProgress->set(pos);
        return false;
    }
    return true;
}
void CrystalLevel::renderLoad() const {

    assert(mProgress);
    if ((!mTitle) || ((mTitle) && (mLoadStep < 15))) {

        glDisable(GL_BLEND);
        mProgress->render(false);
        glEnable(GL_BLEND);

        return;
    }
    assert(mTitle);
    assert(mCrystal);

    mTitle->render(false);
    mCrystal->render(true);

    if (mLoadStep > 16) { // Display it after crystal ball in scale

        glDisable(GL_BLEND);
        mProgress->render(true);
        glEnable(GL_BLEND);
    }
}

void CrystalLevel::reduceVolume(unsigned char track, clock_t delta) {

    LOGV(LOG_LEVEL_CRYSTALLEVEL, 0, LOG_FORMAT(" - t:%d; d:%u"), __PRETTY_FUNCTION__, __LINE__, track, delta);
    if (mPlayer->getStatus(track) == AL_PLAYING) {

        float volume = mPlayer->getVolume(track) - (VOLUME_VELOCITY * delta / DELTA_REF_VOLUME);
        if (volume > 0.f)
            mPlayer->setVolume(track, volume);
    }
#ifdef DEBUG
    else {
        LOGI(LOG_LEVEL_CRYSTALLEVEL, 0, LOG_FORMAT(" - Track %d is not playing"), __PRETTY_FUNCTION__, __LINE__, track);
    }
#endif
}

bool CrystalLevel::update(const Game* game) {

#ifdef DEBUG
    LOGV(LOG_LEVEL_CRYSTALLEVEL, (*game->getLog() % 100), LOG_FORMAT(" - g:%x"), __PRETTY_FUNCTION__, __LINE__, game);
#endif
    mCamera->updateTexture();

    // Finish to load + Display intro (if needed)
    if (mTitle) {

        if (mTitle->hideBall(game->getDelta())) {

            delete mTitle;
            mTitle = NULL;
        }
        return true;
    }
    else if (mBackground->getAlpha() < 1.f) {

        mBackground->setAlpha(mBackground->getAlpha() + (ALPHA_VELOCITY * game->getDelta() / DELTA_REF_BACK));
        mCrystal->setAlpha(mBackground->getAlpha());

        mBall->setRed(mBackground->getAlpha() * MAX_BALL_COLOR);
        mBall->setGreen(mBackground->getAlpha() * MAX_BALL_COLOR);
        mBall->setBlue(mBackground->getAlpha() * MAX_BALL_COLOR);
        if (mStep == DISPLAYING_ANSWER) {

            mResponse->update(game, this);
            mResponse->setAlpha(mBackground->getAlpha() * MAX_ALPHA);
        }
        return true;
    }
    else if (mLoadStep == MAX_LOAD_STEP) { // Load finished...

        LOGI(LOG_LEVEL_CRYSTALLEVEL, 0, LOG_FORMAT(" - Load finished (step:%d)"), __PRETTY_FUNCTION__, __LINE__, mStep);
        ++mLoadStep;

        mResponse->update(game, this);
        if (mStep != CANCELLING_INTRO)
            ++mStep;

        mDelay.reset();
        return true;
    }
#ifdef __ANDROID__
    // Restart player (needed for lock/unlock operation)
    mPlayer->resume();
#endif

    // Touch...
    unsigned char touchCount = game->mTouchCount;
    while (touchCount--) {

        if (game->mTouchData[touchCount].Type != TouchInput::TOUCH_UP)
            continue;

#ifdef __ANDROID__
        if (boost::math::tr1::hypotf((game->getScreen()->width >> 1) - game->mTouchData[touchCount].X,
                (game->getScreen()->height >> 1) - game->mTouchData[touchCount].Y) > static_cast<float>(game->getScreen()->width >> 1))
#else
        if (hypotf((game->getScreen()->width >> 1) - game->mTouchData[touchCount].X,
                   (game->getScreen()->height >> 1) - game->mTouchData[touchCount].Y) > static_cast<float>(game->getScreen()->width >> 1))
#endif
            break; // Crystal ball not touched

        switch (mStep) {
            case DISPLAYING_INTRO: {

                LOGI(LOG_LEVEL_CRYSTALLEVEL, 0, LOG_FORMAT(" - Cancel intro"), __PRETTY_FUNCTION__, __LINE__);
                if (mResponse->getDisplaying())
                    mResponse->cancel();

                mStep += 2;
                break;
            }
            case INTRO_DISPLAYED: {

                LOGI(LOG_LEVEL_CRYSTALLEVEL, 0, LOG_FORMAT(" - Intro displayed"), __PRETTY_FUNCTION__, __LINE__);
                ++mStep;
                break;
            }
            case START_DISPLAYED: {

                LOGI(LOG_LEVEL_CRYSTALLEVEL, 0, LOG_FORMAT(" - Start"), __PRETTY_FUNCTION__, __LINE__);
                mResponse->cancel();
                ++mStep;
                break;
            }
            case WAIT_ANSWER: {

                LOGI(LOG_LEVEL_CRYSTALLEVEL, 0, LOG_FORMAT(" - Get response"), __PRETTY_FUNCTION__, __LINE__);
                mMic->stop();
                mPlayer->setVolume(2, DEFAULT_VOLUME);
                mPlayer->play(2);
                ++mStep;
                break;
            }
            case ANSWERED: {

                LOGI(LOG_LEVEL_CRYSTALLEVEL, 0, LOG_FORMAT(" - Retry..."), __PRETTY_FUNCTION__, __LINE__);
                assert(mResponse->getDisplaying());

                mResponse->cancel();
                mStep = CANCELLING_START;
                break;
            }
#ifdef DEBUG
            default: {

                LOGD(LOG_LEVEL_CRYSTALLEVEL, 0, LOG_FORMAT(" - Touched with step %d"), __PRETTY_FUNCTION__, __LINE__, mStep);
                break;
            }
#endif
        }
        mPlayer->play(1);
    }

    // Working...
    switch (mStep) {

        case WAIT_INTRO: {
#ifdef DEBUG
            if (mDelay.isElapsed(game->getDelta(), DDELAY_B4_INTRO, DELAY_B4_INTRO, __PRETTY_FUNCTION__, __LINE__)) {
#else
            if (mDelay.isElapsed(game->getDelta(), DELAY_B4_INTRO)) {
#endif
                LOGI(LOG_LEVEL_CRYSTALLEVEL, 0, LOG_FORMAT(" - Display intro"), __PRETTY_FUNCTION__, __LINE__);
                mResponse->display(RESPONSE_ID_INTRO);
                ++mStep;
            }
            break;
        }
        case DISPLAYING_INTRO: {
            if (mResponse->getAlpha() < MAX_ALPHA) {

                reduceVolume(0, game->getDelta());
                break; // Not displayed yet
            }
#ifdef DEBUG
            if (mDelay.isElapsed(game->getDelta(), DDELAY_OF_INTRO, DELAY_OF_INTRO, __PRETTY_FUNCTION__, __LINE__)) {
#else
            if (mDelay.isElapsed(game->getDelta(), DELAY_OF_INTRO)) {
#endif
                LOGI(LOG_LEVEL_CRYSTALLEVEL, 0, LOG_FORMAT(" - Intro displayed"), __PRETTY_FUNCTION__, __LINE__);
                mResponse->cancel();
                mPlayer->stop(0);
                ++mStep;
            }
            else
                reduceVolume(0, game->getDelta());
            break;
        }
        case CANCELLING_INTRO: {
            if (!mResponse->toRender()) {

                LOGI(LOG_LEVEL_CRYSTALLEVEL, 0, LOG_FORMAT(" - Display start"), __PRETTY_FUNCTION__, __LINE__);
                mResponse->display(RESPONSE_ID_START);
                ++mStep;
            }
            else
                reduceVolume(0, game->getDelta());
            break;
        }
        case DISPLAYING_START: {
            if (mResponse->getAlpha() < MAX_ALPHA) {

                reduceVolume(0, game->getDelta());
                break; // Not displayed yet
            }
            mPlayer->stop(0);
            ++mStep;
            break;
        }
        case CANCELLING_START: {
            if (!mResponse->toRender()) {

                LOGI(LOG_LEVEL_CRYSTALLEVEL, 0, LOG_FORMAT(" - Start getting query"), __PRETTY_FUNCTION__, __LINE__);
                mMic->start();
                ++mStep;
            }
            break;
        }
        case STARTED: {

#ifdef __ANDROID__
            mMic->start();
#endif
            mBall->update(game, this);
#ifdef DEBUG
            if (mDelay.isElapsed(game->getDelta(), DDELAY_B4_ANSWER, DELAY_B4_ANSWER, __PRETTY_FUNCTION__, __LINE__)) {
#else
            if (mDelay.isElapsed(game->getDelta(), DELAY_B4_ANSWER)) {
#endif
                LOGI(LOG_LEVEL_CRYSTALLEVEL, 0, LOG_FORMAT(" - Get query"), __PRETTY_FUNCTION__, __LINE__);
                mBall->reset();
                ++mStep;
            }
            break;
        }
        case WAIT_ANSWER: {

#ifdef __ANDROID__
            mMic->start();
#endif
            mBall->update(game, this);
            if (mBall->isReady()) {

                LOGI(LOG_LEVEL_CRYSTALLEVEL, 0, LOG_FORMAT(" - Stop getting query"), __PRETTY_FUNCTION__, __LINE__);
                mMic->stop();
                mPlayer->setVolume(2, DEFAULT_VOLUME);
                mPlayer->play(2);
                ++mStep;
            }
            break;
        }
        case DARKEN_BALL: {
#ifdef DEBUG
            if ((game->getDelta()) && (mBall->darken(game->getDelta()))) {

                LOGI(LOG_LEVEL_CRYSTALLEVEL, 0, LOG_FORMAT(" - Activate the ball power"), __PRETTY_FUNCTION__, __LINE__);
                ++mStep;
            }
#else
            if ((game->getDelta()) && (mBall->darken(game->getDelta())))
                ++mStep;
#endif
            else
                mCrystal->setAlpha(mBall->getRed() / MAX_BALL_COLOR);
            break;
        }
        case LIGHTEN_BALL: {

            unsigned char light = mBall->lighten(game->getDelta());
            if (!light) {

                LOGI(LOG_LEVEL_CRYSTALLEVEL, 0, LOG_FORMAT(" - Reply question"), __PRETTY_FUNCTION__, __LINE__);
                mResponse->reply(); // Reply

                ++mStep;

                reduceVolume(2, game->getDelta());
                mPlayer->play(3);
                mPlayer->stop(2);
                mCrystal->setAlpha(1.f);
            }
            else if (light > (INCREASE_BUFFER_SIZE - 1))
                reduceVolume(2, game->getDelta());
            else if (mCrystal->getAlpha() < 1.f)
                mCrystal->setAlpha(mCrystal->getAlpha() + (ALPHA_VELOCITY * game->getDelta() / (DELTA_REF_CRYSTAL >> 1)));
            break;
        }
        case DISPLAYING_ANSWER: {
            if (mResponse->getAlpha() < MAX_ALPHA)
                break; // Not displayed yet

            ++mStep;
            break;
        }
    }
    mResponse->update(game, this);
    return true;
}
void CrystalLevel::renderUpdate() const {

    mBackground->render(false);
    mBall->render();

    if (mResponse->toRender())
        mResponse->render(true);
    if (mTitle)
        mTitle->render(true);

    mCrystal->render(true);
}
