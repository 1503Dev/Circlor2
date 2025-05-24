package dev1503.circlor2;

import android.content.Context;
import android.content.pm.PackageManager;
import android.util.Log;

import java.util.ArrayList;

import dev1503.circlor2.utils.Config;
import dev1503.circlor2.utils.Utils;
import tc.jsin.JSONObject;

public class Global {
    static final String TAG = "Java/Global";

    public static Context context;
    static Config conf;
    static PackageManager pm;
    public static String minecraftNativeLibPath;

    static final String DEFAULT_MINECRAFT_PACKAGE_NAME = "com.mojang.minecraftpe";
    public static final boolean fakeActivityEnabled = true;

    public static void setContext(Context ctx) {
        context = ctx;
        conf = new Config(context);
        pm = context.getPackageManager();
    }

    public static String getMinecraftPackageName() {
        return conf.getString("minecraft_package_name", DEFAULT_MINECRAFT_PACKAGE_NAME);
    }
    public static void setMinecraftPackageName(String packageName) {
        conf.put("minecraft_package_name", packageName);
    }
    public static String getMinecraftVersion() {
        try {
            return pm.getPackageInfo(getMinecraftPackageName(), 0).versionName;
        } catch (PackageManager.NameNotFoundException e) {
            return "";
        }
    }
    public static String[] getRequiredSOs() {
        Log.d(TAG, minecraftNativeLibPath);
        /*System.loadLibrary("c++_shared");
        try {
            System.load(minecraftNativeLibPath + "/libfmod.so");
            System.load(minecraftNativeLibPath + "/libminecraftpe.so");
        } catch (UnsatisfiedLinkError e) {
            Log.e(TAG, "加载 libfmod.so 或 libminecraftpe.so 失败");
            return new String[0];
        }
        System.loadLibrary("circlor2");*/
        ArrayList<String> soList = new ArrayList<>();
        soList.add("c++_shared");
        soList.add("fmod");
        soList.add("minecraftpe");
        soList.add("circlor2");
        return soList.toArray(new String[0]);
    }
    public static String getMinecraftABI() {
        return Utils.getABI(context, getMinecraftPackageName());
    }
}
