#pragma once

#include <Windows.h>

#define BOOLIFY(x) !!(x)

namespace AutoClean
{

    namespace SafeObjectCleanupFnc
    {
        bool ClnCloseHandle(const HANDLE &handle) { return BOOLIFY(CloseHandle(handle)); };
        bool ClnFreeLibrary(const HMODULE &handle) { return BOOLIFY(FreeLibrary(handle)); };
        bool ClnLocalFree(const HLOCAL &handle) { return (LocalFree(handle) == nullptr); };
        bool ClnGlobalFree(const HGLOBAL &handle) { return (GlobalFree(handle) == nullptr); };
        bool ClnUnmapViewOfFile(const PVOID &handle) { return BOOLIFY(UnmapViewOfFile(handle)); };
        bool ClnCloseDesktop(const HDESK &handle) { return BOOLIFY(CloseDesktop(handle)); };
        bool ClnCloseWindowStation(const HWINSTA &handle) { return BOOLIFY(CloseWindowStation(handle)); };
        bool ClnCloseServiceHandle(const SC_HANDLE &handle) { return BOOLIFY(CloseServiceHandle(handle)); };
        bool ClnVirtualFree(const PVOID &handle) { return BOOLIFY(VirtualFree(handle, 0, MEM_RELEASE)); };
    }

    template <typename T, bool (* Cleanup)(const T &), PVOID InvalidValue>
    class SafeObject final
    {
    public:
        SafeObject() = delete;
        SafeObject(const SafeObject &copy) = delete;

        SafeObject(const T &obj) : m_obj(obj)
        {
        }
        
        SafeObject(const SafeObject &&obj)
        {
            *this = std::move(obj);
        }

        ~SafeObject()
        {
            if (IsValid())
            {
                (void)Cleanup(m_obj);
            }
        }

        const bool IsValid() const
        {
            return m_obj != (T)InvalidValue;
        }

        SafeObject &operator=(const SafeObject &copy) = delete;

        const T operator()() const
        {
            return (T)m_obj;
        }

        SafeObject &operator=(SafeObject &&obj)
        {
            if (IsValid())
            {
                (void)Cleanup(m_obj);
            }

            m_obj = std::move(obj.m_obj);
            obj.m_obj = InvalidValue;

            return *this;
        }

        const T &Get() const
        {
            return m_obj;
        }

    private:
        T m_obj;
    };

    using SafeHandle = SafeObject<HANDLE, SafeObjectCleanupFnc::ClnCloseHandle, INVALID_HANDLE_VALUE>;
    using SafeLibrary = SafeObject<HMODULE, SafeObjectCleanupFnc::ClnFreeLibrary, nullptr>;
    using SafeLocal = SafeObject<HLOCAL, SafeObjectCleanupFnc::ClnLocalFree, nullptr>;
    using SafeGlobal = SafeObject<HGLOBAL, SafeObjectCleanupFnc::ClnGlobalFree, nullptr>;
    using SafeMapView = SafeObject<PVOID, SafeObjectCleanupFnc::ClnUnmapViewOfFile, nullptr>;
    using SafeDesktop = SafeObject<HDESK, SafeObjectCleanupFnc::ClnCloseDesktop, nullptr>;
    using SafeWindowStation = SafeObject<HWINSTA, SafeObjectCleanupFnc::ClnCloseWindowStation, nullptr>;
    using SafeService = SafeObject<SC_HANDLE, SafeObjectCleanupFnc::ClnCloseServiceHandle, nullptr>;
    using SafeVirtual = SafeObject<PVOID, SafeObjectCleanupFnc::ClnVirtualFree, nullptr>;
}

