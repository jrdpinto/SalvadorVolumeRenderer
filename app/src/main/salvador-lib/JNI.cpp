#include <jni.h>
#include <string>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "SalvadorMain.h"
#include "Logger/Logger.h"

namespace
{
    std::string TAG_ = "JNI.cpp ";
}

// NOTE: Function definitions must be enclosed within the 'extern' keyword to prevent the compiler
// from mangling the function name. Without this declaration, JNI will not be able to link to these
// functions at runtime.

extern "C"
{
    // Rendering events
    JNIEXPORT void JNICALL Java_jp_volumerenderer_SalvadorLib_initRenderer(JNIEnv *env, jobject);
    JNIEXPORT void JNICALL Java_jp_volumerenderer_SalvadorLib_resizeWindow(JNIEnv *env, jobject, jint width, jint height);
    JNIEXPORT void JNICALL Java_jp_volumerenderer_SalvadorLib_drawFrame(JNIEnv *env, jobject);

    // Application events
    JNIEXPORT void JNICALL Java_jp_volumerenderer_SalvadorLib_loadVolume(JNIEnv *env, jobject, jstring filePath,
                                                                             jobject assetManager);

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

JNIEXPORT void Java_jp_volumerenderer_SalvadorLib_handleTouchUp(JNIEnv *env, jobject, jfloat x, jfloat y)
{
    SalvadorMain::getInstance()->handleTouchUp(x,y);
}

JNIEXPORT void Java_jp_volumerenderer_SalvadorLib_loadVolume(JNIEnv *env, jobject, jstring filePath,
                                                   jobject assetManager)
{
    // TODO: Ideally this code would be part of an Android file manager!
    unsigned short width=0, height=0, depth=0, voxel=0;
    int size = 0;
    std::shared_ptr<std::vector<float>> buffer;
    AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);

    if (mgr != nullptr)
    {
        const char* convertedFilePath = env->GetStringUTFChars(filePath, NULL);
        auto asset = AAssetManager_open(mgr, convertedFilePath, AASSET_MODE_STREAMING);
        if (!(asset == nullptr))
        {
            Logger::logd(std::string(TAG_+"- Found ").append(convertedFilePath));

            AAsset_read(asset, &width, sizeof(unsigned short));
            AAsset_read(asset, &height, sizeof(unsigned short));
            AAsset_read(asset, &depth, sizeof(unsigned short));
            size = width*height*depth;
            Logger::logd(TAG_+" Dataset info: Width-"+std::to_string(width)
                             +" Height-"+std::to_string(height)
                             +" Depth-"+std::to_string(depth)
                             +" Size-"+std::to_string(size));

            // Load volume data into a buffer
            Logger::logd(TAG_+" Loading data from file.");
            buffer = std::make_shared<std::vector<float>>();
            buffer->reserve(size);
            for (unsigned short z = 0; z < depth; ++z)
            {
                for (unsigned short y = 0; y < height; ++y)
                {
                    for (unsigned short x = 0; x < width; ++x)
                    {
                        AAsset_read(asset, &voxel, sizeof(unsigned short));
                        buffer->push_back(float(voxel)/4096);
                    }
                }
            }
            Logger::logd(TAG_+" Done!");

            SalvadorMain::getInstance()->loadVolume(buffer, width, height, depth);

            AAsset_close(asset);
        }
        else
        {
            Logger::loge(std::string(TAG_+"- Asset manager could not open  ")
                                 .append(convertedFilePath));
        }

        env->ReleaseStringUTFChars(filePath, convertedFilePath);
    }
    else
    {
        Logger::loge(TAG_+"- Failed to load asset manager.");
    }
}