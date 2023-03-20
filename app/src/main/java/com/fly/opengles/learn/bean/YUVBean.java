package com.fly.opengles.learn.bean;

public class YUVBean {
    private byte[] yData;
    private byte[] uvData;
    private int width;
    private int height;

    public YUVBean(byte[] yData, byte[] uvData, int width, int height) {
        this.yData = yData;
        this.uvData = uvData;
        this.width = width;
        this.height = height;
    }

    public byte[] getyData() {
        return yData;
    }

    public void setyData(byte[] yData) {
        this.yData = yData;
    }

    public byte[] getUvData() {
        return uvData;
    }

    public void setUvData(byte[] uvData) {
        this.uvData = uvData;
    }

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
}
