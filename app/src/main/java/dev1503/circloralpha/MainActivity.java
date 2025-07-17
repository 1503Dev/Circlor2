package dev1503.circloralpha;

import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.Toast;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import com.google.android.material.dialog.MaterialAlertDialogBuilder;
import com.google.android.material.navigation.NavigationView;
import com.google.android.material.textfield.TextInputEditText;

import dev1503.circlor.Global;
import dev1503.circloralpha.utils.Utils;
import eightbitlab.com.blurview.BlurView;
import tc.jsin.JSONArray;

public class MainActivity extends AppCompatActivity {
    static final String TAG = "Java/MainActivity";
    static final boolean AUTO_LAUNCH = true;

    public static MainActivity self;
    PackageManager pm;

    TextInputEditText editText_packageName;
    NavigationView navigationView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "+onCreate+");

        super.onCreate(savedInstanceState);
        self = this;
        pm = getPackageManager();
        Global.setContext(self);

        hideSystemUI();

        setContentView(R.layout.activity_main);
        initViews();

        Log.d(TAG, "-onCreate-");
    }

    private void hideSystemUI() {
        if (getWindow() == null) return;

        View decorView = getWindow().getDecorView();

        // 设置系统UI标志
        int uiOptions = View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;
        decorView.setSystemUiVisibility(uiOptions);

        decorView.setOnSystemUiVisibilityChangeListener(visibility -> {
            if ((visibility & View.SYSTEM_UI_FLAG_FULLSCREEN) == 0) {
                decorView.setSystemUiVisibility(uiOptions);
            }
        });

        getWindow().setStatusBarColor(Color.TRANSPARENT);

        if (getSupportActionBar() != null) {
            getSupportActionBar().hide();
        }
    }

    void initViews(){
        editText_packageName = findViewById(R.id.editText_packageName);
        editText_packageName.setText(Global.getMinecraftPackageName());
        editText_packageName.addTextChangedListener(new TextWatcher(){

            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                Global.setMinecraftPackageName(s.toString());
            }

            @Override
            public void afterTextChanged(Editable s) {

            }
        });

        findViewById(R.id.button_launch).setOnClickListener(v -> {
            try {
                pm.getApplicationInfo(Global.getMinecraftPackageName(), PackageManager.GET_META_DATA);

                Log.d(TAG, Global.getMinecraftABI());
                if (!Global.getMinecraftABI().startsWith("arm64")){
                    Toast.makeText(self, "MinecraftPE 不是 arm64-v8a 版本", Toast.LENGTH_SHORT).show();
                    return;
                }
                if (!Global.SUPPORTED_VERSION.equals(Global.getMinecraftVersion())){
                    Toast.makeText(self, "不兼容的 MinecraftPE 版本: " + Global.getMinecraftVersion() + "\n悬浮窗将不会被加载，请使用 " + Global.SUPPORTED_VERSION, Toast.LENGTH_SHORT).show();
                }

                Intent intent = new Intent(self, PrelaunchActivity.class);
                startActivity(intent);
            } catch (PackageManager.NameNotFoundException e) {
                Toast.makeText(self, "MinecraftPE 未安装", Toast.LENGTH_SHORT).show();
            }
        });
        if (AUTO_LAUNCH){
            findViewById(R.id.button_launch).performClick();
        }

        navigationView = findViewById(R.id.navigation_view);
        navigationView.setNavigationItemSelectedListener(item -> {
            int id = item.getItemId();
            if (id == R.id.nav_about) {
                AlertDialog dialog = new MaterialAlertDialogBuilder(self, R.style.Theme_BlurDialog)
                        .setTitle("关于")
                        .setMessage("""
CirclorAlpha
Minecraft v""" + Global.getMinecraftVersion() + """
""" + Global.getMinecraftABI() + """


""" + getString(R.string.credits) + """
:
  TheChuan1503

""" + getString(R.string.special_thanks) + """
:
  白无
  神墨

""" + getString(R.string.OSL) + """
:
  Material Icons - Apache-2.0
  Material Components for Android - Apache-2.0
  AndroidX - Apache-2.0
  BlurView - Apache-2.0
  JSIN - Apache-2.0
  LeviLamina - GPL-3.0
  mcpe-headers
  libminecraftpe.so-ida-analysis

Copyright (c) 2023-2025 1503Dev
https://circloralpha.1503dev.top""")
                        .setPositiveButton("确定", null).create();
                dialog.show();
                FrameLayout rootView = (FrameLayout) dialog.getWindow().getDecorView();
                // FrameLayout contentLayout = rootView.findViewById(android.R.id.content);

                dialog.getWindow().setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));
                BlurView blurView = new BlurView(self);
                ViewGroup.LayoutParams params = new ViewGroup.LayoutParams(
                        ViewGroup.LayoutParams.MATCH_PARENT,
                        ViewGroup.LayoutParams.MATCH_PARENT
                );
                blurView.setLayoutParams(params);
                rootView.addView(blurView, 0);
                blurView.setupWith(rootView)
                        .setBlurRadius(20f)
                        .setOverlayColor(Color.argb(100, 0, 0, 0));

                /*dialog.getWindow().setDimAmount(0.2f);
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
                    dialog.getWindow().setBackgroundBlurRadius(100);
                    dialog.getWindow().addFlags(WindowManager.LayoutParams.FLAG_BLUR_BEHIND);
                }*/
            }
            return false;
        });
    }
}