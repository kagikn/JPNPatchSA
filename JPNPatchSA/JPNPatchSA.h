#pragma once
#define WIN32_LEAN_AND_MEAN
#include <string>
#include <vector>

enum class GameVersion
{
    v10US_COMPACT,
    v10US_HOODLUM,
    v10EU,
    v11US,
    v11EU,
    STEAM,
    STEAM_LV,
    UNKNOWN,
};

GameVersion GetGameVersion();
std::wstring GetGameVersionName(GameVersion version);
void Patch10Exe();
void Patch101Exe();
void Main();
void ShowErrorBox(GameVersion gameVersion);
