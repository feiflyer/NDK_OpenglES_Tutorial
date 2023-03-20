//
// Created by Fly on 2022/8/24.
//

#ifndef NDK_OPENGLES_LEARN_LOG_H
#define NDK_OPENGLES_LEARN_LOG_H

#include "android/log.h"

#define LOGD(FORMAT, ...) __android_log_print(ANDROID_LOG_DEBUG, "fly_learn_opengl", FORMAT, ##__VA_ARGS__);
#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR, "fly_learn_opengl", FORMAT, ##__VA_ARGS__);

#endif //NDK_OPENGLES_LEARN_LOG_H
