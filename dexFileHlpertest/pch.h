#if defined(_WIN32)
#include <stdio.h>
#include<fcntl.h>
#include <io.h>
#pragma warning(disable:4996)
#define LOGI(...) {printf(__VA_ARGS__);printf("\n");}
#define LOGW(...) {printf(__VA_ARGS__);printf("\n");}
#define LOGD(...) {printf(__VA_ARGS__);printf("\n");}
#define LOGE(...) {printf(__VA_ARGS__);printf("\n");}

#elif
#include <jni.h>
#include <errno.h>

#include <string.h>
#include <unistd.h>
#include <sys/resource.h>

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, __FILE__, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, __FILE__, __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, __FILE__, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, __FILE__, __VA_ARGS__))
#endif
