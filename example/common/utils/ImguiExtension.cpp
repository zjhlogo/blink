/*********************************************************************
 * \file   ImguiExtension.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   August 2022
 *********************************************************************/
#include "ImguiExtension.h"

struct ReadOnlyTextCallback_UserData
{
    const std::string* str;
    ImGuiInputTextCallback chainCallback;
    void* chainCallbackUserData;
};

static int InputTextCallback(ImGuiInputTextCallbackData* data)
{
    ReadOnlyTextCallback_UserData* user_data = (ReadOnlyTextCallback_UserData*)data->UserData;
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        IM_ASSERT(false);
    }
    else if (user_data->chainCallback)
    {
        // Forward to user callback, if any
        data->UserData = user_data->chainCallbackUserData;
        return user_data->chainCallback(data);
    }
    return 0;
}

bool ImGui::ReadOnlyText(const char* label, const std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* userData)
{
    flags |= ImGuiInputTextFlags_ReadOnly;

    ReadOnlyTextCallback_UserData cb_user_data;
    cb_user_data.str = str;
    cb_user_data.chainCallback = callback;
    cb_user_data.chainCallbackUserData = userData;
    return InputText(label, const_cast<char*>(str->c_str()), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
}
