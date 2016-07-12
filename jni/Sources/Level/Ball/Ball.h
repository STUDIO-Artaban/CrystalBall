#ifndef BALL_H_
#define BALL_H_

#include "Global.h"

#ifdef __ANDROID__
#include "Level/Sphere/Sphere.h"
#else
#include "Sphere.h"
#endif
#include <libeng/Graphic/Object/2D/Static2D.h>
#include <libeng/Features/Mic/Mic.h>
#include <libeng/Tools/Tools.h>

#define MAX_BALL_COLOR      0.8f // Maximum ball color ratio
#define DARK_BALL_COLOR     0.2f
#define DELTA_REF_DARK      15490L

#define FAST_VELOCITY       2
#define SLOW_VELOCITY       20

#ifdef __ANDROID__
#define MIN_DB_VOICE        45.f // Used to determine if the user is talking
#define DELAY_ANSWER        (3 * 50) // 3 seconds: 3 * 60|50 FPS
#else
#define MIN_DB_VOICE        28.f
#define DELAY_ANSWER        (4 * 60)
#endif

using namespace eng;

//////
class Ball : public Sphere {

private:
    Mic* mMic;
    Delay mDelay;

    Static2D* mLight;
    unsigned char mLightIdx;

    enum {

        READY = 0,
        WAIT_SILENCE,
        QUERYING,
        WAIT_QUERY

    };
    unsigned char mStep;
    inline void updateStep(float db, unsigned char &delay) {
        switch (mStep) {

            case WAIT_QUERY: {
#ifdef DEBUG
                if (db > MIN_DB_VOICE) {
                    LOGI(LOG_LEVEL_BALL, 0, LOG_FORMAT(" - Start querying"), __PRETTY_FUNCTION__, __LINE__);
                    --mStep;
                }
#else
                if (db > MIN_DB_VOICE)
                    --mStep;
#endif
                break;
            }
            case QUERYING: {
                if (db < MIN_DB_VOICE) {
                    LOGI(LOG_LEVEL_BALL, 0, LOG_FORMAT(" - Make silence"), __PRETTY_FUNCTION__, __LINE__);
                    --mStep;
                    delay = 0;
                }
                break;
            }
            case WAIT_SILENCE: {
#ifdef DEBUG
                if (db > MIN_DB_VOICE) {
                    LOGI(LOG_LEVEL_BALL, 0, LOG_FORMAT(" - Querying"), __PRETTY_FUNCTION__, __LINE__);
                    ++mStep;
                }
                else if ((++delay) > DELAY_ANSWER) {
                    LOGI(LOG_LEVEL_BALL, 0, LOG_FORMAT(" - Ready"), __PRETTY_FUNCTION__, __LINE__);
                    --mStep;
                }
#else
                if (db > MIN_DB_VOICE)
                    ++mStep;
                else if ((++delay) > DELAY_ANSWER)
                    --mStep;
#endif
                break;
            }
            //case READY: break;
        }
    }
    inline void updateColor(float red, float green, float blue) {

        setRed(getRed() - ((getRed() - red) / ((getRed() > red)? SLOW_VELOCITY:FAST_VELOCITY)));
        setGreen(getGreen() - ((getGreen() - green) / ((getGreen() > green)? SLOW_VELOCITY:FAST_VELOCITY)));
        setBlue(getBlue() - ((getBlue() - blue) / ((getBlue() > blue)? SLOW_VELOCITY:FAST_VELOCITY)));
    }

public:
    Ball();
    virtual ~Ball();

    inline void reset() {

        LOGV(LOG_LEVEL_BALL, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
        mStep = WAIT_QUERY;
    }
    inline bool isReady() const { return !mStep; } // mStep == READY
    inline bool darken(clock_t delta) {

        LOGV(LOG_LEVEL_BALL, 0, LOG_FORMAT(" - d:%u"), __PRETTY_FUNCTION__, __LINE__, delta);
        assert(delta);

        setRed(getRed() - ((getRed() - 0.f) / (SLOW_VELOCITY * DELTA_REF_DARK / delta)));
        setGreen(getGreen() - ((getGreen() - 0.f) / (SLOW_VELOCITY * DELTA_REF_DARK / delta)));
        setBlue(getBlue() - ((getBlue() - 0.f) / (SLOW_VELOCITY * DELTA_REF_DARK / delta)));

        return (getRed() < (DARK_BALL_COLOR + 0.0001f));
    }
    unsigned char lighten(clock_t delta); // Return 0 when done

    //////
    void start(const ShaderParam* shader);
    inline void stop() { // pause

        LOGV(LOG_LEVEL_BALL, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
        Object2D::pause();

        if (mLight) mLight->pause();
        mLightIdx = 0;
    }
    void resume();

    void update(const Game* game, const Level* level);
    void render() const;

};

#endif // BALL_H_
