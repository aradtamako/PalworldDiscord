#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include <filesystem>

#include <MinHook.h>
#include <nlohmann/json.hpp>

#pragma comment(lib, "ws2_32.lib")
#include <cpprest/http_client.h>
#include <cpprest/http_listener.h>

#include "SDK.hpp"
#include "StringUtil.hpp"

#include <winsock2.h>
extern "C" {
#include <mongoose.h>
}

std::string DiscordWebhookUrl;
uint32_t HttpServerPort;

#pragma region Version.dll
#define FUNCTION_COUNT 17
extern "C" uintptr_t functions[FUNCTION_COUNT] = { 0 };
std::string functionNames[] =
{
    "GetFileVersionInfoA",
    "GetFileVersionInfoByHandle",
    "GetFileVersionInfoExA",
    "GetFileVersionInfoExW",
    "GetFileVersionInfoSizeA",
    "GetFileVersionInfoSizeExA",
    "GetFileVersionInfoSizeExW",
    "GetFileVersionInfoSizeW",
    "GetFileVersionInfoW",
    "VerFindFileA",
    "VerFindFileW",
    "VerInstallFileA",
    "VerInstallFileW",
    "VerLanguageNameA",
    "VerLanguageNameW",
    "VerQueryValueA",
    "VerQueryValueW"
};


// [index: 000] [ordinal: 001] GetFileVersionInfoA
#pragma comment(linker, "/EXPORT:GetFileVersionInfoA=func_e68e412056ef401994ed6741244326c6,@1")
extern "C" void func_e68e412056ef401994ed6741244326c6();

// [index: 001] [ordinal: 002] GetFileVersionInfoByHandle
#pragma comment(linker, "/EXPORT:GetFileVersionInfoByHandle=func_8298d3ec827c4f08b4d2bebf4bd466f0,@2")
extern "C" void func_8298d3ec827c4f08b4d2bebf4bd466f0();

// [index: 002] [ordinal: 003] GetFileVersionInfoExA
#pragma comment(linker, "/EXPORT:GetFileVersionInfoExA=func_43e270b0b4594142a0ce3cb99088e036,@3")
extern "C" void func_43e270b0b4594142a0ce3cb99088e036();

// [index: 003] [ordinal: 004] GetFileVersionInfoExW
#pragma comment(linker, "/EXPORT:GetFileVersionInfoExW=func_95b55dcd538d453892f009f984761a60,@4")
extern "C" void func_95b55dcd538d453892f009f984761a60();

// [index: 004] [ordinal: 005] GetFileVersionInfoSizeA
#pragma comment(linker, "/EXPORT:GetFileVersionInfoSizeA=func_350e0f7c4d36499fba2056399892b7de,@5")
extern "C" void func_350e0f7c4d36499fba2056399892b7de();

// [index: 005] [ordinal: 006] GetFileVersionInfoSizeExA
#pragma comment(linker, "/EXPORT:GetFileVersionInfoSizeExA=func_5cf74f9edd9c4fadbb2e311381c9bf00,@6")
extern "C" void func_5cf74f9edd9c4fadbb2e311381c9bf00();

// [index: 006] [ordinal: 007] GetFileVersionInfoSizeExW
#pragma comment(linker, "/EXPORT:GetFileVersionInfoSizeExW=func_80b3492578da41edbd7914a8e8451688,@7")
extern "C" void func_80b3492578da41edbd7914a8e8451688();

// [index: 007] [ordinal: 008] GetFileVersionInfoSizeW
#pragma comment(linker, "/EXPORT:GetFileVersionInfoSizeW=func_9a4ceae2496d4dd89c7d958255810d9d,@8")
extern "C" void func_9a4ceae2496d4dd89c7d958255810d9d();

// [index: 008] [ordinal: 009] GetFileVersionInfoW
#pragma comment(linker, "/EXPORT:GetFileVersionInfoW=func_49b86963ec9a4a489fd7b9285a953808,@9")
extern "C" void func_49b86963ec9a4a489fd7b9285a953808();

// [index: 009] [ordinal: 010] VerFindFileA
#pragma comment(linker, "/EXPORT:VerFindFileA=func_72adfaa679e34e6183e2ce3ad923bd89,@10")
extern "C" void func_72adfaa679e34e6183e2ce3ad923bd89();

// [index: 010] [ordinal: 011] VerFindFileW
#pragma comment(linker, "/EXPORT:VerFindFileW=func_c3ae18a064ae4118b55a0c857fc51cbd,@11")
extern "C" void func_c3ae18a064ae4118b55a0c857fc51cbd();

