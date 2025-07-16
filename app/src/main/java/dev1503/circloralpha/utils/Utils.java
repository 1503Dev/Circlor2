package dev1503.circloralpha.utils;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.graphics.drawable.ColorDrawable;
import android.view.View;

import com.google.android.material.bottomsheet.BottomSheetBehavior;
import com.google.android.material.bottomsheet.BottomSheetDialog;
import com.google.android.material.button.MaterialButton;
import com.google.android.material.textfield.TextInputEditText;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Objects;

import dev1503.circloralpha.MainActivity;
import dev1503.circloralpha.R;

public class Utils {
    static Activity activity;
    public static void setActivity(Activity act) {
        activity = act;
    }
    public static String getABI(Context context, String packageName) {
        try {
            PackageManager pm = context.getPackageManager();
            ApplicationInfo appInfo = pm.getApplicationInfo(packageName, 0);
            if (appInfo.nativeLibraryDir != null) {
                String[] parts = appInfo.nativeLibraryDir.split("/");
                if (parts.length > 0) {
                    return parts[parts.length - 1];
                }
            }
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        return "unknown";
    }
    public static String readStringFromAssets(Context context, String fileName) {
        if (context == null || fileName == null) {
            return "";
        }
        BufferedReader reader = null;
        try {
            InputStream is = context.getAssets().open(fileName);
            reader = new BufferedReader(new InputStreamReader(is));
            StringBuilder sb = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) {
                sb.append(line).append("\n");
            }
            if (sb.length() > 0) {
                sb.deleteCharAt(sb.length() - 1);
            }
            return sb.toString();
        } catch (IOException e) {
            e.printStackTrace();
            return "";
        } finally {
            if (reader != null) {
                try {
                    reader.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
    public static String[] listFilesFromAssets(Context context, String dirName) {
        if (context == null || dirName == null) {
            return new String[0];
        }
        try {
            return context.getAssets().list(dirName);
        } catch (IOException e) {
            e.printStackTrace();
            return new String[0];
        }
    }
    public static void openPositionPicker(OnPositionPicked onPositionPicked) {
        openPositionPicker(onPositionPicked, 0, 0, 0);
    }
    public static void openPositionPicker(OnPositionPicked onPositionPicked, float dx, float dy, float dz){
        View view = View.inflate(activity, R.layout.bottomsheet_position_picker, null);
        BottomSheetDialog bottomSheetDialog = newBottomSheet(view);
//        View view = View.inflate(activity, R.layout.dialog_position_picker, null);
//        android.app.AlertDialog dialog = newDialog(view);

        MaterialButton actionButton = view.findViewById(R.id.ok);
//        MaterialButton cancelButton = view.findViewById(R.id.cancel);
        TextInputEditText xEditText = view.findViewById(R.id.x);
        TextInputEditText yEditText = view.findViewById(R.id.y);
        TextInputEditText zEditText = view.findViewById(R.id.z);

        try {
            xEditText.setText(String.valueOf(dx));
            yEditText.setText(String.valueOf(dy));
            zEditText.setText(String.valueOf(dz));
        } catch (Exception e) {
            e.printStackTrace();
        }

        actionButton.setOnClickListener(v -> {
            float x = 0, y = 0, z = 0;
            if (xEditText.getText().toString().isEmpty() || yEditText.getText().toString().isEmpty() || zEditText.getText().toString().isEmpty()) {
                return;
            }
            try {
                x = Float.parseFloat(xEditText.getText().toString());
            } catch (NumberFormatException e) {
                e.printStackTrace();
            }
            try {
                y = Float.parseFloat(yEditText.getText().toString());
            } catch (NumberFormatException e) {
                e.printStackTrace();
            }
            try {
                z = Float.parseFloat(zEditText.getText().toString());
            } catch (NumberFormatException e) {
                e.printStackTrace();
            }
            if (onPositionPicked != null) {
                onPositionPicked.onPositionPicked(x, y, z);
            }
            bottomSheetDialog.cancel();
        });
//        cancelButton.setOnClickListener(v -> {
//            bottomSheetDialog.cancel();
//        });

        bottomSheetDialog.show();
    }
    public static interface OnPositionPicked {
        void onPositionPicked(float x, float y, float z);
    }

    public static BottomSheetDialog newBottomSheet(View v) {
        BottomSheetDialog bottomSheetDialog = new BottomSheetDialog(activity);
        bottomSheetDialog.setContentView(v);
        bottomSheetDialog.setCancelable(true);
        bottomSheetDialog.setCanceledOnTouchOutside(true);

        BottomSheetBehavior<View> behavior = BottomSheetBehavior.from((View) v.getParent());
        behavior.setState(BottomSheetBehavior.STATE_EXPANDED);
        behavior.setDraggable(false);

        return bottomSheetDialog;
    }
    public static android.app.AlertDialog newDialog(View v) {
        android.app.AlertDialog dialog = new android.app.AlertDialog.Builder(activity)
                .setView(v)
                .setCancelable(false)
                .create();

        Objects.requireNonNull(dialog.getWindow()).setBackgroundDrawable(new ColorDrawable(0xBB1D1B20));

        return dialog;
    }
}
