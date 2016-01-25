/*

Copyright (c) 2015 Joshua Seagoe

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#if __AVR_ARCH__ == 1

/* __AVR_ASM_ONLY__ */
#   if !defined(__AVR_AT90S1200__)
#       define CPU_HAS_LPM 1
#   endif

#elif __AVR_ARCH__ == 2

#   define CPU_HAS_LPM 1
#   define CPU_HAS_SRAM 1

#elif __AVR_ARCH__ == 25

#   define CPU_HAS_LPM 1
#   define CPU_HAS_SRAM 1
/* __AVR_HAVE_MOVW__ */
/* __AVR_HAVE_LPMX__ */
#   define CPU_HAS_SPM   1
#   define CPU_HAS_BREAK 1

#elif __AVR_ARCH__ == 3

#   define CPU_HAS_LPM 1
#   define CPU_HAS_SRAM 1
/* __AVR_MEGA__ */
/* __AVR_HAVE_JMP_CALL__ */

#elif __AVR_ARCH__ == 31

#   define CPU_HAS_LPM 1
#   define CPU_HAS_SRAM 1
/* __AVR_MEGA__ */
/* __AVR_HAVE_ELPM__ */
/* __AVR_HAVE_JMP_CALL__ */

#elif __AVR_ARCH__ == 35

#   define CPU_HAS_LPM  1
#   define CPU_HAS_SRAM 1
/* __AVR_MEGA__ */
/* __AVR_HAVE_MOVW__ */
/* __AVR_HAVE_LPMX__ */
#   define CPU_HAS_SPM   1
#   define CPU_HAS_BREAK 1
/* __AVR_HAVE_JMP_CALL__ */

#elif __AVR_ARCH__ == 4

#   define CPU_HAS_LPM  1
#   define CPU_HAS_SRAM 1
/* __AVR_HAVE_MUL__ */
/* __AVR_HAVE_MOVW__ */
/* __AVR_HAVE_LPMX__ */
#   define CPU_HAS_SPM   1
#   define CPU_HAS_BREAK 1

#elif __AVR_ARCH__ == 5

#   define CPU_HAS_LPM  1
#   define CPU_HAS_SRAM 1
/* __AVR_MEGA__ */
/* __AVR_HAVE_MUL__ */
/* __AVR_HAVE_MOVW__ */
/* __AVR_HAVE_LPMX__ */
#   define CPU_HAS_SPM   1
#   define CPU_HAS_BREAK 1
/* __AVR_HAVE_JMP_CALL__ */

#elif __AVR_ARCH__ == 51

#   define CPU_HAS_LPM  1
#   define CPU_HAS_SRAM 1
/* __AVR_MEGA__ */
/* __AVR_HAVE_MUL__ */
/* __AVR_HAVE_MOVW__ */
/* __AVR_HAVE_LPMX__ */
#   define CPU_HAS_SPM   1
#   define CPU_HAS_BREAK 1
/* __AVR_HAVE_ELPM__ */
/* __AVR_HAVE_ELPMX__ */
/* __AVR_HAVE_JMP_CALL__ */

#elif __AVR_ARCH__ == 6

#   define CPU_HAS_LPM  1
/* __AVR_HAVE_LPMX__ */
#   define CPU_HAS_SRAM 1
/* __AVR_MEGA__ */
/* __AVR_HAVE_MUL__ */
/* __AVR_HAVE_ELPM__ */
/* __AVR_HAVE_ELPMX__ */
#   define CPU_HAS_SPM   1
#   define CPU_HAS_BREAK 1
/* __AVR_HAVE_EIJMP_EICALL__ */
/* __AVR_HAVE_MOVW__ */
/* __AVR_HAVE_JMP_CALL__ */

#elif __AVR_ARCH__ == 100

#   define CPU_HAS_BREAK 1
#   define CPU_HAS_SRAM  1
/* __AVR_TINY__ */

#elif __AVR_ARCH__ == 102

/* __AVR_HAVE_MUL__ */
/* __AVR_HAVE_JMP_CALL__ */
/* __AVR_HAVE_MOVW__ */
/* __AVR_HAVE_LPMX__ */
/* __AVR_XMEGA__ */
#   define CPU_HAS_LPM   1
#   define CPU_HAS_SRAM  1
#   define CPU_HAS_SPM   1
#   define CPU_HAS_BREAK 1
#   define CPU_HAS_SPMX  1
#   define CPU_HAS_DES   1

#elif __AVR_ARCH__ == 104

