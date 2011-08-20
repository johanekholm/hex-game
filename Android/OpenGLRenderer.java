package com.hexgame.game;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLU;
import android.opengl.GLSurfaceView.Renderer;
import android.opengl.GLUtils;
import android.util.Log;

public class OpenGLRenderer implements Renderer {

    static {
        System.loadLibrary("HexGame");
    }

    static GL10 gl10;

	@Override
	public void onDrawFrame(GL10 gl) {
		OpenGLRenderer.update();
		OpenGLRenderer.draw();
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		// Sets the current view port to the new size.
		gl.glViewport(0, 0, width, height);// OpenGL docs.
		// Select the projection matrix
		gl.glMatrixMode(GL10.GL_PROJECTION);// OpenGL docs.
		// Reset the projection matrix
		gl.glLoadIdentity();// OpenGL docs.
		// Calculate the aspect ratio of the window
		
		GLU.gluOrtho2D(gl, 0.0f, 320.0f, 480.0f, 0.0f);

		// Select the modelview matrix
		gl.glMatrixMode(GL10.GL_MODELVIEW);// OpenGL docs.
		// Reset the modelview matrix
		gl.glLoadIdentity();// OpenGL docs.

		OpenGLRenderer.init();
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig eglConfig) {
		// Enable Smooth Shading, default not really needed.
		gl.glShadeModel(GL10.GL_SMOOTH);// OpenGL docs.
		// Depth buffer setup.
		gl.glClearDepthf(1.0f);// OpenGL docs.
		// Enables depth testing.
		gl.glEnable(GL10.GL_DEPTH_TEST);// OpenGL docs.
		// The type of depth testing to do.
		gl.glDepthFunc(GL10.GL_LEQUAL);// OpenGL docs.
		// Really nice perspective calculations.
		gl.glHint(GL10.GL_PERSPECTIVE_CORRECTION_HINT, // OpenGL docs.
                          GL10.GL_FASTEST);

		gl.glTexEnvf(GL10.GL_TEXTURE_ENV, GL10.GL_TEXTURE_ENV_MODE, GL10.GL_MODULATE);

	    gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
	    gl.glEnableClientState(GL10.GL_TEXTURE_COORD_ARRAY);

		gl.glEnable(GL10.GL_BLEND);
		gl.glBlendFunc(GL10.GL_SRC_ALPHA, GL10.GL_ONE_MINUS_SRC_ALPHA);

		gl.glEnable(GL10.GL_TEXTURE_2D);
		gl.glTexParameterf(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MIN_FILTER, GL10.GL_LINEAR);
		gl.glTexParameterf(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MAG_FILTER, GL10.GL_LINEAR);
		
		OpenGLRenderer.gl10 = gl;
		loadResources();
		OpenGLRenderer.gl10 = null;
		OpenGLRenderer.addTexture("hexTiles", loadTexture(gl, R.raw.texmap_hex), 2);
		OpenGLRenderer.addTexture("actions", loadTexture(gl, R.raw.actions), 4);
		OpenGLRenderer.addTexture("icons", loadTexture(gl, R.raw.icons), 4);
		OpenGLRenderer.addTexture("units", loadTexture(gl, R.raw.texmap), 2);
		OpenGLRenderer.addTexture("font", loadTexture(gl, R.raw.font_1), 1);

		OpenGLRenderer.realizeAssetLoader(HexGame.assetManager);
	}
	
	public static int newTextureID(GL10 gl) {
	    int[] temp = new int[1];
	    gl.glGenTextures(1, temp, 0);
	    return temp[0];        
	}

	// Will load a texture out of an asset file, and return an OpenGL texture ID:
	public static int loadTexture(String textureName) {
		GL10 gl = OpenGLRenderer.gl10;
	    int id = OpenGLRenderer.newTextureID(gl);

		AssetManager assetManager = HexGame.context.getResources().getAssets();

		InputStream textureInputStream = null;

		try {
			textureInputStream = assetManager.open(textureName);
		} catch (IOException e) {
			Log.e("HexGame", e.getMessage());
			return 0;
		}

		Bitmap bmp = BitmapFactory.decodeStream(textureInputStream);

	    gl.glBindTexture(GL10.GL_TEXTURE_2D, id);

	    gl.glTexParameterf(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MIN_FILTER, GL10.GL_LINEAR);
	    gl.glTexParameterf(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MAG_FILTER, GL10.GL_LINEAR);

	    // Generate, and load up all of the mipmaps:
        GLUtils.texImage2D(GL10.GL_TEXTURE_2D, 0, bmp, 0);

	    bmp.recycle();
	    return id;
	}

	
	public native void loadResources();
    public static native void init();
    public static native void draw();
    public static native void update();
    public static native void addTexture(String name, int textureId, int tiles);
    public static native void realizeAssetLoader(AssetManager assetManager);
}
