package dev1503.circlor;

import android.animation.Animator;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.res.ColorStateList;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.graphics.Typeface;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.GradientDrawable;
import android.graphics.drawable.RippleDrawable;
import android.os.Build;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Gravity;
import android.view.Menu;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.view.animation.AccelerateInterpolator;
import android.view.animation.DecelerateInterpolator;
import android.widget.CompoundButton;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.widget.AppCompatEditText;

import com.bytedance.android.bytehook.ByteHook;
import com.bytedance.shadowhook.ShadowHook;
import com.google.android.material.materialswitch.MaterialSwitch;
import com.google.android.material.navigation.NavigationView;
import com.google.android.material.slider.Slider;
import com.google.android.material.snackbar.Snackbar;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

import dev1503.circloralpha.FunctionHandler;
import dev1503.circloralpha.R;
import dev1503.circloralpha.utils.Config;
import dev1503.circloralpha.utils.MotionUtils;
import dev1503.circloralpha.utils.Utils;
import dev1503.circloralpha.utils.timerutils.Interval;
import tc.jsin.JSIN;
import tc.jsin.JSONArray;
import tc.jsin.JSONObject;

public class MinecraftActivityHandler {
    static final String TAG = "Java/MinecraftActivityHandler";

    public static Activity activity;
    static Context context;
    static Config conf;

    static int screenWidth;
    static int screenHeight;

    static WindowManager wm;
    static LinearLayout functionsListLayout;
    static Dialog currentDialog;

