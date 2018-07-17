package com.lb.imagesigner.util;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.text.Layout;
import android.text.StaticLayout;
import android.text.TextPaint;

/**
 * Created by Administrator on 2018/7/10.
 */

public class Text2Image {

    private static int imageWidth = 128;
    private static int textWidth = 75;
    private static int textHeight = 25;

    private Text2Image(){}

    public static Bitmap textToImage(String text){
        TextPaint textPaint = new TextPaint();

        textPaint.setColor(Color.WHITE);
        textPaint.setTextSize(textHeight);
        StaticLayout layout = new StaticLayout(text, textPaint,textWidth,
                Layout.Alignment.ALIGN_CENTER, 1.0f, 1.0f, false);

        Bitmap bitmap = Bitmap.createBitmap(imageWidth,
                imageWidth, Bitmap.Config.ARGB_8888);

        Canvas canvas = new Canvas(bitmap);
        canvas.translate(imageWidth-textWidth,imageWidth-textHeight);
        canvas.drawColor(Color.BLACK);

        layout.draw(canvas);
        return bitmap;
    }


    public static void setImageWidth(int width){
        imageWidth = width;
    }

    public static void setTextWidth(int width){
        textWidth = width;
    }

    public static void setTextHeight(int height){
        textHeight = height;
    }
}
