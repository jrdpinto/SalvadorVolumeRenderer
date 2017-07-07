#include <jni.h>
#include "SalvadorMain.h"

// NOTE: Function definitions must be enclosed within the 'extern' keyword to prevent the compiler
// from mangling the function name. Without this declaration, JNI will not be able to link to these
// functions at runtime.

extern "C"
{
    // Rendering events
    JNIEXPORT void JNICALL Java_jp_volumerenderer_SalvadorLib_initRenderer(JNIEnv *env, jobject);
    JNIEXPORT void JNICALL Java_jp_volumerenderer_SalvadorLib_resizeWindow(JNIEnv *env, jobject, jint width, jint height);
    JNIEXPORT void JNICALL Java_jp_volumerenderer_SalvadorLib_drawFrame(JNIEnv *env, jobject);

    // Touch events
    JNIEXPORT void JNICALL Java_jp_volumerenderer_SalvadorLib_handleDragEvent(JNIEnv *env, jobject,
                                                                              jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_jp_volumerenderer_SalvadorLib_handleTouchUp(JNIEnv *env, jobject, jfloat x, jfloat y);
};

JNIEXPORT void JNICALL Java_jp_volumerenderer_SalvadorLib_initRenderer(JNIEnv *env, jobject)
{
    SalvadorMain::getInstance()->initialiseRenderer();
}

JNIEXPORT void JNICALL Java_jp_volumerenderer_SalvadorLib_resizeWindow(JNIEnv *env, jobject, jint width, jint height)
{
    SalvadorMain::getInstance()->resizeWindow(width, height);
}

JNIEXPORT void JNICALL Java_jp_volumerenderer_SalvadorLib_drawFrame(JNIEnv *env, jobject)
{
    SalvadorMain::getInstance()->runTick();
}

JNIEXPORT void Java_jp_volumerenderer_SalvadorLib_handleDragEvent(JNIEnv *env, jobject, jfloat x,
                                                                  jfloat y)
{
    SalvadorMain::getInstance()->handleDragEvent(x, y);
}

void Java_jp_volumerenderer_SalvadorLib_handleTouchUp(JNIEnv *env, jobject, jfloat x, jfloat y)
{
    SalvadorMain::getInstance()->handleTouchUp(x,y);
}
