package com.studio.artaban.crystalball.data;

import android.util.Log;

import com.studio.artaban.crystalball.EngActivity;
import com.studio.artaban.crystalball.EngLibrary;
import com.studio.artaban.crystalball.EngResources;

public class EngData {

    ////// JNI
    static public final String PROJECT_NAME_LIB = "CrystalBall";

    ////// Languages
    static private final short LANG_EN = 0;
    static private final short LANG_FR = 1;
    static private final short LANG_DE = 2;
    static private final short LANG_ES = 3;
    static private final short LANG_IT = 4;
    static private final short LANG_PT = 5;

    static private final short mMainLang = LANG_EN;

    ////// Permissions
    static public final boolean USES_PERMISSION_CAMERA = true; // android.permission.CAMERA
    static public final boolean USES_PERMISSION_MIC = true; // android.permission.RECORD_AUDIO
    static public final boolean USES_PERMISSION_INTERNET = false; // android.permission.INTERNET & ACCESS_NETWORK_STATE
    static public final boolean USES_PERMISSION_STORAGE = false; // android.permission.WRITE_INTERNAL_STORAGE & WRITE_EXTERNAL_STORAGE

    ////// Font
    static public final boolean FONT_TEXTURE_GRAYSCALE = false;

    ////// Textures
    static private final short TEXTURE_ID_TITLE_TOPA = 2; // EngActivity.TEXTURE_ID_FONT + 1
    static private final short TEXTURE_ID_TITLE_TOPB = 3;
    static private final short TEXTURE_ID_COUNTRY1 = 4;
    static private final short TEXTURE_ID_COUNTRY2 = 5;
    static private final short TEXTURE_ID_COUNTRY3 = 6;
    static private final short TEXTURE_ID_QUERY = 7;

    static private final short TEXTURE_ID_CRYSTAL = 8;
    static private final short TEXTURE_ID_ENERGY = 9;

    static private final short TEXTURE_ID_INTRO = 10;
    static private final short TEXTURE_ID_START = 11;

    static private final short TEXTURE_ID_YES1 = 12;
    static private final short TEXTURE_ID_YES2 = 13;
    static private final short TEXTURE_ID_YES3 = 14;
    static private final short TEXTURE_ID_YES4 = 15;
    static private final short TEXTURE_ID_YES5 = 16;
    static private final short TEXTURE_ID_YES6 = 17;
    static private final short TEXTURE_ID_YES7 = 18;
    static private final short TEXTURE_ID_YES8 = 19;
    static private final short TEXTURE_ID_YES9 = 20;
    static private final short TEXTURE_ID_YES10 = 21;
    static private final short TEXTURE_ID_YES11 = 22;

    static private final short TEXTURE_ID_MAYBE1 = 23; 
    static private final short TEXTURE_ID_MAYBE2 = 24; 
    static private final short TEXTURE_ID_MAYBE3 = 25; 
    static private final short TEXTURE_ID_MAYBE4 = 26; 
    static private final short TEXTURE_ID_MAYBE5 = 27; 

    static private final short TEXTURE_ID_NO1 = 28;
    static private final short TEXTURE_ID_NO2 = 29;
    static private final short TEXTURE_ID_NO3 = 30; 
    static private final short TEXTURE_ID_NO4 = 31; 
    static private final short TEXTURE_ID_NO5 = 32; 
    static private final short TEXTURE_ID_NO6 = 33; 
    static private final short TEXTURE_ID_NO7 = 34; 

