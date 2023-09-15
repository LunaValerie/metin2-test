#pragma once
DWORD NetworkStream = *(DWORD*)0x6F4FB8;

uintptr_t ModuleHandleA = (uintptr_t)GetModuleHandleA(NULL);
struct LocalMemory
{
	uintptr_t Adres(uintptr_t pointer, std::vector<unsigned int>adres)
	{
		uintptr_t adres_c = pointer;
		for (unsigned int i = 0; i < adres.size(); ++i)
		{
			adres_c = *(uintptr_t*)adres_c;
			adres_c += adres[i];
		}
		return adres_c;
	}
	__forceinline LocalMemory* operator->() { return this; }
};
struct Svside
{
	void svsidebypass(LPVOID pxAddress, PBYTE code, int size)
	{
		unsigned long Protection;
		VirtualProtect((LPVOID)pxAddress, size, PAGE_READWRITE, &Protection);
		memcpy((LPVOID)pxAddress, (const LPVOID)code, size);
		VirtualProtect((LPVOID)pxAddress, size, Protection, 0);
	}
	__forceinline Svside* operator->() { return this; }
};
struct Memory
{
	Svside Svside;
	LocalMemory LocalMemory;
	__forceinline Memory* operator->() { return this; }
};
inline Memory Mem;


namespace CRpointer
{
	DWORD dwLocalPlayer = 0x2F4FA0;
	DWORD dwLocalPointer = 0x0C;

	DWORD EnableSkipCollision_Collision = 0x744;
	DWORD Network_AttackSpeed = 0x680;
	DWORD Network_MovementSpeed = 0x67C;
	DWORD Network_QuickAge = 0x674;
	DWORD Network_OneHit = 0x5DC;

	DWORD Network_SkillSpeed = 0x50C;
}

class Character
{
public:
	static bool NetworkCollision(DWORD Enable)
	{
		*(int*)Mem->LocalMemory->Adres(ModuleHandleA + CRpointer::dwLocalPlayer,{ CRpointer::dwLocalPointer, CRpointer::EnableSkipCollision_Collision }) = Enable;
	}
	static bool NetworkAttackSpeed(float Enable)
	{
		*(float*)Mem->LocalMemory->Adres(ModuleHandleA + CRpointer::dwLocalPlayer, { CRpointer::dwLocalPointer, CRpointer::Network_AttackSpeed }) = Enable;
	}
	static bool NetworkMovementSpeed(float Enable)
	{
		*(float*)Mem->LocalMemory->Adres(ModuleHandleA + CRpointer::dwLocalPlayer, { CRpointer::dwLocalPointer, CRpointer::Network_MovementSpeed }) = Enable;
	}
	static bool SvsideMoveSpeedBypass(DWORD Limit)
	{
		DWORD gamemodule = 0;
		do {
			gamemodule = (DWORD)GetModuleHandleA(NULL);
			Sleep(100);
		} while (!gamemodule);
		if (gamemodule > 0)
		{
			Mem->Svside->svsidebypass((LPVOID)(gamemodule + 0x5A380), (PBYTE)"\xc7\x81\x00\x00\x00\x00\x00\x00\x00\x00\xc3\xcc\xcc\xcc\xcc\xcc\x56\x8b\xf1\x83\xbe\x00\x00\x00\x00\x00\x74\x00\x68\x00\x00\x00\x00\xff\x15\x00\x00\x00\x00\x50\x68\x00\x00\x00\x00\xff\xb6\x00\x00\x00\x00\xe8\x00\x00\x00\x00\x83\xc4\x00\x8b\xce", Limit);
		}
	}
	static bool NetworkQuickAge(float Enable)
	{
		*(float*)Mem->LocalMemory->Adres(ModuleHandleA + CRpointer::dwLocalPlayer, { CRpointer::dwLocalPointer, CRpointer::Network_QuickAge }) = Enable;
	}
	static bool NetworkOneHit(DWORD Enable)
	{
		*(int*)Mem->LocalMemory->Adres(ModuleHandleA + CRpointer::dwLocalPlayer, { CRpointer::dwLocalPointer, CRpointer::Network_OneHit }) = Enable;
	}
	static bool NetworkSkillSpeed(float Enable)
	{
		*(float*)Mem->LocalMemory->Adres(ModuleHandleA + CRpointer::dwLocalPlayer, { CRpointer::dwLocalPointer, CRpointer::Network_SkillSpeed }) = Enable;
	}
};

class Assembly
{
public:
	static bool SendBattleAttackPacket(DWORD NetworkStream, UINT dwAttack, DWORD VID)
	{
		try
		{
			typedef void(__thiscall* SendBattleAttack)(DWORD NetworkStream, UINT dwAttack, DWORD VID);
			DWORD m_iSendBattleAttackPacketCall = 0x004A5D60;
			SendBattleAttack SendBattleAttackPacket = (SendBattleAttack)m_iSendBattleAttackPacketCall;
			SendBattleAttackPacket(NetworkStream, dwAttack, VID);
			return true;
		}
		catch (...){}
	}
	static void PickCloseItem(DWORD call, DWORD mov) {
		while (true) {
			_asm {
				mov ecx, DWORD ptr ds : [mov]
				Call call
			}
		}
	}
};


bool dwProDamage17 = false;
float dwSpeed17 = 100.0f;

DWORD WINAPI x17(LPVOID lpParam)
{
	while (true)
	{
		DWORD GetTargetVID = *reinterpret_cast<DWORD*>(*reinterpret_cast<DWORD*>(0x006F4FB8) + (0x00012E28));
		if (dwProDamage17)
		{
			if (GetTargetVID > 0 && GetTargetVID < 99999)
			{
				Assembly::SendBattleAttackPacket(NetworkStream, 0, GetTargetVID);
			}
			Assembly::PickCloseItem(0x004B3B90, NetworkStream);
			Character::NetworkCollision(1);
			Character::NetworkAttackSpeed(10);
			Character::NetworkMovementSpeed(10);
			Character::SvsideMoveSpeedBypass(64);
		}
	}
}
