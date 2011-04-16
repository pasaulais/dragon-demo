package fr.free.pasaulais.glinitials;

public class GLInitialsLib
{
     static
     {
    	 System.loadLibrary("tiff");
    	 System.loadLibrary("tiffdecoder");
         System.loadLibrary("glinitials_jni");
     }

     public static native void init(int width, int height);
     public static native void loadMeshFromData(String name, byte[] data);
     public static native void loadTextureFromData(String name, byte[] data, boolean mipmaps);
     public static native void render();
     public static native void dispose();
}
