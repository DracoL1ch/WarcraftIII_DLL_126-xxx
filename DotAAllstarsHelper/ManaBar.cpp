#include "Main.h"



#pragma optimize("",off)


#define ADDRESS LPVOID  // data
#define GADDRESS LPVOID // game call

GADDRESS sub_6F27AE90;   // 27B950 6f27B950
GADDRESS sub_6F334180;   // 334C00 6f334C00
GADDRESS sub_6F6061B0;   // 606860 6f606860
GADDRESS sub_6F605CC0;   // 606370 6f606370
GADDRESS sub_6F359CC0;   // 35A740 6f35A740
GADDRESS sub_6F32C880; // 32D300   6f
ADDRESS Storm_401; //storm 0x191 #401
GADDRESS sub_6F2C74B0;

ADDRESS a16F08C;
ADDRESS a16F004;
ADDRESS a16F090;
//ADDRESS a16F06C;
//ADDRESS a16F070;
ADDRESS a3000AC;
ADDRESS a3000B0;

int aHPBAR;
int aMPBAR;

BYTE BarVtableClone[ 0x80 ];

char* a164684 = "ScaleFactor";

BOOL SkipScaleFactor = FALSE;

double a164A18 = 72; // ds:[00164A18]=72.00000000000000
double aMPBarSizeX = 0.0005; //ds:[00164A10]=0.0005000000237487257
double aMPBarSizeY = 0.004;
double aMPBarOffsetY = 0.006; //ds:[00164A08]=0.006000000052154064
double a1649D4 = 0.03; //ds:[004149D4]=0.03000000

double a1649CC = 0.3;

double aMPBarSizeY_default = 0.004;
double aMPBarSizeX_default = 0.0005; //ds:[00164A10]=0.0005000000237487257
double aMPBarOffsetY_default = 0.006; //ds:[00164A08]=0.006000000052154064

float mpbarscaleHeroX[ 20 ];
float mpbarscaleHeroY[ 20 ];
float mpbaroffsetHeroY[ 20 ];

float mpbarscaleUnitX[ 20 ];
float mpbarscaleUnitY[ 20 ];
float mpbaroffsetUnitY[ 20 ];

float mpbarscaleTowerX[ 20 ];
float mpbarscaleTowerY[ 20 ];
float mpbaroffsetTowerY[ 20 ];

__declspec( dllexport ) void __stdcall SetMPBarXScaleForPlayer( int playerid, float heroscale,
																float unitscale, float towerscale )
{
	if ( playerid >= 0 && playerid < 20 )
	{
		mpbarscaleHeroX[ playerid ] = heroscale;
		mpbarscaleUnitX[ playerid ] = unitscale;
		mpbarscaleTowerX[ playerid ] = towerscale;
	}
}

__declspec( dllexport ) void __stdcall SetMPBarYScaleForPlayer( int playerid, float heroscale,
																float unitscale, float towerscale )
{
	if ( playerid >= 0 && playerid < 20 )
	{
		mpbarscaleHeroY[ playerid ] = heroscale;
		mpbarscaleUnitY[ playerid ] = unitscale;
		mpbarscaleTowerY[ playerid ] = towerscale;
	}
}



__declspec( dllexport ) void __stdcall SetMPBarYOffsetForPlayer( int playerid, float herooffset,
																float unitoffset, float toweroffset )
{
	if ( playerid >= 0 && playerid < 20 )
	{
		mpbaroffsetHeroY[ playerid ] = herooffset;
		mpbaroffsetUnitY[ playerid ] = unitoffset;
		mpbaroffsetTowerY[ playerid ] = toweroffset;
	}
}



