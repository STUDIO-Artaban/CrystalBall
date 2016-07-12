package com.studio.artaban.crystalball.social;

import com.studio.artaban.crystalball.EngActivity;
import com.studio.artaban.crystalball.social.EngSocial;

import android.content.Intent;

////Remove import below when no Facebook social network is used:
//-> Then comment code below and remove from comment the code that follows
/*
import com.studio.artaban.crystalball.data.EngData;
import com.studio.artaban.crystalball.EngLibrary;
import com.studio.artaban.crystalball.EngResources;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import org.json.JSONException;
import org.json.JSONObject;

import com.facebook.FacebookException;
import com.facebook.HttpMethod;
import com.facebook.LoggingBehavior;
import com.facebook.Response;
import com.facebook.Session;
import com.facebook.Request;
import com.facebook.SessionLoginBehavior;
import com.facebook.SessionState;
import com.facebook.Settings;
import com.facebook.FacebookAuthorizationException;
import com.facebook.FacebookDialogException;
import com.facebook.FacebookOperationCanceledException;
import com.facebook.FacebookServiceException;
*/

// WARNING: Add that follows into your project
// * '<string name="app_id">257XXXXXXXXXXXX</string>' into 'res/values/strings.xml' file
// * '<meta-data android:name="com.facebook.sdk.ApplicationId" android:value="@string/app_id" />' into the
//   <application> tag of the manifest file 
// * '<activity android:name="com.facebook.LoginActivity"
//				android:theme="@android:style/Theme.Translucent.NoTitleBar.Fullscreen"
//				android:label="@string/app_name" />' into the <application> tag of the manifest file 

public class EngFacebook extends EngSocial {

