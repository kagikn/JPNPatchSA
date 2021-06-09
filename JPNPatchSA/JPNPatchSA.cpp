#include "JPNPatchSA.h"
#include "ASM_instructions.h"
#include "injector.h"
#include <Windows.h>
#include <array>
#include <string>
#include <iomanip>

//Steam版の検出部分以外はplugin-sdkから流用
GameVersion GetGameVersion()
{
    //Steam版はexeのベースアドレスが0x400000でなく不定なので実行時にexeのベースアドレスを取得する
    const auto baseAddressOfExe = reinterpret_cast<char*>(GetModuleHandle(nullptr));
    const unsigned int AddressOfEntryPoint = *reinterpret_cast<const unsigned int*>(baseAddressOfExe + 0x178);

    if (AddressOfEntryPoint == 0x458E78)
        return GameVersion::STEAM;
    if (AddressOfEntryPoint == 0x458EA8)
        return GameVersion::STEAM_LV;

    if (*reinterpret_cast<unsigned int*>(0x40104C) == 0x50EC83EC)
        return GameVersion::v10US_COMPACT;
    if (*reinterpret_cast<unsigned int*>(0x401000) == 0x16197BE9)
        return GameVersion::v10US_HOODLUM;
    if (*reinterpret_cast<unsigned int*>(0x8245BC) == 0x94BF)
        return GameVersion::v10EU;

    if (*reinterpret_cast<unsigned int*>(0x8252FC) == 0x94BF)
        return GameVersion::v11US;
    if (*reinterpret_cast<unsigned int*>(0x82533C) == 0x94BF)
        return GameVersion::v11EU;

    return GameVersion::UNKNOWN;
}

std::wstring GetGameVersionName(GameVersion version)
{
    switch (version) {
    case GameVersion::v10US_COMPACT:
        return L"1.0 US 'Compact'";
    case GameVersion::v10US_HOODLUM:
        return L"1.0 US 'HoodLum'";
    case GameVersion::v10EU:
        return L"1.0 EU";
    case GameVersion::v11US:
        return L"1.01 US";
    case GameVersion::v11EU:
        return L"1.01 EU";
    case GameVersion::STEAM:
        return L"Steam R2 (2014/11)";
    case GameVersion::STEAM_LV:
        return L"Steam R2 (2014/12)";
    default:
        return L"不明";
    }
}

void ShowErrorBox(GameVersion gameVersion)
{
    const auto message = L"現在起動しているバージョンでは日本語化できません。\n"
        L"v1.0(Compact以外)かv1.01のgta_sa.exeで起動してください。\n"
        L"現在起動中のexeのバージョン: " + GetGameVersionName(gameVersion);

    MessageBox(
        nullptr,
        message.c_str(),
        L"日本語化非対応のバージョンです",
        MB_OK | MB_ICONSTOP);
}

void Patch10Exe()
{
#pragma region Addresses For 1.0 exe
    //0x6D40AB(GetFiringRateMultiplier)と0x6D40C7(GetFiringRateMultiplier)と0x6D40FB+0x6D4117(GetPlaneGunsRateOfFire)と
    //0x6D45AB+0x6D45C7(GetPlaneOrdnanceRateOfFire)と0x6EFCF6+0x6EFD22+0x6EFD51(RenderWater?)は書き換える必要がないので除外済み
    //日本語用の表示に関係ないことが確認できたアドレスは除外済み
    //callやpushなどがあったアドレスの書き換えはゲームが落ちる原因となったのでそれらのアドレスはすべて除外済み
    constexpr std::array<int, 3> AddressesForFMulDWordC9DFF0 = { 0x718B5A, 0x718B74, 0x718BE9 };
    constexpr std::array<int, 4> AddressesForFAddDWordC9DFF0 = { 0x718BD7, 0x718CB6, 0x718D42, 0x718DE0 };
    constexpr std::array<int, 3> AddressesForFAddDWordC9DFF4 = { 0x718CAC, 0x718D2A, 0x718DD6 };
    constexpr int AddressForFMulQWordC9DFF8 = 0x718C37;
#pragma endregion

    injector::MakeJMP(0x7199AB, ASM_HookedDrawCharacter_10); // instrForCFont_RenderFontBuffer1
    injector::MakeJMP(0x718B11, ASM_HookedDrawCharacter2_10);
    injector::MakeJMP(0x718B33, ASM_HookedDrawCharacter3_10);
    injector::MakeJMP(0x719A8D, ASM_HookedDrawCharacter4_10);
    injector::MakeJMP(0x7197A2, ASM_HookedDrawCharacter5_10);
    injector::MakeJMP(0x71A150, ASM_GetStringWidthHooked_10); // in CFont::GetStringWidth at 0x71A0E0

    for (auto address : AddressesForFMulDWordC9DFF0)
    {
        if (address != 0)
        {
            injector::WriteMemory(address + 2, 0xC9DFF0);
        }
    }


    for (auto address : AddressesForFAddDWordC9DFF0)
    {
        if (address != 0)
        {
            injector::WriteMemory(address + 2, 0xC9DFF0);
        }
    }

    for (auto address : AddressesForFAddDWordC9DFF4)
    {
        if (address != 0)
        {
            injector::WriteMemory(address + 2, 0xC9DFF4);
        }
    }

    injector::WriteMemory(AddressForFMulQWordC9DFF8 + 2, 0xC9DFF8);
}

