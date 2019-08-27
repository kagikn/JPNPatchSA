#pragma once
#include <windows.h>
#include <cstdint>

namespace injector
{
    union auto_pointer
    {
    public:
        void* p;
        uintptr_t a;

        auto_pointer() : p(0) {}
        auto_pointer(void* x) : p(x) {}
        auto_pointer(intptr_t x) : a(static_cast<uintptr_t>(x)) {}
        auto_pointer(uintptr_t x) : a(x) {}
        auto_pointer(const auto_pointer& ptr) : p(ptr.p) {}

        auto_pointer operator+(const uintptr_t& rhs) const
        {
            return auto_pointer(this->a + rhs);
        }

        auto_pointer operator-(const uintptr_t& rhs) const
        {
            return auto_pointer(this->a - rhs);
        }

        auto_pointer operator*(const uintptr_t& rhs) const
        {
            return auto_pointer(this->a * rhs);
        }

        auto_pointer operator/(const uintptr_t& rhs) const
        {
            return auto_pointer(this->a / rhs);
        }

        bool is_null() const { return this->p != nullptr; }

    #if __cplusplus >= 201103L || _MSC_VER >= 1800
        explicit operator bool() const { return is_null(); }
    #endif

        template<class T> T* get() const { return (T*)this->p; }

        template<class T>
        operator T* () const { return reinterpret_cast<T*>(p); }
    };

    template<class T>
    inline void WriteMemory(auto_pointer addr, T value)
    {
        DWORD dwProtect[2];

        VirtualProtect(addr.p, sizeof(value), PAGE_EXECUTE_READWRITE, &dwProtect[0]);
        *addr.get<T>() = value;
        VirtualProtect(addr.p, sizeof(value), dwProtect[0], &dwProtect[1]);
    }
    inline void WriteMemory(auto_pointer addr, auto_pointer valueAddr, size_t valueSize)
    {
        DWORD dwProtect[2];

        VirtualProtect(addr.p, valueSize, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
        memcpy(addr, valueAddr, valueSize);
        VirtualProtect(addr.p, valueSize, dwProtect[0], &dwProtect[1]);
    }

    inline int GetRelativeOffset(auto_pointer abs_value, auto_pointer end_of_instruction)
    {
        return uintptr_t(abs_value.get<char>() - end_of_instruction.get<char>());
    }

    inline void MakeRelativeOffset(auto_pointer at, auto_pointer dest)
    {
        WriteMemory(at, static_cast<int32_t>(GetRelativeOffset(dest, at + 4)));
    }

    void MakeJMP(auto_pointer at, auto_pointer dest)
    {
        constexpr unsigned char jmp = 0xE9;
        WriteMemory(at, jmp);
        MakeRelativeOffset(at + 1, dest);
    }
}