int __stdcall  SetMPBarConfigForPlayer( int unitaddr )
{
	int retval = 0;
	__asm mov retval, eax;

	if ( !unitaddr || !IsNotBadUnit( unitaddr ) )
		return retval;


	int unitslot = GetUnitOwnerSlot( unitaddr );
	if ( unitslot > 15 )
		return retval;

	aMPBarSizeX = aMPBarSizeX_default;
	aMPBarSizeY = aMPBarSizeY_default;
	aMPBarOffsetY = aMPBarOffsetY_default;
	SkipScaleFactor = FALSE;

		if ( IsHero( unitaddr ) )
		{
			if ( mpbarscaleHeroX[ unitslot ] != 0.0 )
			{
				SkipScaleFactor = TRUE;

				aMPBarSizeX = mpbarscaleHeroX[ unitslot ];
			}

			if ( mpbarscaleHeroY[ unitslot ] != 0.0 )
			{
				SkipScaleFactor = TRUE;

				aMPBarSizeY = mpbarscaleHeroY[ unitslot ];
			}

			if ( mpbaroffsetHeroY[ unitslot ] != 0.0 )
			{
				SkipScaleFactor = TRUE;

				aMPBarOffsetY = mpbaroffsetHeroY[ unitslot ];
			}

		}
		else if ( IsHero( unitaddr ) )
		{
			if ( mpbarscaleTowerX[ unitslot ] != 0.0 )
			{
				SkipScaleFactor = TRUE;

				aMPBarSizeX = mpbarscaleTowerX[ unitslot ];
			}

			if ( mpbarscaleTowerY[ unitslot ] != 0.0 )
			{
				SkipScaleFactor = TRUE;

				aMPBarSizeY = mpbarscaleTowerY[ unitslot ];
			}

			if ( mpbaroffsetTowerY[ unitslot ] != 0.0 )
			{
				SkipScaleFactor = TRUE;

				aMPBarOffsetY = mpbaroffsetTowerY[ unitslot ];
			}

		}
		else
		{
			if ( mpbarscaleUnitX[ unitslot ] != 0.0 )
			{
				SkipScaleFactor = TRUE;

				aMPBarSizeX = mpbarscaleUnitX[ unitslot ];
			}

			if ( mpbarscaleUnitY[ unitslot ] != 0.0 )
			{
				SkipScaleFactor = TRUE;

				aMPBarSizeY = mpbarscaleUnitY[ unitslot ];
			}

			if ( mpbaroffsetUnitY[ unitslot ] != 0.0 )
			{
				SkipScaleFactor = TRUE;

				aMPBarOffsetY = mpbaroffsetUnitY[ unitslot ];
			}

		}

	return retval;
}

void __declspec( naked ) FillMemoryForMPBar( )
{
	__asm
	{
		push    ebx;
		mov     ebx, sub_6F6061B0;  // ds:[0016FF64]=6F606860 (Game.6F606860)
		push    edi;
		mov     edi, sub_6F605CC0;  // ds:[0016FF5C]=6F606370 (Game.6F606370)
		push    0;
		push    0;
		push    0;
		xor edx, edx;
		mov     ecx, esi;
		call    sub_6F359CC0;    // ds:[0016FF58]=6F35A740 (Game.6F35A740)
		fld     a1649D4;    // ds:[001649D4]=0.03000000
		push    0;
		fstp    dword ptr[ esi + 0x58 ];
		xor edx, edx;
		mov     ecx, esi;
		call    edi;
		fld     aMPBarSizeY;    // ds:[001649D0]=0.004000000
		push    0;
		fstp    dword ptr[ esi + 0x5C ];
		xor edx, edx;
		mov     ecx, esi;
		call    edi;
		fld     a1649CC;       // ds:[001649CC]=0.3000000
		push    1;
		sub     esp, 0x8;
		fst     dword ptr[ esp + 0x4 ];
		xor edx, edx;
		fstp    dword ptr[ esp ];
		push    1;
		mov     ecx, esi;
		call    ebx;

		mov     eax, dword ptr[ esi ];
		mov     eax, dword ptr[ eax + 0x64 ];

		pop     edi;
		xor     edx, edx;
		mov     ecx, esi;
		pop     ebx;
		jmp     eax;
	}

}

