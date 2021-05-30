// dllmain.cpp : Define o ponto de entrada para o aplicativo DLL.
#include "pch.h"
#include <Windows.h>
#include  <iostream>

int this_;
DWORD PTR_PACKET;
int a3;
int i;
DWORD  pular_de_volta = 0x04EDBB6;
char bufferpacket_string[16*4];
char temp_buffer[5];

__declspec(naked) void CastroP2P() {

    _asm {
        pushad /* stack is kept */
        pushfd
    }

    _asm {
        mov[PTR_PACKET], ebx //porque está tambem em ebx(via ida pro, ce e o que aprendemos)
    }

    
    memset(bufferpacket_string, 0, 16 * 4);
    memset(temp_buffer, 0, 5);

    

    for ( i = 0; i < 16; i++) {
        sprintf_s(temp_buffer,5, "%02X ",  (*((BYTE*)(PTR_PACKET + i))));
        strcat_s(bufferpacket_string,16*4, temp_buffer);
    }

    OutputDebugString(bufferpacket_string);

    __asm {
        popfd
        popad

        /* func was hooked at entrypoint */
        push ebp
        mov ebp, esp
        and esp, (-8)
        jmp[pular_de_volta]
    }

}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        OutputDebugStringA("DLL_PROCESS_ATTACH\n");
        (*(BYTE*)0x4EDBB0) = 0xE9;
        (*(DWORD*)(0x4EDBB0 + 1)) = (DWORD)&CastroP2P - 0x4EDBB0 - 5;
        DisableThreadLibraryCalls(hModule);
        return true;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

