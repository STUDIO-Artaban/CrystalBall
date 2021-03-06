package com.studio.artaban.crystalball;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Date;
import java.util.List;
import java.util.concurrent.ExecutionException;

import com.studio.artaban.crystalball.EngAdvertising;
import com.studio.artaban.crystalball.data.EngData;
import com.studio.artaban.crystalball.social.EngFacebook;
import com.studio.artaban.crystalball.social.EngGoogle;
import com.studio.artaban.crystalball.social.EngSocial;
import com.studio.artaban.crystalball.social.EngTwitter;

import android.hardware.Camera;
import android.hardware.Camera.ErrorCallback;
import android.hardware.Camera.Size;
import android.hardware.Camera.Parameters;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.graphics.ImageFormat;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.os.PowerManager;
import android.provider.MediaStore;
import android.provider.Settings;
import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.SurfaceHolder.Callback;
import android.widget.FrameLayout;
import android.widget.RelativeLayout;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;

public class EngActivity extends Activity implements SensorEventListener {

    static private EngResources mResources;
	static public final String nullString = " ";	// Avoid unexplained JNI crash below when return null string after a C++ call (see use & #JNI1):
													// @@@ ABORTING: INVALID HEAP ADDRESS IN dlfree addr=0x5d79b4a0
													// Fatal signal 11 (SIGSEGV) at 0xdeadbaad (code=1), thread 21231 (taban.crystalball)
    ////// Accelerometer
    private SensorManager mSensorMan;
    private Sensor mAccelSensor;

    ////// Textures
    static public final short NO_TEXTURE_LOADED = 0x00FF;

    static private final short TEXTURE_ID_LOGO = 0; // Reserved
    static private final short TEXTURE_ID_FONT = 1; // Reserved

