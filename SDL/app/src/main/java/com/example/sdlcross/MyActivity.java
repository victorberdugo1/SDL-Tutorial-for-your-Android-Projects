package com.example.sdlcross;

import org.libsdl.app.SDLActivity;

public class MyActivity extends SDLActivity {
    protected String[] getLibraries() {
        return new String[] { "simple-triangle-android-opengles20-sdl3-cpp" };
    }
}
