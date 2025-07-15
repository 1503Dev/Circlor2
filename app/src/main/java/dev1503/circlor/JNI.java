package dev1503.circlor;

import android.content.res.AssetManager;

public class JNI {
    public static native double getFunctionValue(String path);
    public static native String getFunctionValueString(String path);
    public static native String invokeFunction(String name);
    public static native boolean isInGame();
    public static native boolean isMinecraftFunctionExists(String name);
    public static native void setFunctionValue(String path, double value);
    public static native void setFunctionValueString(String path, String value);
    public static native void setMinecraftVersion(String version);
    public static native void setAssetManager(AssetManager assetManager);

    public static native void mc_teleport(float x, float y, float z);
}
