	.include "constants/gba_constants.inc"
	.include "constants/misc_constants.inc"
	.include "constants/version.inc"

	.syntax unified

	.global Start

	.text

	.arm

_start: @ 8000000
	b start_vector

	.include "asm/rom_header.inc"

@ 80000C0
	.word 0

	.global GPIOPortData
GPIOPortData: @ 80000C4
	.hword 0

	.global GPIOPortDirection
GPIOPortDirection: @ 80000C6
	.hword 0

	.global GPIOPortReadEnable
GPIOPortReadEnable: @ 80000C8
	.hword 0

	.space 6

@ 80000D0

	.4byte 0xFFFFFFFF
	.4byte 0xFFFFFFFF
	.4byte 0xFFFFFFFF
	.4byte 0xFFFFFFFF
	.4byte 0xFFFFFFFF
	.4byte 0xFFFFFFFF
	.4byte 0xFFFFFFFF
	.4byte 0xFFFFFFFF
	.4byte 0xFFFFFFFF
	.4byte 0xFFFFFFFF
	.4byte 0xFFFFFFFF
	.4byte 0xFFFFFFFF

@ 8000100
	.global CartIdent
CartIdent:
	.4byte GAME_VERSION
	.4byte GAME_LANGUAGE
.game_name:
	.ifdef FIRERED
	.ascii "pokemon red version"
	.else
	.ifdef LEAFGREEN
	.ascii "pokemon green version"
	.endif
	.endif
	.space .game_name+0x20-.
	.4byte  gMonFrontPicTable
	.4byte  gMonBackPicTable
	.4byte  gMonPaletteTable
	.4byte  gMonShinyPaletteTable
	.4byte  gMonIconTable
	.4byte  gMonIconPaletteIndices
	.4byte  gMonIconPaletteTable
	.4byte  gSpeciesNames
	.4byte  gMoveNames
	.4byte  gDecorations
	.4byte      0xEE0 @ offsetof(struct SaveBlock1, flags)
	.4byte     0x1000 @ offsetof(struct SaveBlock1, vars)
	.4byte       0x18 @ offsetof(struct SaveBlock2, pokedex)
	.4byte      0x5F8 @ offsetof(struct SaveBlock1, seen1)
	.4byte     0x3A18 @ offsetof(struct SaveBlock1, seen2)
	.4byte       0x3C
	.4byte      0x838
	.4byte      0x839
	.4byte      0x182
	.4byte  0xA0A0A07
	.4byte  0xC060C0C
	.4byte  0xC121006
	.4byte  0x8010B0F
	.4byte        0xC
	.4byte      0xF24 @ sizeof(struct SaveBlock2)
	.4byte     0x3D68 @ sizeof(struct SaveBlock1)
	.4byte       0x34 @ offsetof(struct SaveBlock1, playerPartyCount)
	.4byte       0x38 @ offsetof(struct SaveBlock1, playerParty)
	.4byte          9 @ offsetof(struct SaveBlock2, specialSaveWarp)
	.4byte        0xA @ offsetof(struct SaveBlock2, playerTrainerId)
	.4byte          0 @ offsetof(struct SaveBlock2, playerName)
	.4byte          8 @ offsetof(struct SaveBlock2, playerGender)

	.4byte       0xAD @ offsetof(struct SaveBlock2, ?????? (0xAD))
	.4byte       0xAD @ offsetof(struct SaveBlock2, ?????? (0xAD))
	.4byte     0x30BB
	.4byte     0x30A7
	.4byte          0
	.4byte  gBaseStats
	.4byte  gAbilityNames
	.4byte  gAbilityDescriptionPointers
	.4byte  gItems
	.4byte  gBattleMoves
	.4byte  gBallSpriteSheets
	.4byte  gBallSpritePalettes
	.4byte       0xA8
	.4byte      0x82C
	.4byte      0x83B
	.4byte 0x3A0D1E2A
	.4byte     0x1E2B
	.4byte      0x298 @ offsetof(struct SaveBlock1, pcItems)  // maybe all items were in a struct together?
	.4byte     0x309C @ offsetof(struct SaveBlock1, giftRibbons)
	.4byte     0x30EC @ offsetof(struct SaveBlock1, enigmaBerry)
	.4byte       0x34 @ size of SaveBlock1 map header reconstruction data?
	.4byte          0
	.4byte 0xFFFFFFFF

	.arm
	.align 2, 0
	.global start_vector
