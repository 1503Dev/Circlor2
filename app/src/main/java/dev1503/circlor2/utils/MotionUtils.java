package dev1503.circlor2.utils;

import android.graphics.Rect;
import android.view.View;

public class MotionUtils {
    public static boolean isTouchInsideFloatingView(View v, float touchX, float touchY) {
        Rect hitRect = new Rect();
        int padding = (int) (v.getContext().getResources().getDisplayMetrics().density * 8);
        hitRect.set(
                v.getLeft() - padding,
                v.getTop() - padding,
                v.getRight() + padding,
                v.getBottom() + padding
        );
        v.getHitRect(hitRect);
        return true;
        // return hitRect.contains((int) touchX, (int) touchY);
    }
}
