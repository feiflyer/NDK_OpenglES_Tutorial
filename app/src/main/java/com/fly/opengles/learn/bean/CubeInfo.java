package com.fly.opengles.learn.bean;

import java.nio.FloatBuffer;

public class CubeInfo {
    private int width;
    private int height;
    private int depth;
    private FloatBuffer buffer;

    public int getWidth() {
        return width;
    }

    public void setWidth(int width) {
        this.width = width;
    }

    public int getHeight() {
        return height;
    }

    public void setHeight(int height) {
        this.height = height;
    }

    public int getDepth() {
        return depth;
    }

    public void setDepth(int depth) {
        this.depth = depth;
    }

    public FloatBuffer getBuffer() {
        return buffer;
    }

    public void setBuffer(FloatBuffer buffer) {
        this.buffer = buffer;
    }
}
