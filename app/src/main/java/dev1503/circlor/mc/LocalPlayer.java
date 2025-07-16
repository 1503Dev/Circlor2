package dev1503.circlor.mc;

public class LocalPlayer {
    public static Vec3 getPos() {
        float[] pos = _getPos();
        return new Vec3(pos[0], pos[1], pos[2]);
    }
    public static void setPos(Vec3 pos) {
        setPos(pos.x, pos.y, pos.z);
    }

    public static native float[] _getPos();
    public static native void setPos(float x, float y, float z);
}
