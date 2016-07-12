#include "Ball.h"

#define MAX_DB_LEVEL            200.f

#define MID_DB_VOICE            70.f
#define MAX_DB_VOICE            90.f

#define DB_CLIP(db)             ((db > MAX_DB_LEVEL)? MAX_DB_LEVEL:db)
#ifdef __ANDROID__
#define DB_TO_COLOR             0.01f
#else
#define DB_TO_COLOR             0.06f
#endif

#define LIGHT_BALL_COLOR        2.f
#define LIGHT_END_IDX           INCREASE_BUFFER_SIZE + 5

// Delay(s)
#ifdef DEBUG
#define DDELAY_OF_LIGHT         ((6 * 50) / INCREASE_BUFFER_SIZE) // 6 sceonds: 6 * 60|50 FPS / INCREASE_BUFFER_SIZE
#endif
#define DELAY_OF_LIGHT          50857.f

// Texture IDs
#define TEXTURE_ID_ENERGY       9

//////
Ball::Ball() : Sphere(true), mStep(0), mLightIdx(0) {

    LOGV(LOG_LEVEL_BALL, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    mMic = Mic::getInstance();

    mLight = NULL;
}
Ball::~Ball() {

    LOGV(LOG_LEVEL_BALL, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    if (mLight) delete mLight;
}

unsigned char Ball::lighten(clock_t delta) {

    LOGV(LOG_LEVEL_BALL, 0, LOG_FORMAT(" - d:%u"), __PRETTY_FUNCTION__, __LINE__, delta);
#ifdef DEBUG
    if (!mDelay.isElapsed(delta, DDELAY_OF_LIGHT, DELAY_OF_LIGHT, __PRETTY_FUNCTION__, __LINE__))
#else
    if (!mDelay.isElapsed(delta, DELAY_OF_LIGHT))
#endif
        return (mLightIdx)? mLightIdx:1;

    ++mLightIdx;

    assert(mLight);
    if (mLightIdx < INCREASE_BUFFER_SIZE)
        mLight->setAlpha(IncreaseBuffer[mLightIdx]);
    else if (mLightIdx == LIGHT_END_IDX) {

        LOGI(LOG_LEVEL_BALL, 0, LOG_FORMAT(" - Lighten"), __PRETTY_FUNCTION__, __LINE__);
        setRed(MAX_BALL_COLOR);
        setGreen(MAX_BALL_COLOR);
        setBlue(MAX_BALL_COLOR);

        mLightIdx = 0;
        return 0;
    }
    return mLightIdx;
}

void Ball::start(const ShaderParam* shader) {

    LOGV(LOG_LEVEL_BALL, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    assert(Camera::getInstance()->getCamTexIdx() != TEXTURE_IDX_INVALID);

    Sphere::start(Camera::getInstance()->getCamTexIdx());

    mLight = new Static2D;
#ifdef DEBUG
    mLight->initialize(mScreen, shader, getShader2D(), mLog);
#else
    mLight->initialize(mScreen, shader, getShader2D());
#endif
    mLight->start(TEXTURE_ID_ENERGY);
    mLight->setTexCoords(FULL_TEXCOORD_BUFFER);
    mLight->setVertices(1, (mScreen->height >> 1) + (mScreen->width >> 1) - 1, mScreen->width - 1,
            (mScreen->height >> 1) - (mScreen->width >> 1) + 1);
#ifndef __ANDROID__
    setBGRA(true);
#endif
}
void Ball::resume() {

    LOGV(LOG_LEVEL_BALL, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    assert(Camera::getInstance()->getCamTexIdx() != TEXTURE_IDX_INVALID);

    Object2D::resume(Camera::getInstance()->getCamTexIdx());
    if (mLight) mLight->resume(TEXTURE_ID_ENERGY);
}

void Ball::update(const Game* game, const Level* level) {

    static unsigned char delay = 0;

#ifdef DEBUG
    LOGV(LOG_LEVEL_BALL, (*game->getLog() % 100), LOG_FORMAT(" - g:%x; l:%x"), __PRETTY_FUNCTION__, __LINE__, game, level);
#endif
    if (mMic->getLevel() < 0.f)
        return;

    float micLevel = DB_CLIP(mMic->getLevel());
    updateStep(micLevel, delay);

    static unsigned char colorID = 0;

    static float redVel = 0;
    static float greenVel = 0;
    static float blueVel = 0;

    if (micLevel < MIN_DB_VOICE) {

        redVel = greenVel = blueVel = MAX_BALL_COLOR + (micLevel * DB_TO_COLOR);
        updateColor(redVel, greenVel, blueVel);
        colorID = std::rand() % 6;
        return;
    }
    if (micLevel < MID_DB_VOICE)
        colorID = std::rand() % 6;

    if (micLevel == MAX_DB_LEVEL) {

        redVel = greenVel = blueVel = LIGHT_BALL_COLOR;
        updateColor(LIGHT_BALL_COLOR, LIGHT_BALL_COLOR, LIGHT_BALL_COLOR);
    }
    else switch (colorID) {

        case 0: redVel = greenVel = MAX_BALL_COLOR + (micLevel * DB_TO_COLOR); break; // Yellow
        case 1: redVel = MAX_BALL_COLOR + (micLevel * DB_TO_COLOR); break; // Red
        case 2: redVel = blueVel = MAX_BALL_COLOR + (micLevel * DB_TO_COLOR); break; // Purple
        case 3: greenVel = MAX_BALL_COLOR + (micLevel * DB_TO_COLOR); break; // Green
        case 4: greenVel = blueVel = MAX_BALL_COLOR + (micLevel * DB_TO_COLOR); break; // Turquoise
        case 5: blueVel = MAX_BALL_COLOR + (micLevel * DB_TO_COLOR); break; // Blue
    }
    updateColor(redVel, greenVel, blueVel);
}
void Ball::render() const {

    glDisable(GL_BLEND);
    Sphere::render(true);
    glEnable(GL_BLEND);

    if (mLightIdx)
        mLight->render(false);
}
