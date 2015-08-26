/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "VibratorService"

#include "jni.h"
#include "JNIHelp.h"
#include "android_runtime/AndroidRuntime.h"

#include <utils/misc.h>
#include <utils/Log.h>
#include <hardware_legacy/vibrator.h>
#include <hardware/vibrator.h>
#include <stdio.h>

namespace android
{
static vibrator_device_t* device;

static void vibratorControlInit()
 {
    assert(!gVibraModule);
    hw_module_t *gVibraModule;
    int err;
    err = hw_get_module(VIBRATOR_HARDWARE_MODULE_ID, (hw_module_t const**)&gVibraModule);
    if (err) {
      ALOGE("Couldn't load %s module (%s)", VIBRATOR_HARDWARE_MODULE_ID, strerror(-err));
    }
    err = vibrator_open(gVibraModule, &device);
}

static jboolean vibratorExists(JNIEnv *env, jobject clazz)
{
    return (device != NULL) ? JNI_TRUE : JNI_FALSE;
}

static void vibratorOn(JNIEnv *env, jobject clazz, jlong timeout_ms)
{
    // ALOGI("vibratorOn\n");
    if (device)
        device->vibrator_on(device, timeout_ms);
}

static void vibratorOff(JNIEnv *env, jobject clazz)
{
    // ALOGI("vibratorOff\n");
    if (device)
        device->vibrator_off(device);
}

static JNINativeMethod method_table[] = {
    { "vibratorExists", "()Z", (void*)vibratorExists },
    { "vibratorOn", "(J)V", (void*)vibratorOn },
    { "vibratorOff", "()V", (void*)vibratorOff }
};

int register_android_server_VibratorService(JNIEnv *env)
{
    vibratorControlInit();
    return jniRegisterNativeMethods(env, "com/android/server/VibratorService",
            method_table, NELEM(method_table));
}

};
