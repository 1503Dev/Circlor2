package dev1503.circloralpha.utils.timerutils;

import android.os.Handler;

public class Interval {
    long interval;

    public Interval(Runnable func, long interval){
        this.interval = interval;
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                func.run();
                new Handler().postDelayed(this, interval);
            }
        };
        new Handler().postDelayed(runnable, interval);
    }
}
