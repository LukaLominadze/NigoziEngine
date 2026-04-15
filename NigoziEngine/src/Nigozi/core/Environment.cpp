#include "ngpch.h"
#ifdef NG_PLATFORM_WINDOWS
    #include <Windows.h>
    #include <ShObjIdl_core.h>
#endif

#include "Environment.h"

namespace Nigozi
{
    std::string Environment::GetVariable(const std::string& name)
    {
    #if NG_PLATFORM_WINDOWS
        std::wstring wname = std::filesystem::path(name).wstring();
        DWORD size = GetEnvironmentVariable(wname.c_str(), nullptr, 0);

        if (size == 0) {
            return std::string();
        }

        std::wstring buffer;
        buffer.resize(size);

        DWORD result = GetEnvironmentVariable(wname.c_str(), buffer.data(), size);

        if (result == 0) {
            return std::string();
        }
        return std::filesystem::path(buffer).string();
    #else
        char* result = std::getenv(name.c_str());
        if (!result) {
            return std::string();
        }
        return std::string(result);
    #endif
    }

    bool Environment::SetLocalVariable(const std::string& name, const std::string& value)
    {
    #if NG_PLATFORM_WINDOWS
        std::wstring wname = std::filesystem::path(name).wstring();
        std::wstring wvalue = std::filesystem::path(value).wstring();
        BOOL result = SetEnvironmentVariable(wname.c_str(), wvalue.c_str());
        if (result == 0) {
            return false;
        }
        return true;
    #else
        static_assert(false);
    #endif
        return false;
    }
}
