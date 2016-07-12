#ifndef CRYSTAL_BALL_H_
#define CRYSTAL_BALL_H_

#include <libeng/Game/2D/Game2D.h>

#ifdef __ANDROID__
#include "Level/CrystalLevel.h"
#else
#include "CrystalLevel.h"
#endif

#define FILE_NAME               "CrystalBall.backup"

// Data key(s)
#define DATA_KEY_COUNTRYIMG     "CountryImage"  // Last country image displayed [1;3]

using namespace eng;

//////
class CrystalBall : public Game2D {

private:
    CrystalBall();
    virtual ~CrystalBall();

    CrystalLevel* mLevel;

public:
    static CrystalBall* getInstance() {
        if (!mThis)
            mThis = new CrystalBall;
        return static_cast<CrystalBall*>(mThis);
    }
    static void freeInstance() {
        if (mThis) {
            delete mThis;
            mThis = NULL;
        }
    }

    //////
    void init() { }

    bool start();
    void pause();
    void destroy();

    void wait(float millis);
    void update();

    void render() const;

};

#endif // CRYSTAL_BALL_H_