    static public short loadPNG(short id) {

        switch (id) {
            case TEXTURE_ID_LOGO: { // Logo
                EngResources.BmpInfo bmpInfo = mResources.getBufferPNG("logo.png");
                return EngLibrary.loadTexture(TEXTURE_ID_LOGO, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }
            case TEXTURE_ID_FONT: { // Font
                EngResources.BmpInfo bmpInfo = mResources.getBufferPNG("font.png");
                return EngLibrary.loadTexture(TEXTURE_ID_FONT, bmpInfo.width, bmpInfo.height, bmpInfo.buffer,
                        EngData.FONT_TEXTURE_GRAYSCALE);
            }
            case EngFacebook.TEXTURE_ID: {
                EngResources.BmpInfo bmpInfo = mResources.getBufferPNG("facebook.png");
                return EngLibrary.loadTexture(EngFacebook.TEXTURE_ID, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }
            case EngTwitter.TEXTURE_ID: {
                EngResources.BmpInfo bmpInfo = mResources.getBufferPNG("twitter.png");
                return EngLibrary.loadTexture(EngTwitter.TEXTURE_ID, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }
            case EngGoogle.TEXTURE_ID: {
                EngResources.BmpInfo bmpInfo = mResources.getBufferPNG("google.png");
                return EngLibrary.loadTexture(EngGoogle.TEXTURE_ID, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }
            default: {
                return EngData.loadTexture(mResources, id);
            }
        }
    }

    ////// Alert message
    public void alert(String message) {
    	runOnUiThread(new EngResources.AlertRunnable(this, message));
    }

    ////// Sounds
    static private final short SOUND_ID_LOGO = 0; // Reserved

    static public void loadOGG(short id) {

        if (id != SOUND_ID_LOGO)
            EngData.loadOgg(mResources, id);
        else
            EngLibrary.loadOgg(SOUND_ID_LOGO, mResources.getBufferOGG("logo.ogg"), true);
    }

    ////// Storage
    static public boolean readFILE(String file) {
        return EngLibrary.loadFile(file, mResources.getFileBuffer(file));
    }
    static public boolean writeFILE(String file, byte[] content, int len) {
        return mResources.setFileBuffer(file, content, len);
    }

    static private final short STORAGE_RESULT_SUCCEEDED = 1; // See status in 'Storage' module of libeng
    static private final short STORAGE_RESULT_FAILED = 2;
    static private final short STORAGE_RESULT_ALREADY_EXISTS = 3;
    static private final short STORAGE_RESULT_CREATE_FOLDER = 4;

    private class SaveBmpTask extends AsyncTask<byte[], Void, Void> {

    	private String mBmpFile;
    	private short mWidth;
    	private short mHeight;
    	private boolean mAlertMsg;

    	public SaveBmpTask(String bmpFile, boolean alert, short width, short height) {

    		mBmpFile = bmpFile;
    		mWidth = width;
    		mHeight = height;
    		mAlertMsg = alert;
    	}
        @Override protected Void doInBackground(byte[]... rgba) {

        	File picFile = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES), mBmpFile);
            if (picFile.exists()) {

                Log.e("EngActivity", "BMP file '" + mBmpFile + "' already exists");
                if (mAlertMsg)
                	alert("ERROR: Failed to save picture");

                EngLibrary.loadStore(STORAGE_RESULT_ALREADY_EXISTS);
            	return null;
            }
            if (!picFile.mkdirs()) {

                Log.e("EngActivity", "Failed to create BMP file '" + mBmpFile + "' directory");
                if (mAlertMsg)
                	alert("ERROR: Failed to save picture");

                EngLibrary.loadStore(STORAGE_RESULT_CREATE_FOLDER);
            	return null;
            }
            picFile.delete(); // Delete directory named as the BMP file

        	if (!mResources.saveBMP(picFile, mWidth, mHeight, rgba[0])) {

                if (mAlertMsg)
                	alert("ERROR: Failed to save picture");

                EngLibrary.loadStore(STORAGE_RESULT_FAILED);
        		return null;
        	}

        	boolean gallerySaved = true;
            try { MediaStore.Images.Media.insertImage(getContentResolver(), picFile.getAbsolutePath(), "", ""); }
            catch (FileNotFoundException e) {

            	Log.w("EngActivity", "Failed to insert BMP into the Gallery");
            	gallerySaved = false;
            }
            if (mAlertMsg) {

            	String picPath = mBmpFile.substring(0, mBmpFile.lastIndexOf('/') + 1);
            	if (gallerySaved)
            		alert("Saved in '" + picPath + "' folder & in your Gallery");
            	else
            		alert("Saved in '" + picPath + "' pictures folder");
            }
            EngLibrary.loadStore(STORAGE_RESULT_SUCCEEDED);
            return null;
        }
    }
    public boolean saveBMP(String picPathFile, boolean alertMsg, short width, short height, byte[] rgba) {

    	if (!EngData.USES_PERMISSION_STORAGE) {
            Log.wtf("EngActivity", "No USES_PERMISSION_STORAGE permission flag");
    		return false;
    	}
        SaveBmpTask asyncTask = new SaveBmpTask(picPathFile, alertMsg, width, height);
        asyncTask.execute(rgba.clone());
    	return true;
    }

    ////// Camera
    static private Camera mCamera;
    static private boolean mCamStarted;
    static private boolean mCamRunning;
    static private Size mCamSize;

    static private boolean openCamera() {

        try { mCamera = Camera.open(); } // Try to open back-facing camera
        catch (RuntimeException e) {

            Log.e("EngActivity", "Failed to open back-facing camera: " + e.getMessage());
            return false;
        }
        if (mCamera == null) {

            Log.w("EngActivity", "Failed to open back-facing camera");
            try { mCamera = Camera.open(0); } // Try to open default camera
            catch (RuntimeException e) {

                Log.e("EngActivity", "Failed to open camera: " + e.getMessage());
                return false;
            }
            if (mCamera == null) {

                Log.e("EngActivity", "Failed to open camera");
                return false;
            }
        }
        return true;
    }

    private class CameraTask extends AsyncTask<SurfaceHolder, Void, Void> {

        @Override protected Void doInBackground(SurfaceHolder... surfaces) {

            if (!EngData.USES_PERMISSION_CAMERA)
                return null;

            if (!openCamera())
                return null;

            Parameters camParams = mCamera.getParameters(); // Let's crash if null

            //camParams.setAntibanding(Parameters.ANTIBANDING_OFF);
            //camParams.setColorEffect(Parameters.EFFECT_NONE);
            //camParams.setFlashMode(Parameters.FLASH_MODE_OFF);
            //camParams.setFocusMode(Parameters.FOCUS_MODE_INFINITY);
            camParams.setPreviewFormat(ImageFormat.NV21);
            mCamera.setParameters(camParams);

            try {
                mCamera.setPreviewDisplay(surfaces[0]);
            }
            catch (IOException e) {

                Log.e("EngActivity", "Failed to set preview display holder");
                return null;
            }
            mCamera.setErrorCallback(new ErrorCallback() {

                public void onError(int error, Camera camera) {
                    Log.e("EngActivity", "Camera error: " + error);
                }
            });

            if (mCamStarted)
                startCamera((short)mCamSize.width, (short)mCamSize.height);
            return null;
        }
    }
    static private CameraTask mCamTask;
    static private void waitCamTaskDone() {

        if (mCamTask != null) {

            try {
                mCamTask.get();
            }
            catch (InterruptedException e) {
                Log.w("EngActivity", "Camera task has been interrupted");
            }
            catch (ExecutionException e) {
                Log.w("EngActivity", "Camera task has generated an exception");
            }
            mCamTask = null;
        }
    }

    static public boolean startCamera(short width, short height) {

        if (mCamera == null) {

            Log.e("EngActivity", "Failed to start camera: Camera not ready");
            return false;
        }
        mCamera.setDisplayOrientation(0); // Landscape (width > height)
        if (width < height) {

            Log.e("EngActivity", "Wrong camera preview format requested: Portrait");
            return false;
        }
        Parameters camParams = mCamera.getParameters();
        mCamSize = null;

        List<Size> listPrevSizes = camParams.getSupportedPreviewSizes();
        for (int i = 0; i < listPrevSizes.size(); ++i)
            if ((width == (short)listPrevSizes.get(i).width) && (height == (short)listPrevSizes.get(i).height))
                mCamSize = listPrevSizes.get(i);

        if (mCamSize == null) {

            Log.e("EngActivity", "No supported camera preview size: " + width + "*" + height);
            return false;
        }
        camParams.setPreviewSize(mCamSize.width, mCamSize.height);
        mCamera.setParameters(camParams);
        mCamera.setPreviewCallback(new Camera.PreviewCallback() {

            public synchronized void onPreviewFrame(byte[] data, Camera camera) {
                EngLibrary.loadCamera(data);
            }
        });

        mCamera.startPreview();
        mCamStarted = true;
        mCamRunning = true;
        return true;
    }
    static public boolean stopCamera() {

        if (mCamera == null) {

            Log.e("EngActivity", "Failed to stop camera: Camera not ready");
            return false;
        }
        mCamera.stopPreview();
        mCamera.setPreviewCallback(null);
        mCamStarted = false;
        mCamRunning = false;
        return true;
    }
    static private boolean resetCamera() {

    	if (mCamera == null)
    		return true;

        try {
            mCamera.setPreviewDisplay(null);
        }
        catch (IOException e) {
            Log.e("EngActivity", "Failed to reset preview display");
            return false;
        }
        mCamera.release();
        mCamera = null;
        return true;
    }

    ////// Mic
    static private final int MIC_RATE = 44100;

    static private AudioRecord mMic;
    static private short[] mMicBuffer;
    static private int mMicSize;

    static public boolean startMic() {

        if (!EngData.USES_PERMISSION_MIC) {

            Log.e("EngActivity", "Mic not ready: No 'android.permission.RECORD_AUDIO' permission");
            return false;
        }
        if (mMic != null) {

            Log.w("EngActivity", "Mic already started");
            return true;
        }

        try {

            mMicSize = AudioRecord.getMinBufferSize(MIC_RATE, AudioFormat.CHANNEL_IN_MONO,
                    AudioFormat.ENCODING_PCM_16BIT);
            mMic = new AudioRecord(MediaRecorder.AudioSource.MIC, MIC_RATE, AudioFormat.CHANNEL_IN_MONO,
                    AudioFormat.ENCODING_PCM_16BIT, mMicSize);
            mMicBuffer = new short[mMicSize];
            mMic.startRecording();
            return true;
        }
        catch (IllegalArgumentException e) { Log.e("EngActivity", "Failed to get micro recorder/info: " + e.getMessage()); }
        catch (Exception e) { Log.e("EngActivity", "Failed to get micro info/recorder: " + e.getMessage()); }

        return false;
    }
    static public void loadMic() { EngLibrary.loadMic(mMic.read(mMicBuffer, 0, mMicSize), mMicBuffer); }
    static public void stopMic() {

        if (mMic == null) {

            Log.w("EngActivity", "Mic not started");
            return;
        }
        mMic.stop();
        mMic.release();
        mMic = null;
        mMicBuffer = null;
    }

    ////// Advertising
    static private class AdRunnable implements Runnable {

    	static public final short AD_RUN_LOAD = 0;
    	static public final short AD_RUN_DISPLAY = 1;
    	static public final short AD_RUN_HIDE = 2;

    	private EngActivity mActivity;
    	private short mToDo;
    	private short mAdvertisingID;

    	public AdRunnable(EngActivity activity, short todo, short advertisingID) {

    		mActivity = activity;
    		mToDo = todo;
    		mAdvertisingID = advertisingID;
    	}
		@Override public void run() {

			switch (mToDo) {
				case AD_RUN_LOAD: EngData.loadAd(mActivity); break;
				case AD_RUN_DISPLAY: EngData.displayAd(mAdvertisingID, mActivity); break;
				case AD_RUN_HIDE: EngData.hideAd(mAdvertisingID, mActivity); break;
			}
		}
    };

    public void loadAd() { runOnUiThread(new AdRunnable(this, AdRunnable.AD_RUN_LOAD, (short)0)); }
    public void displayAd(short id) { runOnUiThread(new AdRunnable(this, AdRunnable.AD_RUN_DISPLAY, id)); }
    public void hideAd(short id) { runOnUiThread(new AdRunnable(this, AdRunnable.AD_RUN_HIDE, id)); }

    static public short getAdStatus() { return EngAdvertising.mStatus; }
    static public void setAdReqInfo(EngSocial social) {

    	EngAdvertising.mAdReqGender = social.getUserGender();
    	String birthday = social.getUserBirthday();
    	if ((birthday != null) && (birthday.length() > 9)) // ...Date format: MM/dd/yyyy
    		EngAdvertising.mAdReqBirthday = new Date(Integer.parseInt(birthday.substring(6)) - 1900, // Year
    				Integer.parseInt(birthday.substring(0, 2)) - 1, // Month
    				Integer.parseInt(birthday.substring(3, 5))); // Day
    	if ((social.getUserLocation() != null) && (!social.getUserLocation().equals(nullString)))
    		EngAdvertising.mAdReqLocation = social.getUserLocation();
    }

    ////// Internet
    public boolean isOnline() {

    	if (!EngData.USES_PERMISSION_INTERNET)
    		return false;

    	NetworkInfo netInfo = ((ConnectivityManager)getSystemService(Context.CONNECTIVITY_SERVICE)).getActiveNetworkInfo();
    	return ((netInfo != null) && (netInfo.isConnected()));
    }

    ////// UID
    static private final short UID_TYPE_ANDROID_ID = 0;
    static private final short UID_TYPE_DEVICE_ID = 1;

    public String getUID(short type) {

    	String UID = null;
    	switch (type) {
	    	case UID_TYPE_ANDROID_ID: UID = Settings.Secure.getString(getContentResolver(), Settings.Secure.ANDROID_ID); break;
	    	case UID_TYPE_DEVICE_ID: UID = ((TelephonyManager)getSystemService(Context.TELEPHONY_SERVICE)).getDeviceId(); break;
	    	// WARNING: When using the 'UID_TYPE_DEVICE_ID' type add 'android.permission.READ_PHONE_STATE' permission
    	}
    	return (UID != null)? UID:nullString;
    }

    ////// Activity
    public RelativeLayout mSurfaceLayout;

    public boolean launchApp(String Package, String activity, String data) {

    	try {
	        Intent launchIntent = getPackageManager().getLaunchIntentForPackage(Package);
	        if (launchIntent == null) {

	            Log.w("EngActivity", "Package '" + Package + "' not found");
	        	return false;
	        }
	        ComponentName component = new ComponentName(Package, activity);
	        launchIntent.setComponent(component);
	        launchIntent.setData(Uri.parse(data));

	        startActivity(launchIntent);
    	}
    	//catch (PackageManager.NameNotFoundException e) // BUG: Unreachable catch block for NameNotFoundException.
    	catch (android.content.ActivityNotFoundException e) {

            Log.w("EngActivity", e.getMessage());
    		return false;
    	}
    	catch (Exception e) {

            Log.w("EngActivity", e.getMessage());
    		return false;
    	}
    	return true;
    }

    @Override protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);

        // Always in Portrait orientation
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

        mSensorMan = (SensorManager)getSystemService(SENSOR_SERVICE);
        mAccelSensor = mSensorMan.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);

