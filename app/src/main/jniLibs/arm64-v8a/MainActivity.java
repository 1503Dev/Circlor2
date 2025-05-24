package dev1503.circlor2;


import android.animation.AnimatorSet;
import android.animation.ObjectAnimator;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.graphics.drawable.GradientDrawable;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Base64;
import android.util.DisplayMetrics;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.View;
import android.view.animation.DecelerateInterpolator;
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

public class MainActivity extends Activity {

 
	private static final String MC_PACKAGE_NAME = "com.mojang.minecraftpe";
    private static final String LAUNCHER_DEX_NAME = "launcher.dex";
    
    public static LinearLayout Main;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
		

        setContentView(R.layout.activity_main);
        JNI.setContext(this);
     StartGame();
    }
        public void StartGame(){
        /*
   如果想要在启动mc时添加自己的代码，需要去launcher.dex里面的com.mojang.minecraftpe.Launcher类里面的onCreate里面写调用的代码，既可
   如果发现运行后闪退，那么可能就是launcher.dex没有去调用so，需要去手动添加适配
   目前启动器可以设置成，支持启动32位启动器，Application.mk里面APP_ABI :=armeabi-v7a既可
   如果需要支持64位那么改成APP_ABI :=arm64-v8a既可
   //加油!!!
    
*/
        final Handler handler = new Handler(Looper.getMainLooper());
        Executors.newSingleThreadExecutor().execute(new Runnable(){


                @Override
                public void run()
                {
                    try {
                        
                        
                        File cacheDexDir = new File(getCodeCacheDir(), "dex");
                        TextView listener =findViewById(R.id.listener);
                        handleCacheCleaning(cacheDexDir, handler, listener);
                        ApplicationInfo mcInfo = getPackageManager().getApplicationInfo(MC_PACKAGE_NAME, PackageManager.GET_META_DATA);
                        Object pathList = getPathList(getClassLoader());
                        processDexFiles(mcInfo, cacheDexDir, pathList, handler, listener);
                        processNativeLibraries(mcInfo, pathList, handler, listener);
                        launchMinecraft(mcInfo);



                    } catch (Exception e) {
                        Intent fallbackActivity = new Intent(MainActivity.this, Fallback.class);
                        handleException(e, fallbackActivity);
                    }
                }


            });
    }
    @SuppressLint("SetTextI18n")
    private void handleCacheCleaning( final File cacheDexDir, Handler handler, final TextView listener) {
        if (cacheDexDir.exists() && cacheDexDir.isDirectory()) {
            handler.post(new Runnable() {
                    @Override
                    public void run() {
                        listener.setText("-> " + cacheDexDir.getAbsolutePath() + " not empty, do cleaning");
                    }
                });

            File[] files = cacheDexDir.listFiles();
            if (files != null) {
                for (final File file : files) {
                    if (file.delete()) {
                        handler.post(new Runnable() {
                                @Override
                                public void run() {
                                    listener.append("\n-> " + file.getName() + " deleted");
                                }
                            });
                    }
                }
            } else {
                handler.post(new Runnable() {
                        @Override
                        public void run() {
                            listener.append("\n-> listFiles() returned null");
                        }
                    });
            }
        } else {
            handler.post(new Runnable() {
                    @Override
                    public void run() {
                        listener.setText("-> " + cacheDexDir.getAbsolutePath() + " is empty, skip cleaning");
                    }
                });
        }
    }
    
    private Object getPathList( ClassLoader classLoader) throws Exception {
        Field pathListField = Objects.requireNonNull(classLoader.getClass().getSuperclass()).getDeclaredField("pathList");
        pathListField.setAccessible(true);
        return pathListField.get(classLoader);
    }

    
    private void processDexFiles(final ApplicationInfo mcInfo, File cacheDexDir,  Object pathList,  Handler handler, final TextView listener) throws Exception {
        Method addDexPath = pathList.getClass().getDeclaredMethod("addDexPath", String.class, File.class);
        final File launcherDex = new File(cacheDexDir, LAUNCHER_DEX_NAME);

        copyFile(getAssets().open(LAUNCHER_DEX_NAME), launcherDex);
        handler.post(new Runnable(){

                @Override
                public void run() {
                    listener.append("\n-> " + LAUNCHER_DEX_NAME + " copied to " + launcherDex.getAbsolutePath());
                }
            });

        if (launcherDex.setReadOnly()) {
            addDexPath.invoke(pathList, launcherDex.getAbsolutePath(), null);
            handler.post(new Runnable(){

                    @Override
                    public void run() {
                        listener.append("\n-> " + LAUNCHER_DEX_NAME + " added to dex path list");
                    }
                });
            
            
            
        }
      
        try (ZipFile zipFile = new ZipFile(mcInfo.sourceDir)) {
            for (int i = 2; i >= 0; i--) {
                final String dexName = "classes" + (i == 0 ? "" : i) + ".dex";
                ZipEntry dexFile = zipFile.getEntry(dexName);
                if (dexFile != null) {
                    final File mcDex = new File(cacheDexDir, dexName);
                    copyFile(zipFile.getInputStream(dexFile), mcDex);
                    handler.post(new Runnable(){

                            @Override
                            public void run() {
                                listener.append("\n-> " + mcInfo.sourceDir + "/" + dexName + " copied to " + mcDex.getAbsolutePath());
                            }
                        }); 
                 
                        if (mcDex.setReadOnly()) {
                            addDexPath.invoke(pathList, mcDex.getAbsolutePath(), null);
                            handler.post(new Runnable(){

                                    @Override
                                    public void run() {
                                        listener.append("\n-> " + dexName + " added to dex path list");
                                    }
                                    
                                
                            });
                            
                            
                            
                            }
                }
            }
        }
    }
    
    
    private void processNativeLibraries( final ApplicationInfo mcInfo,  Object pathList,  Handler handler, final TextView listener) throws Exception {
        Method addNativePath = pathList.getClass().getDeclaredMethod("addNativePath", Collection.class);
        ArrayList<String> libDirList = new ArrayList<>();
        libDirList.add(mcInfo.nativeLibraryDir);
        addNativePath.invoke(pathList, libDirList);
        handler.post(new Runnable(){

                @Override
                public void run() {
                    listener.append("\n-> " + mcInfo.nativeLibraryDir + " added to native library directory path");
                }
            })
            ;
        
    }

    
    private void launchMinecraft(ApplicationInfo applicationInfo) throws ClassNotFoundException
   
   
   
     {
        // 动态加载 Launcher 类
        Class<?> launcherClass = getClassLoader().loadClass("com.mojang.minecraftpe.Launcher");
        // 创建 Intent 并设置目标类
        Intent intent = new Intent(this, launcherClass);

        // 添加额外的信息
        intent.putExtra("MC_SRC", applicationInfo.sourceDir);
        if (applicationInfo.splitSourceDirs != null) {
            ArrayList<String> splitSources = new ArrayList<>(Arrays.asList(applicationInfo.splitSourceDirs));
            intent.putExtra("MC_SPLIT_SRC", splitSources);
        }

        // 启动活动
        startActivity(intent);

              // 结束当前活动
        finish();
    }
    
    private void handleException( Exception e,  Intent fallbackActivity) {
        String logMessage = e.getCause() != null ? e.getCause().toString() : e.toString();
        fallbackActivity.putExtra("LOG_STR", logMessage);
        startActivity(fallbackActivity);
        finish();
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
    /**
     * Base64编码
     *
     * @param input 要编码的字符串
     * @return Base64编码后的字符串
     */
    public static String base64Encode(String input) {
        return base64Encode(input.getBytes());
    }

    /**
     * Base64编码
     *
     * @param input 要编码的字节数组
     * @return Base64编码后的字符串
     */
    public static String base64Encode(byte[] input) {
        return Base64.encodeToString(input, Base64.DEFAULT);
    }

    /**
     * Base64解码
     *
     * @param input 要解码的字符串
     * @return Base64解码后的字符串
     */
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
}
