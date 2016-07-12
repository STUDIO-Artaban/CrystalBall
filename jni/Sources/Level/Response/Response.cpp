#include "Response.h"

#ifdef __ANDROID__
#include "Level/Title/Title.h"
#else
#include "Title.h"
#endif
#include <libeng/Tools/Tools.h>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#define MAX_SCALE               0.8f
#define SCALE_VELOCITY          ((ALPHA_VELOCITY * MAX_SCALE) / MAX_ALPHA)
#define DELTA_REF               14068L
#define ROTATION_CORRECTION     ((-52.5f * PI_F) / 180.f)
#define VELOCITY                7.f // Increase this value will decrease speed changes (see use)
#define COMMON_TEXLAG           -0.120117f  // Used to determine the texture lag according the common device Z orientation
                                            // -> Not completely vertical
//#ifdef __ANDROID__
#define ACCEL_LIMITS_SUP        9.80665f // Android: = 'mAccelSensor.getMaximumRange' / 2  (Samsung Galaxy Tab2)
#define ROTATION_PRECISION      2.f
#define TEXLAG_PRECISION        5.f
//#else // iOS
//#endif
#define ACCEL_LIMITS_INF        -ACCEL_LIMITS_SUP
// Accelerometer limits when used in a normal way (without shaking the device)

#define ORIENTATION_SUP         (ACCEL_LIMITS_SUP / 2.f)
#define ORIENTATION_INF         -ORIENTATION_SUP

#define ACCEL_PRECISION         0.2f // Avoid to rotate when the device do not move (too specific)
#define ACCEL_CHANGED(XYZ, xyz) (((XYZ - ACCEL_PRECISION) > xyz) || ((XYZ + ACCEL_PRECISION) < xyz))
#define ACCEL_CLIP(xyz)         ((xyz > ACCEL_LIMITS_SUP)? ACCEL_LIMITS_SUP:(xyz < ACCEL_LIMITS_INF)? ACCEL_LIMITS_INF:xyz)
#define ACCEL_TO_ROTATION       (PI_F / ((4.f + ROTATION_PRECISION) * ORIENTATION_INF))
#define ACCEL_TO_TEXLAG         (1.f / ((3.f + TEXLAG_PRECISION) * ORIENTATION_SUP))

// Texture IDs
#define TEXTURE_ID_INTRO        10
#define TEXTURE_ID_START        11

#define TEXTURE_ID_YES1         12
#define TEXTURE_ID_YES2         13
#define TEXTURE_ID_YES3         14
#define TEXTURE_ID_YES4         15
#define TEXTURE_ID_YES5         16
#define TEXTURE_ID_YES6         17
#define TEXTURE_ID_YES7         18
#define TEXTURE_ID_YES8         19
#define TEXTURE_ID_YES9         20
#define TEXTURE_ID_YES10        21
#define TEXTURE_ID_YES11        22

#define TEXTURE_ID_MAYBE1       23
#define TEXTURE_ID_MAYBE2       24
#define TEXTURE_ID_MAYBE3       25
#define TEXTURE_ID_MAYBE4       26
#define TEXTURE_ID_MAYBE5       27

#define TEXTURE_ID_NO1          28
#define TEXTURE_ID_NO2          29
#define TEXTURE_ID_NO3          30
#define TEXTURE_ID_NO4          31
#define TEXTURE_ID_NO5          32
#define TEXTURE_ID_NO6          33
#define TEXTURE_ID_NO7          34

