package dev1503.circloralpha;

import android.app.Activity;

import dev1503.circlor.MinecraftActivityHandler;

public class FakeActivity extends Activity {
    static {
        System.loadLibrary("circlor");
    }
    @Override
    protected void onCreate(android.os.Bundle savedInstanceState) {
        MinecraftActivityHandler.onCreate(this);
        super.onCreate(savedInstanceState);
        MinecraftActivityHandler.onCreating(this);
        MinecraftActivityHandler.onCreated(this);
    }
}
