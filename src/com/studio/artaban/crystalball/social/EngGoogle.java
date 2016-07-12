package com.studio.artaban.crystalball.social;

import android.content.Intent;

import com.studio.artaban.crystalball.EngActivity;

public class EngGoogle extends EngSocial {

	//static private final short PICTURE_SIZE = 200;

	public EngGoogle(EngActivity activity, boolean displayError) {

		mDisplayError = displayError;
		mActivity = activity;
		mUserGender = GENDER_NONE;
	}

	public void start() { }
	public void launch(int req, int res, Intent data) { }
	public void stop() { }

	@Override public boolean login() {

        if (mDisplayError)
    		alert("ERROR: Not implemented yet!");
		return false;
	}
	@Override public void logout() { }
	@Override public boolean isLogged() { return false; }
	@Override public boolean getUserPicture() { return false; }
	@Override public boolean getUserInfo() { return false; }
	@Override public boolean share(String[] data) { return false; }

    ////// Texture ID
    static public final short TEXTURE_ID = 251; // Reserved

}
