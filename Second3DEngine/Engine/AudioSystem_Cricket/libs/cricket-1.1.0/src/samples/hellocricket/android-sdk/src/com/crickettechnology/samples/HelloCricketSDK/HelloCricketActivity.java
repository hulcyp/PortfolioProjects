package com.crickettechnology.samples.HelloCricketSDK;

import android.app.Activity;
import android.os.Bundle;

import com.crickettechnology.audio.*;

public class HelloCricketActivity extends Activity 
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) 
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        // init cricket
        Config config = new Config();
        config.NativeDebug = true; // load debug native libraries rather than release
        Ck.init(this, config);

        Bank bank = null;
        Sound sound = null;
        try
        {
            bank = Bank.newBank("hellocricket.ckb");
            sound = Sound.newBankSound(bank, 0);
            sound.play();

            while (sound.isPlaying())
            {
                Ck.update();
                Thread.sleep(30);
            }
        }
        catch (java.lang.Exception ex)
        {
            System.out.printf("exception: %s\n", ex.toString());
        }
        finally
        {
            if (sound != null)
            {
                sound.destroy();
            }
            if (bank != null)
            {
                bank.destroy();
            }
        }

        Ck.shutdown();
    }
}