        DisplayMetrics metrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(metrics);
        EngLibrary.init(this, (int)System.currentTimeMillis(), mAccelSensor.getMaximumRange(), metrics.xdpi, metrics.ydpi);

        mResources = new EngResources(this);
        EngSurface mainSurface = new EngSurface(this);

        mCamera = null;
        SurfaceView camSurface = new SurfaceView(this);
        camSurface.getHolder().setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
        camSurface.getHolder().addCallback(new Callback() {

            public void surfaceDestroyed(SurfaceHolder holder) {

                waitCamTaskDone();
                if (EngData.USES_PERMISSION_CAMERA)
                    resetCamera();
            }
            public void surfaceCreated(SurfaceHolder holder) {
                mCamTask = (CameraTask)new CameraTask().execute(holder);
            }
            public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
                waitCamTaskDone();
            }
        });
        mCamStarted = false;
        mCamRunning = false;
        mCamSize = null;

        mMic = null;

        FrameLayout mainLayout = new FrameLayout(this);
        mainLayout.addView(camSurface);

        mSurfaceLayout = new RelativeLayout(this);
        mSurfaceLayout.setLayoutParams(new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT,
        		RelativeLayout.LayoutParams.MATCH_PARENT));
        mSurfaceLayout.addView(mainSurface);
        mSurfaceLayout.bringToFront();

        EngAdvertising.initialize(this);

        mainLayout.addView(mSurfaceLayout);
        setContentView(mainLayout);
    }
    @Override protected void onStart() {

        super.onStart();
        EngLibrary.start();
    }
    @Override protected void onResume() {

        super.onResume();
        mSensorMan.registerListener(this, mAccelSensor, SensorManager.SENSOR_DELAY_GAME);
        EngAdvertising.resume();

        EngLibrary.resume((int)System.currentTimeMillis());
        if (mCamStarted) {

            waitCamTaskDone();
            if ((mCamera != null) && (!mCamRunning))
                startCamera((short)mCamSize.width, (short)mCamSize.height);
        }
    }
    @Override protected void onPause() {

        super.onPause();
        mSensorMan.unregisterListener(this);

        if (mCamStarted) {

            waitCamTaskDone();
            if (mCamera != null)
                stopCamera();
            mCamStarted = true;
        }
        PowerManager powerManager = (PowerManager)getSystemService(POWER_SERVICE);

        EngLibrary.pause(isFinishing(), powerManager.isScreenOn());
        EngAdvertising.pause();
    }
    @Override protected void onStop() {

        EngLibrary.stop();
        super.onStop();
    }
    @Override protected void onDestroy() {

        super.onDestroy();
        EngAdvertising.destroy();
        EngLibrary.free();

        System.exit(0);
    }

    @Override protected void onActivityResult(int requestCode, int resultCode, Intent data) {

        super.onActivityResult(requestCode, resultCode, data);
        EngLibrary.result(requestCode, resultCode, data);
    }
    @Override public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
    }

    ////// Sensor
    @Override public void onAccuracyChanged(Sensor sensor, int accuracy) { }

    @Override public void onSensorChanged(SensorEvent event) {

        if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
            switch (getWindowManager().getDefaultDisplay().getRotation()) {

	            // For natural portrait orientation devices
	            case Surface.ROTATION_0:  EngLibrary.accelerometer(event.values[0], event.values[1], event.values[2]); break;
	
	            // For natural landscape orientation devices
	            case Surface.ROTATION_90: EngLibrary.accelerometer(-event.values[1], event.values[0], event.values[2]); break; 
	            case Surface.ROTATION_270: EngLibrary.accelerometer(event.values[1], -event.values[0], event.values[2]); break;
            }
        }
    }
}