// [index: 011] [ordinal: 012] VerInstallFileA
#pragma comment(linker, "/EXPORT:VerInstallFileA=func_604fdffb467842fd9a8c2e7937483e1f,@12")
extern "C" void func_604fdffb467842fd9a8c2e7937483e1f();

// [index: 012] [ordinal: 013] VerInstallFileW
#pragma comment(linker, "/EXPORT:VerInstallFileW=func_6319208d9f9e4ad9a6dcb51e63f12d92,@13")
extern "C" void func_6319208d9f9e4ad9a6dcb51e63f12d92();

// [index: 013] [ordinal: 014] VerLanguageNameA
#pragma comment(linker, "/EXPORT:VerLanguageNameA=func_2a15e63917404d51af65578fc74e62ed,@14")
extern "C" void func_2a15e63917404d51af65578fc74e62ed();

// [index: 014] [ordinal: 015] VerLanguageNameW
#pragma comment(linker, "/EXPORT:VerLanguageNameW=func_4297147a9105453c813edcface14430b,@15")
extern "C" void func_4297147a9105453c813edcface14430b();

// [index: 015] [ordinal: 016] VerQueryValueA
#pragma comment(linker, "/EXPORT:VerQueryValueA=func_05659a79b0214c9fa9e9ab4f6093c143,@16")
extern "C" void func_05659a79b0214c9fa9e9ab4f6093c143();

// [index: 016] [ordinal: 017] VerQueryValueW
#pragma comment(linker, "/EXPORT:VerQueryValueW=func_eece0e562d334b38891e4e7553db8203,@17")
extern "C" void func_eece0e562d334b38891e4e7553db8203();
#pragma endregion

void SendToDiscord(
    std::string sender,
    std::string message)
{
    try
    {
        web::json::value postData;
        postData[L"username"] = web::json::value::string(utility::conversions::to_string_t(sender));
        postData[L"content"] = web::json::value::string(utility::conversions::to_string_t(message));

        web::http::client::http_client client(utility::conversions::to_string_t(DiscordWebhookUrl));
        client.request(web::http::methods::POST, L"", postData.serialize(), L"application/json");
    }
    catch (...)
    {
        std::cout << "Discord webhook error" << std::endl;
    }
}

SDK::UObject* GetInstanceOf(SDK::UClass* Class)
{
    for (int32_t i = 0; i < SDK::UObject::GObjects->Num(); i++)
    {
        auto Object = SDK::UObject::GObjects->GetByIndex(i);

        if (Object != nullptr && Object->IsA(Class))
        {
            if (!strstr(Object->GetFullName().c_str(), "Default"))
            {
                return Object;
            }
        }
    }

    return nullptr;
}

void HookProcessEvents()
{
    typedef void(__thiscall* tProcessEvent)(SDK::UObject* _this, SDK::UFunction* function, void* params);

    auto mBaseAddress = reinterpret_cast<uintptr_t>(GetModuleHandleW(L"PalServer-Win64-Test-Cmd.exe"));
    auto pProcessEvent = reinterpret_cast<tProcessEvent>(mBaseAddress + Offsets::ProcessEvent);
    static tProcessEvent OriginalProcessEvent = pProcessEvent;

    tProcessEvent HookProcessEvent = [](
        SDK::UObject* _this,
        SDK::UFunction* function,
        void* params
        ) -> void
        {
            auto functionFullName = function->GetFullName();
            // std::cout << "FunctionName: " << functionFullName << std::endl;

            if (functionFullName.find("BroadcastChatMessage") != std::string::npos)
            {
                auto palChatMessage = reinterpret_cast<SDK::FPalChatMessage*>(params);

                // std::ofstream ofs("c:\\data.bin", std::ios_base::out | std::ios_base::binary);
                // ofs.write(reinterpret_cast<char*>(palChatMessage), sizeof(SDK::FPalChatMessage));
                // ofs.close();

                auto sender = StringUtil::wide_to_utf8_winapi(palChatMessage->Sender.ToWString());
                auto message = StringUtil::wide_to_utf8_winapi(palChatMessage->Message.ToWString());

                std::printf("[%s]: %s\n", sender.c_str(), message.c_str());

                if (
                    palChatMessage->SenderPlayerUId.A == 0 &&
                    palChatMessage->SenderPlayerUId.B == 0 &&
                    palChatMessage->SenderPlayerUId.C == 0 &&
                    palChatMessage->SenderPlayerUId.D == 0
                    )
                {
                    // Discordに通知しない
                }
                else
                {
                    // Discordに通知する
                    SendToDiscord(sender, message);
                }
            }

            OriginalProcessEvent(_this, function, params);
        };

    MH_Initialize();
    auto result = MH_CreateHook(reinterpret_cast<void*>(pProcessEvent), reinterpret_cast<void*>(HookProcessEvent), reinterpret_cast<void**>(&OriginalProcessEvent));
    if (result != MH_OK)
    {
        std::printf("Failed to hook\n");
    }
    MH_EnableHook(MH_ALL_HOOKS);
}

