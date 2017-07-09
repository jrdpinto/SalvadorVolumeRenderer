package jp.volumerenderer;

import android.content.res.AssetManager;

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

    // Rendering events
    public native static void initRenderer();
    public native static void drawFrame();
    public native static void resizeWindow(int width, int height);

    // Application events
    public native static void loadVolume(String filePath, AssetManager assetManager);

    // Touch events
    public native static void handleDragEvent(float x, float y);
    public native static void handleTouchUp(float x, float y);
}