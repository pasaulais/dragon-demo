package fr.free.pasaulais.glinitials;

import android.app.Activity;
import android.os.Bundle;

public class GLInitialsActivity extends Activity {

    GLInitialsView mView;

    @Override protected void onCreate(Bundle icicle)
    {
        super.onCreate(icicle);
        mView = new GLInitialsView(getApplication());
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
}
