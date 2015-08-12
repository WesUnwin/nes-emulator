#ifndef CPU_OPCODES_H
#define CPU_OPCODES_H

#include "../globals.h"

#include "Operations_A_to_B.h"
#include "Operations_C_to_I.h"
#include "Operations_J_to_P.h"
#include "Operations_R_to_T.h"
#include "Operations_Undoc_A_to_L.h"
#include "Operations_Undoc_M_to_Z.h"
#include "AddressModes.h"

namespace CPUCore
{
	class CPUState;

	struct OPCODE_INFO
	{
		void(*operationRoutine)(CPUState* c);
		unsigned short(*addressModeRoutine)(CPUState*);
		int CpuClocks;
		char* assemblySyntax;
	};

		
	const OPCODE_INFO opcodes[256] =
	{
		{ Ops::OP_BRK,			NULL,                        7, "BRK"              },  //00
		{ Ops::OP_ORA,			AddressModes::AM_INDIRECT_X, 2, "ORA [$aa,X]"      },  //01
		{ Ops::OP_KIL,			NULL,                        2, "(Invalid OpCode)" },  //02 Undoc
		{ Ops::OP_SLO,			AddressModes::AM_INDIRECT_X, 2, "SLO [$aa,X]"      },  //03 Undoc
		{ Ops::OP_NOP_AMODE,	AddressModes::AM_ZEROPAGE,   3, "NOP"              },  //04	Undoc
		{ Ops::OP_ORA,			AddressModes::AM_ZEROPAGE,   3, "ORA"              },  //05
		{ Ops::OP_ASL,			AddressModes::AM_ZEROPAGE,   3, "ORA"              },  //06
		{ Ops::OP_SLO,			AddressModes::AM_ZEROPAGE,   2, "SLO <$aa"         },  //07 Undoc
		{ Ops::OP_PHP,			NULL,                        2, "PHP"              },  //08
		{ Ops::OP_ORA,			AddressModes::AM_IMMEDIATE,  2, "ORA #$aa"         },  //09
		{ Ops::OP_ASL_A,		NULL,                        2, "ASL A"            },  //0A
		{ Ops::OP_INVALID,		NULL,                        2, ""                 },  //0B
		{ Ops::OP_NOP_AMODE,	AddressModes::AM_ABSOLUTE,   4, "NOP"              },  //0C	Undoc
		{ Ops::OP_ORA,			AddressModes::AM_ABSOLUTE,   4, "ORA $bbaa"        },  //0D
		{ Ops::OP_ASL,			AddressModes::AM_ABSOLUTE,   4, "ASL $bbaa"        },  //0E
		{ Ops::OP_INVALID,		NULL,                        2, ""                 },  //0F
		{ Ops::OP_BPL,			NULL,                        2, ""                 },  //10
		{ Ops::OP_ORA,			AddressModes::AM_INDIRECT_Y, 5, "ORA(<$aa), Y"     },  //11
		{ Ops::OP_INVALID,		NULL,                        2, ""                 },  //12
		{ Ops::OP_INVALID,		NULL,                        2, ""                 },  //13
		{ Ops::OP_INVALID,		NULL,                        2, ""                 },  //14
		{ Ops::OP_ORA,			AddressModes::AM_ZEROPAGE_X, 4, "ORA <$aa, X"      },  //15
		{ Ops::OP_ASL,			AddressModes::AM_ZEROPAGE_X, 4, "ASL <$aa, X"      },  //16
		{ Ops::OP_INVALID,		NULL,                        2, ""                 },  //17
		{ Ops::OP_CLC,			NULL,                        2, "CLC"              },  //18
		{ Ops::OP_ORA,			AddressModes::AM_ABSOLUTE_Y, 4, "ORA $bbaa, Y"     },  //19
		{ Ops::OP_INVALID,		NULL,                        2, ""                 },  //1A
		{ Ops::OP_INVALID,		NULL,                        2, ""                 },  //1B
		{ Ops::OP_INVALID,		NULL,                        2, ""                 },  //1C
		{ Ops::OP_ORA,			AddressModes::AM_ABSOLUTE_X, 4, "ORA $bbaa, X"     },  //1D
		{ Ops::OP_ASL,			AddressModes::AM_ABSOLUTE_X, 4, "ASL $bbaa, X"     },  //1E
		{ Ops::OP_INVALID,		NULL,                        2, ""                 },  //1F
		{ Ops::OP_JSR,			AddressModes::AM_ABSOLUTE,   6, "JSR $bbaa"        },  //20  
		{ Ops::OP_AND,			AddressModes::AM_INDIRECT_X, 6, "AND(<$aa, X)"     },  //21
		{ Ops::OP_INVALID,		NULL,                        2, ""                 },  //22
		{ Ops::OP_INVALID,		NULL,                        2, ""                 },  //23
		{ Ops::OP_BIT,			AddressModes::AM_ZEROPAGE,   3, "BIT <$aa"         },  //24
		{ Ops::OP_AND,			AddressModes::AM_ZEROPAGE,   3, "AND <$aa"         },  //25
		{ Ops::OP_ROL,			AddressModes::AM_ZEROPAGE,   3, "ROL <$aa"         },  //26
		{ Ops::OP_INVALID,		NULL,                        2, ""                 },  //27
		{ Ops::OP_PLP,			NULL,                        2, "PLP"              },  //28
		{ Ops::OP_AND,			AddressModes::AM_IMMEDIATE,  2, "AND #$aa"         },  //29
		{ Ops::OP_ROL_A,		NULL,                        2, "ROL A"            },  //2A
		{ Ops::OP_INVALID,		NULL,                        2, ""                 },  //2B
		{ Ops::OP_BIT,			AddressModes::AM_ABSOLUTE,   4, "BIT $bbaa"        },  //2C
		{ Ops::OP_AND,			AddressModes::AM_ABSOLUTE,   4, "AND $bbaa"        },  //2D
		{ Ops::OP_ROL,			AddressModes::AM_ABSOLUTE,   4, "ROL $bbaa"        },  //2E
		{ Ops::OP_INVALID,		NULL,                        2, ""                 },  //2F
		{ Ops::OP_BMI,			NULL,                        2, "BMI"              },  //30
		{ Ops::OP_AND,			AddressModes::AM_INDIRECT_Y, 5, "AND(<$aa), Y"     },  //31
		{ Ops::OP_INVALID,		NULL,                        2, ""                 },  //32
		{ Ops::OP_INVALID,		NULL,                        2, ""                 },  //33
		{ Ops::OP_INVALID,		NULL,                        2, ""                 },  //34
		{ Ops::OP_AND,			AddressModes::AM_ZEROPAGE_X, 4, "AND <$aa, X"      },  //35
		{ Ops::OP_ROL,			AddressModes::AM_ZEROPAGE_X, 4, "ROL <$aa, X"      },  //36
		{ Ops::OP_INVALID,		NULL,                        2, ""                 },  //37
		{ Ops::OP_SEC,			NULL,                        2, ""                 },  //38
		{ Ops::OP_AND,			AddressModes::AM_ABSOLUTE_Y, 4, "AND $bbaa, Y"     },  //39
		{ Ops::OP_INVALID,		NULL,                        2, ""                 },  //3A
		{ Ops::OP_INVALID,		NULL,                        2, ""                 },  //3B
		{ Ops::OP_INVALID,		NULL,                        2, ""                 },  //3C
		{ Ops::OP_AND,			AddressModes::AM_ABSOLUTE_X, 4, "AND $bbaa, X"     },  //3D
		{ Ops::OP_ROL,			AddressModes::AM_ABSOLUTE_X, 4, "ROL $bbaa, X"     },  //3E
		{ Ops::OP_INVALID,		NULL,                        2, ""                 },  //3F
		{ Ops::OP_RTI,			NULL,                        2, "RTI"              },  //40
		{ Ops::OP_EOR,			AddressModes::AM_INDIRECT_X, 6, "EOR (<$aa, X)"    },  //41
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //42
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //43
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //44
		{ Ops::OP_EOR,			AddressModes::AM_ZEROPAGE,   3, "EOR <$aa"         },  //45
		{ Ops::OP_LSR,			AddressModes::AM_ZEROPAGE,   3, "LSR <$aa"         },  //46
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //47
		{ Ops::OP_PHA,			NULL,                        2, "PHA"              },  //48
		{ Ops::OP_EOR,			AddressModes::AM_IMMEDIATE,  2, "EOR #$aa"         },  //49
		{ Ops::OP_LSR_A,		NULL,                        2, "LSR A"            },  //4A
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //4B
		{ Ops::OP_JMP,			AddressModes::AM_ABSOLUTE,   3, "JMP $bbaa"        },  //4C
		{ Ops::OP_EOR,			AddressModes::AM_ABSOLUTE,   4, "EOR $bbaa"        },  //4D
		{ Ops::OP_LSR,			AddressModes::AM_ABSOLUTE,   4, "LSR $bbaa"        },  //4E
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //4F
		{ Ops::OP_BVC,			NULL,                        2, "BVC"              },  //50
		{ Ops::OP_EOR,			AddressModes::AM_INDIRECT_Y, 5, "EOR (<$aa), Y"    },  //51
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //52
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //53
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //54
		{ Ops::OP_EOR,			AddressModes::AM_ZEROPAGE_X, 4, "EOR <$aa, X"      },  //55
		{ Ops::OP_LSR,			AddressModes::AM_ZEROPAGE_X, 4, "LSR <$aa, X"      },  //56
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //57
		{ Ops::OP_CLI,			NULL,                        2, "CLI"              },  //58
		{ Ops::OP_EOR,			AddressModes::AM_ABSOLUTE_Y, 4, "EOR $bbaa, Y"     },  //59
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //5A
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //5B
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //5C
		{ Ops::OP_EOR,			AddressModes::AM_ABSOLUTE_X, 4, "EOR $bbaa, X"     },  //5D
		{ Ops::OP_LSR,			AddressModes::AM_ABSOLUTE_X, 4, "LSR $bbaa, X"     },  //5E
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //5F
		{ Ops::OP_RTS,			NULL,                        2, "RTS"              },  //60
		{ Ops::OP_ADC,			AddressModes::AM_INDIRECT_X, 6, "ADC (<$aa, X)"    },  //61
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //62
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //63
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //64
		{ Ops::OP_ADC,			AddressModes::AM_ZEROPAGE,   3, "ADC <$aa"         },  //65
		{ Ops::OP_ROR,			AddressModes::AM_ZEROPAGE,   3, "ROR <$aa"         },  //66
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //67
		{ Ops::OP_PLA,			NULL,                        2, "PLA"              },  //68
		{ Ops::OP_ADC,			AddressModes::AM_IMMEDIATE,  2, "ADC #$aa"         },  //69
		{ Ops::OP_ROR_A,		NULL,                        2, "ROR A"            },  //6A
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //6B
		{ Ops::OP_JMP,			AddressModes::AM_INDIRECT_ABSOLUTE, 5, "JMP ($bbaa)"},  //6C
		{ Ops::OP_ADC,			AddressModes::AM_ABSOLUTE,   4, "ADC $bbaa"        },  //6D
		{ Ops::OP_ROR,			AddressModes::AM_ABSOLUTE,   4, "ROR $bbaa"        },  //6E
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //6F
		{ Ops::OP_BVS,			NULL,                        2, "BVS"              },  //70
		{ Ops::OP_ADC,			AddressModes::AM_INDIRECT_Y, 5, "ADC (<$aa), Y"    },  //71
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //72
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //73
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //74
		{ Ops::OP_ADC,			AddressModes::AM_ZEROPAGE_X, 4, "ADC <$aa, X"      },  //75
		{ Ops::OP_ROR,			AddressModes::AM_ZEROPAGE_X, 4, "ROR <$aa, X"      },  //76
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //77
		{ Ops::OP_SEI,			NULL,                        2, "SEI"              },  //78
		{ Ops::OP_ADC,			AddressModes::AM_ABSOLUTE_Y, 4, "ADC $bbaa, Y"     },  //79
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //7A
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //7B
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //7C
		{ Ops::OP_ADC,			AddressModes::AM_ABSOLUTE_X, 4, "ADC $bbaa, X"     },  //7D
		{ Ops::OP_ROR,			AddressModes::AM_ABSOLUTE_X, 4, "ROR $bbaa, X"     },  //7E
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //7F
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //80
		{ Ops::OP_STA,			AddressModes::AM_INDIRECT_X, 6, "STA (<$aa, X)"    },  //81
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //82
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //83
		{ Ops::OP_STY,			AddressModes::AM_ZEROPAGE,   3, "STY <$aa"         },  //84
		{ Ops::OP_STA,			AddressModes::AM_ZEROPAGE,   3, "STA <$aa"         },  //85
		{ Ops::OP_STX,			AddressModes::AM_ZEROPAGE,   3, "STX <4aa"         },  //86
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //87
		{ Ops::OP_DEY,			NULL,                        2, "DEY"              },  //88
		{ Ops::OP_NOP_AMODE,	AddressModes::AM_IMMEDIATE,  2, "NOP"              },  //89	Illegal
		{ Ops::OP_TXA,			NULL,                        2, "TXA"              },  //8A
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //8B
		{ Ops::OP_STY,			AddressModes::AM_ABSOLUTE,   4, "STY $bbaa"        },  //8C
		{ Ops::OP_STA,			AddressModes::AM_ABSOLUTE,   4, "STA $bbaa"        },  //8D
		{ Ops::OP_STX,			AddressModes::AM_ABSOLUTE,   4, "STX $bbaa"        },  //8E
		{ Ops::OP_STA,			AddressModes::AM_ABSOLUTE,   4, "---"              },  //8F
		{ Ops::OP_BCC,			NULL,                        2, "BCC"              },  //90
		{ Ops::OP_STA,			AddressModes::AM_INDIRECT_Y, 5, "STA (<$aa), Y"    },  //91
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //92
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //93
		{ Ops::OP_STY,			AddressModes::AM_ZEROPAGE_X, 4, "STY <$aa, X"      },  //94
		{ Ops::OP_STA,			AddressModes::AM_ZEROPAGE_X, 4, "STA <$aa, X"      },  //95
		{ Ops::OP_STX,			AddressModes::AM_ZEROPAGE_Y, 4, "STX <$aa, Y"      },  //96
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //97
		{ Ops::OP_TYA,			NULL,                        2, "TYA"              },  //98
		{ Ops::OP_STA,			AddressModes::AM_ABSOLUTE_Y, 4, "STA $bbaa, Y"     },  //99
		{ Ops::OP_TXS,			NULL,                        2, "TXS"              },  //9A
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //9B
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //9C
		{ Ops::OP_STA,			AddressModes::AM_ABSOLUTE_X, 4, "STA $bbaa, X"     },  //9D
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //9E
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //9F
		{ Ops::OP_LDY,			AddressModes::AM_IMMEDIATE,  2, "LDY #$aa"         },  //A0
		{ Ops::OP_LDA,			AddressModes::AM_INDIRECT_X, 6, "LDA [$aa,X]"      },  //A1   //PROPER SYNTAX FOR NESASM
		{ Ops::OP_LDX,			AddressModes::AM_IMMEDIATE,  2, "LDX #$aa"         },  //A2
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //A3
		{ Ops::OP_LDY,			AddressModes::AM_ZEROPAGE,   3, "LDY <$aa"         },  //A4
		{ Ops::OP_LDA,			AddressModes::AM_ZEROPAGE,   3, "LDA <$aa"         },  //A5
		{ Ops::OP_LDX,			AddressModes::AM_ZEROPAGE,   3, "LDX <$aa"         },  //A6
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //A7
		{ Ops::OP_TAY,			NULL,                        2, "TAY"              },  //A8
		{ Ops::OP_LDA,			AddressModes::AM_IMMEDIATE,  2, "LDA #aa"          },  //A9
		{ Ops::OP_TAX,			NULL,                        2, "TAX"              },  //AA
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //AB
		{ Ops::OP_LDY,			AddressModes::AM_ABSOLUTE,   4, "LDY $bbaa"        },  //AC
		{ Ops::OP_LDA,			AddressModes::AM_ABSOLUTE,   4, "LDA $bbaa"        },  //AD
		{ Ops::OP_LDX,			AddressModes::AM_ABSOLUTE,   4, "LDX $bbaa"        },  //AE
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //AF
		{ Ops::OP_BCS,			NULL,                        2, "BCS"              },  //B0
		{ Ops::OP_LDA,			AddressModes::AM_INDIRECT_Y, 5, "LDA (<$aa), y"    },  //B1
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //B2
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //B3
		{ Ops::OP_LDY,			AddressModes::AM_ZEROPAGE_X, 4, "LDY <4aa, X"      },  //B4
		{ Ops::OP_LDA,			AddressModes::AM_ZEROPAGE_X, 4, "LDA <$aa, X"      },  //B5
		{ Ops::OP_LDX,			AddressModes::AM_ZEROPAGE_Y, 4, "LDX <$aa, Y"      },  //B6
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //B7
		{ Ops::OP_CLV,			NULL,                        2, "CLV"              },  //B8
		{ Ops::OP_LDA,			AddressModes::AM_ABSOLUTE_Y, 4, "LDA $bbaa, y"     },  //B9
		{ Ops::OP_TSX,			NULL,                        2, "TSX"              },  //BA
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //BB
		{ Ops::OP_LDY,			AddressModes::AM_ABSOLUTE_X, 4, "LDY $bbaa, X"     },  //BC
		{ Ops::OP_LDA,			AddressModes::AM_ABSOLUTE_X, 4, "LDA $bbaa, X"     },  //BD
		{ Ops::OP_LDX,			AddressModes::AM_ABSOLUTE_Y, 4, "LDX $bbaa, Y"     },  //BE
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //BF
		{ Ops::OP_CPY,			AddressModes::AM_IMMEDIATE,  2, "CPY #$aa"         },  //C0
		{ Ops::OP_CMP,			AddressModes::AM_INDIRECT_X, 6, "CMP (<$aa, X)"    },  //C1
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //C2
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //C3
		{ Ops::OP_CPY,			AddressModes::AM_ZEROPAGE,   3, "CPY <$aa"         },  //C4
		{ Ops::OP_CMP,			AddressModes::AM_ZEROPAGE,   3, "CMP <$aa"         },  //C5
		{ Ops::OP_DEC,			AddressModes::AM_ZEROPAGE,   3, "DEC <$aa"         },  //C6
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //C7
		{ Ops::OP_INY,			NULL,                        2, "INY"              },  //C8
		{ Ops::OP_CMP,			AddressModes::AM_IMMEDIATE,  2, "CMP #$aa"         },  //C9
		{ Ops::OP_DEX,			NULL,                        2, "DEX"              },  //CA
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //CB
		{ Ops::OP_CPY,			AddressModes::AM_ABSOLUTE,   4, "CPY $bbaa"        },  //CC
		{ Ops::OP_CMP,			AddressModes::AM_ABSOLUTE,   4, "CMP $bbaa"        },  //CD
		{ Ops::OP_DEC,			AddressModes::AM_ABSOLUTE,   4, "DEC $bbaa"        },  //CE
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //CF
		{ Ops::OP_BNE,			NULL,                        2, "BNE"              },  //D0
		{ Ops::OP_CMP,			AddressModes::AM_INDIRECT_Y, 5, "CMP (<$aa), Y"    },  //D1
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //D2
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //D3
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //D4
		{ Ops::OP_CMP,			AddressModes::AM_ZEROPAGE_X, 4, "CMP <$aa, X"      },  //D5
		{ Ops::OP_DEC,			AddressModes::AM_ZEROPAGE_X, 4, "DEC <$aa, X"      },  //D6
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //D7
		{ Ops::OP_CLD,			NULL,                        2, "CLD"              },  //D8
		{ Ops::OP_CMP,			AddressModes::AM_ABSOLUTE_Y, 4, "CMP $bbaa, Y"     },  //D9
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //DA
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //DB
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //DC
		{ Ops::OP_CMP,			AddressModes::AM_ABSOLUTE_X, 4, "CMP $bbaa, X"     },  //DD
		{ Ops::OP_DEC,			AddressModes::AM_ABSOLUTE_X, 4, "DEC $bbaa, X"     },  //DE
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //DF
		{ Ops::OP_CPX,			AddressModes::AM_IMMEDIATE,  2, "CPX #$aa"         },  //E0
		{ Ops::OP_SBC,			AddressModes::AM_INDIRECT_X, 6, "SBC (<$aa, X)"    },  //E1
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //E2
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //E3
		{ Ops::OP_CPX,			AddressModes::AM_ZEROPAGE,   3, "CPX <$aa"         },  //E4
		{ Ops::OP_SBC,			AddressModes::AM_ZEROPAGE,   3, "SBC <$aa"         },  //E5
		{ Ops::OP_INC,			AddressModes::AM_ZEROPAGE,   3, "INC <$aa"         },  //E6
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //E7
		{ Ops::OP_INX,			NULL,                        2, "INX"              },  //E8
		{ Ops::OP_SBC,			AddressModes::AM_IMMEDIATE,  2, "SBC #$aa"         },  //E9
		{ Ops::OP_NOP,			NULL,                        2, "NOP"              },  //EA
		{ Ops::OP_SBC,			AddressModes::AM_IMMEDIATE,  2, "SBC #$aa"         },  //EB   Illegal
		{ Ops::OP_CPX,			AddressModes::AM_ABSOLUTE,   4, "CPX $bbaa"        },  //EC
		{ Ops::OP_SBC,			AddressModes::AM_ABSOLUTE,   4, "SBC $bbaa"        },  //ED
		{ Ops::OP_INC,			AddressModes::AM_ABSOLUTE,   4, "INC $bbaa"        },  //EE
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //EF
		{ Ops::OP_BEQ,			NULL,                        2, "BEQ"              },  //F0
		{ Ops::OP_SBC,			AddressModes::AM_INDIRECT_Y, 5, "SBC (<$aa), Y"    },  //F1
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //F2
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //F3
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //F4
		{ Ops::OP_SBC,			AddressModes::AM_ZEROPAGE_X, 4, "SBC <$aa, X"      },  //F5
		{ Ops::OP_INC,			AddressModes::AM_ZEROPAGE_X, 4, "INC <$aa, X"      },  //F6
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //F7
		{ Ops::OP_SED,			NULL,                        2, "SED"              },  //F8
		{ Ops::OP_SBC,			AddressModes::AM_ABSOLUTE_Y, 4, "SBC $bbaa, Y"     },  //F9
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //FA
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //FB
		{ Ops::OP_INVALID,		NULL,                        2, "---"              },  //FC
		{ Ops::OP_SBC,			AddressModes::AM_ABSOLUTE_X, 4, "SBC $bbaa, X"     },  //FD
		{ Ops::OP_INC,			AddressModes::AM_ABSOLUTE_X, 4, "INC $bbaa, X"     },  //FE
		{ Ops::OP_INVALID,		NULL,                        2, "---"              }   //FF

	};
	
}




#endif