/* __AVR_HAVE_MUL__ */
/* __AVR_HAVE_JMP_CALL__ */
/* __AVR_HAVE_MOVW__ */
/* __AVR_HAVE_LPMX__ */
/* __AVR_HAVE_ELPM__ */
/* __AVR_HAVE_ELPMX__ */
/* __AVR_XMEGA__ */
#   define CPU_HAS_LPM   1
#   define CPU_HAS_SRAM  1
#   define CPU_HAS_SPM   1
#   define CPU_HAS_BREAK 1
#   define CPU_HAS_SPMX  1
#   define CPU_HAS_DES   1

#elif __AVR_ARCH__ == 105

/* __AVR_HAVE_MUL__ */
/* __AVR_HAVE_JMP_CALL__ */
/* __AVR_HAVE_MOVW__ */
/* __AVR_HAVE_LPMX__ */
/* __AVR_HAVE_ELPM__ */
/* __AVR_HAVE_ELPMX__ */
/* __AVR_XMEGA__ */
/* __AVR_HAVE_RAMPD__ */
#   define CPU_HAS_LPM   1
#   define CPU_HAS_SRAM  1
#   define CPU_HAS_SPM   1
#   define CPU_HAS_BREAK 1
#   define CPU_HAS_SPMX  1
#   define CPU_HAS_DES   1

#elif __AVR_ARCH__ == 106

/* __AVR_HAVE_MUL__ */
/* __AVR_HAVE_JMP_CALL__ */
/* __AVR_HAVE_MOVW__ */
/* __AVR_HAVE_LPMX__ */
/* __AVR_HAVE_ELPM__ */
/* __AVR_HAVE_ELPMX__ */
/* __AVR_XMEGA__ */
/* __AVR_HAVE_EIJMP_EICALL__ */
#   define CPU_HAS_LPM   1
#   define CPU_HAS_SRAM  1
#   define CPU_HAS_SPM   1
#   define CPU_HAS_BREAK 1
#   define CPU_HAS_SPMX  1
#   define CPU_HAS_DES   1

#elif __AVR_ARCH__ == 107

/* __AVR_HAVE_MUL__ */
/* __AVR_HAVE_JMP_CALL__ */
/* __AVR_HAVE_MOVW__ */
/* __AVR_HAVE_LPMX__ */
/* __AVR_HAVE_ELPM__ */
/* __AVR_HAVE_ELPMX__ */
/* __AVR_XMEGA__ */
/* __AVR_HAVE_EIJMP_EICALL__ */
/* __AVR_HAVE_RAMPD__ */
#   define CPU_HAS_LPM   1
#   define CPU_HAS_SRAM  1
#   define CPU_HAS_SPM   1
#   define CPU_HAS_BREAK 1
#   define CPU_HAS_SPMX  1
#   define CPU_HAS_DES   1

#else

#   error "Unknown CPU architecture."

#endif

// GCC doesn't support any Reduced Core microcontrollers.
#define CPU_REDUCED_CORE 0

// Convert GCC defines to stuff I can do math with.
#ifdef __AVR_3_BYTE_PC__
#   define CPU_3_BYTE_PC 1
#else
#   define CPU_3_BYTE_PC 0
#endif
#ifdef __AVR_XMEGA__
#   define CPU_XMEGA 1
#else
#   define CPU_XMEGA 0
#endif

#ifndef CPU_HAS_LPM
#   define CPU_HAS_LPM 0
#endif
#ifndef CPU_HAS_SRAM
#   define CPU_HAS_SRAM 0
#endif
#ifndef CPU_HAS_SPM
#   define CPU_HAS_SPM 0
#endif
#ifndef CPU_HAS_BREAK
#   define CPU_HAS_BREAK 0
#endif
#ifndef CPU_HAS_SPMX
#   define CPU_HAS_SPMX 0
#endif
#ifndef CPU_HAS_DES
#   define CPU_HAS_DES 0
#endif

#if CPU_HAS_SRAM
#   define CPU_HAS_IJMP  1
#   define CPU_HAS_ICALL 1
#   define CPU_HAS_PUSH  1
#   define CPU_HAS_POP   1
#   define CPU_HAS_LDS   1
#   define CPU_HAS_STS   1
#else
#   define CPU_HAS_IJMP  0
#   define CPU_HAS_ICALL 0
#   define CPU_HAS_PUSH  0
#   define CPU_HAS_POP   0
#   define CPU_HAS_LDS   0
#   define CPU_HAS_STS   0
#endif

