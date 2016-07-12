#ifndef RESPONSE_H_
#define RESPONSE_H_

#include "Global.h"

#ifdef __ANDROID__
#include "Level/Sphere/Sphere.h"
#else
#include "Sphere.h"
#endif
#include <libeng/Inputs/Accel/Accel.h>
#include <boost/random/mersenne_twister.hpp>

// Response item IDs
enum {

    RESPONSE_ID_INTRO = 20, // For value 20 reason see 'CrystalLevel::loaded' method
    RESPONSE_ID_START,

    RESPONSE_ID_YES1,
    RESPONSE_ID_YES2,
    RESPONSE_ID_YES3,
    RESPONSE_ID_YES4,
    RESPONSE_ID_YES5,
    RESPONSE_ID_YES6,
    RESPONSE_ID_YES7,
    RESPONSE_ID_YES8,
    RESPONSE_ID_YES9,
    RESPONSE_ID_YES10,
    RESPONSE_ID_YES11,
    RESPONSE_ID_MAYBE1,
    RESPONSE_ID_MAYBE2,
    RESPONSE_ID_MAYBE3,
    RESPONSE_ID_MAYBE4,
    RESPONSE_ID_MAYBE5,
    RESPONSE_ID_NO1,
    RESPONSE_ID_NO2,
    RESPONSE_ID_NO3,
    RESPONSE_ID_NO4,
    RESPONSE_ID_NO5,
    RESPONSE_ID_NO6,
    RESPONSE_ID_NO7

};
#define TEXIDX_BUFFER_SIZE  ((RESPONSE_ID_NO7 - RESPONSE_ID_INTRO) + 1)
#define MAX_ALPHA           0.6f

//////
class Response : public Sphere {

private:
    const float* mFullTexCoords; // Used to display the start message
    float* mReplyTexCoords; // Used to display everything excepted the start message

    bool mToRender;
    boost::random::mt19937 Responses;

    unsigned char mToDisplay; // Response item ID to display (0: Nothing to display)
    bool mCancel; // TRUE: Cancel what is displayed; FALSE: Let's displaying (if any)
    bool mForce; // TRUE: Force to display content immediately; FALSE: Use alpha & scale velocity

    float mScale;

    enum {

        PORTRAIT = 1,
        REV_PORTRAIT,
        LANDSCAPE,
        REV_LANDSCAPE,
        FLAT,
        REVERSED

    };
    unsigned char mOrientation;
    AccelData mCurAccel;
    float mRotateVel; // Rotation velocity
    float mRotation;

    unsigned char mTexIdx[TEXIDX_BUFFER_SIZE];

    float mTexLag; // Used to display everything excepted the start message
    float mTexLagVel; // Texture lag velocity
    inline void lag(float texLag) {

        for (short i = 1; i < TEXCOORD_BUFFER_SIZE; i += 2)
            mReplyTexCoords[i] -= texLag;
    }
    inline void resetLag() {

        lag(-mTexLag);
        mTexLag = 0.f;
        mTexLagVel = 0.f;
    }

public:
    Response();
    virtual ~Response();

    //
    inline bool toRender() const { return mToRender; } // Check if ready as well (when return false)
    inline unsigned char getDisplaying() const { return mToDisplay; }

    inline void display(unsigned char responseId) {

        LOGV(LOG_LEVEL_RESPONSE, 0, LOG_FORMAT(" - r:%d"), __PRETTY_FUNCTION__, __LINE__, responseId);
        assert((responseId >= RESPONSE_ID_INTRO) && (responseId <= RESPONSE_ID_NO7));
        assert(!mToRender);

        mToDisplay = responseId;
        mTextureIdx = mTexIdx[responseId - RESPONSE_ID_INTRO];
        if (responseId == RESPONSE_ID_START)
            mTexCoords = const_cast<float*>(mFullTexCoords);
        else {

            mTexCoords = mReplyTexCoords;
            resetLag();
        }
        mToRender = true;

        setAlpha(0.f);
        mScale = 0.00001f; // != 0.f coz using delta can add 0 to this value B4 scaling
    }
    inline void cancel() {

        LOGV(LOG_LEVEL_RESPONSE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
        mCancel = true;
        mForce = false;
    }
    inline bool isCancelled() const { return mCancel; }

    inline void reset() {

        LOGV(LOG_LEVEL_RESPONSE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
        mToDisplay = 0;
        mToRender = false;
        mCancel = false;
        mForce = false;
    }
    inline void force() { // Force to display content (no alpha & scale velocity)

        assert(mToDisplay);
        mForce = true;
    }
    void reply();

    //////
    void start();
    inline void stop() { // pause

        LOGV(LOG_LEVEL_RESPONSE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
        Object2D::pause();

        std::memset(mTexIdx, 0, TEXIDX_BUFFER_SIZE);
    }

    void load(unsigned char responseId);
    void update(const Game* game, const Level* level);

};

#endif // RESPONSE_H_
