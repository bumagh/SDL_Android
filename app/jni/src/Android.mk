LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL
SDL_MIXER_PATH :=../SDL2_mixer
SDL_TTF_PATH :=../SDL2_ttf
LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include $(LOCAL_PATH)/$(SDL_MIXER_PATH)/include $(LOCAL_PATH)/$(SDL_TTF_PATH)

# Add your application source files here...
LOCAL_SRC_FILES :=  main.c draw.c home.c player.c skillButton.c textTexture.c touchButton.c uiText.c util.c

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_mixer SDL2_ttf

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -lOpenSLES -llog -landroid

include $(BUILD_SHARED_LIBRARY)
