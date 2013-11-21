package com.crickettechnology.samples.HelloCricketNDK;

import android.app.Activity;
import android.os.Bundle;

public class HelloCricketActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        helloCricketNative();
    }

    private native void helloCricketNative();

    static
    {
        // load our native library
        System.loadLibrary("hellocricket");
    }
}
