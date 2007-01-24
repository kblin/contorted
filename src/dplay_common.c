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

#include "dplay_common.h"

uint16_t sizeNeeded(DPlayPacket *packet)
{
    return DPLAY_HEADER_SIZE + packet->data_size;
}

uint16_t packShort(uint8_t *data, uint16_t item, uint16_t offset)
{
    union short_to_byte {
        uint16_t twobyte;
        uint8_t byte[2];
    } tmp;

    tmp.twobyte = item;

    memcpy(data+offset, tmp.byte, 2);

    return offset + 2;
}

int preparePacket(uint8_t *binary, DPlayPacket *packet)
{
    uint16_t offset = 0;

    offset = packShort(binary, packet->size, offset);

    offset = packShort(binary, packet->dplay_id, offset);

    memcpy(binary + offset, packet->sa, sizeof(struct sockaddr_in)); 
    offset += sizeof(struct sockaddr_in);

    memcpy(binary + offset, packet->action, 4);
    offset += 4;

    offset = packShort(binary, packet->type, offset);

    offset = packShort(binary, packet->version, offset);

    memcpy(binary + offset, packet->data, packet->data_size);

    return 0;
}

int createType02Data(uint8_t *data, GUID *guid, uint8_t *payload)
{
    memcpy(data, guid, 16);
    memcpy(data+16, payload, 8);
    return 24;
}

