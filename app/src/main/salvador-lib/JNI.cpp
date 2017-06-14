#include <jni.h>
#include <string>

// NOTE: Function definitions must be enclosed within the 'extern' keyword to prevent the compiler
// from mangling the function name. Without this declaration, JNI will not be able to link to these
// functions at runtime.

extern "C"
{
    JNIEXPORT jstring JNICALL Java_jp_volumerenderer_SalvadorLib_stringFromJNI(JNIEnv *env, jobject);
};

JNIEXPORT jstring JNICALL Java_jp_volumerenderer_SalvadorLib_stringFromJNI(JNIEnv *env, jobject)
{
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