void __declspec( naked ) ReallocateMemoryForMPBar( )
{
	__asm {
		pop     a16F08C;
		pop     eax;
		add     eax, eax;
		push    eax;
		call    Storm_401;
		pushad;
		mov     a16F004, eax;
		mov     esi, a16F004;
		add     esi, 0x158;
		call    FillMemoryForMPBar;


		popad;
		push    a16F08C;
		retn;
	}
}

void __declspec( naked ) f152950( )
{
	__asm {

		mov     eax, a16F090;
		mov     eax, dword ptr[ eax + 0x64 ];
		push    esi;
		push    edi;
		mov     esi, edx;
		mov     edi, ecx;
		call    eax;
		mov     eax, a16F090;
		mov     eax, dword ptr[ eax + 0x64 ];
		lea     ecx, dword ptr[ edi + 0x158 ];
		pop     edi;
		mov     edx, esi;
		pop     esi;
		jmp     eax;

	}
}
void __declspec( naked ) f152980( )
{
	__asm {
		mov     eax, a16F090;
		mov     eax, dword ptr[ eax + 0x68 ];
		jmp     eax;
	}
}

void __declspec( naked ) f00152710( )
{
	__asm {
		push eax;
		push ebx;
		mov     a16F090, ecx;
		xor     eax, eax;
		jmp L004;
		lea     ebx, dword ptr[ ebx ];
		L004:
		mov     dl, byte ptr[ eax + ecx ];
		mov     byte ptr[ eax + BarVtableClone ], dl;
		inc     eax;
		cmp     eax, 0x80;
		jb L004;   // copy
		lea     eax, BarVtableClone;
		add     eax, 0x64;
		lea     ebx, f152950;
		mov     dword ptr[ eax ], ebx;
		lea     eax, BarVtableClone;
		add     eax, 0x68;
		lea     ebx, f152980;
		mov     dword ptr[ eax ], ebx;
		pop ebx;
		pop eax;
		retn;
	}
}