// TODO: maybe clean up the condition here
#if CPU_HAS_SRAM && CPU_HAS_LPM
#   define CPU_HAS_ADIW 1
#   define CPU_HAS_SBIW 1
#   define CPU_HAS_LDD  1
#   define CPU_HAS_STD  1
#else
#   define CPU_HAS_ADIW 0
#   define CPU_HAS_SBIW 0
#   define CPU_HAS_LDD  0
#   define CPU_HAS_STD  0
#endif

#if __AVR_ISA_RMW__
#   define CPU_HAS_XCH 1
#   define CPU_HAS_LAS 1
#   define CPU_HAS_LAC 1
#   define CPU_HAS_LAT 1
#else
#   define CPU_HAS_XCH 0
#   define CPU_HAS_LAS 0
#   define CPU_HAS_LAC 0
#   define CPU_HAS_LAT 0
#endif

#ifdef __AVR_HAVE_JMP_CALL__
#   define CPU_HAS_JMP  1
#   define CPU_HAS_CALL 1
#else
#   define CPU_HAS_JMP  0
#   define CPU_HAS_CALL 0
#endif

#ifdef __AVR_HAVE_EIJMP_EICALL__
#   define CPU_HAS_EIJMP  1
#   define CPU_HAS_EICALL 1
#else
#   define CPU_HAS_EIJMP  0
#   define CPU_HAS_EICALL 0
#endif

#ifdef __AVR_HAVE_MUL__
#   define CPU_HAS_MUL    1
#   define CPU_HAS_MULS   1
#   define CPU_HAS_MULSU  1
#   define CPU_HAS_FMUL   1
#   define CPU_HAS_FMULS  1
#   define CPU_HAS_FMULSU 1
#else
#   define CPU_HAS_MUL    0
#   define CPU_HAS_MULS   0
#   define CPU_HAS_MULSU  0
#   define CPU_HAS_FMUL   0
#   define CPU_HAS_FMULS  0
#   define CPU_HAS_FMULSU 0
#endif

#ifdef __AVR_HAVE_LPMX__
#   define CPU_HAS_LPM_Z 1
#else
#   define CPU_HAS_LPM_Z 0
#endif
#ifdef __AVR_HAVE_ELPM__
#   define CPU_HAS_ELPM 1
#else
#   define CPU_HAS_ELPM 0
#endif
#ifdef __AVR_HAVE_ELPMX__
#   define CPU_HAS_ELPM_Z 1
#else
#   define CPU_HAS_ELPM_Z 0
#endif

// Note: do not use ternary (?:) operator here.
// This needs to work in assembly code and gas doesn't like it.

/*
 * Data Transfer
 */

#define CPU_CYCLES_MOV 1
#define CPU_CYCLES_LDI 1
#define CPU_CYCLES_IN  1
#define CPU_CYCLES_OUT 1
#if CPU_HAS_PUSH
#   define CPU_CYCLES_PUSH (2 - CPU_XMEGA)
#endif
#if CPU_HAS_POP
#   define CPU_CYCLES_POP 2
#endif
#if CPU_HAS_MOVW
#   define CPU_CYCLES_MOVW 1
#endif
#if CPU_HAS_LDS
#   define CPU_CYCLES_LDS7  1
#   define CPU_CYCLES_LDS16 2
#endif
#if CPU_HAS_STS
#   define CPU_CYCLES_STS7  1
#   define CPU_CYCLES_STS16 2
#endif
#if CPU_HAS_SRAM
#   define CPU_CYCLES_LD_X  1
#   define CPU_CYCLES_LD_Y  1
#   define CPU_CYCLES_LD_Z  1
#   define CPU_CYCLES_LD_XI (2 - CPU_XMEGA)
#   define CPU_CYCLES_LD_YI (2 - CPU_XMEGA)
#   define CPU_CYCLES_LD_ZI (2 - CPU_XMEGA)
#   define CPU_CYCLES_LD_DX (3 - CPU_XMEGA)
#   define CPU_CYCLES_LD_DY (3 - CPU_XMEGA)
#   define CPU_CYCLES_LD_DZ (3 - CPU_XMEGA)
#endif
#if CPU_HAS_SRAM
#   define CPU_CYCLES_ST_X  (2 - CPU_XMEGA - CPU_REDUCED_CORE)
#   define CPU_CYCLES_ST_Y  (2 - CPU_XMEGA - CPU_REDUCED_CORE)
#   define CPU_CYCLES_ST_Z  (2 - CPU_XMEGA - CPU_REDUCED_CORE)
#   define CPU_CYCLES_ST_XI (2 - CPU_XMEGA - CPU_REDUCED_CORE)
#   define CPU_CYCLES_ST_YI (2 - CPU_XMEGA - CPU_REDUCED_CORE)
#   define CPU_CYCLES_ST_ZI (2 - CPU_XMEGA - CPU_REDUCED_CORE)
#   define CPU_CYCLES_ST_DX 2
#   define CPU_CYCLES_ST_DY 2
#   define CPU_CYCLES_ST_DZ 2
#endif
#if CPU_HAS_LDD
#   define CPU_CYCLES_LDD_Y 2
#   define CPU_CYCLES_LDD_Z 2
#endif
#if CPU_HAS_STD
#   define CPU_CYCLES_STD_Y 2
#   define CPU_CYCLES_STD_Z 2
#endif
#if CPU_HAS_LPM
#   define CPU_CYCLES_LPM    3
#endif
#if CPU_HAS_LPM_Z
#   define CPU_CYCLES_LPM_Z  3
#   define CPU_CYCLES_LPM_ZI 3
#endif
#if CPU_HAS_ELPM
#   define CPU_CYCLES_ELPM    3
#endif
#if CPU_HAS_ELPM_Z
#   define CPU_CYCLES_ELPM_Z  3
#   define CPU_CYCLES_ELPM_ZI 3
#endif

