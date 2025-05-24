package dev1503.circlor2;

import android.app.Activity;

public class FakeActivity extends Activity {
    static {
        System.loadLibrary("circlor2");
    }
    @Override
    protected void onCreate(android.os.Bundle savedInstanceState) {
        MinecraftActivityHandler.onCreate(this);
        super.onCreate(savedInstanceState);
        MinecraftActivityHandler.onCreating(this);
        MinecraftActivityHandler.onCreated(this);
    }
}
