package jp.volumerenderer;

/**
 * Created by jp on 13/06/2017.
 * Description: An interface to the Salvador library that utilises JNI.
 */

public class SalvadorLib
{
    static
    {
        System.loadLibrary("salvador-lib");
    }

    public native static void initRenderer();
    public native static void drawFrame();
    public native static void resizeWindow(int width, int height);
}