    public static void onCreate(Activity activity) {
        MinecraftActivityHandler.activity = activity;
        MinecraftActivityHandler.context = activity.getApplicationContext();
        Utils.setActivity(activity);
        conf = Global.conf;
        /*try {
            FunctionHandler.S_NOT_IN_GAME = activity.getString(activity.getResources().getIdentifier("not_in_game", "string", activity.getPackageName()));
        } catch (Exception ignored) {}*/

//        JNI.setMinecraftVersion(Global.getMinecraftVersion());
//        JNI.setAssetManager(activity.getAssets());

        DisplayMetrics displayMetrics = context.getResources().getDisplayMetrics();
        screenWidth = displayMetrics.widthPixels;
        screenHeight = displayMetrics.heightPixels;

        wm = activity.getWindowManager();
        TextView textView = new TextView(context);
        textView.setText("CirclorAlpha v" + activity.getString(R.string.version_name));
        textView.setTextSize(18);
        textView.setTextColor(0xFFFFFFFF);
        textView.setTypeface(Typeface.MONOSPACE);
        WindowManager.LayoutParams lp = new WindowManager.LayoutParams(
                ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.WRAP_CONTENT,
                WindowManager.LayoutParams.TYPE_APPLICATION,
                WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE | WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE | WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL,
                PixelFormat.TRANSLUCENT
        );
        lp.x = 96;
        lp.y = 96;
         wm.addView(textView, lp);
    }
    public static void onCreating(Activity activity) {
        if (!Global.SUPPORTED_VERSION.equals(Global.getMinecraftVersion()) && !Global.fakeActivityEnabled){
            return;
        }
        try {
            functionsListLayout = (LinearLayout) LinearLayout.inflate(activity, R.layout.layout_circlor_overlay_1, null);
        } catch (Exception e) {
            Log.e(TAG, "Failed to inflate layout_circlor2_overlay_1", e);
            return;
        }
        NavigationView navigationView = (NavigationView) functionsListLayout.findViewById(R.id.navigation_view);
        FrameLayout functionsListContainer = (FrameLayout) functionsListLayout.findViewById(R.id.content);
        Menu navMenu = navigationView.getMenu();

        String[] functionClassesPaths = Utils.listFilesFromAssets(activity, "circlor/ui/functions_list");
        ArrayList<JSONObject> functionClassesArrayList = new ArrayList<>();
        LinearLayout[] functionLayouts = new LinearLayout[functionClassesPaths.length];
        for (int i = 0; i < functionClassesPaths.length; i++) {
            functionClassesArrayList.add(new JSONObject(Utils.readStringFromAssets(activity, "circlor/ui/functions_list/" + functionClassesPaths[i])));
        }
        functionClassesArrayList.sort((o1, o2) -> {
            int o1Index = o1.getInt("index", 0);
            int o2Index = o2.getInt("index", 0);
            if (o1Index == o2Index) {
                return 0;
            } else {
                return o1Index < o2Index ? -1 : 1;
            }
        });
        JSONObject[] functionClasses = functionClassesArrayList.toArray(new JSONObject[0]);
        for (int i = 0; i < functionClasses.length; i++) {
            JSONObject functionClass = functionClasses[i];
            if (functionClass.getBoolean("hidden", false)) {
                continue;
            }
            if ((!functionClass.getString("hide_if_version", "").isEmpty() && Global.getMinecraftVersion().startsWith(functionClass.getString("hide_if_version", "")))) {
                continue;
            }
            if ((!functionClass.getString("show_if_version", "").isEmpty() && !Global.getMinecraftVersion().startsWith(functionClass.getString("show_if_version", "")))) {
                continue;
            }

            String name = functionClass.getString("name", "");
            if (!functionClass.getBoolean("translation_disabled", false)){
                try {
                    name = activity.getString(activity.getResources().getIdentifier(name, "string", activity.getPackageName()));
                } catch (Exception e) {
                    Log.e(TAG, "Function translation not found: circlor2.function." + name);
                }
            }
            int icon = 0;
            if (functionClass.hasKey("icon")){
                if(functionClass.getValueType("icon") == JSIN.VALUE_TYPE_INT || functionClass.getValueType("icon") == JSIN.VALUE_TYPE_LONG) {
                    icon = functionClass.getInt("icon", 0);
                } else {
                    try {
                        icon = activity.getResources().getIdentifier(functionClass.getString("icon", ""), "drawable", activity.getPackageName());
                    } catch (Exception e) {
                        Log.e(TAG, "Function icon not found: circlor.function." + name);
                    }
                }
            }

            LinearLayout functionLayout = new LinearLayout(activity);

            navMenu.add(0, Menu.NONE, Menu.NONE, name)
                    .setIcon(icon)
                    .setOnMenuItemClickListener(item -> {
                        for (LinearLayout functionLayout1: functionLayouts) {
                            if (functionLayout1 != null) {
                                functionLayout1.setVisibility(View.GONE);
                            }
                        }
                        functionLayout.setVisibility(View.VISIBLE);
                        return true;
                    });

            functionLayout.setOrientation(LinearLayout.VERTICAL);
            LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(
                    ViewGroup.LayoutParams.MATCH_PARENT,
                    ViewGroup.LayoutParams.WRAP_CONTENT
            );

            functionClass.getJSONArray("items", new JSONArray()).forEach((value, valueType, index) -> {
                if (valueType == JSIN.VALUE_TYPE_OBJECT) {

                    LinearLayout itemLayout;
                    View parentView = functionsListLayout;

                    JSONObject item = (JSONObject) value;
                    String path = item.getString("path", "");
                    itemLayout = getFunctionItemLayout(item, parentView, path);

                    if (itemLayout!= null) {
                        functionLayout.addView(itemLayout);
                    }
                }
            });

            functionLayouts[i] = functionLayout;
            functionLayout.setVisibility(View.GONE);
            functionsListContainer.addView(functionLayout, lp);
        }
        if (functionLayouts.length > 0) {
            functionLayouts[0].setVisibility(View.VISIBLE);
            new Interval(()->{
                FunctionHandler.update(activity);
            }, 100);
        }
    }
    static LinearLayout getFunctionItemLayout(JSONObject item, View parentView, String path) {
        if (item.getBoolean("hidden", false)) {
            return null;
        }
        if ((!item.getString("hide_if_version", "").isEmpty() && Global.getMinecraftVersion().startsWith(item.getString("hide_if_version", "")))) {
            return null;
        }
        if ((!item.getString("show_if_version", "").isEmpty() && !Global.getMinecraftVersion().startsWith(item.getString("show_if_version", "")))) {
            return null;
        }
        String itemName = item.getString("name", "");
        if (!item.getBoolean("translation_disabled", false)){
            try {
                itemName = activity.getString(activity.getResources().getIdentifier(itemName, "string", activity.getPackageName()));
            } catch (Exception e) {
                Log.e(TAG, "Function translation not found: circlor.function." + itemName);
            }
        }
        LinearLayout itemLayout = null;
        RippleDrawable rippleDrawable = new RippleDrawable(
                ColorStateList.valueOf(Color.parseColor("#40FFFFFF")), // 涟漪颜色（半透明白色）
                null,
                null
        );
        LinearLayout subItemsContainer;
        switch (item.getString("widget_type", "").toLowerCase()) {
            case "action":
                itemLayout = (LinearLayout) LinearLayout.inflate(activity, R.layout.funclist_widget_action, null);
                if (!path.isEmpty()){
                    itemLayout.setOnClickListener(v -> {
                        if (item.hasKey("value")) {
                            int valueType = item.getValueType("value");
                            if (valueType == JSIN.VALUE_TYPE_INT || valueType == JSIN.VALUE_TYPE_LONG) {
                                FunctionHandler.setFloat(path, item.getInt("value", 0));
                            } else if (valueType == JSIN.VALUE_TYPE_BIG_DECIMAL) {
                                FunctionHandler.setFloat(path, item.getBigDecimal("value", new BigDecimal(0)).floatValue());
                            } else if (valueType == JSIN.VALUE_TYPE_BOOLEAN) {
                                FunctionHandler.setEnabled(path, item.getBoolean("value", false));
                            } else if (valueType == JSIN.VALUE_TYPE_STRING) {
                                FunctionHandler.setString(path, item.getString("value", ""));
                            }
                            FunctionHandler.update(activity);
                        } else {
                            String actionRez = FunctionHandler.invokeFunction(path);
                            if (!actionRez.isEmpty()) {
                                try {
                                    actionRez = activity.getString(activity.getResources().getIdentifier(actionRez, "string", activity.getPackageName()));
                                } catch (Exception ignored) {}
                                Snackbar.make(parentView, actionRez, Snackbar.LENGTH_SHORT).show();
                            }
                        }
                    });
                }
                // itemLayout.setBackground(rippleDrawable);
                itemLayout.setClickable(true);
                FunctionHandler.addFunctionView(itemLayout, path);
                break;
            case "toggle":
                itemLayout = (LinearLayout) LinearLayout.inflate(activity, R.layout.funclist_widget_toggle, null);
                LinearLayout header = itemLayout.findViewById(R.id.header);
                View statusBar = itemLayout.findViewById(R.id.statusBar);
                subItemsContainer = itemLayout.findViewById(R.id.subItemContainer);
                MaterialSwitch toggle = header.findViewById(R.id.Switch);
                toggle.setChecked(item.getBoolean("value", false));

                subItemsContainer.setVisibility(View.GONE);
                subItemsContainer.setAlpha(0f);
                statusBar.setVisibility(View.INVISIBLE);

                subItemsContainer.post(() -> {
                    subItemsContainer.setTranslationX(-subItemsContainer.getWidth());
                });

                LinearLayout finalItemLayout = itemLayout;
                CompoundButton.OnCheckedChangeListener onCheckedChangeListener = (buttonView, isChecked) -> {
                    LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(
                            ViewGroup.LayoutParams.MATCH_PARENT,
                            ViewGroup.LayoutParams.WRAP_CONTENT
                    );
                    finalItemLayout.setLayoutParams(lp);
                    if (!path.isEmpty()) {
                        FunctionHandler.setEnabled(path, isChecked);
                        FunctionHandler.update(activity);
                    }

                    if (isChecked) {
                        subItemsContainer.setVisibility(View.VISIBLE);
                        subItemsContainer.setTranslationX(-subItemsContainer.getWidth());
                        subItemsContainer.animate()
                                .translationX(0)
                                .alpha(1f)
                                .setDuration(220)
                                .setInterpolator(new DecelerateInterpolator())
                                .start();

                        statusBar.setVisibility(View.VISIBLE);
                        statusBar.animate()
                                .translationX(0)
                                .setDuration(175)
                                .withEndAction(() -> {
                                    if (!toggle.isChecked()) {
                                        statusBar.setVisibility(View.INVISIBLE);
                                    } else {
                                        statusBar.setVisibility(View.VISIBLE);
                                    }
                                })
                                .start();
                    } else {
                        subItemsContainer.animate()
                                .translationX(-subItemsContainer.getWidth())
                                .alpha(0f)
                                .setDuration(200)
                                .setInterpolator(new AccelerateInterpolator())
                                .withEndAction(() -> subItemsContainer.setVisibility(View.GONE))
                                .start();

                        statusBar.animate()
                                .translationX(-statusBar.getWidth())
                                .setDuration(175)
                                .withEndAction(() -> {
                                    if (!toggle.isChecked()) {
                                        statusBar.setVisibility(View.INVISIBLE);
                                    } else {
                                        statusBar.setVisibility(View.VISIBLE);
                                    }
                                })
                                .start();
                    }
                };
                toggle.setOnCheckedChangeListener(onCheckedChangeListener);

                JSONArray subItems = item.getJSONArray("items", new JSONArray());
                subItems.forEach((value, valueType, index) -> {
                    if (valueType == JSIN.VALUE_TYPE_OBJECT) {
                        JSONObject subItem = (JSONObject) value;
                        LinearLayout subItemView = getFunctionItemLayout(subItem, parentView, subItem.getString("path", ""));
                        subItemsContainer.addView(subItemView);
                    }
                });

                FunctionHandler.addFunctionView(toggle, path);
                onCheckedChangeListener.onCheckedChanged(toggle, toggle.isChecked());
                break;
            case "collapse":
                AtomicBoolean expanded = new AtomicBoolean(!item.getBoolean("value", false));
                itemLayout = (LinearLayout) LinearLayout.inflate(activity, R.layout.funclist_widget_collapse, null);
                LinearLayout header1 = itemLayout.findViewById(R.id.header);
                View statusBar1 = itemLayout.findViewById(R.id.statusBar);
                subItemsContainer = itemLayout.findViewById(R.id.subItemContainer);
                ImageView arrow = header1.findViewById(R.id.arrow);
                header1.setClickable(true);

                subItemsContainer.setVisibility(View.GONE);
                subItemsContainer.setAlpha(0f);
                statusBar1.setVisibility(View.INVISIBLE);

                LinearLayout finalItemLayout1 = itemLayout;
                LinearLayout.LayoutParams lp1 = new LinearLayout.LayoutParams(
                        ViewGroup.LayoutParams.MATCH_PARENT,
                        ViewGroup.LayoutParams.WRAP_CONTENT
                );
                header1.setOnClickListener((v) -> {
                    expanded.set(!expanded.get());

                    lp1.height = ViewGroup.LayoutParams.WRAP_CONTENT;
                    finalItemLayout1.setLayoutParams(lp1);

                    if (expanded.get()) {
                        subItemsContainer.setVisibility(View.VISIBLE);
                        subItemsContainer.setTranslationX(-subItemsContainer.getWidth());
                        statusBar1.setVisibility(View.VISIBLE);

                        // Status bar animation
                        statusBar1.animate()
                                .translationX(0)
                                .setDuration(175)
                                .withEndAction(() -> {
                                    if (!expanded.get()) {
                                        statusBar1.setVisibility(View.INVISIBLE);
                                    } else {
                                        statusBar1.setVisibility(View.VISIBLE);
                                    }
                                })
                                .start();

                        // Arrow animation
                        arrow.animate()
                                .rotation(arrow.getRotation() + 180f)
                                .setDuration(220)
                                .setInterpolator(new DecelerateInterpolator())
                                .start();

                        // Subitems container animation
                        subItemsContainer.animate()
                                .translationX(0)
                                .alpha(1f)
                                .setDuration(220)
                                .setInterpolator(new DecelerateInterpolator())
                                .start();
                    } else {
                        // Status bar animation
                        statusBar1.animate()
                                .translationX(-statusBar1.getWidth())
                                .setDuration(175)
                                .withEndAction(() -> {
                                    if (!expanded.get()) {
                                        statusBar1.setVisibility(View.INVISIBLE);
                                    } else {
                                        statusBar1.setVisibility(View.VISIBLE);
                                    }
                                })
                                .start();

                        // Arrow animation
                        arrow.animate()
                                .rotation(arrow.getRotation() + 180f)
                                .setDuration(200)
                                .setInterpolator(new AccelerateInterpolator())
                                .setListener(new android.animation.Animator.AnimatorListener() {
                                    @Override
                                    public void onAnimationStart(android.animation.Animator animation) {}
                                    @Override
                                    public void onAnimationEnd(android.animation.Animator animation) {
                                        if (!expanded.get()) {
                                            arrow.setRotation(0f);
                                        } else {
                                            arrow.setRotation(180f);
                                        }
                                    }
                                    @Override
                                    public void onAnimationCancel(@NonNull Animator animation) {
                                        if (!expanded.get()) {
                                            arrow.setRotation(0f);
                                        } else {
                                            arrow.setRotation(180f);
                                        }
                                    }
                                    @Override
                                    public void onAnimationRepeat(@NonNull Animator animation) {}
                                })
                                .start();

                        // Subitems container animation
                        subItemsContainer.animate()
                                .translationX(-subItemsContainer.getWidth())
                                .alpha(0f)
                                .setDuration(200)
                                .setInterpolator(new AccelerateInterpolator())
                                .withEndAction(() -> subItemsContainer.setVisibility(View.GONE))
                                .start();
                    }
                });

                JSONArray subItems1 = item.getJSONArray("items", new JSONArray());
                subItems1.forEach((value, valueType, index) -> {
                    if (valueType == JSIN.VALUE_TYPE_OBJECT) {
                        JSONObject subItem = (JSONObject) value;
                        LinearLayout subItemView = getFunctionItemLayout(subItem, parentView, subItem.getString("path", ""));
                        subItemsContainer.addView(subItemView);
                    }
                });

                FunctionHandler.addFunctionView(header1, path);
                header1.callOnClick();
                break;
            case "slider":
                itemLayout = (LinearLayout) LinearLayout.inflate(activity, R.layout.funclist_widget_slider, null);
                Slider slider = itemLayout.findViewById(R.id.slider);

                float min = (float) item.getDouble("min", 0);
                float max = (float) item.getDouble("max", 100);
                float step = (float) item.getDouble("step", 1);
                float value = (float) item.getDouble("value", 0);

                if (value < min) {
                    value = min;
                } else if (value > max) {
                    value = max;
                }

                slider.setValueFrom(min);
                slider.setValueTo(max);
                slider.setStepSize(step);
                slider.setValue(value);

                if (!path.isEmpty()){
                    FunctionHandler.setFloat(path, value);
                    slider.addOnChangeListener((slider1, value1, fromUser) -> {
                        FunctionHandler.setFloat(path, value1);
                        FunctionHandler.update(activity);
                    });
                }

                FunctionHandler.addFunctionView(slider, path);
                break;
            case "edit_text":
                itemLayout = (LinearLayout) LinearLayout.inflate(activity, R.layout.funclist_widget_edit_text, null);
                AppCompatEditText editText = itemLayout.findViewById(R.id.edit_text);

                String inputType = item.getString("input_type", "");
                editText.setMaxLines(1);
                editText.setSingleLine(true);
                editText.setTextSize(13);
                if (inputType.equals("number")) {
                    editText.setInputType(android.text.InputType.TYPE_CLASS_NUMBER | android.text.InputType.TYPE_NUMBER_FLAG_DECIMAL | android.text.InputType.TYPE_NUMBER_FLAG_SIGNED);
                }

                String text = item.getString("value", "");
                editText.setText(text);

                if (!path.isEmpty()){
                    if (!text.isEmpty()){
                        FunctionHandler.setString(path, text);
                        FunctionHandler.update(activity);
                    }
                    editText.addTextChangedListener(new TextWatcher() {
                        @Override
                        public void beforeTextChanged(CharSequence s, int start, int count, int after) {

                        }

                        @Override
                        public void onTextChanged(CharSequence s, int start, int before, int count) {
                            FunctionHandler.setString(path, s.toString());
                            FunctionHandler.update(activity);
                        }

                        @Override
                        public void afterTextChanged(Editable s) {

                        }
                    });
                }

                FunctionHandler.addFunctionView(editText, path);
                break;
            case "text_short":
                itemLayout = (LinearLayout) LinearLayout.inflate(activity, R.layout.funclist_widget_text_short, null);
                break;
        }
        try {
            TextView nameTextView = itemLayout.findViewById(R.id.name);
            nameTextView.setText(itemName);
            if (!item.getString("text_style","").isEmpty()) {
                String textStyle = item.getString("text_style","").toLowerCase();
                if (textStyle.equals("bold")) {
                    nameTextView.setTypeface(Typeface.DEFAULT_BOLD);
                }
            }
        } catch (Exception e) {
        }
        if (!item.hasKey("description")) {
            try {
                itemLayout.findViewById(R.id.description).setVisibility(View.GONE);
            } catch (Exception e) {
            }
        } else {
            String itemDescription = item.getString("description", "");
            if (!item.getBoolean("translation_disabled", false)){
                try {
                    itemDescription = activity.getString(activity.getResources().getIdentifier(itemDescription, "string", activity.getPackageName()));
                } catch (Exception e) {
                }
            }
            try {
                ((TextView) itemLayout.findViewById(R.id.description)).setText(itemDescription);
            } catch (Exception e) {
            }
        }
        itemLayout.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                // activity.getResources().getDimensionPixelSize(R.dimen.funclist_item_height)
                ViewGroup.LayoutParams.WRAP_CONTENT
        ));
        return itemLayout;
    }
    public static void onCreated(Activity activity) {
        loadFloatingViews();
    }

    static void loadFloatingViews() {
        if (!Global.getMinecraftVersion().equals(Global.SUPPORTED_VERSION)){
            if (!(Global.fakeActivityEnabled && Build.VERSION.SDK_INT <= 27)) {
                return;
            }
        }
        loadFloatingBall();
    }
    @SuppressLint("ClickableViewAccessibility")
    static void loadFloatingBall() {
        Log.d(TAG, "loadFloatingView");

        LinearLayout layout = new LinearLayout(activity);
        GradientDrawable circleDrawable = new GradientDrawable();
        circleDrawable.setShape(GradientDrawable.OVAL);
        circleDrawable.setStroke(6, activity.getResources().getColor(R.color.blue_900));
        circleDrawable.setColor(Color.TRANSPARENT);
        layout.setBackground(circleDrawable);

        ImageView imageView = new ImageView(activity);
        imageView.setImageResource(R.drawable.ic_launcher_foreground);
        ViewGroup.MarginLayoutParams  imageViewLayoutParams = new ViewGroup.MarginLayoutParams (
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT
        );
        imageViewLayoutParams.setMargins(-16, -16, -16, -16);
        imageView.setLayoutParams(imageViewLayoutParams);
        imageView.setScaleType(ImageView.ScaleType.FIT_CENTER);

        WindowManager.LayoutParams lp = getGeneralLayoutParams();
        lp.width = 128;
        lp.height = lp.width;


        AtomicInteger lastX = new AtomicInteger();
        AtomicInteger lastY = new AtomicInteger();

        AtomicInteger lastTouchX = new AtomicInteger();
        AtomicInteger lastTouchY = new AtomicInteger();

        layout.setOnTouchListener((v, event) -> {
            int action = event.getActionMasked();
            int pointerIndex = event.getActionIndex();
            int x = (int) event.getRawX();
            int y = (int) event.getRawY();

            switch (action) {
                case MotionEvent.ACTION_DOWN:
                case MotionEvent.ACTION_POINTER_DOWN:
                    if (MotionUtils.isTouchInsideFloatingView(layout, event.getX(pointerIndex), event.getY(pointerIndex))) {
                        lastX.set(x);
                        lastY.set(y);
                        lastTouchX.set(x);
                        lastTouchY.set(y);
                        return true;
                    }
                    break;

                case MotionEvent.ACTION_MOVE:
                    for (int i = 0; i < event.getPointerCount(); i++) {
                        if (MotionUtils.isTouchInsideFloatingView(layout, event.getX(i), event.getY(i))) {
                            int dx = x - lastX.get();
                            int dy = y - lastY.get();

                            lp.x += dx;
                            lp.y += dy;

                            if (lp.x < 0) {
                                lp.x = 0;
                            }
                            if (lp.x > screenWidth - layout.getWidth()) {
                                lp.x = screenWidth - layout.getWidth();
                            }
                            if (lp.y < 0) {
                                lp.y = 0;
                            }
                            if (lp.y > screenHeight - layout.getHeight()) {
                                lp.y = screenHeight - layout.getHeight();
                            }

                            runOnUI(() -> {
                                wm.updateViewLayout(layout, lp);
                            });

                            lastX.set(x);
                            lastY.set(y);
                            return true;
                        }
                    }
                    break;

                case MotionEvent.ACTION_UP:
                case MotionEvent.ACTION_POINTER_UP:
                    conf.put("floating_ball_x", lp.x);
                    conf.put("floating_ball_y", lp.y);

                    int dx = x - lastTouchX.get();
                    int dy = y - lastTouchY.get();

                    if (Math.abs(dx) < 5 && Math.abs(dy) < 5) {
                        floatingBall_onClick(v);
                        Log.d(TAG, "Floating ball clicked");
                    }
                    break;
            }
            return false;
        });

        layout.addView(imageView);

        lp.x = conf.getInt("floating_ball_x", 0);
        lp.y = conf.getInt("floating_ball_y", 0);

        runOnUI(()->{
            wm.addView(layout, lp);
//            PopupWindow popupWindow = new PopupWindow(
//                    layout,                  // 布局视图
//                    128, // 宽度
//                    128, // 高度
//                    true                        // 是否可获取焦点
//            );
//            popupWindow.setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));
//            popupWindow.showAtLocation(
//                    null,                 // 父视图
//                    Gravity.CENTER,             // 位置
//                    0,                          // x偏移
//                    0                           // y偏移
//            );
        });
    }
    static void floatingBall_onClick(View v) {
        if (functionsListLayout != null) {
            runOnUI(()->{
                if (currentDialog!= null) {
                    currentDialog.dismiss();
                    currentDialog = null;
                }
                if (functionsListLayout.getParent()!= null) {
                    ((ViewGroup) functionsListLayout.getParent()).removeView(functionsListLayout);
                }

                AlertDialog dialog = new AlertDialog.Builder(activity)
                        .setView(functionsListLayout)
                        .create();

                currentDialog = dialog;

                dialog.setOnCancelListener(dialog1 -> {
                    v.setVisibility(View.VISIBLE);
                });

                dialog.show();
                v.setVisibility(View.GONE);

                dialog.getWindow().setBackgroundDrawable(new ColorDrawable(0x60000000));
                dialog.getWindow().setDimAmount(0.2f);
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
                    dialog.getWindow().setFlags(WindowManager.LayoutParams.FLAG_BLUR_BEHIND, WindowManager.LayoutParams.FLAG_BLUR_BEHIND);
                    dialog.getWindow().setBackgroundBlurRadius(10);
                }

                /*FrameLayout rootView = (FrameLayout) dialog.getWindow().getDecorView();
                dialog.getWindow().setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));
                BlurView blurView = new BlurView(activity);
                ViewGroup.LayoutParams params = new ViewGroup.LayoutParams(
                        ViewGroup.LayoutParams.MATCH_PARENT,
                        ViewGroup.LayoutParams.MATCH_PARENT
                );
                blurView.setLayoutParams(params);
                rootView.addView(blurView, 0);
                blurView.setupWith(rootView)
                        .setBlurRadius(20f)
                        .setOverlayColor(Color.argb(100, 0, 0, 0));*/
            });
        }
    }

    static void runOnUI(Runnable runnable) {
        activity.runOnUiThread(runnable);
    }
    public static void snack(String text) {
        try {
            text = activity.getString(activity.getResources().getIdentifier(text, "string", activity.getPackageName()));
        } catch (Exception ignored) { }
        try {
            Snackbar.make(currentDialog.findViewById(android.R.id.content), text, Snackbar.LENGTH_SHORT).show();
        } catch (Exception e) {
            Toast.makeText(activity, text, Toast.LENGTH_SHORT).show();
        }
    }

    static WindowManager.LayoutParams getGeneralLayoutParams() {
        WindowManager.LayoutParams lp = new WindowManager.LayoutParams(
                ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.WRAP_CONTENT,
                WindowManager.LayoutParams.TYPE_APPLICATION,
                WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
                PixelFormat.TRANSLUCENT
        );
        lp.gravity = Gravity.TOP | Gravity.LEFT;
        return lp;
    }
}
