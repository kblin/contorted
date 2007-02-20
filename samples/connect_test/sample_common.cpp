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

   Functions common for all samples.
*/
#define INITGUID
#define _WIN32_DCOM
#include <dplay8.h>
#include <cstdio>

HRESULT WINAPI dPlayMsgHandler(void * user_context, DWORD msg_id, void * msg_buff);
extern IDirectPlay8Peer* pDP;

HRESULT initDPlay()
{
    HRESULT hr = S_OK;

    if(FAILED(hr = CoCreateInstance( CLSID_DirectPlay8Peer, NULL,
                    CLSCTX_INPROC_SERVER, IID_IDirectPlay8Peer, (LPVOID*) &pDP)))
    {
        fprintf(stderr, "Failed to create the IDirectPlay8Peer object: 0x%08x\n", hr);
        return hr;
    }

    if(FAILED(hr = pDP->Initialize(NULL, dPlayMsgHandler, 0)))
    {
        fprintf(stderr, "Failed to initialize DirectPlay: 0x%08x\n", hr);
    }
    return hr;
}

void cleanupDPlay()
{
    if(pDP)
    {
        pDP->Close(0);
        pDP->Release();
    }
    pDP = NULL;
}
