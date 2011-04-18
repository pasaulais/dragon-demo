package fr.free.pasaulais.glinitials;

import java.io.*;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.res.AssetFileDescriptor;
import android.opengl.GLSurfaceView;
import android.os.Bundle;

public class GLInitialsActivity extends Activity implements GLSurfaceView.Renderer
{
    GLInitialsView mView;

    @Override protected void onCreate(Bundle icicle)
    {
        super.onCreate(icicle);
        mView = new GLInitialsView(getApplication(), this);
        setContentView(mView);
    }

    @Override protected void onPause()
    {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume()
    {
        super.onResume();
        mView.onResume();
    }
    
    public void onDrawFrame(GL10 gl)
    {
        GLInitialsLib.render();
    }

    public void onSurfaceChanged(GL10 gl, int width, int height)
    {
        GLInitialsLib.initRender(width, height);
        loadMeshFromAsset("floor", "floor.obj");
        loadMeshFromAsset("letter_p", "LETTER_P.obj");
        loadMeshFromAsset("letter_a", "LETTER_A.obj");
        loadMeshFromAsset("letter_s", "LETTER_S.obj");
        loadMeshFromAsset("wing_membrane", "dragon_wing_membrane.obj");
        loadMeshFromAsset("joint", "dragon_joint_spin.obj");
        loadMeshFromAsset("dragon_chest", "dragon_chest.obj");
        loadMeshFromAsset("dragon_head", "dragon_head.obj");
        loadMeshFromAsset("dragon_tail_end", "dragon_tail_end.obj");
        loadTextureFromAsset("lava_green", "lava_green.tiff", true);
        loadTextureFromAsset("scale_gold", "scale_gold.tiff", false);
        loadTextureFromAsset("scale_black", "scale_black.tiff", false);
        loadTextureFromAsset("scale_bronze", "scale_bronze.tiff", false);
        GLInitialsLib.initScene();
    }

    public void onSurfaceCreated(GL10 gl, EGLConfig config)
    {
    }
    
    private void loadMeshFromAsset(String name, String fileName)
    {
    	byte[] data = loadAsset(fileName + ".mp3");
    	if(data != null)
    		GLInitialsLib.loadMeshFromData(name, data);
    }
    
    private void loadTextureFromAsset(String name, String fileName, boolean mipmaps)
    {
    	byte[] data = loadAsset(fileName + ".mp3");
    	if(data != null)
    		GLInitialsLib.loadTextureFromData(name, data, mipmaps);
    }
    
    private byte[] loadAsset(String fileName)
    {
    	try
    	{
    		AssetFileDescriptor fd = getAssets().openFd(fileName);
			byte[] buffer = new byte[(int)fd.getLength()];
			InputStream s = fd.createInputStream();
			s.read(buffer);
			s.close();
			fd.close();
			return buffer;
		}
    	catch(IOException e)
    	{
			e.printStackTrace();
			return null;
		}
    }
}
