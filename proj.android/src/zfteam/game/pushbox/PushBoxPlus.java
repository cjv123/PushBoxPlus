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

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.cocos2dx.lib.Cocos2dxHelper;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;

public class PushBoxPlus extends Cocos2dxActivity{
	
	static PushBoxPlus mPushBoxPlusActivity;
	
    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);	
		initJVM();
		mPushBoxPlusActivity = this;
	}

    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// PushBoxPlus should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }
    
    public static void setAdViewVisable(int showflag)
	{
        
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
        		intent.putExtra(Intent.EXTRA_SUBJECT, "·ÖÏí");
        		if(null != copyfilepath)
        			intent.putExtra(Intent.EXTRA_STREAM, Uri.parse("file:///"+mPushBoxPlusActivity.getContext().getExternalCacheDir()+"/share.jpg"));
        		intent.putExtra(Intent.EXTRA_TEXT, "test");

        		mPushBoxPlusActivity.startActivity(Intent.createChooser(intent, mPushBoxPlusActivity.getTitle()));
			}
			
            
        }).start();

	}
	
	public static native void initJVM();

    static {
        System.loadLibrary("cocos2dcpp");
    }     
}
