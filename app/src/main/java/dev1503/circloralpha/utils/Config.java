package dev1503.circloralpha.utils;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.util.Log;

public class Config {
    public static String TAG = "Java/Config";

    String name;
    Context context;
    SharedPreferences sp;

    public Config(Context context, String name) {
        this.context = context;
        this.name = name;
        sp = context.getSharedPreferences(name, Context.MODE_PRIVATE);
    }
    public Config(Context context) {
        this.context = context;
        this.name = context.getPackageName() + "_shared_preferences";
        sp = PreferenceManager.getDefaultSharedPreferences(context);
    }
    public Config put(String key, String value) {
        sp.edit().putString(key, value).apply();
        Log.d(TAG, "<" + name + "> " + key + " = " + value);
        return this;
    }
    public Config put(String key, int value) {
        sp.edit().putInt(key, value).apply();
        Log.d(TAG, "<" + name + "> " + key + " = " + value);
        return this;
    }
    public Config put(String key, float value) {
        sp.edit().putFloat(key, value).apply();
        Log.d(TAG, "<" + name + "> " + key + " = " + value);
        return this;
    }
    public Config put(String key, boolean value) {
        sp.edit().putBoolean(key, value).apply();
        Log.d(TAG, "<" + name + "> " + key + " = " + value);
        return this;
    }
    public String getString(String key, String def) {
        return sp.getString(key, def);
    }
    public int getInt(String key, int def) {
        try {
            return sp.getInt(key, def);
        } catch (Exception e) {
            Log.e(TAG, "Error when getting int value: " + key, e);
            return def;
        }
    }
    public boolean getBoolean(String key, boolean def) {
        return sp.getBoolean(key, def);
    }
    public float getFloat(String key, float def) {
        return sp.getFloat(key, def);
    }

    public void remove(String key) {
        sp.edit().remove(key).apply();
    }
    public String[] getKeys() {
        return sp.getAll().keySet().toArray(new String[0]);
    }
    public boolean has(String key) {
        return sp.contains(key);
    }
    public boolean renameStringItem(String oldKey, String newKey) {
        if (has(oldKey)) {
            if (has(newKey)) {
                Log.e(TAG, "Key " + newKey + " already exists");
                return false;
            }
            put(newKey, getString(oldKey, ""));
            remove(oldKey);
            return true;
        }
        return false;
    }
}

