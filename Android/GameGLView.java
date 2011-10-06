package com.hexgame.game;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

public class GameGLView extends GLSurfaceView {

	public GameGLView(Context context) {
		super(context);
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		switch (event.getAction()) {
			case MotionEvent.ACTION_DOWN:
				GameGLView.touchBegan(event.getX(), event.getY());
				return true;
			case MotionEvent.ACTION_UP:
				GameGLView.touchEnded(event.getX(), event.getY());
				return true;
			case MotionEvent.ACTION_MOVE:
				GameGLView.touchMoved(event.getX(), event.getY());
				return true;
		}
		return false;
	}
	
    public static native void touchBegan(float x, float y);
    public static native void touchEnded(float x, float y);
    public static native void touchMoved(float x, float y);
}
