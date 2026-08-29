LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := loader
LOCAL_SRC_FILES := loader.cpp
LOCAL_CPPFLAGS := -std=c++20
include $(BUILD_EXECUTABLE)
