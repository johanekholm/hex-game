package com.hexgame.game;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.os.Bundle;

public class HexGame extends Activity {
    /** Called when the activity is first created. */
	public static AssetManager assetManager;
	public static Context context;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
		assetManager = getAssets();
		context = getApplicationContext();
		GameGLView view = new GameGLView(this);
   		view.setRenderer(new OpenGLRenderer());
   		setContentView(view);
    }
}