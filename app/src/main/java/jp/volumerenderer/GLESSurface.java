package jp.volumerenderer;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.MotionEvent;

/**
 * Created by jp on 4/07/2017.
 * Description: OpenGL surface that sub-classes the standard GLSurfaceView in order to override
 * its touch event callbacks.
 */

public class GLESSurface extends GLSurfaceView
{
    static String TAG_ = "GLESSurface";
    float prevX = 0, prevY = 0;

    public GLESSurface(Context context)
    {
        super(context);

        setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        setEGLContextClientVersion(3);
        setRenderer(new RendererWrapper());
    }

    @Override
    public boolean onTouchEvent(MotionEvent e)
    {
        // X & Y coordinates need to be normalised for OpenGL ES (i.e: -1 to 1 on both axes)
        final float normalizedX = (e.getX() / (float) getWidth()) * 2 - 1;
        final float normalizedY = -((e.getY() / (float) getHeight())*2-1);

        switch (e.getAction())
        {
            case MotionEvent.ACTION_MOVE:
                final float vectorX = normalizedX-prevX;
                final float vectorY = normalizedY-prevY;

                this.queueEvent(new Runnable() {
                    @Override
                    public void run() {
                        SalvadorLib.handleTouchDrag(vectorX, vectorY);
                    }});

                break;
            case MotionEvent.ACTION_UP:
                this.queueEvent(new Runnable() {
                    @Override
                    public void run() {
                        SalvadorLib.handleTouchUp(normalizedX, normalizedY);
                    }});
        }

        prevX = normalizedX;
        prevY = normalizedY;

        return true;
    }
}