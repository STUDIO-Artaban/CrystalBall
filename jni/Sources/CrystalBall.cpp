#include "CrystalBall.h"
#include "Global.h"

#include <libeng/Log/Log.h>
#include <libeng/Storage/Storage.h>
#include <boost/thread.hpp>

//////
CrystalBall::CrystalBall() : Game2D(0) {

    LOGV(LOG_LEVEL_CRYSTALBALL, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    if (!mGameLevel)
        mGameIntro = new Intro(Intro::LANG_EN);
    mGameData = new StoreData;
    Dynamic2D::initIncreaseBuffer();

    mLevel = NULL;
}
CrystalBall::~CrystalBall() {

    LOGV(LOG_LEVEL_CRYSTALBALL, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    Dynamic2D::freeIncreaseBuffer();
}

bool CrystalBall::start() {

    LOGV(LOG_LEVEL_CRYSTALBALL, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    if (Game2D::start()) {

        if (Storage::loadFile(FILE_NAME)) {

            Storage* store = Storage::getInstance();
            if (!store->isEmpty(FILE_NAME)) {
                if (!Storage::File2Data(mGameData, (*store)[FILE_NAME])) {

                    LOGW(LOG_FORMAT(" - Failed to get data from '%s' file"), __PRETTY_FUNCTION__, __LINE__, FILE_NAME);
                    mGameData->add(DATA_CHAR, DATA_KEY_COUNTRYIMG, static_cast<void*>(new unsigned char(1)));
                    assert(NULL);
                }
            }
            else // No data stored: Create data
                mGameData->add(DATA_CHAR, DATA_KEY_COUNTRYIMG, static_cast<void*>(new unsigned char(1)));
        }
#ifdef DEBUG
        else {

            LOGW(LOG_FORMAT(" - Failed to open existing '%s' file"), __PRETTY_FUNCTION__, __LINE__, FILE_NAME);
            assert(NULL);
        }
#endif
        return true;
    }
    else if (mGameLevel) {
        if (!(mGameLevel & 0x01))
            --mGameLevel;
    }
    return false;
}
void CrystalBall::pause() {

    LOGV(LOG_LEVEL_CRYSTALBALL, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    Game2D::pause();

    if (mLevel) mLevel->pause();
}
void CrystalBall::destroy() {

    LOGV(LOG_LEVEL_CRYSTALBALL, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    Game2D::destroy();

    if (mLevel) delete mLevel;
}

void CrystalBall::wait(float millis) {

#ifdef DEBUG
    LOGV(LOG_LEVEL_CRYSTALBALL, (mLog % 100), LOG_FORMAT(" - Delay: %f milliseconds"), __PRETTY_FUNCTION__, __LINE__, millis);
#endif
    boost::this_thread::sleep(boost::posix_time::microseconds(static_cast<unsigned long>(millis * 500)));
}
void CrystalBall::update() {

    switch (mGameLevel) {
        case 0: updateIntro(); break; // Introduction

        //////
        case 1: {

            if (!mLevel) {
                mLevel = new CrystalLevel(this);
                mLevel->initialize();
            }
            if (!mLevel->load(this))
                break;

            ++mGameLevel;
            //break;
        }
        case 2: {

            mLevel->update(this);
            break;
        }
#ifdef DEBUG
        default: {

            LOGW(LOG_FORMAT(" - Unknown game level to update: %d"), __PRETTY_FUNCTION__, __LINE__, mGameLevel);
            assert(NULL);
            break;
        }
#endif
    }
}

void CrystalBall::render() const {
    switch (mGameLevel) {

        case 0: renderIntro(); break; // Introduction
        case 1: if (mLevel) mLevel->renderLoad(); break;
        case 2: mLevel->renderUpdate(); break;

#ifdef DEBUG
        default: {

            LOGW(LOG_FORMAT(" - Unknown game level to render: %d"), __PRETTY_FUNCTION__, __LINE__, mGameLevel);
            assert(NULL);
            break;
        }
#endif
    }
}
