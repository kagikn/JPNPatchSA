#pragma once

void __declspec(naked) ASM_HookedDrawCharacter() {
    _asm {
        xor    ecx, ecx
        mov    cl, byte ptr ds : 0xc71ac9
        xor    eax, eax
        mov    al, byte ptr[ebx]
        cmp    al, 0x81
        jb     _1c
        cmp    al, 0x9f
        jbe    _4b
        cmp    al, 0xe0
        jb     _1c
        cmp    al, 0xef
        jbe    _4b
_1c:
        mov    byte ptr ds : 0xc9dfe0, 0x0
        push   edx
        mov    edx, 0xc9dff0
        mov    dword ptr[edx], 0x3c800000
        mov    edx, 0xc9dff4
        mov    dword ptr[edx], 0x3d200000
        mov    edx, 0xc9dffc
        mov    dword ptr[edx], 0x3fa40000
        pop    edx
        mov    edi, 0x7199b3
        jmp    edi
_4b:
        mov    ah, al
        mov    al, byte ptr[ebx + 0x1]
        cmp    al, 0x40
        jb     _1c
        cmp    al, 0xfc
        ja     _1c
        cmp    ah, 0x98
        jb     _63
        ja     _77
        cmp    al, 0x72
        ja     _77
_63:
        sub    ah, 0x81
        shl    ah, 1
        cmp    al, 0xa0
        jae    _70
        sub    al, 0x20
        jmp    _7b
_70:
        add    ah, 0x1
        sub    al, 0x80
        jmp    _7b
_77:
        mov    ah, 0x0
        mov    al, 0x37
_7b:
        inc    ebx
        mov    byte ptr ds : 0xc9dfe0, 0x1
        push   edx
        mov    edx, 0xc9dff0
        mov    dword ptr[edx], 0x3c000000
        mov    edx, 0xc9dff4
        mov    dword ptr[edx], 0x3ca00000
        mov    edx, 0xc9dffc
        mov    dword ptr[edx], 0x3f940000
        pop    edx
        mov    ecx, eax
        mov    edi, 0x7199cd
        jmp    edi
    }
}

void __declspec(naked) ASM_HookedDrawCharacter2() {
    _asm {
        cmp    byte ptr ds : 0xc9dfe0, 0x1
        jne    _ce
        sub    esp, 0x4
        mov    eax, 0x20
        mov    dword ptr[esp + 0x4], eax
        fild   dword ptr[esp + 0x4]
        add    esp, 0x4
        jmp    _d3
_ce:
        mov    edx, 0x718770
        call   edx
_d3:
        mov    ecx, 0x718b16
        jmp    ecx
    }
}

void __declspec(naked) ASM_HookedDrawCharacter3() {
    _asm {
        cmp    byte ptr ds : 0xc9dfe0, 0x1
        jne    _fe
        mov    ebx, dword ptr[esp + 0x30]
        mov    edx, ebx
        shr    edx, 0x8
        mov    eax, ebx
        and    eax, 0xff
        mov    ecx, 0x718b3f
        jmp    ecx
_fe:
        mov    cl, bl
        shr    cl, 0x4
        mov    edx, 0x718b38
        jmp    edx
    }
}

void __declspec(naked) ASM_HookedDrawCharacter4() {
    _asm {
            cmp    byte ptr ds : 0xc9dfe0, 0x1
            jne    _120
            mov    eax, 0x20
            jmp    _128
_120:
            movzx  eax, ds : 0xC718B0[edi + eax]
_128:
            mov    edi, 0x719a95
            jmp    edi
    }
}

void __declspec(naked) ASM_HookedDrawCharacter5() {
    _asm {
        cmp    byte ptr ds : 0xc9dfe0, 0x1
        jne    _140
        mov    eax, 0x20
        jmp    _148
_140:
        movzx  eax, ds : 0xC718B0[ecx + edx]
_148:
        mov    edx, 0x7197aa
        jmp    edx
    }
}

void __declspec(naked) ASM_GetStringWidthHooked() {
    _asm {
        mov    al, byte ptr[esi]
        cmp    al, 0x81
        jb     _162
        cmp    al, 0x9f
        jbe    _16b
        cmp    al, 0xe0
        jb     _162
        cmp    al, 0xef
        jbe    _16b
_162:
        mov    byte ptr ds : 0xc9dfe0, 0x0
        jmp    _180
_16b:
        mov    ah, byte ptr[esi + 0x1]
        cmp    ah, 0x40
        jb     _162
        cmp    ah, 0xfc
        ja     _162
        mov    byte ptr ds : 0xc9dfe0, 0x1
        inc    esi
_180:
        cmp    al, 0x20
        mov    cl, byte ptr[esp + 0x1a8]

        //ここはレジスターではどうやってもゲームを壊さずにジャンプできないので少し速度が犠牲になるがpush+retで代用
        push   0x71a15b
        ret
    }
}