void Patch101Exe()
{
#pragma region Addresses For 1.01 exe
    //0x6D40AB(GetFiringRateMultiplier)と0x6D40C7(GetFiringRateMultiplier)と0x6D40FB+0x6D4117(GetPlaneGunsRateOfFire)と
    //0x6D45AB+0x6D45C7(GetPlaneOrdnanceRateOfFire)と0x6EFCF6+0x6EFD22+0x6EFD51(RenderWater?)は書き換える必要がないので除外済み
    //日本語用の表示に関係ないことが確認できたアドレスは除外済み
    //callやpushなどがあったアドレスの書き換えはゲームが落ちる原因となったのでそれらのアドレスはすべて除外済み
    constexpr std::array<int, 3> AddressesForFMulDWordC9FCB0 = { 0x71938A, 0x7193A4, 0x719419 };
    constexpr std::array<int, 4> AddressesForFAddDWordC9FCB0 = { 0x719407, 0x7194E6, 0x719572, 0x719610 };
    constexpr std::array<int, 3> AddressesForFAddDWordC9FCB4 = { 0x7194DC, 0x71955A, 0x719606 };
    constexpr int AddressForFMulQWordC9FCB8 = 0x719467;
#pragma endregion

    injector::MakeJMP(0x71A1DB, ASM_HookedDrawCharacter_101); // instrForCFont_RenderFontBuffer1
    injector::MakeJMP(0x719341, ASM_HookedDrawCharacter2_101);
    injector::MakeJMP(0x719363, ASM_HookedDrawCharacter3_101);
    injector::MakeJMP(0x71A2BD, ASM_HookedDrawCharacter4_101);
    injector::MakeJMP(0x719FD2, ASM_HookedDrawCharacter5_101);
    injector::MakeJMP(0x71A980, ASM_GetStringWidthHooked_101); // in CFont::GetStringWidth at 0x71A0E0

    for (auto address : AddressesForFMulDWordC9FCB0)
    {
        if (address != 0)
        {
            injector::WriteMemory(address + 2, 0xc9fcb0);
        }
    }


    for (auto address : AddressesForFAddDWordC9FCB0)
    {
        if (address != 0)
        {
            injector::WriteMemory(address + 2, 0xc9fcb0);
        }
    }

    for (auto address : AddressesForFAddDWordC9FCB4)
    {
        if (address != 0)
        {
            injector::WriteMemory(address + 2, 0xc9fcb4);
        }
    }

    injector::WriteMemory(AddressForFMulQWordC9FCB8 + 2, 0xc9fcb8);
}

void Main()
{

    const auto version = GetGameVersion();
    //正常に日本語化できない（と思われる）バージョンでは日本語化できない旨を知らせてexeの起動を止める
    if (version == GameVersion::v10US_COMPACT || version == GameVersion::STEAM || version == GameVersion::STEAM_LV || version == GameVersion::UNKNOWN)
    {
        ShowErrorBox(version);
        ExitProcess(0);
    }

    bool isExeVersion10 = (version == GameVersion::v10US_HOODLUM || version == GameVersion::v10EU);

    // "mov byte ptr [esp+0x10], cl (88 4c 24 10)" を "mov DWORD PTR [esp+0x10], ecx" (89 4c 24 10) に変更して2バイトの文字を読み取れるようにする
    injector::WriteMemory(isExeVersion10 ? 0x7199CD : 0x71A1FD, static_cast<unsigned char>(0x89));

    //Pricedownで','と'.'が出るようにする
    constexpr unsigned char comp = 0xFF;
    injector::WriteMemory(isExeVersion10 ? 0x718C6E : 0x71949E, comp);

    if (isExeVersion10)
    {
        Patch10Exe();
    }
    else
    {
        Patch101Exe();
    }
}
