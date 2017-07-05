package jp.volumerenderer;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.WindowManager;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity
{
    private GLSurfaceView glSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();

        if (configurationInfo.reqGlEsVersion >= 0x30000 )
        {
            glSurfaceView = new GLESSurface(this);
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

        if (glSurfaceView != null)
        {
            glSurfaceView.onPause();
        }
    }

    @Override
    protected void onResume()
    {
        super.onResume();

        if (glSurfaceView != null)
        {
            glSurfaceView.onResume();
        }
    }
}
