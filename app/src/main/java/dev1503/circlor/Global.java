package dev1503.circlor;

import android.content.Context;
import android.content.pm.PackageManager;
import android.util.Log;

import java.util.ArrayList;

import dev1503.circloralpha.utils.Config;
import dev1503.circloralpha.utils.Utils;

public class Global {
    static final String TAG = "Java/Global";

    public static Context context;
    public static Config conf;
    static PackageManager pm;
    public static String minecraftNativeLibPath;

    static final String DEFAULT_MINECRAFT_PACKAGE_NAME = "com.mojang.minecraftpe";
    public static final String SUPPORTED_VERSION = "1.16.201.01";
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
        Log.d(TAG, "getMinecraftVersion");
        try {
            return pm.getPackageInfo(getMinecraftPackageName(), 0).versionName;
        } catch (PackageManager.NameNotFoundException e) {
            return "";
        }
    }
    public static String[] getRequiredSOs() {
        Log.d(TAG, minecraftNativeLibPath);
        ArrayList<String> soList = new ArrayList<>();
        soList.add("c++_shared");
        soList.add("fmod");
        soList.add("minecraftpe");
        soList.add("circlor");
        return soList.toArray(new String[0]);
    }
    public static String getMinecraftABI() {
        return Utils.getABI(context, getMinecraftPackageName());
    }
}
