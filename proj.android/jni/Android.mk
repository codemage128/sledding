LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := hellocpp_shared

LOCAL_MODULE_FILENAME := libhellocpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/Sleding.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../../../../cocos2dx/include \
					$(LOCAL_PATH)/../../../../../cocos2dx/kazmath/include \
					$(LOCAL_PATH)/../../../../../cocos2dx/platform/android \
					$(LOCAL_PATH)/../../../../../external/Box2D \
					$(LOCAL_PATH)/../../../../../CocosDenshion/android

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static \
								box2d_static \
								cocosdenshion_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,external/Box2D)
$(call import-module,CocosDenshion/android)