    static public short loadTexture(EngResources resources, short id) {

        switch (id) {
            case TEXTURE_ID_TITLE_TOPA: {
                EngResources.BmpInfo bmpInfo = resources.getBufferPNG("topA.png");
                return EngLibrary.loadTexture(TEXTURE_ID_TITLE_TOPA, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }
            case TEXTURE_ID_TITLE_TOPB: {
                EngResources.BmpInfo bmpInfo = resources.getBufferPNG("topB.png");
                return EngLibrary.loadTexture(TEXTURE_ID_TITLE_TOPB, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }
            case TEXTURE_ID_COUNTRY1:
            case TEXTURE_ID_COUNTRY2:
            case TEXTURE_ID_COUNTRY3: {

                EngResources.BmpInfo bmpInfo;
                int numImg = id - TEXTURE_ID_COUNTRY1 + 1;
                switch (mMainLang) {

                    case LANG_EN: bmpInfo = resources.getBufferPNG("countryEN" + numImg + ".png"); break; 
                    case LANG_FR: bmpInfo = resources.getBufferPNG("countryFR" + numImg + ".png"); break; 
                    case LANG_DE: bmpInfo = resources.getBufferPNG("countryDE" + numImg + ".png"); break; 
                    case LANG_ES: bmpInfo = resources.getBufferPNG("countryES" + numImg + ".png"); break; 
                    case LANG_IT: bmpInfo = resources.getBufferPNG("countryIT" + numImg + ".png"); break; 
                    case LANG_PT: bmpInfo = resources.getBufferPNG("countryPT" + numImg + ".png"); break;
                    default: {
                        bmpInfo = resources.getBufferPNG("countryEN" + numImg + ".png");
                        break;
                    }
                }
                return EngLibrary.loadTexture(id, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }
            case TEXTURE_ID_QUERY: {
                EngResources.BmpInfo bmpInfo = resources.getBufferPNG("query.png");
                return EngLibrary.loadTexture(TEXTURE_ID_QUERY, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }
            case TEXTURE_ID_CRYSTAL: {
                EngResources.BmpInfo bmpInfo = resources.getBufferPNG("crystal.png");
                return EngLibrary.loadTexture(TEXTURE_ID_CRYSTAL, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }
            case TEXTURE_ID_ENERGY: {
                EngResources.BmpInfo bmpInfo = resources.getBufferPNG("energy.png");
                return EngLibrary.loadTexture(TEXTURE_ID_ENERGY, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, true);
            }
            case TEXTURE_ID_INTRO: {

                EngResources.BmpInfo bmpInfo;
                switch (mMainLang) {

                    case LANG_EN: bmpInfo = resources.getBufferPNG("introEN.png"); break; 
                    case LANG_FR: bmpInfo = resources.getBufferPNG("introFR.png"); break; 
                    case LANG_DE: bmpInfo = resources.getBufferPNG("introDE.png"); break; 
                    case LANG_ES: bmpInfo = resources.getBufferPNG("introES.png"); break; 
                    case LANG_IT: bmpInfo = resources.getBufferPNG("introIT.png"); break; 
                    case LANG_PT: bmpInfo = resources.getBufferPNG("introPT.png"); break;
                    default: {
                        bmpInfo = resources.getBufferPNG("introEN.png");
                        break;
                    }
                }
                return EngLibrary.loadTexture(id, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }
            case TEXTURE_ID_START: {

                EngResources.BmpInfo bmpInfo;
                switch (mMainLang) {

                    case LANG_EN: bmpInfo = resources.getBufferPNG("startEN.png"); break; 
                    case LANG_FR: bmpInfo = resources.getBufferPNG("startFR.png"); break; 
                    case LANG_DE: bmpInfo = resources.getBufferPNG("startDE.png"); break; 
                    case LANG_ES: bmpInfo = resources.getBufferPNG("startES.png"); break; 
                    case LANG_IT: bmpInfo = resources.getBufferPNG("startIT.png"); break; 
                    case LANG_PT: bmpInfo = resources.getBufferPNG("startPT.png"); break;
                    default: {
                        bmpInfo = resources.getBufferPNG("startEN.png");
                        break;
                    }
                }
                return EngLibrary.loadTexture(id, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }

            ////// Responses
            case TEXTURE_ID_YES1:
            case TEXTURE_ID_YES2:
            case TEXTURE_ID_YES3:
            case TEXTURE_ID_YES4:
            case TEXTURE_ID_YES5:
            case TEXTURE_ID_YES6:
            case TEXTURE_ID_YES7:
            case TEXTURE_ID_YES8:
            case TEXTURE_ID_YES9:
            case TEXTURE_ID_YES10:
            case TEXTURE_ID_YES11: {

                EngResources.BmpInfo bmpInfo;
                int numYes = id - TEXTURE_ID_YES1 + 1;
                switch (mMainLang) {

                    case LANG_EN: bmpInfo = resources.getBufferPNG("yesEN" + numYes + ".png"); break; 
                    case LANG_FR: bmpInfo = resources.getBufferPNG("yesFR" + numYes + ".png"); break; 
                    case LANG_DE: bmpInfo = resources.getBufferPNG("yesDE" + numYes + ".png"); break; 
                    case LANG_ES: bmpInfo = resources.getBufferPNG("yesES" + numYes + ".png"); break; 
                    case LANG_IT: bmpInfo = resources.getBufferPNG("yesIT" + numYes + ".png"); break; 
                    case LANG_PT: bmpInfo = resources.getBufferPNG("yesPT" + numYes + ".png"); break;
                    default: {
                        bmpInfo = resources.getBufferPNG("yesEN" + numYes + ".png");
                        break;
                    }
                }
                return EngLibrary.loadTexture(id, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }

            case TEXTURE_ID_MAYBE1:
            case TEXTURE_ID_MAYBE2:
            case TEXTURE_ID_MAYBE3:
            case TEXTURE_ID_MAYBE4:
            case TEXTURE_ID_MAYBE5: {

                EngResources.BmpInfo bmpInfo;
                int numMaybe = id - TEXTURE_ID_MAYBE1 + 1;
                switch (mMainLang) {

                    case LANG_EN: bmpInfo = resources.getBufferPNG("maybeEN" + numMaybe + ".png"); break; 
                    case LANG_FR: bmpInfo = resources.getBufferPNG("maybeFR" + numMaybe + ".png"); break; 
                    case LANG_DE: bmpInfo = resources.getBufferPNG("maybeDE" + numMaybe + ".png"); break; 
                    case LANG_ES: bmpInfo = resources.getBufferPNG("maybeES" + numMaybe + ".png"); break; 
                    case LANG_IT: bmpInfo = resources.getBufferPNG("maybeIT" + numMaybe + ".png"); break; 
                    case LANG_PT: bmpInfo = resources.getBufferPNG("maybePT" + numMaybe + ".png"); break;
                    default: {
                        bmpInfo = resources.getBufferPNG("maybeEN" + numMaybe + ".png");
                        break;
                    }
                }
                return EngLibrary.loadTexture(id, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }

            case TEXTURE_ID_NO1:
            case TEXTURE_ID_NO2:
            case TEXTURE_ID_NO3:
            case TEXTURE_ID_NO4:
            case TEXTURE_ID_NO5:
            case TEXTURE_ID_NO6:
            case TEXTURE_ID_NO7: {

                EngResources.BmpInfo bmpInfo;
                int numNo = id - TEXTURE_ID_NO1 + 1;
                switch (mMainLang) {

                    case LANG_EN: bmpInfo = resources.getBufferPNG("noEN" + numNo + ".png"); break; 
                    case LANG_FR: bmpInfo = resources.getBufferPNG("noFR" + numNo + ".png"); break; 
                    case LANG_DE: bmpInfo = resources.getBufferPNG("noDE" + numNo + ".png"); break; 
                    case LANG_ES: bmpInfo = resources.getBufferPNG("noES" + numNo + ".png"); break; 
                    case LANG_IT: bmpInfo = resources.getBufferPNG("noIT" + numNo + ".png"); break; 
                    case LANG_PT: bmpInfo = resources.getBufferPNG("noPT" + numNo + ".png"); break;
                    default: {
                        bmpInfo = resources.getBufferPNG("noEN" + numNo + ".png");
                        break;
                    }
                }
                return EngLibrary.loadTexture(id, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }

            default: {
                Log.e("EngActivity", "Failed to load PNG ID: " + Integer.toString(id));
                return EngActivity.NO_TEXTURE_LOADED;
            }
        }
    }

    ////// Sounds
    static private final short SOUND_ID_INTRO = 1; // EngActivity.SOUND_ID_LOGO + 1
    static private final short SOUND_ID_TOUCHED = 2;
    static private final short SOUND_ID_RESPONSE = 3;
    static private final short SOUND_ID_REPLIED = 4;

    static public void loadOgg(EngResources resources, short id) {

        switch (id) {
            case SOUND_ID_INTRO: {
                EngLibrary.loadOgg(SOUND_ID_INTRO, resources.getBufferOGG("intro.ogg"), true);
                break;
            }
            case SOUND_ID_TOUCHED: {
                EngLibrary.loadOgg(SOUND_ID_TOUCHED, resources.getBufferOGG("touched.ogg"), false);
                break;
            }
            case SOUND_ID_RESPONSE: {
                EngLibrary.loadOgg(SOUND_ID_RESPONSE, resources.getBufferOGG("response.ogg"), true);
                break;
            }
            case SOUND_ID_REPLIED: {
                EngLibrary.loadOgg(SOUND_ID_REPLIED, resources.getBufferOGG("replied.ogg"), true);
                break;
            }
            default: {
                Log.e("EngActivity", "Failed to load OGG ID: " + Integer.toString(id));
                break;
            }
        }
    }

    ////// Advertising
    //static public final boolean TEST_ADVERTISING = true; // Set to 'false' in release mode
    //static private final String ADV_UNIT_ID = "";

    static public void loadAd(EngActivity activity) { }
    static public void displayAd(short id, EngActivity activity) { }
    static public void hideAd(short id, EngActivity activity) { }

    ////// Social

}
