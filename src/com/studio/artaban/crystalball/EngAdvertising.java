package com.studio.artaban.crystalball;

import com.studio.artaban.crystalball.social.EngSocial;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Date;

import android.content.ContentResolver;
import android.content.Context;
import android.util.Log;
import android.view.View;

//// Remove import below when no advertising are used:
// -> Then comment code below and remove from comment the code that follows
/*
import android.graphics.Color;
import android.location.Location;
import android.provider.Settings;

import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
*/

public class EngAdvertising {

	/*
    static private AdView mAdView;

    static public View getView() { return mAdView; }
    static public void initialize(Context context) {

    	mStatus = AD_STATUS_READY;

    	mAdView = new AdView(context);
    	mAdView.setVisibility(View.INVISIBLE);
        mAdView.setBackgroundColor(Color.BLACK);
        mAdView.setAdListener(new AdListener() {

            @Override public void onAdClosed() { }
            @Override public void onAdFailedToLoad(int error) {

            	switch (error) {
					case AdRequest.ERROR_CODE_INTERNAL_ERROR: Log.e("EngAdvertising", "Failed to load: Internal error"); break;
					case AdRequest.ERROR_CODE_INVALID_REQUEST: Log.e("EngAdvertising", "Failed to load: Invalid request"); break;
					case AdRequest.ERROR_CODE_NETWORK_ERROR: Log.e("EngAdvertising", "Failed to load: Network Error"); break;
					case AdRequest.ERROR_CODE_NO_FILL: Log.e("EngAdvertising", "Failed to load: No fill"); break;
            	}
            	mStatus = AD_STATUS_FAILED;
            }

            @Override public void onAdLeftApplication() { }
            @Override public void onAdOpened() { }
            @Override public void onAdLoaded() {
            	if (mStatus == AD_STATUS_LOADING) {

            		if (mAdView.getVisibility() == View.INVISIBLE)
            			mStatus = AD_STATUS_LOADED;
            		else
            			mStatus = AD_STATUS_DISPLAYED;
            	}
            }

          });
    }
    static public void setBanner(short bannerID) {

    	if (mAdView.getAdSize() != null)
    		return; // The ad size can be set only once!

    	switch (bannerID) {
    		case BANNER_ID_BANNER: mAdView.setAdSize(AdSize.BANNER); break;
    		case BANNER_ID_FULL_BANNER: mAdView.setAdSize(AdSize.FULL_BANNER); break;
    		case BANNER_ID_LARGE_BANNER: mAdView.setAdSize(AdSize.LARGE_BANNER); break;
    		case BANNER_ID_LEADERBOARD: mAdView.setAdSize(AdSize.LEADERBOARD); break;
    		case BANNER_ID_MEDIUM_RECTANGLE: mAdView.setAdSize(AdSize.MEDIUM_RECTANGLE); break;
    		case BANNER_ID_WIDE_SKYSCRAPER: mAdView.setAdSize(AdSize.WIDE_SKYSCRAPER); break;
    		case BANNER_ID_SMART_BANNER: mAdView.setAdSize(AdSize.SMART_BANNER); break;
    	}
    }
    static public void setUnitID(String unitID) {

    	if (mAdView.getAdUnitId() != null)
    		return; // The ad unit ID can be set only once!

    	mAdView.setAdUnitId(unitID);
    }
    static public void load(ContentResolver contentResolver) {

		AdRequest.Builder reqBuilder = new AdRequest.Builder();
		if (contentResolver != null) // Test
	    	reqBuilder.addTestDevice(MD5(Settings.Secure.getString(contentResolver, Settings.Secure.ANDROID_ID)));

		switch (mAdReqGender) {
			case EngSocial.GENDER_MALE: reqBuilder.setGender(AdRequest.GENDER_MALE); break;
			case EngSocial.GENDER_FEMALE: reqBuilder.setGender(AdRequest.GENDER_FEMALE); break;
		}
		if (mAdReqBirthday != null)
			reqBuilder.setBirthday(mAdReqBirthday);
		if (mAdReqLocation != null)
			reqBuilder.setLocation(new Location(mAdReqLocation));

        mStatus = AD_STATUS_LOADING;
    	try {
    		mAdView.loadAd(reqBuilder.build());
    	}
    	catch (IllegalStateException e) {
    		Log.e("EngAdvertising", "Illegal state (size:" + mAdView.getAdSize() + ";unit:" + mAdView.getAdUnitId() + ")");
    		mStatus = AD_STATUS_FAILED;
    	}
    }
    static public void pause() { mAdView.pause(); }
    static public void resume() { mAdView.resume(); }
    static public void destroy() {

    	mAdView.destroy();
    	mAdView = null;
    }
    */

    static public View getView() { return null; }
    static public void initialize(Context context) {

    	mStatus = AD_STATUS_NONE;
    	mAdReqGender = EngSocial.GENDER_NONE;
	}
    static public void setBanner(short bannerID) { }
    static public void setUnitID(String unitID) { }
    static public void load() { }
    static public void load(ContentResolver contentResolver) { }
    static public void pause() { }
    static public void resume() { }
    static public void destroy() { }

    ////// Banner ID
    static public final short BANNER_ID_BANNER = 0;
    static public final short BANNER_ID_FULL_BANNER = 1;
    static public final short BANNER_ID_LARGE_BANNER = 2;
    static public final short BANNER_ID_LEADERBOARD = 3;
    static public final short BANNER_ID_MEDIUM_RECTANGLE = 4;
    static public final short BANNER_ID_WIDE_SKYSCRAPER = 5;
    static public final short BANNER_ID_SMART_BANNER = 6;

    ////// Status
    static public final short AD_STATUS_NONE = 0;
    static public final short AD_STATUS_READY = 1;
    static public final short AD_STATUS_LOADING = 2;
    static public final short AD_STATUS_LOADED = 3;
    static public final short AD_STATUS_FAILED = 4;
    static public final short AD_STATUS_DISPLAYING = 5;
    static public final short AD_STATUS_DISPLAYED = 6;

    static public short mStatus;

    ////// Request info
    static public short mAdReqGender;
    static public Date mAdReqBirthday;
    static public String mAdReqLocation;

	@SuppressWarnings("unused")
	private static String MD5(String deviceID) {

    	MessageDigest digest;
  	    try {
  	    	digest = java.security.MessageDigest.getInstance("MD5");
  	    }
  	    catch (NoSuchAlgorithmException e) {

  	    	Log.e("EngAdvertising", "MD5: " + e.getMessage());
  	  	    return "";
  	    }
        digest.update(deviceID.getBytes());
        byte messageDigest[] = digest.digest();

        StringBuffer hexString = new StringBuffer();
        for (int i = 0; i < messageDigest.length; ++i) {

            String h = Integer.toHexString(0xFF & messageDigest[i]);
            while (h.length() < 2)
                h = "0" + h;
            hexString.append(h);
        }
        return hexString.toString().toUpperCase();
  	}
}