start_vector:
	mov r0, #PSR_IRQ_MODE
	msr cpsr_cf, r0
	ldr sp, sp_irq
	mov r0, #PSR_SYS_MODE
	msr cpsr_cf, r0
	ldr sp, sp_usr
	ldr r1, =INTR_VECTOR
	adr r0, intr_main
	str r0, [r1]
	ldr r1, =AgbMain
	mov lr, pc
	bx r1
	b start_vector

	.align 2, 0
sp_usr: .word IWRAM_END - 0x1C0
sp_irq: .word IWRAM_END - 0x60

	.pool

	.arm
	.align 2, 0
	.global intr_main
intr_main:
	mov r3, #REG_BASE
	add r3, r3, #OFFSET_REG_IE
	ldr r2, [r3]
	ldrh r1, [r3, #REG_IME - REG_IE]
	mrs r0, spsr
	stmdb sp!, {r0-r3,lr}
	mov r0, #0
	strh r0, [r3, #REG_IME - REG_IE]
	and r1, r2, r2, lsr #16
	mov r12, #0
	ands r0, r1, #INTR_FLAG_VCOUNT
	bne jump_intr
	add r12, r12, 0x4
	mov r0, 0x1
	strh r0, [r3, #REG_IME - REG_IE]
	ands r0, r1, #INTR_FLAG_SERIAL
	bne jump_intr
	add r12, r12, 0x4
	ands r0, r1, #INTR_FLAG_TIMER3
	bne jump_intr
	add r12, r12, 0x4
	ands r0, r1, #INTR_FLAG_HBLANK
	bne jump_intr
	add r12, r12, 0x4
	ands r0, r1, #INTR_FLAG_VBLANK
	bne jump_intr
	add r12, r12, 0x4
	ands r0, r1, #INTR_FLAG_TIMER0
	bne jump_intr
	add r12, r12, 0x4
	ands r0, r1, #INTR_FLAG_TIMER1
	bne jump_intr
	add r12, r12, 0x4
	ands r0, r1, #INTR_FLAG_TIMER2
	bne jump_intr
	add r12, r12, 0x4
	ands r0, r1, #INTR_FLAG_DMA0
	bne jump_intr
	add r12, r12, 0x4
	ands r0, r1, #INTR_FLAG_DMA1
	bne jump_intr
	add r12, r12, 0x4
	ands r0, r1, #INTR_FLAG_DMA2
	bne jump_intr
	add r12, r12, 0x4
	ands r0, r1, #INTR_FLAG_DMA3
	bne jump_intr
	add r12, r12, 0x4
	ands r0, r1, #INTR_FLAG_KEYPAD
	bne jump_intr
	add r12, r12, 0x4
	ands r0, r1, #INTR_FLAG_GAMEPAK
	strbne r0, [r3, #REG_SOUNDCNT_X - REG_IE]
loop:
	bne loop @ spin
jump_intr:
	strh r0, [r3, #REG_IF - REG_IE]
	bic r2, r2, r0
	ldr r0, =gSTWIStatus
	ldr r0, [r0]
	ldrb r0, [r0, 0xA]
	mov r1, #INTR_FLAG_TIMER0
	lsl r0, r1, r0
	orr r0, r0, #INTR_FLAG_GAMEPAK
	orr r1, r0, #INTR_FLAG_SERIAL | INTR_FLAG_TIMER3 | INTR_FLAG_VCOUNT | INTR_FLAG_HBLANK
	and r1, r1, r2
	strh r1, [r3, #0]
	mrs r3, cpsr
	bic r3, r3, #PSR_I_BIT | PSR_F_BIT | PSR_MODE_MASK
	orr r3, r3, #PSR_SYS_MODE
	msr cpsr_cf, r3
	ldr r1, =gIntrTable
	add r1, r1, r12
	ldr r0, [r1]
	stmdb sp!, {lr}
	adr lr, intr_return
	bx r0
intr_return:
	ldmia sp!, {lr}
	mrs r3, cpsr
	bic r3, r3, #PSR_I_BIT | PSR_F_BIT | PSR_MODE_MASK
	orr r3, r3, #PSR_I_BIT | PSR_IRQ_MODE
	msr cpsr_cf, r3
	ldmia sp!, {r0-r3,lr}
	strh r2, [r3, #0]
	strh r1, [r3, #REG_IME - REG_IE]
	msr spsr_cf, r0
	bx lr

	.pool

	.align 2, 0 @ Don't pad with nop.
