#include <jni.h>
#include "SalvadorMain.h"

// NOTE: Function definitions must be enclosed within the 'extern' keyword to prevent the compiler
// from mangling the function name. Without this declaration, JNI will not be able to link to these
// functions at runtime.

extern "C"
{
    // Renderer events
    JNIEXPORT void JNICALL Java_jp_volumerenderer_SalvadorLib_initRenderer(JNIEnv *env, jobject);
    JNIEXPORT void JNICALL Java_jp_volumerenderer_SalvadorLib_resizeWindow(JNIEnv *env, jobject, jint width, jint height);
    JNIEXPORT void JNICALL Java_jp_volumerenderer_SalvadorLib_drawFrame(JNIEnv *env, jobject);
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