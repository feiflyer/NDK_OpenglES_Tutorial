package com.fly.opengles.learn;

import android.content.Intent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.ArrayList;
import java.util.List;

public class DemoAdapter extends RecyclerView.Adapter<DemoAdapter.ViewHolder> {

    private static final List<DemoBean> dataList = new ArrayList<>();

    static {
        dataList.add(new DemoBean("EGL环境搭建",EGLActivity.class));
        dataList.add(new DemoBean("绘制三角形",DrawTriangleActivity.class));
        dataList.add(new DemoBean("绘制四边形",DrawRectActivity.class));
        dataList.add(new DemoBean("纹理贴图",TextureMapActivity.class));
        dataList.add(new DemoBean("VAO/VBO",VAOActivity.class));
        dataList.add(new DemoBean("EBO",EBOActivity.class));
        dataList.add(new DemoBean("FBO",FBOActivity.class));
        dataList.add(new DemoBean("PBO",PBOActivity.class));
        dataList.add(new DemoBean("YUV渲染",YUVRenderActivity.class));
        dataList.add(new DemoBean("RGB转YUV",RGBToYUVActivity.class));
        dataList.add(new DemoBean("YUV转RGB",YUVToRGBActivity.class));
        dataList.add(new DemoBean("矩阵变换(上)",MatrixActivity.class));
        dataList.add(new DemoBean("矩阵变换(下)",MvpMatrixActivity.class));
        dataList.add(new DemoBean("水印贴图",WaterMarkActivity.class));
        dataList.add(new DemoBean("文字渲染",TextRenderActivity.class));
        dataList.add(new DemoBean("纹理数组",TextureArrayActivity.class));
        dataList.add(new DemoBean("MRT多目标渲染",MRTRenderActivity.class));
        dataList.add(new DemoBean("LUT(2D图)滤镜", LUT2DActivity.class));
        dataList.add(new DemoBean("LUT(cube)滤镜", LUTCubeActivity.class));
        dataList.add(new DemoBean("转场动画",TransitionsActivity.class));
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View itemView = LayoutInflater.from(parent.getContext()).inflate(R.layout.adapter_item_demo,parent,false);
        ViewHolder viewHolder = new ViewHolder(itemView);
        return viewHolder;
    }

    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
        holder.tv_title.setText(dataList.get(position).getTitle());
        final int finalPosition = position;
        holder.tv_title.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(holder.tv_title.getContext(),dataList.get(finalPosition).getDemoActivityCls());
                holder.tv_title.getContext().startActivity(intent);
            }
        });
    }

    @Override
    public int getItemCount() {
        return dataList.size();
    }

    protected static class ViewHolder extends RecyclerView.ViewHolder{
        private TextView tv_title;
        public ViewHolder(@NonNull View itemView) {
            super(itemView);
            tv_title = itemView.findViewById(R.id.tv_title);
        }
    }
}
