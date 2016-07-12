#include "Main.h"

////// Languages
typedef enum {

    LANG_EN = 0,
    LANG_FR,
    LANG_DE,
    LANG_ES,
    LANG_IT,
    LANG_PT

} Language;
static const Language g_MainLang = LANG_FR;

////// Textures
#define NO_TEXTURE_LOADED       0xFF

BOOL engGetFontGrayscale() { return NO; }

#define TEXTURE_ID_TITLE_TOPA   2 // TEXTURE_ID_FONT + 1
#define TEXTURE_ID_TITLE_TOPB   3
#define TEXTURE_ID_COUNTRY1     4
#define TEXTURE_ID_COUNTRY2     5
#define TEXTURE_ID_COUNTRY3     6
#define TEXTURE_ID_QUERY        7

#define TEXTURE_ID_CRYSTAL      8
#define TEXTURE_ID_ENERGY       9

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

unsigned char engLoadTexture(EngResources* resources, unsigned char Id) {
    switch (Id) {

        case TEXTURE_ID_TITLE_TOPA: {

            unsigned char* data = [resources getBufferPNG:@"topA" inGrayScale:NO];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_TITLE_TOPA, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }
        case TEXTURE_ID_TITLE_TOPB: {

            unsigned char* data = [resources getBufferPNG:@"topB" inGrayScale:NO];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_TITLE_TOPB, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }
        case TEXTURE_ID_COUNTRY1:
        case TEXTURE_ID_COUNTRY2:
        case TEXTURE_ID_COUNTRY3: {

            NSInteger imgNum = (Id - TEXTURE_ID_COUNTRY1) + 1;
            NSString* countryImg;
            switch (g_MainLang) {

            case LANG_EN: countryImg = [[NSString alloc] initWithFormat:@"%@%d", @"countryEN", imgNum]; break;
            case LANG_FR: countryImg = [[NSString alloc] initWithFormat:@"%@%d", @"countryFR", imgNum]; break;
            case LANG_DE: countryImg = [[NSString alloc] initWithFormat:@"%@%d", @"countryDE", imgNum]; break;
            case LANG_ES: countryImg = [[NSString alloc] initWithFormat:@"%@%d", @"countryES", imgNum]; break;
            case LANG_IT: countryImg = [[NSString alloc] initWithFormat:@"%@%d", @"countryIT", imgNum]; break;
            case LANG_PT: countryImg = [[NSString alloc] initWithFormat:@"%@%d", @"countryPT", imgNum]; break;

            default: countryImg = [[NSString alloc] initWithFormat:@"%@%d", @"countryEN", imgNum]; break;
            
            }
            unsigned char* data = [resources getBufferPNG:countryImg inGrayScale:NO];
            [countryImg release];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(Id, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }
        case TEXTURE_ID_QUERY: {
            
            unsigned char* data = [resources getBufferPNG:@"query" inGrayScale:NO];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_QUERY, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }
        case TEXTURE_ID_CRYSTAL: {
            
            unsigned char* data = [resources getBufferPNG:@"crystal" inGrayScale:NO];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_CRYSTAL, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }
        case TEXTURE_ID_ENERGY: {
            
            unsigned char* data = [resources getBufferPNG:@"energy" inGrayScale:NO];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_ENERGY, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }
        case TEXTURE_ID_INTRO: {

            unsigned char* data = NULL;
            switch (g_MainLang) {

                case LANG_EN: data = [resources getBufferPNG:@"introEN" inGrayScale:NO]; break;
                case LANG_FR: data = [resources getBufferPNG:@"introFR" inGrayScale:NO]; break;
                case LANG_DE: data = [resources getBufferPNG:@"introDE" inGrayScale:NO]; break;
                case LANG_ES: data = [resources getBufferPNG:@"introES" inGrayScale:NO]; break;
                case LANG_IT: data = [resources getBufferPNG:@"introIT" inGrayScale:NO]; break;
                case LANG_PT: data = [resources getBufferPNG:@"introPT" inGrayScale:NO]; break;

                default: data = [resources getBufferPNG:@"introEN" inGrayScale:NO]; break;
            }
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_INTRO, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }
        case TEXTURE_ID_START: {

            unsigned char* data = NULL;
            switch (g_MainLang) {

                case LANG_EN: data = [resources getBufferPNG:@"startEN" inGrayScale:NO]; break;
                case LANG_FR: data = [resources getBufferPNG:@"startFR" inGrayScale:NO]; break;
                case LANG_DE: data = [resources getBufferPNG:@"startDE" inGrayScale:NO]; break;
                case LANG_ES: data = [resources getBufferPNG:@"startES" inGrayScale:NO]; break;
                case LANG_IT: data = [resources getBufferPNG:@"startIT" inGrayScale:NO]; break;
                case LANG_PT: data = [resources getBufferPNG:@"startPT" inGrayScale:NO]; break;

                default: data = [resources getBufferPNG:@"startEN" inGrayScale:NO]; break;
            }
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_START, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }
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

            NSInteger imgNum = (Id - TEXTURE_ID_YES1) + 1;
            NSString* respImg;
            switch (g_MainLang) {

            case LANG_EN: respImg = [[NSString alloc] initWithFormat:@"%@%d", @"yesEN", imgNum]; break;
            case LANG_FR: respImg = [[NSString alloc] initWithFormat:@"%@%d", @"yesFR", imgNum]; break;
            case LANG_DE: respImg = [[NSString alloc] initWithFormat:@"%@%d", @"yesDE", imgNum]; break;
            case LANG_ES: respImg = [[NSString alloc] initWithFormat:@"%@%d", @"yesES", imgNum]; break;
            case LANG_IT: respImg = [[NSString alloc] initWithFormat:@"%@%d", @"yesIT", imgNum]; break;
            case LANG_PT: respImg = [[NSString alloc] initWithFormat:@"%@%d", @"yesPT", imgNum]; break;

            default: respImg = [[NSString alloc] initWithFormat:@"%@%d", @"yesEN", imgNum]; break;

            }
            unsigned char* data = [resources getBufferPNG:respImg inGrayScale:NO];
            [respImg release];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(Id, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }
        case TEXTURE_ID_MAYBE1:
        case TEXTURE_ID_MAYBE2:
        case TEXTURE_ID_MAYBE3:
        case TEXTURE_ID_MAYBE4:
        case TEXTURE_ID_MAYBE5: {

            NSInteger imgNum = (Id - TEXTURE_ID_MAYBE1) + 1;
            NSString* respImg;
            switch (g_MainLang) {

                case LANG_EN: respImg = [[NSString alloc] initWithFormat:@"%@%d", @"maybeEN", imgNum]; break;
                case LANG_FR: respImg = [[NSString alloc] initWithFormat:@"%@%d", @"maybeFR", imgNum]; break;
                case LANG_DE: respImg = [[NSString alloc] initWithFormat:@"%@%d", @"maybeDE", imgNum]; break;
                case LANG_ES: respImg = [[NSString alloc] initWithFormat:@"%@%d", @"maybeES", imgNum]; break;
                case LANG_IT: respImg = [[NSString alloc] initWithFormat:@"%@%d", @"maybeIT", imgNum]; break;
                case LANG_PT: respImg = [[NSString alloc] initWithFormat:@"%@%d", @"maybePT", imgNum]; break;

                default: respImg = [[NSString alloc] initWithFormat:@"%@%d", @"maybeEN", imgNum]; break;

            }
            unsigned char* data = [resources getBufferPNG:respImg inGrayScale:NO];
            [respImg release];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(Id, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }
        case TEXTURE_ID_NO1:
        case TEXTURE_ID_NO2:
        case TEXTURE_ID_NO3:
        case TEXTURE_ID_NO4:
        case TEXTURE_ID_NO5:
        case TEXTURE_ID_NO6:
        case TEXTURE_ID_NO7: {

            NSInteger imgNum = (Id - TEXTURE_ID_NO1) + 1;
            NSString* respImg;
            switch (g_MainLang) {

                case LANG_EN: respImg = [[NSString alloc] initWithFormat:@"%@%d", @"noEN", imgNum]; break;
                case LANG_FR: respImg = [[NSString alloc] initWithFormat:@"%@%d", @"noFR", imgNum]; break;
                case LANG_DE: respImg = [[NSString alloc] initWithFormat:@"%@%d", @"noDE", imgNum]; break;
                case LANG_ES: respImg = [[NSString alloc] initWithFormat:@"%@%d", @"noES", imgNum]; break;
                case LANG_IT: respImg = [[NSString alloc] initWithFormat:@"%@%d", @"noIT", imgNum]; break;
                case LANG_PT: respImg = [[NSString alloc] initWithFormat:@"%@%d", @"noPT", imgNum]; break;

                default: respImg = [[NSString alloc] initWithFormat:@"%@%d", @"noEN", imgNum]; break;

            }
            unsigned char* data = [resources getBufferPNG:respImg inGrayScale:NO];
            [respImg release];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(Id, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }
        default: {
            
            NSLog(@"ERROR: Failed to load PNG ID %d", Id);
            break;
        }
    }
    return NO_TEXTURE_LOADED;
}

////// Sounds
#define SOUND_ID_INTRO          1 // SOUND_ID_LOGO + 1
#define SOUND_ID_TOUCHED        2
#define SOUND_ID_RESPONSE       3
#define SOUND_ID_REPLIED        4

void engLoadSound(EngResources* resources, unsigned char Id) {
    switch (Id) {

        case SOUND_ID_INTRO: {

            unsigned char* data = [resources getBufferOGG:@"intro"];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get OGG buffer (line:%d)", __LINE__);
                break;
            }
            platformLoadOgg(SOUND_ID_INTRO, resources.oggLength, data);
            break;
        }
        case SOUND_ID_TOUCHED: {
            
            unsigned char* data = [resources getBufferOGG:@"touched"];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get OGG buffer (line:%d)", __LINE__);
                break;
            }
            platformLoadOgg(SOUND_ID_TOUCHED, resources.oggLength, data, false);
            break;
        }
        case SOUND_ID_RESPONSE: {
            
            unsigned char* data = [resources getBufferOGG:@"response"];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get OGG buffer (line:%d)", __LINE__);
                break;
            }
            platformLoadOgg(SOUND_ID_RESPONSE, resources.oggLength, data);
            break;
        }
        case SOUND_ID_REPLIED: {

            unsigned char* data = [resources getBufferOGG:@"replied"];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get OGG buffer (line:%d)", __LINE__);
                break;
            }
            platformLoadOgg(SOUND_ID_REPLIED, resources.oggLength, data);
            break;
        }
        default: {
            
            NSLog(@"ERROR: Failed to load OGG ID %d", Id);
            break;
        }
    }
}

////// Advertising
#ifdef LIBENG_ENABLE_ADVERTISING

static NSString* ADV_UNIT_ID = @"ca-app-pub-XXXXXXXXXX";
#ifdef DEBUG
static const NSString* IPAD_DEVICE_UID = @"655799b1c803de3417cbb36833b6c40c";
static const NSString* IPHONE_YACIN_UID = @"10053bb6983c6568b88812fbcfd7ab89";
#endif

BOOL engGetAdType() { return FALSE; } // TRUE: Interstitial; FALSE: Banner
void engLoadAd(EngAdvertising* ad, GADRequest* request) { }
void engDisplayAd(EngAdvertising* ad, unsigned char Id) { }
void engHideAd(EngAdvertising* ad, unsigned char Id) { }
#endif

////// Social
#ifdef LIBENG_ENABLE_SOCIAL
BOOL engReqInfoField(SocialField field, unsigned char socialID) { return NO; }
#endif
