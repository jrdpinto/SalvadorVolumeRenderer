package jp.volumerenderer;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity
{
    private GLSurfaceView glSurfaceView;
    boolean rendererSet = false;

    private boolean emulatorDetected()
    {
        return Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH_MR1
                && (Build.FINGERPRINT.startsWith("generic")
                || Build.FINGERPRINT.startsWith("unknown")
                || Build.MODEL.contains("google_sdk")
                || Build.MODEL.contains("Emulator")
                || Build.MODEL.contains("Android SDK built for x86"));
    }

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // TODO: Remove! Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.editText);
        tv.setText(SalvadorLib.stringFromJNI());

        ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();
        final boolean supportsEs3 = configurationInfo.reqGlEsVersion >= 0x30000 || emulatorDetected();

        if (supportsEs3)
        {
            glSurfaceView = new GLSurfaceView(this);

            if (emulatorDetected())
            {
                // Avoids crashes on startup with some emulator images.
                glSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
            }

            glSurfaceView.setEGLContextClientVersion(3);
            glSurfaceView.setRenderer(new RendererWrapper());
            rendererSet = true;
            setContentView(glSurfaceView);
        }
        else
        {
            Toast.makeText(this, "This device does not support OpenGL ES 3.0.", Toast.LENGTH_LONG).show();
            return;
        }
    }

    @Override
    protected void onPause()
    {
        super.onPause();

        if (rendererSet)
        {
            glSurfaceView.onPause();
        }
    }

    @Override
    protected void onResume()
    {
        super.onResume();

        if (rendererSet)
        {
            glSurfaceView.onResume();
        }
    }
}
