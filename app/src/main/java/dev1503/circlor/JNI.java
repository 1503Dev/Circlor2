package dev1503.circlor;

import android.content.res.AssetManager;

import com.bytedance.android.bytehook.ByteHook;
import com.bytedance.shadowhook.ShadowHook;

public class JNI {
//    static {
//        System.loadLibrary("circlor");
//    }

    public static String[] functionNames;
    public static long[] functionAddresses;

    public static native double getFunctionValue(String path);
    public static native String getFunctionValueString(String path);
    public static native String invokeFunction(String name);
    public static native boolean isInGame();
    public static native boolean isMinecraftFunctionExists(String name);
    public static native void setFunctionValue(String path, double value);
    public static native void setFunctionValueString(String path, String value);
    public static native void setMinecraftVersion(String version);
    public static native void setAssetManager(AssetManager assetManager);

    public static String[] getFunctionNames(){
        return functionNames;
    }
    public static long[] getFunctionAddresses(){
        return functionAddresses;
    }
    public static void setFunctionTable(String[] functionName, long[] functionAddress){
        functionNames = functionName;
        functionAddresses = functionAddress;
    }
    public static void initHook(){
        ByteHook.init();
        ShadowHook.init(new ShadowHook.ConfigBuilder()
                .setMode(ShadowHook.Mode.SHARED)
                .build());
    }

    public static native void mc_teleport(float x, float y, float z);
}