void SendChat(std::string username, std::string message)
{
    auto palGameStateInGame = reinterpret_cast<SDK::APalGameStateInGame*>(GetInstanceOf(SDK::APalGameStateInGame::StaticClass()));

    std::wstring wideUsername(StringUtil::utf8_to_wide_winapi(username));
    std::wstring wideMessage(StringUtil::utf8_to_wide_winapi(message));

    uint8_t unknownBytes[] = { 0xB6, 0xF7, 0xFD, 0xC9, 0x00, 0x00, 0x00 };
    SDK::FPalChatMessage palChatMessage;
    palChatMessage.Category = SDK::EPalChatCategory::Global;
    memcpy(palChatMessage.Pad_42EC, unknownBytes, sizeof(palChatMessage.Pad_42EC));
    palChatMessage.Sender = SDK::FString(wideUsername.c_str());
    palChatMessage.SenderPlayerUId = SDK::FGuid();
    palChatMessage.Message = SDK::FString(wideMessage.c_str());
    palChatMessage.ReceiverPlayerUId = SDK::FGuid();
    palGameStateInGame->BroadcastChatMessage(palChatMessage);
}

static void fn(struct mg_connection* c, int ev, void* ev_data, void* fn_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message* hm = (struct mg_http_message*)ev_data;
        if (mg_http_match_uri(hm, "/test")) {
            mg_http_reply(c, 200, "", "ok");
        }
        else if (mg_http_match_uri(hm, "/discord")) {
            auto inputBuffer = std::string(hm->body.ptr, hm->body.len);
            auto inputBufferSize = inputBuffer.size();

            auto json = nlohmann::json::parse(inputBuffer);
            std::string username(json["username"].get<std::string>());
            std::string content(json["content"].get<std::string>());

            SendChat(username, content);

            mg_http_reply(c, 200, "", "ok");
        }
        else {
            mg_http_reply(c, 404, "", "");
        }
    }
    (void)fn_data;
}

void HttpServerThread()
{
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);

    std::stringstream ss;
    ss << "http://127.0.0.1:";
    ss << HttpServerPort;

    mg_http_listen(&mgr, ss.str().c_str(), fn, NULL);

    while (true) {
        mg_mgr_poll(&mgr, 1000);
    }
}

std::string GetSettingFilePath()
{
    char _path[MAX_PATH];
    GetModuleFileNameA(NULL, _path, MAX_PATH);

    std::filesystem::path path(_path);

    std::stringstream ss;
    ss << path.parent_path().generic_string();
    ss << "/";
    ss << "DiscordPluginConfig.json";
    return ss.str();
}

std::string ReadAllString(std::string path)
{
    std::ifstream ifs(path);
    std::ostringstream x;
    x << ifs.rdbuf();
    ifs.close();

    return x.str();
}

void MainThread()
{
    Sleep(3 * 1000);

    SDK::InitGObjects();

    SetConsoleOutputCP(CP_UTF8);
    std::cout << StringUtil::multi_to_utf8_winapi("Discord プラグインを読み込みました") << std::endl;

    auto settingFilePath = GetSettingFilePath();
    auto str = ReadAllString(settingFilePath);
    auto json = nlohmann::json::parse(str);
    DiscordWebhookUrl = json["discord_webhook_url"].get<std::string>();
    HttpServerPort = json["http_server_port"].get<uint32_t>();

    std::cout << StringUtil::multi_to_utf8_winapi("Discord Webhook URL: ") << DiscordWebhookUrl << std::endl;
    
    CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(&HttpServerThread), NULL, NULL, NULL);

    HookProcessEvents();
}

BOOL WINAPI DllMain(HINSTANCE hinstModule, DWORD dwReason, LPVOID lpvReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinstModule);

        wchar_t buffer[MAX_PATH];
        auto len = GetSystemDirectory(buffer, MAX_PATH);
        std::wstring originalModulePath(buffer, len);
        auto moduleHandle = LoadLibrary(originalModulePath.append(L"\\version.dll").c_str());
        if (moduleHandle != nullptr)
        {
            for (auto i = 0; i < FUNCTION_COUNT; i++)
            {
                auto address = GetProcAddress(moduleHandle, functionNames[i].c_str());
                functions[i] = reinterpret_cast<uintptr_t>(address);
            }
        }

        LoadLibrary(L"cpprest_2_10.dll");
        LoadLibrary(L"minhook.x64.dll");

        CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(&MainThread), NULL, NULL, NULL);
    }

    return TRUE;
}
