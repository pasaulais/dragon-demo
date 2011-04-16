package fr.free.pasaulais.glinitials;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.*;
import javax.microedition.khronos.opengles.*;

class GLInitialsView extends GLSurfaceView
{
    public GLInitialsView(Context context)
    {
        super(context);
        init(false, 0, 0);
    }

    public GLInitialsView(Context context, boolean translucent, int depth, int stencil)
    {
        super(context);
        init(translucent, depth, stencil);
    }

    private void init(boolean translucent, int depth, int stencil)
    {
        if(translucent)
            this.getHolder().setFormat(PixelFormat.TRANSLUCENT);
        setEGLConfigChooser(translucent ?
                            new ConfigChooser(8, 8, 8, 8, depth, stencil) :
                            new ConfigChooser(5, 6, 5, 0, depth, stencil));
        setRenderer(new Renderer());
    }

    private static class ConfigChooser implements GLSurfaceView.EGLConfigChooser
    {
        public ConfigChooser(int r, int g, int b, int a, int depth, int stencil)
        {
            mRedSize = r;
            mGreenSize = g;
            mBlueSize = b;
            mAlphaSize = a;
            mDepthSize = depth;
            mStencilSize = stencil;
        }

        private static int EGL_OPENGL_ES_BIT = 1;
        //private static int EGL_OPENGL_ES2_BIT = 4;
        private static int[] s_configAttribs2 =
        {
            EGL10.EGL_RED_SIZE, 4,
            EGL10.EGL_GREEN_SIZE, 4,
            EGL10.EGL_BLUE_SIZE, 4,
            EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
            EGL10.EGL_NONE
        };

        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display)
        {
            int[] num_config = new int[1];
            egl.eglChooseConfig(display, s_configAttribs2, null, 0, num_config);

            int numConfigs = num_config[0];
            if(numConfigs <= 0)
                throw new IllegalArgumentException("No configs match configSpec");
            EGLConfig[] configs = new EGLConfig[numConfigs];
            egl.eglChooseConfig(display, s_configAttribs2, configs, numConfigs, num_config);

            return chooseConfig(egl, display, configs);
        }

        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display, EGLConfig[] configs)
        {
            for(EGLConfig config : configs)
            {
                int d = findConfigAttrib(egl, display, config, EGL10.EGL_DEPTH_SIZE, 0);
                int s = findConfigAttrib(egl, display, config, EGL10.EGL_STENCIL_SIZE, 0);

                // We need at least mDepthSize and mStencilSize bits
                if(d < mDepthSize || s < mStencilSize)
                    continue;

                // We want an *exact* match for red/green/blue/alpha
                int r = findConfigAttrib(egl, display, config, EGL10.EGL_RED_SIZE, 0);
                int g = findConfigAttrib(egl, display, config, EGL10.EGL_GREEN_SIZE, 0);
                int b = findConfigAttrib(egl, display, config, EGL10.EGL_BLUE_SIZE, 0);
                int a = findConfigAttrib(egl, display, config, EGL10.EGL_ALPHA_SIZE, 0);

                if(r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize)
                    return config;
            }
            return null;
        }

        private int findConfigAttrib(EGL10 egl, EGLDisplay display, EGLConfig config, int attribute, int defaultValue)
        {
            if(egl.eglGetConfigAttrib(display, config, attribute, mValue))
                return mValue[0];
            return defaultValue;
        }

        // Subclasses can adjust these values:
        protected int mRedSize;
        protected int mGreenSize;
        protected int mBlueSize;
        protected int mAlphaSize;
        protected int mDepthSize;
        protected int mStencilSize;
        private int[] mValue = new int[1];
    }

    private static class Renderer implements GLSurfaceView.Renderer
    {
        public void onDrawFrame(GL10 gl)
        {
            GLInitialsLib.render();
        }

        public void onSurfaceChanged(GL10 gl, int width, int height)
        {
            GLInitialsLib.init(width, height);
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config)
        {
            // Do nothing.
        }
    }
}
