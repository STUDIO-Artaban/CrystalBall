#ifndef TITLE_H_
#define TITLE_H_

#include <libeng/Graphic/Object/2D/Element2D.h>
#include "Global.h"

#ifdef __ANDROID__
#include "Level/Sphere/Sphere.h"
#else
#include "Sphere.h"
#endif

#define ALPHA_VELOCITY      0.005f
#define DELTA_REF_TITLE     16220L

// Texture IDs
#define TEXTURE_ID_CRYSTAL  8

// 'TEXTURE_ID_CRYSTAL' texture size
#define TEX_CRYSTAL_SIZE    1024.f

using namespace eng;

//////
class Title {

public:
    // Title object IDs
    enum {

        OBJ_ID_BACKGROUND = 3, // For value 3 reason see 'CrystalLevel::loaded' method
        OBJ_ID_CRYSTAL,
        OBJ_ID_BALL,
        OBJ_ID_QUERY,
        OBJ_ID_C,
        OBJ_ID_R,
        OBJ_ID_Y,
        OBJ_ID_S,
        OBJ_ID_T,
        OBJ_ID_A,
        OBJ_ID_L
    };

private:
    const Screen* mScreen;
    const ShaderParam* mShader;
    const ShaderParam2D* mShader2D;
#ifdef DEBUG
    const unsigned int* mLog;
#endif

    unsigned char mTexCountry;
    Back2D* mBackground;

    Sphere* mCrystal;
    Element2D* mQuery;
    float mScale;

    Static2D* mC;
    Static2D* mR;
    Static2D* mY;
    Static2D* mS;
    Static2D* mT;
    Static2D* mA;
    Static2D* mL;

    Static2D* mBall;

public:
    Title();
    virtual ~Title();

#ifdef DEBUG
    inline void initialize(const Screen* screen, const ShaderParam* shader, const ShaderParam2D* shader2D,
            const unsigned int* log) {

        LOGV(LOG_LEVEL_TITLE, 0, LOG_FORMAT(" - s:%x; s1:%x; s2:%x; l:%l"), __PRETTY_FUNCTION__, __LINE__, screen, shader,
                shader2D, log);
#else
    inline void initialize(const Screen* screen, const ShaderParam* shader, const ShaderParam2D* shader2D) {
#endif
        mScreen = screen;
        mShader = shader;
        mShader2D = shader2D;

#ifdef DEBUG
        mLog = log;
#endif
    }

    //
    inline float getScale() const { return mScale; }
    inline float showTitle(float alpha) {

        LOGV(LOG_LEVEL_TITLE, 0, LOG_FORMAT(" - a:%f"), __PRETTY_FUNCTION__, __LINE__, alpha);
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

        mCrystal->setAlpha(alpha);
        mQuery->setAlpha(alpha);
        mC->setAlpha(alpha);
        mR->setAlpha(alpha);
        mY->setAlpha(alpha);
        mS->setAlpha(alpha);
        mT->setAlpha(alpha);
        mA->setAlpha(alpha);
        mL->setAlpha(alpha);
        mBall->setAlpha(alpha);

        // Increase scale from 'ALPHA_VELOCITY' to 'mScale'
        float scale = (mScale - ALPHA_VELOCITY) * alpha;
        mQuery->scale(scale, scale);
        mCrystal->scale(scale * MAX_SPHERE_SCALE, scale * MAX_SPHERE_SCALE);

        return scale;
    }
    inline float hideTitle() {

        LOGV(LOG_LEVEL_TITLE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
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

        mQuery->setAlpha(mQuery->getAlpha() - ALPHA_VELOCITY);
        mC->setAlpha(mQuery->getAlpha());
        mR->setAlpha(mQuery->getAlpha());
        mY->setAlpha(mQuery->getAlpha());
        mS->setAlpha(mQuery->getAlpha());
        mT->setAlpha(mQuery->getAlpha());
        mA->setAlpha(mQuery->getAlpha());
        mL->setAlpha(mQuery->getAlpha());
        mBall->setAlpha(mQuery->getAlpha());

        if (!(mScale < 1.f))
            return (1.f - mQuery->getAlpha());

        // Increase scale from 'mScale' to 1
        float scale = 1.f - mQuery->getAlpha() + (mScale * mQuery->getAlpha());
        mQuery->scale(scale, scale);
        mCrystal->scale(scale * MAX_SPHERE_SCALE, scale * MAX_SPHERE_SCALE);

        return scale;
    }
    inline bool hideBall(clock_t delta) {

        LOGV(LOG_LEVEL_TITLE, 0, LOG_FORMAT(" - d:%u"), __PRETTY_FUNCTION__, __LINE__, delta);
        assert(mBackground);
        assert(mCrystal);

        mBackground->setAlpha(mBackground->getAlpha() - (ALPHA_VELOCITY * delta / DELTA_REF_TITLE));
        mCrystal->setAlpha(mBackground->getAlpha());

        return (mBackground->getAlpha() < ALPHA_VELOCITY);
    }

    //
    void load(const Game* game, unsigned char objectId);
    void pause();

    void render(bool resetUniform) const;

};

#endif // TITLE_H_
