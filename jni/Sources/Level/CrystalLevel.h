#ifndef CRYSTAL_LEVEL_H_
#define CRYSTAL_LEVEL_H_

#include <libeng/Game/Level.h>
#include <libeng/Graphic/Progress/Progress.h>
#include <libeng/Graphic/Object/2D/Element2D.h>
#include <libeng/Tools/Tools.h>

#include <libeng/Features/Camera/Camera.h>
#include <libeng/Features/Mic/Mic.h>

#ifdef __ANDROID__
#include "Ball/Ball.h"
#include "Title/Title.h"
#include "Sphere/Sphere.h"
#include "Response/Response.h"

#else
#include "Ball.h"
#include "Title.h"
#include "Sphere.h"
#include "Response.h"

#endif

using namespace eng;

//////
class CrystalLevel : public Level {

private:
    Player* mPlayer;
    Camera* mCamera;
    Mic* mMic;
    Delay mDelay;

    enum { // Application step

        WAIT_INTRO = 1,
        DISPLAYING_INTRO,
        INTRO_DISPLAYED,
        CANCELLING_INTRO,
        DISPLAYING_START,
        START_DISPLAYED,
        CANCELLING_START,
        STARTED,
        WAIT_ANSWER,
        DARKEN_BALL,
        LIGHTEN_BALL,
        DISPLAYING_ANSWER,
        ANSWERED

    };
    unsigned char mStep;

    //
    Progress* mProgress;
    Title* mTitle;

    Static2D* mBackground;
    Response* mResponse;
    Ball* mBall;
    Element2D* mCrystal;

    void reduceVolume(unsigned char track, clock_t delta);

public:
    CrystalLevel(Game* game);
    virtual ~CrystalLevel();

    //////
    void init() { }
    void pause();

    void wait(float millis) { }

protected:
    bool loaded(const Game* game);
public:
    bool update(const Game* game);

    void renderLoad() const;
    void renderUpdate() const;

};

#endif // CRYSTAL_LEVEL_H_
