package dev1503.circloralpha;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Build;
import android.os.Bundle;
import android.util.Base64;
import android.util.Log;
import android.view.Gravity;
import android.view.WindowManager;
import android.widget.LinearLayout;
import android.widget.TextView;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.nio.channels.FileChannel;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Objects;
import java.util.concurrent.Executors;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

import dev1503.circlor.Global;

public class PrelaunchActivity extends Activity {
    static final String TAG = "Java/PrelaunchActivity";

    private static final String MC_PACKAGE_NAME = Global.getMinecraftPackageName();
    private static final String LAUNCHER_DEX_NAME = "circlor/core.dex";
    static final String LOCAL_LAUNCHER_DEX_NAME = "core.dex";

    public static TextView textView;

    static PrelaunchActivity self;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "+onCreate+");
        super.onCreate(savedInstanceState);
        getWindow().setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_NAVIGATION);
        self = this;

        textView = new TextView(this);
        textView.setLayoutParams(new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.MATCH_PARENT,
                LinearLayout.LayoutParams.MATCH_PARENT
        ));
        textView.setGravity(Gravity.CENTER);
        textView.setText("PrelaunchActivity");
        textView.setBackgroundColor(0x60000000);
        setContentView(textView);

        launch();
        Log.d(TAG, "-onCreate-");
    }
    public void launch(){
        Log.i(TAG, "Launching minecraft");
        Executors.newSingleThreadExecutor().execute(new Runnable(){
            @Override
            public void run()
            {
                try {
                    if (Build.VERSION.SDK_INT <= 27 && Global.fakeActivityEnabled) {
                        Intent intent = new Intent(self, FakeActivity.class);
                        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                        startActivity(intent);
                        return;
                    }
                    File cacheDexDir = new File(getCodeCacheDir(), "dex");
                    handleCacheCleaning(cacheDexDir);
                    ApplicationInfo mcInfo = getPackageManager().getApplicationInfo(MC_PACKAGE_NAME, PackageManager.GET_META_DATA);
                    Object pathList = getPathList(getClassLoader());
                    processDexFiles(mcInfo, cacheDexDir, pathList);
                    Global.minecraftNativeLibPath = new File(mcInfo.nativeLibraryDir).getAbsolutePath();
                    processNativeLibraries(mcInfo, pathList);
                    launchMinecraft(mcInfo);

                    if (MainActivity.self != null) {
                        MainActivity.self.finish();
                    }
                } catch (Exception e) {
                    Log.e(TAG, "Exception in run()", e);
                    if (Build.VERSION.SDK_INT <= 27) {
                        Logger.e("不兼容安卓 8.1 及以下的设备");
                        if (Global.fakeActivityEnabled) {
                            Intent intent = new Intent(self, FakeActivity.class);
                            intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                            startActivity(intent);
                        }
                        // self.finish();
                    } else {
//                        throw new RuntimeException(e);
                        Logger.e("加载失败: " + e.getClass().toString());
                    }
                }
            }
        });
    }
    @SuppressLint("SetTextI18n")
    private void handleCacheCleaning( final File cacheDexDir) {
        Logger.i("Cleaning cache");
        if (cacheDexDir.exists() && cacheDexDir.isDirectory()) {
            Log.i(TAG, "Cache directory exists: " + cacheDexDir.getAbsolutePath());
            File[] files = cacheDexDir.listFiles();
            if (files != null) {
                for (final File file : files) {
                    if (file.delete()) {
                        Log.i(TAG, "Deleted " + file.getAbsolutePath());
                    }
                }
            } else {
                Log.i(TAG, "No files to delete in " + cacheDexDir.getAbsolutePath());
            }
        } else {
            Log.i(TAG, "Cache directory does not exist or is not a directory: " + cacheDexDir.getAbsolutePath());
        }
    }

    private Object getPathList( ClassLoader classLoader) throws Exception {
        Field pathListField = Objects.requireNonNull(classLoader.getClass().getSuperclass()).getDeclaredField("pathList");
        pathListField.setAccessible(true);
        return pathListField.get(classLoader);
    }


    private void processDexFiles(final ApplicationInfo mcInfo, File cacheDexDir,  Object pathList) throws Exception {
        Logger.i("Patching dex files");
        Method addDexPath = pathList.getClass().getDeclaredMethod("addDexPath", String.class, File.class);
        final File launcherDex = new File(cacheDexDir, LOCAL_LAUNCHER_DEX_NAME);

        copyFile(getAssets().open(LAUNCHER_DEX_NAME), launcherDex);
        Log.i(TAG, "Copied " + LAUNCHER_DEX_NAME + " to " + launcherDex.getAbsolutePath());

        if (launcherDex.setReadOnly()) {
            addDexPath.invoke(pathList, launcherDex.getAbsolutePath(), null);
            Log.i(TAG, "Added " + LAUNCHER_DEX_NAME + " to dex path list");
        }

        try (ZipFile zipFile = new ZipFile(mcInfo.sourceDir)) {
            for (int i = 2; i >= 0; i--) {
                final String dexName = "classes" + (i == 0 ? "" : i) + ".dex";
                ZipEntry dexFile = zipFile.getEntry(dexName);
                if (dexFile != null) {
                    final File mcDex = new File(cacheDexDir, dexName);
                    copyFile(zipFile.getInputStream(dexFile), mcDex);
                    Log.i(TAG, "Copied " + dexName + " to " + mcDex.getAbsolutePath());

                    if (mcDex.setReadOnly()) {
                        addDexPath.invoke(pathList, mcDex.getAbsolutePath(), null);
                        Log.i(TAG, "Added " + dexName + " to dex path list");
                    }
                }
            }
        }
        Logger.i("Dex files patched");

    }


    private void processNativeLibraries( final ApplicationInfo mcInfo,  Object pathList) throws Exception {
        Logger.i("Patching native libraries");
        Method addNativePath = pathList.getClass().getDeclaredMethod("addNativePath", Collection.class);
        ArrayList<String> libDirList = new ArrayList<>();
        libDirList.add(mcInfo.nativeLibraryDir);
        addNativePath.invoke(pathList, libDirList);
        Log.i(TAG, "Added native library directory: " + mcInfo.nativeLibraryDir);
        Logger.i("Native libraries patched");
    }


    private void launchMinecraft(ApplicationInfo applicationInfo) throws ClassNotFoundException {
        try {
            Class<?> launcherClass = getClassLoader().loadClass("dev1503.circlor.MinecraftActivity");
            if (launcherClass == null) {
                throw new ClassNotFoundException("MinecraftActivity not found");
            }
            Intent intent = new Intent(this, launcherClass);
            intent.putExtra("MC_SRC", applicationInfo.sourceDir);
            if (applicationInfo.splitSourceDirs != null) {
                ArrayList<String> splitSources = new ArrayList<>(Arrays.asList(applicationInfo.splitSourceDirs));
                intent.putExtra("MC_SPLIT_SRC", splitSources);
            }
            Logger.i("Launching minecraft");
            startActivity(intent);
            finish();
        } catch (ClassNotFoundException e) {
            Log.e(TAG, "MinecraftActivity not found", e);
            throw e;
        }
    }
    private static void copyFile(InputStream from,  File to) throws IOException {
        File parentDir = to.getParentFile();
        if (parentDir != null && !parentDir.exists() && !parentDir.mkdirs()) {
            throw new IOException("Failed to create directories");
        }
        if (!to.exists() && !to.createNewFile()) {
            throw new IOException("Failed to create new file");
        }
        try (BufferedInputStream input = new BufferedInputStream(from);
             BufferedOutputStream output = new BufferedOutputStream(Files.newOutputStream(to.toPath()))) {
            byte[] buffer = new byte[8192];
            int bytesRead;
            while ((bytesRead = input.read(buffer)) != -1) {
                output.write(buffer, 0, bytesRead);
            }
        }
    }
    public static String base64Encode(String input) {
        return base64Encode(input.getBytes());
    }

    public static String base64Encode(byte[] input) {
        return Base64.encodeToString(input, Base64.DEFAULT);
    }
    public static String base64Decode(String input) {
        return new String(Base64.decode(input, Base64.DEFAULT));
    }
    public static void copy(File from, File to) throws IOException {
        if (!from.exists())
            throw new IOException("File " + from.getAbsolutePath() + " does not exists.");
        if (!to.exists()) {
            if (to.getParentFile() != null && !to.getParentFile().exists()) {
                boolean mkdirsResult = to.getParentFile().mkdirs();
                if (!mkdirsResult)
                    throw new IOException("Failed to mkdirs: " + to.getParentFile().getAbsolutePath() + ".");
            }
            boolean createdNewFile = to.createNewFile();
            if (!createdNewFile)
                throw new IOException("Failed to create new file at: " + to.getAbsolutePath() + ".");
        }
        FileChannel input = new FileInputStream(from).getChannel();
        FileChannel output = new FileOutputStream(to).getChannel();
        output.transferFrom(input, 0, input.size());
        input.close();
        output.close();
    }

    static class Logger {
        static final String TAG = "Java/PrelaunchActivity$Logger";

        public static void i(String message) {
            runOnUI(() -> {
                textView.setTextColor(0xFFFFFFFF);
                textView.setText(message);
            });
        }
        public static void e(String message) {
            runOnUI(() -> {
                textView.setTextColor(0xFFFF0000);
                textView.setText(message);
            });
        }
        public static void e(Throwable throwable) {
            runOnUI(() -> {
                textView.setTextColor(0xFFFF0000);
                textView.setText(throwable.toString());
            });
        }
        public static void d(String message) {
            runOnUI(() -> {
                textView.setTextColor(0xFF00FF00);
                textView.setText(message);
            });
        }

        static void runOnUI(Runnable runnable) {
            self.runOnUiThread(runnable);
        }
    }
}
