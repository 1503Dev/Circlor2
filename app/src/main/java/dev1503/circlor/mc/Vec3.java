package dev1503.circlor.mc;

public class Vec3 {
    public float x;
    public float y;
    public float z;

    public Vec3(float x, float y, float z) {
        this.x = x;
        this.y = y;
        this.z = z;
    }
    public Vec3() {
        this(0, 0, 0);
    }

    public Vec3 toFixed(){
        this.x = Math.round(this.x * 100) / 100f;
        this.y = Math.round(this.y * 100) / 100f;
        this.z = Math.round(this.z * 100) / 100f;
        return this;
    }
}
