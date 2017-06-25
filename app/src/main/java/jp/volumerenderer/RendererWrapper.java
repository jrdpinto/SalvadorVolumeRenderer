package jp.volumerenderer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.opengl.GLSurfaceView.Renderer;
import android.util.Log;

public class RendererWrapper implements Renderer
{
    final String TAG = "RendererWrapper";

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config)
    {
        SalvadorLib.initRenderer();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height)
    {
        Log.d(TAG, "Window resized to " + width + "x" + height);
        SalvadorLib.resizeWindow(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl)
    {
        SalvadorLib.drawFrame();
    }
}
