package dev1503.circlor2;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.net.Uri;
import android.os.Build;
import android.view.View;

import androidx.appcompat.widget.AppCompatEditText;

import com.google.android.material.materialswitch.MaterialSwitch;
import com.google.android.material.slider.Slider;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;
import java.util.concurrent.atomic.AtomicReference;

import dev1503.circlor2.utils.Utils;

public class FunctionHandler {
    static final String TAG = "Java/FunctionHandler";

    public static Map<View, String> functionViews = new HashMap<>();

    public static String S_NOT_IN_GAME = "not_in_game";

    public static String invokeFunction(String path) {
        String originalPath = path;
        path = path.toLowerCase();
        if (path.startsWith(":")) {
            if (path.startsWith(":jump_url/")) {
                Intent intent = new Intent(Intent.ACTION_VIEW);
                intent.setData(Uri.parse(path.substring(":jump_url/".length())));
                intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                MinecraftActivityHandler.activity.startActivity(intent);
            }
            return "";
        }
        switch (path) {
            case "exit":
                System.exit(0);
                break;
            case "teleport/picker":
                if (!JNI.isInGame()) {
                    return S_NOT_IN_GAME;
                }
                Utils.openPositionPicker(JNI::mc_teleport);
                break;
            default:
                return JNI.invokeFunction(path);
        }
        return "";
    }

    public static boolean getEnabled(String path) {
        double value = JNI.getFunctionValue(path);
        return value == 1;
    }
    public static void setEnabled(String path, boolean enabled) {
        JNI.setFunctionValue(path, enabled ? 1 : 0);
    }
    public static float getFloat(String path) {
        return (float) JNI.getFunctionValue(path);
    }
    public static void setFloat(String path, float value) {
        JNI.setFunctionValue(path, value);
    }
    public static String getString(String path) {
        return JNI.getFunctionValueString(path);
    }
    public static void setString(String path, String value) {
        JNI.setFunctionValueString(path, value);
    }

    public static void addFunctionView(View view, String path) {
        functionViews.put(view, path);
    }
    public static void update(Activity activity) {
        // 更新 Views
        for (Map.Entry<View, String> entry : functionViews.entrySet()) {
            try {
                View view = entry.getKey();
                String path = entry.getValue();
                if(view == null || path.isEmpty()) continue;
                if (view instanceof MaterialSwitch) {
                    MaterialSwitch materialSwitch = (MaterialSwitch) view;
                    if (materialSwitch.isChecked() != getEnabled(path)) {
                        materialSwitch.setChecked(getEnabled(path));
                    }
                } else if (view instanceof Slider) {
                    Slider slider = (Slider) view;
                    if (slider.getValue()!= getFloat(path)) {
                        float value = getFloat(path);
                        if (value >= slider.getValueFrom() && value <= slider.getValueTo()) {
                            slider.setValue(value);
                        }
                    }
                } else if (view instanceof AppCompatEditText){
                    AppCompatEditText editText = (AppCompatEditText) view;
                    if (!Objects.requireNonNull(editText.getText()).toString().equals(getString(path))) {
                        editText.setText(getString(path));
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        if (FunctionHandler.getEnabled("portrait_screen")){
            if (activity.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE){
                activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
            }
        } else {
            if (activity.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT){
                activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
            }
        }
    }
}
