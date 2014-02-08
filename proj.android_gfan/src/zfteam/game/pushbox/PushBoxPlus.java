/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package zfteam.game.pushbox;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

import net.youmi.android.AdManager;
import net.youmi.android.banner.AdSize;
import net.youmi.android.banner.AdView;
import net.youmi.android.banner.AdViewLinstener;
import net.youmi.android.smart.SmartBannerManager;
import net.youmi.android.spot.SpotDialogLinstener;
import net.youmi.android.spot.SpotManager;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.cocos2dx.lib.Cocos2dxHelper;

import android.R;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.DialogInterface.OnClickListener;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.text.style.ClickableSpan;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.widget.RelativeLayout;

public class PushBoxPlus extends Cocos2dxActivity{
	
	private static PushBoxPlus mPushBoxPlusActivity;
	private static RelativeLayout mRl;
	private static MyHandler mHandler;
	
	static public String m_youmi_id = "2333405a4f43a131";
	static public String m_youmi_key ="17a669a8f5ffe625";
	private static boolean isaddAdview=true;
	private AdView adView;
	
	public static native void initJVM();
	public static native void onClickAd();
	public static native void closeLoading();
	
    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);	
		initJVM();
		mPushBoxPlusActivity = this;
		
		AdManager.getInstance(this).init(m_youmi_id,m_youmi_key, false); 
		SmartBannerManager.init(this);
		//SmartBannerManager.show(this);
		SpotManager.getInstance(this).loadSpotAds();
		
		adView = new AdView(this, AdSize.FIT_SCREEN);
	    mRl = new RelativeLayout(this);
		mRl.setGravity(Gravity.BOTTOM | Gravity.CENTER_HORIZONTAL);
		mRl.bringToFront();
		mRl.addView(adView);
		LayoutParams params = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
		addContentView(mRl, params);
		
		Looper curLooper = Looper.myLooper (); 
        Looper mainLooper = Looper.getMainLooper (); 
        if (curLooper== null ){ 
               mHandler = new MyHandler(mainLooper); 
        } else { 
               mHandler = new MyHandler(curLooper); 
        } 
		
		
	}

    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// PushBoxPlus should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }
    
    public static void setAdViewVisable(int showflag)
	{
    	Message m = mHandler.obtainMessage(1, showflag, 1,"");
		mHandler.sendMessage(m); 
	}
	
	public static void shareToFreinds()
	{
		Log.i("weibo","autho");
		new Thread(new Runnable(){

			public void run()
			{
				String imgpath = Cocos2dxHelper.getCocos2dxWritablePath() + "/share.jpg";
				File copyfilepath = mPushBoxPlusActivity.getContext().getExternalCacheDir();
				if(null != copyfilepath){
					try {
						FileInputStream is = mPushBoxPlusActivity.getContext().openFileInput("share.jpg");
						File newImgFile = new File(copyfilepath.getPath()+"/share.jpg");
						OutputStream os = new FileOutputStream(newImgFile);
						byte[] buf = new byte[1024];
						int len;
						while ((len = is.read(buf)) != -1) 
							os.write(buf,0,len);
					} catch (FileNotFoundException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}catch (IOException e) {
						e.printStackTrace();
					}
				}
				
        		Intent intent=new Intent(Intent.ACTION_SEND);
        		intent.setType("image/*");
        		intent.putExtra(Intent.EXTRA_SUBJECT, "分享");
        		if(null != copyfilepath)
        			intent.putExtra(Intent.EXTRA_STREAM, Uri.parse("file:///"+mPushBoxPlusActivity.getContext().getExternalCacheDir()+"/share.jpg"));
        		intent.putExtra(Intent.EXTRA_TEXT, "这个推箱子关卡好难，我不会啊，求解！\n来自:推箱子");

        		mPushBoxPlusActivity.startActivity(Intent.createChooser(intent, mPushBoxPlusActivity.getTitle()));
			}
			
            
        }).start();

	}
	
	public static void onClickBackButton()
	{
		Message m = mHandler.obtainMessage(1, 2, 1,"");
		mHandler.sendMessage(m); 
	}
	
	

    static {
        System.loadLibrary("cocos2dcpp");
    }

	
	
	
	private class MyHandler extends Handler{              

        public MyHandler(Looper looper){ 
               super (looper); 
        } 

        @Override 
        public void handleMessage(Message msg) { // 处理消息 
        	
        	if (msg.arg1 == 0)
        	{
        		if(isaddAdview)
        		{
        			mRl.removeView(adView);
        			isaddAdview = false;
        		}
        	}
        	else if(msg.arg1 == 1)
        	{
        		if(!isaddAdview)
        		{
        			mRl.addView(adView);
        			isaddAdview = true;
        		}
        	}
        	else if(msg.arg1 == 2)
        	{
        		new AlertDialog.Builder(mPushBoxPlusActivity)
        		.setTitle(zfteam.game.pushbox.gfan.R.string.app_name)
        		.setMessage("退出游戏?")
        		.setPositiveButton(android.R.string.ok,
        				new OnClickListener() {
        					public void onClick(DialogInterface dialog,
        							int which) {
        						dialog.dismiss();
        						android.os.Process.killProcess(android.os.Process.myPid());
        					}
        				})
        		.setNegativeButton(android.R.string.cancel,null).
        		create().show();
        	}
        	else if(msg.arg1 == 3)
        	{
        		SpotManager.getInstance(mPushBoxPlusActivity).showSpotAds(mPushBoxPlusActivity,new YouMiAdViewLinstener());
        	}
        }             

	} 
	
	private class YouMiAdViewLinstener implements SpotDialogLinstener{

		@Override
		public void onClicked() {
			// TODO Auto-generated method stub
			onClickAd();
		}

		@Override
		public void onShowFailed() {
			// TODO Auto-generated method stub
			closeLoading();
		}

		@Override
		public void onShowSuccess() {
			// TODO Auto-generated method stub
			closeLoading();
		}

		
	}
}
