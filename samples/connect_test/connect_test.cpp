#include <dplay.h>
#include <dplobby.h>
#include <iostream>

#define INITGUID

// {EF016237-BEEE-402a-A2D6-A2694C109574}
DEFINE_GUID(CONNECT_TEST, 
0xef016237, 0xbeee, 0x402a, 0xa2, 0xd6, 0xa2, 0x69, 0x4c, 0x10, 0x95, 0x74);

//------------------ Function prototypes ----------------
DWORD handleArgs(int argc, char **argv);

typedef struct _dp_player_list {
    DPID dpid;
    char name[15];
    DWORD flags;
    struct _dp_player_list *next;
} dp_player_list;

//------------------------ Globals ----------------------
LPDIRECTPLAY4A lpdp = NULL;
LPDIRECTPLAYLOBBY3A lpdplobby = NULL;
BOOL is_server = FALSE;
dp_player_list *plist;
char player_name[] = "testplayer";
char session_name[] = "testsession";
char tcp_address[15];

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

    if(FAILED(hr = CoInitialize(NULL))){
        return -1;
    }

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
                std::cout << "S";
            else
                std::cout << "c";
            if(i > 25)
                break;
        }
    }
    std::cout << std::endl;

    CoUninitialize();
}

DWORD handleArgs(int argc, char **argv)
{
    if(argc < 2)
    {
        std::cout << "Too few arguments." << std::endl;
        return -1;
    }
    if(strcmp(argv[1], "server") == 0)
        is_server = TRUE;

    return 0;
}