#if CPU_HAS_XCH
#   define CPU_CYCLES_XCH 1
#endif
#if CPU_HAS_LAS
#   define CPU_CYCLES_LAS 1
#endif
#if CPU_HAS_LAC
#   define CPU_CYCLES_LAC 1
#endif
#if CPU_HAS_LAT
#   define CPU_CYCLES_LAT 1
#endif

/*
 * Arithmetic and Logic
 */

#define CPU_CYCLES_ADC  1
#define CPU_CYCLES_ADD  1
#define CPU_CYCLES_SUB  1
#define CPU_CYCLES_SUBI 1
#define CPU_CYCLES_SBC  1
#define CPU_CYCLES_SBCI 1
#define CPU_CYCLES_AND  1
#define CPU_CYCLES_ANDI 1
#define CPU_CYCLES_OR   1
#define CPU_CYCLES_ORI  1
#define CPU_CYCLES_EOR  1
#define CPU_CYCLES_COM  1
#define CPU_CYCLES_NEG  1
#define CPU_CYCLES_SBR  CPU_CYCLES_ORI
#define CPU_CYCLES_CBR  CPU_CYCLES_ANDI
#define CPU_CYCLES_INC  1
#define CPU_CYCLES_DEC  1
#define CPU_CYCLES_TST  CPU_CYCLES_AND
#define CPU_CYCLES_CLR  CPU_CYCLES_EOR
#define CPU_CYCLES_SER  CPU_CYCLES_LDI
#if CPU_HAS_ADIW
#   define CPU_CYCLES_ADIW 2
#endif
#if CPU_HAS_SBIW
#   define CPU_CYCLES_SBIW 2
#endif
#if CPU_HAS_MUL
#   define CPU_CYCLES_MUL    2
#endif
#if CPU_HAS_MULS
#   define CPU_CYCLES_MULS   2
#endif
#if CPU_HAS_MULSU
#   define CPU_CYCLES_MULSU  2
#endif
#if CPU_HAS_FMUL
#   define CPU_CYCLES_FMUL   2
#endif
#if CPU_HAS_FMULS
#   define CPU_CYCLES_FMULS  2
#endif
#if CPU_HAS_FMULSU
#   define CPU_CYCLES_FMULSU 2
#endif
#if CPU_HAS_DES
#   define CPU_CYCLES_DES 1 /* +1 if prev inst is not DES */
#endif

/*
 * Bit and Bit-test
 */