	/*
    private Session.StatusCallback statusCallback = new SessionStatusCallback();

	public EngFacebook(EngActivity activity, boolean displayError) {

		mDisplayError = displayError;
		mActivity = activity;
		mUserGender = GENDER_NONE;

		//
        Settings.addLoggingBehavior(LoggingBehavior.INCLUDE_ACCESS_TOKENS);
        Session session = Session.getActiveSession();
        if (session == null) {

            session = new Session(activity);
            Session.setActiveSession(session);
            Session.getActiveSession().addCallback(statusCallback);
        }
	}

	public void start() { Session.getActiveSession().addCallback(statusCallback); }
	public void launch(int req, int res, Intent data) {
		Session.getActiveSession().onActivityResult(mActivity, req, res, data);
	}
	public void stop() { Session.getActiveSession().removeCallback(statusCallback); }

	//////
	@Override public boolean login() {

		if (!mActivity.isOnline()) {

    		Log.w("EngFacebook", "Device not connected");
            if (mDisplayError)
        		alert("ERROR: Device not connected!");
			return false;
		}
        Session session = Session.getActiveSession();
        if ((!session.isOpened()) && (!session.isClosed())) {

            Session.OpenRequest request = new Session.OpenRequest(mActivity);
            request.setLoginBehavior(SessionLoginBehavior.SUPPRESS_SSO);
            List<String> permArray = new ArrayList<String>();
            permArray.add("publish_actions");
            if (EngData.INFO_FIELD_FACEBOOK_BIRTHDAY)
            	permArray.add("user_birthday");
            if (EngData.INFO_FIELD_FACEBOOK_LOCATION)
            	permArray.add("user_location");
            request.setPermissions(permArray);
            request.setCallback(statusCallback);
            try {
            	session.openForPublish(request); // Login
            }
            catch (FacebookException e) {

        		Log.w("EngFacebook", "Login exception: " + e.getMessage());
                if (mDisplayError)
            		alert("ERROR: Failed to open login dialog!");
            	return false;
            }
        }        	
        else
            Session.openActiveSession(mActivity, true, statusCallback);

        return true;
	}
	@Override public void logout() {

        Session session = Session.getActiveSession();
        if (!session.isClosed())
            session.closeAndClearTokenInformation();
	}
	@Override public boolean isLogged() {

        Session session = Session.getActiveSession();
        return session.isOpened();
	}

	////// Info
	static private final short INFO_RESULT_JSON = 2; // EngSocial.INFO_RESULT_FAILED + 1
	static private final short INFO_RESULT_EMPTY = 3;

	private void onRequestCompleted(short request, Response response) {

        if (response.getError() != null) {

        	Log.e("EngFacebook", "Response " + request + " error: " + response.getError().getErrorMessage());
        	switch (request) {

				case REQUEST_INFO: {
					EngLibrary.loadSocial(NETWORK_ID_FACEBOOK, REQUEST_INFO, REQ_RESULT_FAILED, (short)0, (short)0,
							null);
	                if (mDisplayError)
                    	Toast.makeText(mActivity, "ERROR: Failed to get user info!", Toast.LENGTH_SHORT).show();
					break;
				}
				case REQUEST_SHARE: {
					EngLibrary.loadSocial(NETWORK_ID_FACEBOOK, REQUEST_SHARE, REQ_RESULT_FAILED, (short)0, (short)0,
							null);
	                if (mDisplayError)
                		Toast.makeText(mActivity, "ERROR: Failed to share link!", Toast.LENGTH_SHORT).show();
					break;
				}
        	}
        	return;
        }
		switch (request) {

			case REQUEST_INFO: {

				//{
				//    "id":"881525691862086",
				//    "first_name":"Pascal",
				//    "birthday":"12\/17\/1975",
				//    "timezone":2,
				//    "location":
				//    {
				//        "id":"116217801725646",
				//        "name":"Clapiers, Languedoc-Roussillon, France"
				//    },
				//    "verified":true,
				//    "name":"Pascal Dufresne",
				//    "locale":"fr_FR",
				//    "link":"https:\/\/www.facebook.com\/app_scoped_user_id\/881525691862086\/",
				//    "last_name":"Dufresne",
				//    "gender":"male",
				//    "updated_time":"2014-07-02T15:20:01+0000"
				//}

                JSONObject graphResponse = response.getGraphObject().getInnerJSONObject();
                try {

	                // Check valid user ID (always needed)
					mUserID = graphResponse.getString("id");
					if (mUserID.isEmpty()) {

	                    Log.e("EngFacebook", "Info error: User ID empty");
	                    if (mDisplayError)
	                    	Toast.makeText(mActivity, "ERROR: Failed to get user info!", Toast.LENGTH_SHORT).show();
	            		EngLibrary.loadSocial(NETWORK_ID_FACEBOOK, REQUEST_INFO, INFO_RESULT_EMPTY, (short)0,
	            				(short)0, null);
	            		break;
					}
					mUserName = graphResponse.getString("name");
                	if (graphResponse.getString("gender").equals("male"))
                		mUserGender = GENDER_MALE;
                	else
                		mUserGender = GENDER_FEMALE;
                }
                catch (JSONException e) {

                    Log.e("EngFacebook", "JSON error: " + e.getMessage());
                    if (mDisplayError)
						Toast.makeText(mActivity, "ERROR: Failed to get user info!", Toast.LENGTH_SHORT).show();
            		EngLibrary.loadSocial(NETWORK_ID_FACEBOOK, REQUEST_INFO, INFO_RESULT_JSON, (short)0, (short)0,
            				null);
            		break;
                }
                if (EngData.INFO_FIELD_FACEBOOK_BIRTHDAY) {
                    try {

                		String birthday = graphResponse.getString("birthday");
                		mUserBirthday = new Date(Integer.parseInt(birthday.substring(6)) - 1900, // Year
                				Integer.parseInt(birthday.substring(0, 2)) - 1, // Month
                				Integer.parseInt(birthday.substring(3, 5))); // Day
    				}
                    catch (JSONException e) {
                        Log.w("EngFacebook", "JSON error: " + e.getMessage());
    				}
                    catch (NumberFormatException e) {
                        Log.w("EngFacebook", "Date error: " + e.getMessage());
    				}
                }
                if (EngData.INFO_FIELD_FACEBOOK_LOCATION) {
                    try {

                   		mUserLocation = graphResponse.getJSONObject("location").getString("name");
    				}
                    catch (JSONException e) {
                        Log.w("EngFacebook", "JSON error: " + e.getMessage());
    				}
                }
        		EngLibrary.loadSocial(NETWORK_ID_FACEBOOK, REQUEST_INFO, REQ_RESULT_SUCCEEDED, (short)0, (short)0,
        				null);
				break;
			}
			case REQUEST_SHARE: {

                if (mDisplayError)
					Toast.makeText(mActivity, "INFO: Facebook link shared!", Toast.LENGTH_SHORT).show();
				EngLibrary.loadSocial(NETWORK_ID_FACEBOOK, REQUEST_SHARE, REQ_RESULT_SUCCEEDED, (short)0, (short)0,
						null);
				break;
			}
		}
	}

    private class RequestRunnable implements Runnable {

    	private Request mRequest;
    	public RequestRunnable(Request request) {
    		mRequest = request;
    	}
		@Override public void run() {
        	mRequest.executeAsync();
		}
    };

	@Override public boolean getUserInfo() {

        Session session = Session.getActiveSession();
        if (!session.isOpened()) {

    		Log.w("EngFacebook", "The session is closed");
            if (mDisplayError)
            	alert("ERROR: Failed to get user info!");
        	return false;
        }
        Request reqInfo = new Request(session, "/me", null, HttpMethod.GET, new Request.Callback() {
    	       public void onCompleted(Response response) {

    	    	   onRequestCompleted(REQUEST_INFO, response);
    	        }
       	    });

        mActivity.runOnUiThread(new RequestRunnable(reqInfo));        
		return true;
	}

	////// Picture
    static private final short PICTURE_SIZE = 200;

	static private final String PICTURE_URL_A = "https://graph.facebook.com/";
	static private final String PICTURE_URL_B = "/picture?redirect=1&height=";
	static private final String PICTURE_URL_C = "&type=square&width=";

	private AsyncTask<Void, Void, Void> mPicTask;

	private class PictureTask extends AsyncTask<Void, Void, Void> {
		@Override protected Void doInBackground(Void... params) {
            try {

            	URL reqURL = new URL(PICTURE_URL_A + mUserID + PICTURE_URL_B + PICTURE_SIZE +
            			PICTURE_URL_C + PICTURE_SIZE);
            	PictureStream inStream = new PictureStream(reqURL.openConnection().getInputStream());
		        Bitmap bmpPIC = BitmapFactory.decodeStream(inStream);

		        EngResources resources = new EngResources(mActivity);			        
		        EngResources.BmpInfo bmpInfo = resources.getBufferBMP(bmpPIC);
		        if ((bmpInfo.width == PICTURE_SIZE) && (bmpInfo.height == PICTURE_SIZE))
		            EngLibrary.loadSocial(NETWORK_ID_FACEBOOK, REQUEST_PICTURE, REQ_RESULT_SUCCEEDED, bmpInfo.width,
		            		bmpInfo.height, bmpInfo.buffer);
		        else
		            EngLibrary.loadSocial(NETWORK_ID_FACEBOOK, REQUEST_PICTURE, REQ_RESULT_FAILED, (short)0, (short)0,
		            		null);
            }
            catch (MalformedURLException e) {
	            Log.e("EngFacebook", "Wrong picture URL: " + e.getMessage());
	            EngLibrary.loadSocial(NETWORK_ID_FACEBOOK, REQUEST_PICTURE, REQ_RESULT_FAILED, (short)0, (short)0,
	            		null);
            }
            catch (Exception e) {
	            Log.e("EngFacebook", "Failed to get user picture: " + e.getMessage());
	            EngLibrary.loadSocial(NETWORK_ID_FACEBOOK, REQUEST_PICTURE, REQ_RESULT_FAILED, (short)0, (short)0,
	            		null);
            }
			return null;
		}
	};

	@Override public boolean getUserPicture() {

        Session session = Session.getActiveSession();
        if (!session.isOpened())
        	return false;

    	mPicTask = new PictureTask();
    	mPicTask.execute();
        return true;
	}

	////// Share fields (index)
	static private final short SHARE_FIELD_NAME = 0;
	static private final short SHARE_FIELD_CAPTION = 1;
	static private final short SHARE_FIELD_DESCRIPTION = 2;
	static private final short SHARE_FIELD_LINK = 3;
	static private final short SHARE_FIELD_PICTURE = 4;

	@Override public boolean share(String[] data) {

        if (data == null) {

    		Log.w("EngFacebook", "No link data to share");
            if (mDisplayError)
            	alert("ERROR: Failed to share link!");
        	return false;
        }

        //
        Session session = Session.getActiveSession();
        if (!session.isOpened()) {

    		Log.w("EngFacebook", "The session is closed");
            if (mDisplayError)
            	alert("ERROR: Failed to share link!");
        	return false;
        }
        Bundle postData = new Bundle();
        postData.putString("name", data[SHARE_FIELD_NAME]);
        postData.putString("caption", data[SHARE_FIELD_CAPTION]);
        postData.putString("description", data[SHARE_FIELD_DESCRIPTION]);
        postData.putString("link", data[SHARE_FIELD_LINK]);
        postData.putString("picture", data[SHARE_FIELD_PICTURE]);

        Request reqShare = new Request(Session.getActiveSession(), "me/feed", postData, HttpMethod.POST,
        		new Request.Callback() {
	 	       public void onCompleted(Response response) {

	 	    	   onRequestCompleted(REQUEST_SHARE, response);
	 	        }
    	    });

        mActivity.runOnUiThread(new RequestRunnable(reqShare));        
		return true;
	}

	////// Login
	static private final short LOGIN_RESULT_AUTHORIZATION = 4; // EngSocial.LOGGED_RESULT_FAILED + 1
	static private final short LOGIN_RESULT_DIALOG = 5;
	static private final short LOGIN_RESULT_SERVICE = 6;

    private class SessionStatusCallback implements Session.StatusCallback {

        @Override public void call(Session session, SessionState state, Exception exception) {

        	if (exception != null) {

        		if (exception instanceof FacebookOperationCanceledException) {
            		EngLibrary.loadSocial(NETWORK_ID_FACEBOOK, REQUEST_LOGIN, REQ_RESULT_CANCELED, (short)0, (short)0,
            				null);
        			return;
        		}
        		Log.w("EngFacebook", "Session exception: " + exception.getMessage());
                if (mDisplayError)
                	alert("ERROR: Failed to login!");

        		short loginResult = REQ_RESULT_FAILED;
        		if (exception instanceof FacebookAuthorizationException)
        			loginResult = LOGIN_RESULT_AUTHORIZATION;
        		else if (exception instanceof FacebookDialogException)
        			loginResult = LOGIN_RESULT_DIALOG;
        		else if (exception instanceof FacebookServiceException)
        			loginResult = LOGIN_RESULT_SERVICE;
        		//else // FacebookException

        		EngLibrary.loadSocial(NETWORK_ID_FACEBOOK, REQUEST_LOGIN, loginResult, (short)0, (short)0, null);
        		return;
        	}
        	switch (state) {

        		case OPENED: { // Session opened
            		EngLibrary.loadSocial(NETWORK_ID_FACEBOOK, REQUEST_LOGIN, REQ_RESULT_SUCCEEDED, (short)0, (short)0,
            				null);
        			break;
        		}
        		case OPENING: // Login dialog displaying
        		case CREATED:
	    		case CREATED_TOKEN_LOADED:
        		case OPENED_TOKEN_UPDATED:
        		case CLOSED: // Session closed
        		case CLOSED_LOGIN_FAILED: // Login processus failed (check exception)
        			break;
        	}
        }
    }
    */

	public EngFacebook(EngActivity activity, boolean displayError) { }

	public void start() { }
	public void launch(int req, int res, Intent data) { }
	public void stop() { }

	@Override public boolean login() { return false; }
	@Override public void logout() { }
	@Override public boolean isLogged() { return false; }
	@Override public boolean getUserPicture() { return false; }
	@Override public boolean getUserInfo() { return false; }
	@Override public boolean share(String[] data) { return false; }

    ////// Texture ID
    static public final short TEXTURE_ID = 253; // Reserved

}
