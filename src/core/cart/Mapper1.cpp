#include "Mapper1.h"



Mapper1::Mapper1(iNesRom* rom) : CartMapper(rom)
{
	shiftRegister = 0x10;

	controlReg = 0x0C; //TODO confirm this is initial state on power up
	PRGBankReg = 0;
	CHRBank0Reg = 0;
	CHRBank1Reg = 0;

	setupPRGMapping();
	setupCHRMapping();
	//NT Mirroring will be initialized in setupMapper()
}


Mapper1::~Mapper1()
{

}


void Mapper1::setupMapper(CPUCore::CPU* cpu, PPUCore::PPU* ppu)
{
	this->ppu = ppu;

	//Direct PPU to location of CHR ROM/RAM
	if (this->rom->getCHRROMSize() == 0)
		ppu->setCHRMemPointer(0); //Tell PPU to use internal 8KB of CHR Ram
	else
		ppu->setCHRMemPointer(this->CHRROMBuffer);

	setNTMirroring();
	setupCHRMapping();
}


unsigned char Mapper1::handleCartAccess(unsigned short cpuAddress, bool write, unsigned short writeValue)
{
	//TODO apparently, if a Read-Modify-Write instruction such as INC $8000 is called,
	//RMW make to consecutive writes but only the first write counts

	if (write == false)
	{
		if (cpuAddress >= 0x8000)
			return this->PRGROMBuffer[cpuAddress & 0x7FFF];  ///PRG-ROM Read  (use buffer for quick-reads)
		else if (cpuAddress >= 0x6000)
		{
			//Battery Backed Ram Read

			return 0;
		}
	}
	else
	{
		if ((cpuAddress >= 0x6000) && (cpuAddress < 0x8000))
		{
			//Write to Battery Backed Ram

			return 0;
		}

		//Remaining code handles writes >= $8000
		if (writeValue & 0x80)
		{
			shiftRegister = 0x10; //Reset shift register
			controlReg |= 0x0C;  //TODO confirm this occurs
		}
		else
		{
			//Write lowest bit of data into shift register
			bool fifthWrite = (shiftRegister & 1) ? true : false;

			shiftRegister = (shiftRegister >> 1);
			if (writeValue & 1) shiftRegister |= 16;

			//On Fifth Write since last shift reg reset, transfer the 5 bit pattern now in the shift register
			//to the designated internal register (based on state of Address Lines 14,13 on the fifth write)
			if (fifthWrite)
			{
				transferShiftRegToInternalReg(cpuAddress);
				shiftRegister = 0x10;  //Shift Register automatically resets after 5th write
			}
		}

		return 0;
	}


	return 0;
}


void Mapper1::transferShiftRegToInternalReg(unsigned short cpuAddressFifthWrite)
{
	if (cpuAddressFifthWrite >= 0xE000)
	{
		//to PRG Bank Register
		PRGBankReg = (shiftRegister & 0x1F);
		setupPRGMapping();
	}
	else if (cpuAddressFifthWrite >= 0xC000)
	{
		//to CHR Bank 1 Register
		CHRBank1Reg = (shiftRegister & 0x1F);
		setupCHRMapping();
	}
	else if (cpuAddressFifthWrite >= 0xA000)
	{
		//to CHR Bank 0 Register
		CHRBank0Reg = (shiftRegister & 0x1F);
		setupCHRMapping();
	}
	else if (cpuAddressFifthWrite >= 0x8000)
	{
		//to Control Register
		controlReg = (shiftRegister & 0x1F);
		setNTMirroring();
		setupPRGMapping();
		setupCHRMapping();
	}
}


void Mapper1::setNTMirroring()
{
	int mirroringType = (controlReg & 0x3);
	switch (mirroringType)
	{
	case 0:  //One Screen, lower bank
		ppu->setNTMirroring(PPUCore::PPUState::NTMIRRORING::ONESCREEN_LOWER);
		break;
	case 1:  //One Screen, upper bank
		ppu->setNTMirroring(PPUCore::PPUState::NTMIRRORING::ONESCREEN_UPPER);
		break;
	case 2:  //Vertical
		ppu->setNTMirroring(PPUCore::PPUState::NTMIRRORING::VERTICAL);
		break;
	case 3:  //Horizontal
		ppu->setNTMirroring(PPUCore::PPUState::NTMIRRORING::HORIZONTAL);
		break;
	default:
		break;
	}
}


void Mapper1::setupPRGMapping()
{
	int PRGBankMode = ((controlReg >> 2) & 0x3);
	switch (PRGBankMode)
	{
	case 0:
	case 1:  //For both case 0 and 1:
		//Set all 32KB PRG ROM window to 32KB bank specified by PRG Bank Reg
		setPRGBank(false, ((PRGBankReg & 0x0F) >> 1) * 2);
		setPRGBank(true, (((PRGBankReg & 0x0F) >> 1) * 2) + 1);
		break;
	case 2:  
		//Set $8000-$BFFF to first 16KB bank of ROM
		setPRGBank(false, 0);

		//Set $C000-$FFFF to bank specified by PRG Bank Reg
		setPRGBank(true, (PRGBankReg & 0xF));

		break;
	case 3:
		//Set $8000-$BFFF to bank specified by PRG Bank Reg
		setPRGBank(false, (PRGBankReg & 0xF));

		//Set $C000-$FFFF to last bank of PRG ROM
		setPRGBank(true, (rom->getPRGROMSize() / 16384) - 1); 
	}
}


void Mapper1::setupCHRMapping()
{
	if (controlReg & 16)
	{
		//Two Separate 4KB CHR banks set to each of $0000 and $1000
		//based on the two internal CHR Bank registers
		setCHRBank(false, CHRBank0Reg);
		setCHRBank(true, CHRBank1Reg);
	}
	else
	{
		//Switch 8KB at a time
		int chrBank32 = (CHRBank0Reg >> 1);
		setCHRBank(false, chrBank32 * 2);
		setCHRBank(true, (chrBank32 * 2) + 1);
	}
}


void Mapper1::setPRGBank(bool upperPRGSpace, int bankNum)
{
	unsigned char* PBuffer = (upperPRGSpace ? &PRGROMBuffer[16384] : PRGROMBuffer); //Destination
	int PRGROMSize = rom->getPRGROMSize();
	int PRGROMByteIndex = (bankNum * 16384);
	for (int i = 0; i < 16384; i++)
	{
		if (PRGROMByteIndex >= PRGROMSize)
			PBuffer[i] = 0;
		else
			PBuffer[i] = rom->getPRGROMByte(PRGROMByteIndex);
		PRGROMByteIndex++;
	}
}


void Mapper1::setCHRBank(bool upperPatternTable, int bankNum)
{
	unsigned char* CBuffer = (upperPatternTable ? &CHRROMBuffer[4096] : CHRROMBuffer); //Destination
	int CHRROMSize = rom->getCHRROMSize();
	int CHRROMByteIndex = (bankNum * 4096);
	for (int i = 0; i < 4096; i++)
	{
		if (CHRROMByteIndex >= CHRROMSize)
			CBuffer[i] = 0;
		else
			CBuffer[i] = rom->getCHRROMByte(CHRROMByteIndex);
		CHRROMByteIndex++;
	}	
}