//////
Response::Response() : Sphere(false), mToRender(false), mToDisplay(0), mCancel(false), mForce(false), mScale(0.f),
        mOrientation(0), mRotateVel(0.f), mRotation(0.f), mTexLag(0.f), mTexLagVel(0.f),
        Responses(static_cast<unsigned int>(std::time(0))) {

    LOGV(LOG_LEVEL_RESPONSE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    std::memset(mTexIdx, 0, TEXIDX_BUFFER_SIZE);
    std::memset(&mCurAccel, 0, sizeof(AccelData));

    mFullTexCoords = mTexCoords;
    mReplyTexCoords = new float[TEXCOORD_BUFFER_SIZE];
    std::memcpy(mReplyTexCoords, mFullTexCoords, sizeof(float) * TEXCOORD_BUFFER_SIZE);
}
Response::~Response() {

    LOGV(LOG_LEVEL_RESPONSE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    delete [] mReplyTexCoords;
    for (unsigned char i = 0; i < TEXIDX_BUFFER_SIZE; ++i)
        if (mTexIdx[i] != 0)
            mTextures->delTexture(mTexIdx[i]);
}

void Response::start() {

    LOGV(LOG_LEVEL_RESPONSE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);

    std::memcpy(mTransform, Object2D::TransformBuffer, sizeof(float) * TRANSFORM_BUFFER_SIZE);

    // Make a correction on indices (that start from -50°)
    rotate(ROTATION_CORRECTION); // => -2.5° for texture coordinates correction (that start from -2.5°)
}
void Response::load(unsigned char responseId) {

    LOGV(LOG_LEVEL_RESPONSE, 0, LOG_FORMAT(" - r:%d"), __PRETTY_FUNCTION__, __LINE__, responseId);
    switch (responseId) {

        case RESPONSE_ID_INTRO:     mTexIdx[0] = Textures::loadTexture(TEXTURE_ID_INTRO); break;
        case RESPONSE_ID_START:     mTexIdx[1] = Textures::loadTexture(TEXTURE_ID_START); break;

        case RESPONSE_ID_YES1:      mTexIdx[2] = Textures::loadTexture(TEXTURE_ID_YES1); break;
        case RESPONSE_ID_YES2:      mTexIdx[3] = Textures::loadTexture(TEXTURE_ID_YES2); break;
        case RESPONSE_ID_YES3:      mTexIdx[4] = Textures::loadTexture(TEXTURE_ID_YES3); break;
        case RESPONSE_ID_YES4:      mTexIdx[5] = Textures::loadTexture(TEXTURE_ID_YES4); break;
        case RESPONSE_ID_YES5:      mTexIdx[6] = Textures::loadTexture(TEXTURE_ID_YES5); break;
        case RESPONSE_ID_YES6:      mTexIdx[7] = Textures::loadTexture(TEXTURE_ID_YES6); break;
        case RESPONSE_ID_YES7:      mTexIdx[8] = Textures::loadTexture(TEXTURE_ID_YES7); break;
        case RESPONSE_ID_YES8:      mTexIdx[9] = Textures::loadTexture(TEXTURE_ID_YES8); break;
        case RESPONSE_ID_YES9:      mTexIdx[10] = Textures::loadTexture(TEXTURE_ID_YES9); break;
        case RESPONSE_ID_YES10:     mTexIdx[11] = Textures::loadTexture(TEXTURE_ID_YES10); break;
        case RESPONSE_ID_YES11:     mTexIdx[12] = Textures::loadTexture(TEXTURE_ID_YES11); break;
        case RESPONSE_ID_MAYBE1:    mTexIdx[13] = Textures::loadTexture(TEXTURE_ID_MAYBE1); break;
        case RESPONSE_ID_MAYBE2:    mTexIdx[14] = Textures::loadTexture(TEXTURE_ID_MAYBE2); break;
        case RESPONSE_ID_MAYBE3:    mTexIdx[15] = Textures::loadTexture(TEXTURE_ID_MAYBE3); break;
        case RESPONSE_ID_MAYBE4:    mTexIdx[16] = Textures::loadTexture(TEXTURE_ID_MAYBE4); break;
        case RESPONSE_ID_MAYBE5:    mTexIdx[17] = Textures::loadTexture(TEXTURE_ID_MAYBE5); break;
        case RESPONSE_ID_NO1:       mTexIdx[18] = Textures::loadTexture(TEXTURE_ID_NO1); break;
        case RESPONSE_ID_NO2:       mTexIdx[19] = Textures::loadTexture(TEXTURE_ID_NO2); break;
        case RESPONSE_ID_NO3:       mTexIdx[20] = Textures::loadTexture(TEXTURE_ID_NO3); break;
        case RESPONSE_ID_NO4:       mTexIdx[21] = Textures::loadTexture(TEXTURE_ID_NO4); break;
        case RESPONSE_ID_NO5:       mTexIdx[22] = Textures::loadTexture(TEXTURE_ID_NO5); break;
        case RESPONSE_ID_NO6:       mTexIdx[23] = Textures::loadTexture(TEXTURE_ID_NO6); break;
        case RESPONSE_ID_NO7:       mTexIdx[24] = Textures::loadTexture(TEXTURE_ID_NO7); break;
#ifdef DEBUG
        default: {

            LOGW(LOG_FORMAT(" - Unknown response item to load (%d)"), __PRETTY_FUNCTION__, __LINE__, responseId);
            assert(NULL);
            return;
        }
#endif
    }
    assert(mTexIdx[responseId - RESPONSE_ID_INTRO] != 0);
    mTextures->genTexture(mTexIdx[responseId - RESPONSE_ID_INTRO]);

    if (mToDisplay == responseId) // Needed when resume
        mTextureIdx = mTexIdx[responseId - RESPONSE_ID_INTRO];
}

void Response::reply() {

    LOGV(LOG_LEVEL_RESPONSE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    boost::random::uniform_int_distribution<> Reply(RESPONSE_ID_YES1, RESPONSE_ID_NO7);
    display(static_cast<unsigned char>(Reply(Responses)));
}

void Response::update(const Game* game, const Level* level) {

#ifdef DEBUG
    LOGV(LOG_LEVEL_RESPONSE, (*game->getLog() % 100), LOG_FORMAT(" - g:%x; l:%x (r:%s)"), __PRETTY_FUNCTION__, __LINE__, game,
            level, (mToRender)? "true":"false");
#endif
    if (!mToRender)
        return;

    // Rotation according device position
    if (game->mAccelData) {

#ifdef __ANDROID__
#ifdef DEBUG
        const Input* accelInput = Inputs::getInstance()->get(Inputs::ACCEL);
        assert((*static_cast<const float*>(accelInput->getConfig())) < ((ACCEL_LIMITS_SUP * 2.f) + 1.f));
        assert((*static_cast<const float*>(accelInput->getConfig())) > ((ACCEL_LIMITS_SUP * 2.f) - 1.f));
#endif
        float x = ACCEL_CLIP(game->mAccelData->X);
        float y = ACCEL_CLIP(game->mAccelData->Y);
        float z = ACCEL_CLIP(game->mAccelData->Z);

#else
        // No maximum accelerometer value available on iOS
        // -> Using a common range of [-11;11] values (after having * 10)

        float x = ACCEL_CLIP(-game->mAccelData->X * 10.f);
        float y = ACCEL_CLIP(-game->mAccelData->Y * 10.f);
        float z = ACCEL_CLIP(-game->mAccelData->Z * 10.f);
#endif
        unsigned char orientation = 0;
        if ((ORIENTATION_INF < z) && (z < ORIENTATION_SUP)) {

            if ((ORIENTATION_INF < x) && (x < ORIENTATION_SUP)) orientation = (ORIENTATION_SUP < y)? PORTRAIT:REV_PORTRAIT;
            else if ((ORIENTATION_INF < y) && (y < ORIENTATION_SUP)) orientation = (ORIENTATION_SUP < x)? LANDSCAPE:REV_LANDSCAPE;
        }
        if ((!orientation) && (ORIENTATION_INF < x) && (x < ORIENTATION_SUP) &&
                (ORIENTATION_INF < y) && (y < ORIENTATION_SUP)) orientation = (ORIENTATION_SUP < z)? FLAT:REVERSED;

        unsigned char prevOrientation = mOrientation;
        if (orientation)
            mOrientation = orientation;

        switch (mOrientation) {
            default: {

                mOrientation = PORTRAIT;
                //break;
            }
            case PORTRAIT: { // Portrait (home button at bottom)

                if (prevOrientation == LANDSCAPE) {

                    mRotation -= (2.f * PI_F);
                    mRotateVel -= (2.f * PI_F);
                }
                else if (ACCEL_CHANGED(mCurAccel.X, x))
                    mRotateVel = x * ACCEL_TO_ROTATION;
                break;
            }
            case REV_PORTRAIT: { // Reversed portrait

                if (ACCEL_CHANGED(mCurAccel.X, x))
                    mRotateVel = (x * -ACCEL_TO_ROTATION) + PI_F;
                break;
            }
            case LANDSCAPE: { // Landscape (home button at right)

                if (prevOrientation == PORTRAIT) {

                    mRotation += (2.f * PI_F);
                    mRotateVel += (2.f * PI_F);
                }
                else if (ACCEL_CHANGED(mCurAccel.Y, y))
                    mRotateVel = (y * -ACCEL_TO_ROTATION) + ((3.f * PI_F) / 2.f);
                break;
            }
            case REV_LANDSCAPE: { // Reversed landscape

                if (ACCEL_CHANGED(mCurAccel.Y, y))
                    mRotateVel = (y * ACCEL_TO_ROTATION) + (PI_F / 2.f);
                break;
            }
            case FLAT: // Flat
            case REVERSED: { // Reversed

                if ((!ACCEL_CHANGED(mCurAccel.X, x)) && (!ACCEL_CHANGED(mCurAccel.Y, y)))
                    break;
                resetLag();

                static unsigned char prevPos = 0;
                if ((y > 0.f) && (x < 0.f)) {

                    mRotateVel = std::atan(-x / y);
                    prevPos = 0;
                }
                else if ((y < 0.f) && (x < 0.f)) {

                    mRotateVel = std::atan(y / x) + (PI_F / 2.f);
                    prevPos = 0;
                }
                else if ((y < 0.f) && (x > 0.f)) {

                    if (prevPos == 2)
                        mRotation += (2.f * PI_F);
                    mRotateVel = PI_F - std::atan(x / y);
                    prevPos = 1;
                }
                else if ((y > 0.f) && (x > 0.f)) { // 'else if' instead of 'else' coz 'y' must be > 0

                    if (prevPos == 1)
                        mRotation -= (2.f * PI_F);
                    mRotateVel = -std::atan(x / y);
                    prevPos = 2;
                }
                //else // 'x' & 'y' are equal to 0: Nothing to do
                break;
            }
        }
        if (ACCEL_CHANGED(mCurAccel.X, x)) mCurAccel.X = x;
        if (ACCEL_CHANGED(mCurAccel.Y, y)) mCurAccel.Y = y;
        if (ACCEL_CHANGED(mCurAccel.Z, z)) {

            if ((mToDisplay != RESPONSE_ID_START) && (mOrientation < FLAT))
                mTexLagVel = (z * ACCEL_TO_TEXLAG) + COMMON_TEXLAG;
            mCurAccel.Z = z;
        }

        if (mToDisplay != RESPONSE_ID_START) {

            float texLag = mTexLag - ((mTexLag - mTexLagVel) / VELOCITY);
            lag(texLag - mTexLag);
            mTexLag = texLag;
        }
        mRotation = mRotation - ((mRotation - mRotateVel) / VELOCITY);
        rotate(ROTATION_CORRECTION + mRotation);
    }

    // Hide & Display response
    if (mCancel) {

        if (getAlpha() < ALPHA_VELOCITY)
            reset();
        else
            setAlpha(getAlpha() - (ALPHA_VELOCITY * game->getDelta() / DELTA_REF));

        if (mScale < MAX_SCALE) {

            mScale += (SCALE_VELOCITY * game->getDelta() / DELTA_REF);
            scale(mScale, mScale);
        }
        return;
    }

    if (mForce) {

        mScale = MAX_SCALE;
        //setAlpha(MAX_ALPHA); // Let caller manage this
        scale(MAX_SCALE, MAX_SCALE);
        return;
    }
    if (getAlpha() < MAX_ALPHA) {

        setAlpha(getAlpha() + (ALPHA_VELOCITY * game->getDelta() / DELTA_REF));
        mScale += (SCALE_VELOCITY * game->getDelta() / DELTA_REF);
        scale(mScale, mScale);
    }
}
