/*
   A dplay test server.

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

#include "dpsvr.h"

#ifndef max
#define max(x,y) ((x) > (y)) ? (x) : (y)
#endif

static const uint8_t chatconnect_guid[] = { 0x43, 0x41, 0xeb, 0xe9, 0xa4, 0x0f,
    0x0b, 0x4e, 0xbe, 0xb3, 0xc5, 0x22, 0x26, 0x57, 0xf9, 0xf2};

static const uint8_t fake_guid[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

void signal_handler(int signum){
    switch(signum){
        case SIGINT:
            puts("Caught SIGINT, shutting down");
            exit(0);
        default:
            fprintf(stderr, "Ignoring unknown signal %d\n", signum);
    }
}

int handleSocketBroadcast(unsigned char *data, int data_len, struct sockaddr_in *sa_client,
        socklen_t sa_client_len, struct sockaddr_in *sa_recv)
{
    struct sockaddr_in sa_send;
    socklen_t sa_send_len = sizeof(struct sockaddr_in);
    int send_sock, sent = 0, offset = 0;
    DPlayPacket lobby_info = {0, DPLAY_ID, NULL, ACTION_STRING, 0,0,0,NULL};
    unsigned char lobby_info_body[512], temp[1024];
    unsigned char backslash[] = { '\\', '\0', '\0', '\0'};
    unsigned char game_name[] = { 't', '\0', 'e', '\0', 's', '\0', 't', '\0',
        'g', '\0', 'a', '\0', 'm', '\0', 'e', '\0', '\0', '\0'};
    unsigned char dpid[] = { 0x00, 0x54, 0x32, 0x10};
    unsigned char magic_number[] = {0x44, 0x00, 0x00, 0x00};
    unsigned char lobby_info_size[] = {0x50, 0x00, 0x00, 0x00};
    unsigned char magic_16_bytes[] = { 0x0a, 0, 0, 0, 0x04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    unsigned char unknown[] = { 0x0a, 0, 0, 0, 0x0f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    memset(&sa_send, 0, sa_send_len);

    lobby_info.type = LOBBY_INFORMATION;
    lobby_info.version = DPLAY7;
    lobby_info.sa = sa_recv;
    lobby_info.data_size = 80 + sizeof(backslash) + sizeof(game_name);
    lobby_info.data = lobby_info_body;
    lobby_info.size = sizeNeeded(&lobby_info);

    memcpy(lobby_info_body + offset, lobby_info_size, 4); offset += 4;
    memcpy(lobby_info_body + offset, magic_number, 4); offset += 4;
    memcpy(lobby_info_body + offset, fake_guid, 16); offset += 16;
    memcpy(lobby_info_body + offset, chatconnect_guid, 16); offset += 16;
    memcpy(lobby_info_body + offset, magic_16_bytes, 16); offset += 16;
    memcpy(lobby_info_body + offset, dpid, 4); offset += 4;
    memcpy(lobby_info_body + offset, unknown, 20); offset += 20;
    memcpy(lobby_info_body + offset, backslash, sizeof(backslash)); offset += sizeof(backslash);
    memcpy(lobby_info_body + offset, game_name, sizeof(game_name)); offset += sizeof(game_name);

    memcpy(&sa_send, data+4, 16);
    sa_send.sin_addr = sa_client->sin_addr;

    preparePacket(temp, &lobby_info);

    printf("Sending Lobby Information packet to %s:%d\n", inet_ntoa(sa_send.sin_addr),
            ntohs(sa_send.sin_port));

    if((send_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_IP)) < 0)
    {
        perror("socket");
        return 1;
    }

    if(connect(send_sock, (struct sockaddr *)&sa_send, sa_send_len) < 0)
    {
        perror("connect");
        return 9;
    }

    while(sent < lobby_info.size)
    {
        sent = send(send_sock, temp+sent, lobby_info.size - sent, 0);
        if(sent < 0)
        {
            perror("sent");
            return 10;
        }
    }

    return 0;
}

int main()
{
    struct sockaddr_in sa_udp, sa_recv, sa_client, sa_udp_cl;
    socklen_t sa_udp_len    = sizeof(struct sockaddr_in);
    socklen_t sa_recv_len   = sa_udp_len;
    socklen_t sa_client_len = sa_udp_len;
    socklen_t sa_udp_cl_len = sa_udp_len;
    int udp_sock, recv_sock, client_sock, maxfd = -1;
    int udp_port = 47624, recv_port = 2350;
    unsigned char temp[512];
    int i, ret = 0;
    fd_set recv_set, master_set;

    FD_ZERO(&recv_set);
    FD_ZERO(&master_set);

    memset(&sa_udp, 0, sa_udp_len);
    memset(&sa_recv, 0, sa_recv_len);
    memset(&sa_client, 0, sa_client_len);
    memset(&sa_udp_cl, 0, sa_udp_cl_len);

    printf("Listening on udp port %d\n", udp_port);
    sa_udp.sin_family = AF_INET;
    sa_udp.sin_port = htons(udp_port);
    sa_udp.sin_addr.s_addr = htonl(INADDR_ANY);

    if((udp_sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP)) < 0)
    {
        perror("socket");
        exit(1);
    }

    maxfd = max(maxfd, udp_sock);

    if(bind(udp_sock, (struct sockaddr *)&sa_udp, sa_udp_len) < 0)
    {
        perror("bind");
        exit(2);
    }

    FD_SET(udp_sock, &master_set);

    printf("Listening on tcp port %d\n", recv_port);
    sa_recv.sin_family = AF_INET;
    sa_recv.sin_port = htons(recv_port);
    sa_recv.sin_addr.s_addr = htonl(INADDR_ANY);

    if((recv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_IP)) < 0)
    {
        perror("socket");
        exit(3);
    }

    maxfd = max(maxfd, recv_sock);

    if(bind(recv_sock, (struct sockaddr*)&sa_recv, sa_recv_len) < 0)
    {
        perror("bind");
        exit(4);
    }

    if(listen(recv_sock, 5)< 0)
    {
        perror("listen");
        exit(5);
    }

    FD_SET(recv_sock, &master_set);

    signal(SIGINT, signal_handler);

    puts("Starting up the select loop.");

    for(;;)
    {
        recv_set = master_set;
        if(select(maxfd+1, &recv_set, NULL, NULL, NULL) < 0)
        {
            perror("select");
            exit(6);
        }
        for(i = 0; i <= maxfd; ++i)
        {
            if(FD_ISSET(i, &recv_set))
            {
                if(i == udp_sock)
                {
                    int recv_len;
                    printf("Got an udp packet!\n");
                    if((recv_len = recvfrom(udp_sock, temp, 512, 0, (struct sockaddr *)&sa_udp_cl, 
                                    &sa_udp_cl_len)) < 0)
                    {
                        perror("recvfrom");
                        exit(8);
                    }

                    if(handleSocketBroadcast(temp, recv_len, &sa_udp_cl, sa_udp_cl_len, &sa_recv) != 0)
                    {
                        fprintf(stderr, "Something bad happened in handleSocketBroadcast\n");
                    }

                    /* just throw away the rest of the data */
                    if(recv_len == 512)
                        recvfrom(udp_sock, temp, 512, 0, (struct sockaddr *)&sa_udp_cl, &sa_udp_cl_len);
                }else if(i == recv_sock)
                {
                    printf("Got a tcp packet on the server port\n");
                    if((client_sock = accept(recv_sock, (struct sockaddr *)&sa_client, &sa_client_len)) < 0)
                    {
                        perror("accept");
                        exit(7);
                    }
                    else
                    {
                        FD_SET(client_sock, &master_set);
                        maxfd = max(maxfd, client_sock);
                    }
                }
                else
                {
                    int recv_len;
                    if((recv_len = recv(i, temp, 512, 0)) < 0)
                    {
                        perror("recv");
                        exit(8);
                    }
                    if(recv_len == 0)
                    {
                        puts("Client closed socket");
                    }
                    close(i);
                    FD_CLR(i, &master_set);
                }
            }
        }
    }

    return ret;
}
