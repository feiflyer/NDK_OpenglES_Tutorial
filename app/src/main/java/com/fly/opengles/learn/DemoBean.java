package com.fly.opengles.learn;

public class DemoBean {
    private String title;
    private Class<?> demoActivityCls;

    public DemoBean(String title, Class<?> demoActivityCls) {
        this.title = title;
        this.demoActivityCls = demoActivityCls;
    }

    public String getTitle() {
        return title;
    }

    public Class<?> getDemoActivityCls() {
        return demoActivityCls;
    }
}
