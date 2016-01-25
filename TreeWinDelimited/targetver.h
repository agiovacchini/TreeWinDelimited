#pragma once

// L'inclusione di SDKDDKVer.h definisce la piattaforma Windows più recente disponibile.

// Se si desidera compilare l'applicazione per una piattaforma Windows precedente, includere WinSDKVer.h e
// impostare la macro _WIN32_WINNT sulla piattaforma da supportare prima di includere SDKDDKVer.h.

// #include <SDKDDKVer.h>
#include <winsdkver.h>

#define _WIN32_WINNT 0x0501
#define WINVER 0x0501
#define NTDDI_VERSION 0x0501

#include <SDKDDKVer.h>