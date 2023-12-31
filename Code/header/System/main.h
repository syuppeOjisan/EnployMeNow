#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <functional>
#include <locale.h>
#include <string>
#include <DirectXMath.h>
#include <WICTextureLoader.h>

#pragma warning(push)
#pragma warning(disable:4005)

#include <d3d11.h>

#pragma warning(pop)

#pragma comment (lib,"winmm.lib")


constexpr uint32_t SCREEN_WIDTH = 1280;
constexpr uint32_t SCREEN_HEIGHT = 720;

// 度からラジアンへの変換
#define DEGREE_TO_RADIAN(deg) ((deg) * M_PI / 180.0)

// ラジアンから度への変換
#define RADIAN_TO_DEGREE(rad) ((rad) * 180.0 / M_PI)