#define CPU_CYCLES_ASR 1
#define CPU_CYCLES_LSL CPU_CYCLES_ADD
#define CPU_CYCLES_LSR 1
#define CPU_CYCLES_ROL CPU_CYCLES_ADC
#define CPU_CYCLES_ROR 1
#define CPU_CYCLES_SWAP 1
#define CPU_CYCLES_BCLR 1
#define CPU_CYCLES_CLC CPU_CYCLES_BCLR
#define CPU_CYCLES_CLH CPU_CYCLES_BCLR
#define CPU_CYCLES_CLI CPU_CYCLES_BCLR
#define CPU_CYCLES_CLN CPU_CYCLES_BCLR
#define CPU_CYCLES_CLS CPU_CYCLES_BCLR
#define CPU_CYCLES_CLT CPU_CYCLES_BCLR
#define CPU_CYCLES_CLV CPU_CYCLES_BCLR
#define CPU_CYCLES_CLZ CPU_CYCLES_BCLR
#define CPU_CYCLES_BSET 1
#define CPU_CYCLES_SEC CPU_CYCLES_BSET
#define CPU_CYCLES_SEH CPU_CYCLES_BSET
#define CPU_CYCLES_SEI CPU_CYCLES_BSET
#define CPU_CYCLES_SEN CPU_CYCLES_BSET
#define CPU_CYCLES_SES CPU_CYCLES_BSET
#define CPU_CYCLES_SET CPU_CYCLES_BSET
#define CPU_CYCLES_SEV CPU_CYCLES_BSET
#define CPU_CYCLES_SEZ CPU_CYCLES_BSET
#define CPU_CYCLES_CBI (2 - CPU_XMEGA - CPU_REDUCED_CORE)
#define CPU_CYCLES_SBI (2 - CPU_XMEGA - CPU_REDUCED_CORE)

/*
 * Branch
 */

#define CPU_CYCLES_RJMP  2
#define CPU_CYCLES_RCALL (3 + CPU_3_BYTE_PC + CPU_REDUCED_CORE - CPU_XMEGA)
#define CPU_CYCLES_RET   (4 + CPU_3_BYTE_PC)
#define CPU_CYCLES_RETI  (4 + CPU_3_BYTE_PC)
#if CPU_HAS_IJMP
#   define CPU_CYCLES_IJMP 2
#endif
#if CPU_HAS_ICALL
#   define CPU_CYCLES_ICALL (3 + CPU_3_BYTE_PC - CPU_XMEGA)
#endif
#if CPU_HAS_JMP
#   define CPU_CYCLES_JMP 3
#endif
#if CPU_HAS_CALL
#   define CPU_CYCLES_CALL (4 + CPU_3_BYTE_PC - CPU_XMEGA)
#endif
#if CPU_HAS_EIJMP
#   define CPU_CYCLES_EIJMP 2
#endif
#if CPU_HAS_EICALL
#   define CPU_CYCLES_EICALL (4 - CPU_XMEGA)
#endif

#define CPU_CYCLES_CPSE 1 /* + length of next instruction if taken */
#define CPU_CYCLES_CP   1
#define CPU_CYCLES_CPC  1
#define CPU_CYCLES_CPI  1
#define CPU_CYCLES_SBRC 1 /* + length of next instruction if taken */
#define CPU_CYCLES_SBRS 1 /* + length of next instruction if taken */
#define CPU_CYCLES_SBIC (1 + CPU_XMEGA) /* + length of next instruction if taken */
#define CPU_CYCLES_SBIS (1 + CPU_XMEGA) /* + length of next instruction if taken */
#define CPU_CYCLES_BRBC 1 /* +1 if taken */
#define CPU_CYCLES_BRBS 1 /* +1 if taken */
#define CPU_CYCLES_BRCC 1 /* +1 if taken */
#define CPU_CYCLES_BRCS 1 /* +1 if taken */
#define CPU_CYCLES_BREQ 1 /* +1 if taken */
#define CPU_CYCLES_BRGE 1 /* +1 if taken */
#define CPU_CYCLES_BRHC 1 /* +1 if taken */
#define CPU_CYCLES_BRHS 1 /* +1 if taken */
#define CPU_CYCLES_BRID 1 /* +1 if taken */
#define CPU_CYCLES_BRIE 1 /* +1 if taken */
#define CPU_CYCLES_BRLO 1 /* +1 if taken */
#define CPU_CYCLES_BRLT 1 /* +1 if taken */
#define CPU_CYCLES_BRMI 1 /* +1 if taken */
#define CPU_CYCLES_BRNE 1 /* +1 if taken */
#define CPU_CYCLES_BRPL 1 /* +1 if taken */
#define CPU_CYCLES_BRSH 1 /* +1 if taken */
#define CPU_CYCLES_BRTC 1 /* +1 if taken */
#define CPU_CYCLES_BRTS 1 /* +1 if taken */
#define CPU_CYCLES_BRVC 1 /* +1 if taken */
#define CPU_CYCLES_BRVS 1 /* +1 if taken */

/*
 * Other
 */

#if CPU_HAS_BREAK
#   define CPU_CYCLES_BREAK 1
#endif
#define CPU_CYCLES_NOP 1
#define CPU_CYCLES_SLEEP 1
#define CPU_CYCLES_WDR 1
 
#endif /* CPU_H_INCLUDED */
