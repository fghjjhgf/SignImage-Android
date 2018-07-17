package com.lb.imagesigner;

import android.Manifest;
import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.SeekBar;

import com.lb.imagesigner.JNIUtil.JNIUtils;
import com.lb.imagesigner.util.GetFilePath;
import com.lb.imagesigner.util.Text2Image;

public class MainActivity extends AppCompatActivity {

    private static String TAG = "MainActivity";

    private Button createButton, selectButton, signButton;
    private ImageView srcImageView, signImageView, redChanel, blueChanel, greenChanel, signRedImage, signBlueImage, signGreenImage;
    private EditText editText;
    private CheckBox redcheckbox, greencheckbox, bluecheckbox;
    private static int rgb_check = 0;
    private SeekBar powerSeekbar;
    private int powerMax = 10;

    Bitmap signbitmap, sourcebitmap, signedbitmap, redchanelbm, bluechanelbm, greenchanelbm, signredbm, signbluebm, signgreenbm, tempbm, destbitmap;

    private static int signwidth = 128;
    private static int signheight = 128;
    private static int sourcesignwidth = signwidth * 2;
    private static int sourcesignheight = signheight * 2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        findView();
        createBitmap();
        initListener();

    }

    private void initListener(){

        createButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                signbitmap = Text2Image.textToImage(editText.getText().toString());
                tempbm = Bitmap.createBitmap(signwidth,signheight, Bitmap.Config.ARGB_8888);

                JNIUtils.imageEncrypt(signbitmap,tempbm);
                signImageView.setImageBitmap(tempbm);
            }
        });

        selectButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
                intent.setType("image/*");//设置类型，我这里是任意类型，任意后缀的可以这样写。
                intent.addCategory(Intent.CATEGORY_OPENABLE);
                startActivityForResult(intent,1);
            }
        });

        signButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (redcheckbox.isChecked()){
                    rgb_check |= 1 ;
                }if (greencheckbox.isChecked()){
                    rgb_check |= 2 ;
                }if (bluecheckbox.isChecked()){
                    rgb_check |= 4 ;
                }

                powerSeekbar.setMax(powerMax);
                int power = powerSeekbar.getProgress() * powerMax;
                JNIUtils.signImage(sourcebitmap,signbitmap,destbitmap,power,rgb_check,redchanelbm,greenchanelbm,bluechanelbm,signredbm,signgreenbm,signbluebm);
            }
        });
    }

    private void findView(){
        createButton = (Button)findViewById(R.id.create_signimage_button);
        selectButton = (Button)findViewById(R.id.select_button);
        signButton = (Button)findViewById(R.id.signButton);

        srcImageView = (ImageView)findViewById(R.id.source_imageview);
        signImageView = (ImageView)findViewById(R.id.sign_imageview);
        redChanel = (ImageView)findViewById(R.id.red_chanel_imageview);
        blueChanel = (ImageView)findViewById(R.id.blue_chanel_imageview);
        greenChanel = (ImageView)findViewById(R.id.green_chanel_imageview);
        signRedImage = (ImageView)findViewById(R.id.red_sign_imageview);
        signBlueImage = (ImageView)findViewById(R.id.blue_sign_imageview);
        signGreenImage = (ImageView)findViewById(R.id.green_sign_imageview);

        editText = (EditText)findViewById(R.id.editText);

        redcheckbox = (CheckBox)findViewById(R.id.r_checkbox);
        greencheckbox = (CheckBox)findViewById(R.id.g_checkbox);
        bluecheckbox = (CheckBox)findViewById(R.id.b_checkbox);

        powerSeekbar = (SeekBar)findViewById(R.id.powerseekbar);
    }

    private void createBitmap(){
        signedbitmap = Bitmap.createBitmap(signwidth,signheight, Bitmap.Config.ARGB_8888);
        redchanelbm = Bitmap.createBitmap(sourcesignwidth,sourcesignheight, Bitmap.Config.ARGB_8888);
        bluechanelbm = Bitmap.createBitmap(sourcesignwidth,sourcesignheight, Bitmap.Config.ARGB_8888);
        greenchanelbm = Bitmap.createBitmap(sourcesignwidth,sourcesignheight,Bitmap.Config.ARGB_8888);
        signredbm = Bitmap.createBitmap(sourcesignwidth,sourcesignheight,Bitmap.Config.ARGB_8888);
        signbluebm = Bitmap.createBitmap(sourcesignwidth,sourcesignheight,Bitmap.Config.ARGB_8888);
        signgreenbm = Bitmap.createBitmap(sourcesignwidth,sourcesignheight,Bitmap.Config.ARGB_8888);
    }

    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (resultCode == Activity.RESULT_OK) {
            if (requestCode == 1) {
                Uri uri = data.getData();
                String path = null;
                if ("file".equalsIgnoreCase(uri.getScheme())){//使用第三方应用打开
                    path = uri.getPath();
                } else if (Build.VERSION.SDK_INT > Build.VERSION_CODES.KITKAT) {//4.4以后
                    path = GetFilePath.getPath(this, uri);
                } else {//4.4以下下系统调用方法
                    path = GetFilePath.getRealPathFromURI(this, uri);
                }
                Log.i(TAG + " onActivityResult path ",path);
                if (path == null){
                    return;
                }else {
                    //get permission
                    if (Build.VERSION.SDK_INT >= 23) {
                        int REQUEST_CODE_CONTACT = 101;
                        String[] permissions = {Manifest.permission.WRITE_EXTERNAL_STORAGE};
                        //验证是否许可权限
                        for (String str : permissions) {
                            if (this.checkSelfPermission(str) != PackageManager.PERMISSION_GRANTED) {
                                //申请权限
                                this.requestPermissions(permissions, REQUEST_CODE_CONTACT);
                            }
                        }
                    }

                    destbitmap = sourcebitmap = BitmapFactory.decodeFile(path);
                    srcImageView.setImageBitmap(sourcebitmap);
                }
            }
        }

        super.onActivityResult(requestCode, resultCode, data);
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
