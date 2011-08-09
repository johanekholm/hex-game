package com.hexgame.game;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;

public class HexGame extends Activity {
    /** Called when the activity is first created. */
	public static Context context;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
		context = getApplicationContext();
		GameGLView view = new GameGLView(this);
   		view.setRenderer(new OpenGLRenderer());
   		setContentView(view);
    }
}