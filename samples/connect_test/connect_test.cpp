/*
   Copyright (C) Kai Blin <kai.blin@gmail.com>  2007

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA

   A client/server connect test for dplay.
*/
#define INITGUID
#define _WIN32_DCOM
#include <dplay8.h>
#include <dplobby.h>
#include <cstdio>
#include <vector>

// {EF016237-BEEE-402a-A2D6-A2694C109574}
DEFINE_GUID(CONNECT_TEST, 
0xef016237, 0xbeee, 0x402a, 0xa2, 0xd6, 0xa2, 0x69, 0x4c, 0x10, 0x95, 0x74);

typedef struct _dp_player_list {
    DPID dpid;
    char name[15];
    DWORD flags;
    struct _dp_player_list *next;
} dp_player_list;

//------------------------ Globals ----------------------
IDirectPlay8Peer *pDP = NULL;
IDirectPlay8Address *pDP_addr = NULL;
BOOL is_server = FALSE;

//------------------ Function prototypes ----------------
DWORD handleArgs(int argc, char **argv);
HRESULT WINAPI dPlayMsgHandler(void * user_context, DWORD msg_id, void * msg_buff);
HRESULT initDPlay();
void cleanupDPlay();
HRESULT enumDPlaySP();
HRESULT getDPlayAddr();
HRESULT hostGame();

int main(int argc, char** argv)
{
    HRESULT hr;
    DWORD dwresult;
    MSG msg;
    BOOL done = FALSE;
    int i = 0;

    if((dwresult = handleArgs(argc, argv)) == -1)
    {
        return dwresult;
    }

    if(FAILED(hr = CoInitialize(NULL)))
    {
        return -1;
    }

    if(FAILED(hr = initDPlay()))
    {
        fprintf(stderr, "Failed to init dplay: 0x%08x\n", hr);
        goto cleanup;
    }

    if(FAILED(hr = enumDPlaySP()))
    {
        fprintf(stderr, "Failed enumerating service providers: 0x%08x\n", hr);
        goto cleanup;
    }

    if(FAILED(hr = getDPlayAddr()))
    {
        fprintf(stderr, "Failed to get IDirectPlay8Address com object: 0x%08x\n", hr);
        goto cleanup;
    }

    if(is_server)
    {
        printf("Selected server mode, hosting a game.\n");
        if(FAILED(hr = hostGame()))
        {
            fprintf(stderr, "Failed to host game: 0x%08x\n", hr);
            goto cleanup;
        }
    }
    else
    {
        printf("Selected client mode, enumerating games.\n");
    }

#if 0
    while(!done)
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // fake game loop
            ++i;
            if(is_server)
                printf("S");
            else
                printf("c");
            if(i > 25)
                break;
        }
    }
#endif
    puts("Press any key to exit.");
    getchar();

cleanup:
    cleanupDPlay();
    CoUninitialize();
}

DWORD handleArgs(int argc, char **argv)
{
    if(argc < 2)
    {
        is_server = FALSE;
        return 0;
    }
    if(strcmp(argv[1], "server") == 0)
        is_server = TRUE;

    return 0;
}

HRESULT WINAPI dPlayMsgHandler(void * user_context, DWORD msg_id, void * msg_buff)
{
    return S_OK;
}

HRESULT enumDPlaySP()
{
    HRESULT hr;
    DWORD items = 0, size = 0, i;
    DPN_SERVICE_PROVIDER_INFO *dp_sp_info, *dp_sp_item;

    hr = pDP->EnumServiceProviders(NULL, NULL, NULL, &size, &items, 0);

    if(hr != DPNERR_BUFFERTOOSMALL)
    {
        fprintf(stderr, "Failed to get the service provider array size: 0x%08x\n", hr);
        return hr;
    }

    dp_sp_info = static_cast<DPN_SERVICE_PROVIDER_INFO *>(CoTaskMemAlloc(size));

    if(FAILED(hr = pDP->EnumServiceProviders(NULL, NULL, dp_sp_info, &size, &items, 0)))
    {
        fprintf(stderr, "Failed to enumerate service providers: 0x%08x\n", hr);
        return hr;
    }

    dp_sp_item = dp_sp_info;
    printf("Found %d providers:\n", items);
    for(i = 0; i < items; ++i)
    {
        printf("Provider: %S\n", dp_sp_item->pwszName);
        ++dp_sp_item;
    }

    CoTaskMemFree(dp_sp_info);
    return hr;
}

HRESULT getDPlayAddr()
{
    HRESULT hr;

    if(FAILED(hr = CoCreateInstance(CLSID_DirectPlay8Address, NULL, CLSCTX_INPROC_SERVER, IID_IDirectPlay8Address,
        (LPVOID *)&pDP_addr)))
    {
        fprintf(stderr, "Failed to create IDirectPlay8Address instance: 0x%08x\n", hr);
        return hr;
    }

    if(FAILED(pDP_addr->SetSP(&CLSID_DP8SP_TCPIP)))
    {
        fprintf(stderr, "Failed to set TCP/IP service provider: 0x%08x\n", hr);
        return hr;
    }

    printf("Loaded the TCP/IP service provider.\n");
    return hr;
}

HRESULT hostGame()
{
    HRESULT hr;
    DPN_APPLICATION_DESC app_desc;

    ZeroMemory(&app_desc, sizeof(DPN_APPLICATION_DESC));
    app_desc.dwSize = sizeof(DPN_APPLICATION_DESC);
    app_desc.guidApplication = CONNECT_TEST;

    if(FAILED(hr = pDP->Host(&app_desc, &pDP_addr, 1, NULL, NULL, NULL, 0)))
    {
        fprintf(stderr, "Failed to host a game: 0x%08x\n", hr);
        return hr;
    }

    printf("Hosted game.\n");
    
    return hr;
}