void __declspec( naked ) f001527F0( )
{
	__asm {
		sub     esp, 0x10;
		push    edi;
		mov     edi, a16F004;
		cmp     a3000AC, 0;
		je L093;
		push edi;
		call IsHero;
		cmp eax, 0;
		je L093;
		push edi;
		call IsEnemy;
		cmp eax, 1;
		je L093;
		push edi;
		call SetMPBarConfigForPlayer;
		mov     eax, dword ptr[ edi + 0x50 ];
		test    eax, eax;
		je L093;
		push    ebx;
		mov     ebx, dword ptr[ eax + 0xC ];
		mov aHPBAR, ebx;
		push    esi;
		lea     esi, dword ptr[ ebx + 0x158 ];
		mov aMPBAR, esi;
		cmp     a3000B0, 0;
		jnz L017;
		mov     ecx, dword ptr[ ebx ];
		call    f00152710;
		mov ebx, dword ptr[ eax + 0xC ];
		mov     a3000B0, 1;
		L017:
		push    0;
		lea     eax, dword ptr[ esp + 0x10 ];
		push    eax;
		xor     edx, edx;
		mov     ecx, edi;
		push eax;
		lea eax, BarVtableClone;
		mov     dword ptr[ ebx ], eax;
		pop eax;
		call    sub_6F27AE90; // ds:[0016FF24]=6F27B950 (Game.6F27B950)
		fldz;
		fcomp   dword ptr[ esp + 0xC ];
		fstsw   ax;
		test    ah, 1;
		je L091;
		push    3;
		lea     ecx, dword ptr[ esp + 0x10 ];
		push    ecx;
		xor     edx, edx;
		mov     ecx, edi;
		call    sub_6F27AE90;     // ds:[0016FF24]=6F27B950 (Game.6F27B950)
		fldz;
		fcomp   dword ptr[ esp + 0xC ];
		fstsw   ax;
		test    ah, 5;
		jpe L091;
		mov     eax, dword ptr[ esi ];
		cmp		eax, _BarVTable; // FIX CRASH
		je OkayBar;
		lea		ecx, BarVtableClone
		cmp		eax, ecx; // FIX CRASH
		je OkayBar;
		jmp L091;
		OkayBar:
		//no
		mov     eax, dword ptr[ eax + 0x74 ];
		push    ebp;
		push    edi;
		xor     edx, edx;
		mov     ecx, esi;
		call    eax;
		//no
		mov     ebx, sub_6F6061B0;  //ds:[0016FF64]=6F606860 (Game.6F606860)
		mov     ebp, sub_6F605CC0;   // ds:[0016FF5C]=6F606370 (Game.6F606370)
		lea     ecx, dword ptr[ esp + 0x1C ];
		push    ecx;
		lea     edx, dword ptr[ esp + 0x18 ];
		mov     ecx, edi;
		//no
		call    sub_6F334180;   // ds:[0016FF68]=6F334C00 (Game.6F334C00)
		//no
		mov     ecx, dword ptr[ edi + 0x30 ];
		mov     eax, 0;  
		cmp		SkipScaleFactor, 1;
		je		WithoutScaleFactor;
		mov     eax, sub_6F32C880;  // [0016FF20]=6F32D300
		lea     edx, a164684;
		call    eax;
		//no
		test    eax, eax;
		jnz L062;
		WithoutScaleFactor:
		fld1;
		jmp L063;
		L062:
		fld     dword ptr[ eax + 0x54 ];
		L063:
		fstp    dword ptr[ esp + 0x10 ];
		push    0;
		fld     dword ptr[ esp + 0x14 ];
		xor edx, edx;
		fmul    qword ptr[ a164A18 ];
		mov     ecx, esi;
		fmul    qword ptr[ aMPBarSizeX ];
		fstp    dword ptr[ esi + 0x58 ];
		call    ebp;
		//no
		fld     dword ptr[ esp + 0x18 ];
		push    1;
		fsub    qword ptr[ aMPBarOffsetY ];
		sub     esp, 8;
		xor edx, edx;
		mov     ecx, esi;
		fstp    dword ptr[ esp + 0x24 ];
		fld     dword ptr[ esp + 0x24 ];
		fstp    dword ptr[ esp + 0x4 ];
		fld     dword ptr[ esp + 0x20 ];
		fstp    dword ptr[ esp ];
		push    1;
		call    ebx;
		//no
		mov     eax, dword ptr[ esi ];
		mov     eax, dword ptr[ eax + 0x68 ];
		xor edx, edx;
		mov     ecx, esi;
		call    eax;
		//no
		pop     ebp;
		L091:
		pop     esi;
		pop     ebx;
		L093:



		pop     edi;
		add     esp, 0x10;
		retn;
	}
}
void __declspec( naked ) RedrawMPBar( )
{
	__asm {
		pushad;
		mov     a16F004, ecx;
		call    f001527F0;
		popad;
		//mov     eax, GameDLL    // game.dll base
		mov     eax, sub_6F2C74B0;
		jmp     eax;
	}
}

BOOL ManabarInitialized = FALSE;
BOOL ManabarEnabled = FALSE;

char STORM_401_malloc_old[ 5 ];
char HPMP_DRAW_old[ 5 ];

ADDRESS STORM_401_malloc;
ADDRESS HPMP_DRAW;

