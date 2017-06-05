package jp.volumerenderer;

import static android.opengl.GLES30.*;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.opengl.GLSurfaceView.Renderer;

public class RendererWrapper implements Renderer
{
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config)
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height)
    {
        // No-op
    }

    @Override
    public void onDrawFrame(GL10 gl)
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }
}
