/*
   Common functions for all dplay test programs

   Copyright (C) Kai Blin <kai.blin@gmail.com>  2006

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
*/

#ifndef DPLAY_COMMON_H
#define DPLAY_COMMON_H

#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define DPLAY_ID 0xfab0
#define ACTION_STRING {'p','l','a','y'}
#define DPLAY_HEADER_SIZE 28

typedef enum _packet_type {
    LOBBY_INFORMATION = 0x0001,
    LOBBY_BROADCAST,
    TYPE_05 = 0x0005,
    TYPE_07 = 0x0007,
    TYPE_08,
    CONN_SHUTDOWN = 0x000b,
    TYPE_0f = 0x000f,
    TYPE_13 = 0x0013,
    ENCAP_MSG = 0x0015,
    TYPE_29 = 0x0029
} PacketType;

typedef enum _dplay_version {
    DPLAY7 = 0x000b,
    DPLAY9 = 0x000e
} DPlayVersion;

typedef struct _dplay_packet
{
    uint16_t size;
    uint16_t dplay_id;
    struct sockaddr_in *sa;
    unsigned char action[4];
    PacketType type;
    DPlayVersion version;
    uint16_t data_size;
    void* data;
} DPlayPacket;

typedef struct _GUID
{
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t Data4[8];
} GUID;

uint16_t sizeNeeded(DPlayPacket *packet);

uint16_t packShort(uint8_t *data, uint16_t item, uint16_t offset);

int preparePacket(uint8_t *data, DPlayPacket *packet);

int parsePacket(uint8_t *data, DPlayPacket *packet);

int createType02Data(uint8_t *data, GUID *guid, uint8_t *payload);

#endif