void Hook( )
{
	if ( !ManabarEnabled )
	{
		DWORD old1, old2, old3;
		ManabarEnabled = TRUE;

		VirtualProtect( STORM_401_malloc, 5, PAGE_EXECUTE_READWRITE, &old1 );
		VirtualProtect( HPMP_DRAW, 5, PAGE_EXECUTE_READWRITE, &old2 );

		CopyMemory( STORM_401_malloc_old, STORM_401_malloc, 5 );
		CopyMemory( HPMP_DRAW_old, HPMP_DRAW, 5 );

		{
			unsigned char* p = reinterpret_cast< unsigned char* >( STORM_401_malloc );
			*p = 0xe8;
			p += 5;
			int X = ( int ) ReallocateMemoryForMPBar - ( int ) p;
			p -= 5;
			*reinterpret_cast< DWORD* >( p + 1 ) = X;
		}
		{
			unsigned char* p = reinterpret_cast< unsigned char* >( HPMP_DRAW );
			*p = 0xe8;
			p += 5;
			int X = ( int ) RedrawMPBar - ( int ) p;
			p -= 5;
			*reinterpret_cast< DWORD* >( p + 1 ) = X;
		}

		VirtualProtect( STORM_401_malloc, 5, old1, &old3 );
		VirtualProtect( HPMP_DRAW, 5, old2, &old3 );

	}

}


void Unhook( )
{
	if ( ManabarEnabled )
	{
		DWORD old1, old2, old3;
		ManabarEnabled = FALSE;

		VirtualProtect( STORM_401_malloc, 5, PAGE_EXECUTE_READWRITE, &old1 );
		VirtualProtect( HPMP_DRAW, 5, PAGE_EXECUTE_READWRITE, &old2 );

		CopyMemory( STORM_401_malloc, STORM_401_malloc_old, 5 );
		CopyMemory( HPMP_DRAW, HPMP_DRAW_old, 5 );


		VirtualProtect( STORM_401_malloc, 5, old1, &old3 );
		VirtualProtect( HPMP_DRAW, 5, old2, &old3 );

	}

}


void ManaBarSwitch( int GameDLL, HMODULE StormDLL, BOOL b )
{
	*( int* ) &a3000AC = 1;

	HMODULE hMod = StormDLL;
	Storm_401 = ( ADDRESS ) GetProcAddress( hMod, ( LPCSTR ) 401 );

	if ( GameVersion == 0x26a )
	{
		*( int* ) &sub_6F27AE90 = ( int ) GameDLL + 0x27AE90;  // 6F27B9B0 
		*( int* ) &sub_6F334180 = ( int ) GameDLL + 0x334180;  // 0x6f334CC0
		*( int* ) &sub_6F6061B0 = ( int ) GameDLL + 0x6061B0;  // 0x6f606950
		*( int* ) &sub_6F605CC0 = ( int ) GameDLL + 0x605CC0;  // 0x6f606460
		*( int* ) &sub_6F359CC0 = ( int ) GameDLL + 0x359CC0;  // 0x6f35A800
		*( int* ) &sub_6F32C880 = ( int ) GameDLL + 0x32C880;  // 0x32D3C0
		*( int* ) &sub_6F2C74B0 = ( int ) GameDLL + 0x2C74B0;    // 0x6f2C7FD0??

		*( int* ) &STORM_401_malloc = ( int ) GameDLL + 0x379AE3;  // 0x6f37A623
		*( int* ) &HPMP_DRAW = ( int ) GameDLL + 0x379EE8;  // 0x6F37AA28

		if ( b )
			Hook( );
		else
		{
			memset( mpbarscaleHeroX, 0, sizeof( mpbarscaleHeroX ) );
			memset( mpbarscaleHeroY, 0, sizeof( mpbarscaleHeroY ) );
			memset( mpbaroffsetHeroY, 0, sizeof( mpbaroffsetHeroY ) );

			memset( mpbarscaleUnitX, 0, sizeof( mpbarscaleUnitX ) );
			memset( mpbarscaleUnitY, 0, sizeof( mpbarscaleUnitY ) );
			memset( mpbaroffsetUnitY, 0, sizeof( mpbaroffsetUnitY ) );

			Unhook( );
		}

		ManabarInitialized = TRUE;
	}
}


__declspec( dllexport ) BOOL __stdcall SetManabarEnabled( BOOL enabled )
{
	if ( ManabarInitialized )
	{
		*( int* ) &a3000AC = enabled;
	}
	return ManabarInitialized;
}

#pragma optimize("",on)