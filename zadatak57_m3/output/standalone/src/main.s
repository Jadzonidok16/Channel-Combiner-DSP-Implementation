	.extern ___TIMER0_COUNT
	.extern ___TIMER1_COUNT
	.extern ___TIMER1_ENABLE
	.public _enabled
	.public _limiterThreshold
	.public _outputMode
	.public _sampleBuffer
	.public _variablesGain
	.extern _cl_get_cycle_count
	.extern _cl_wavread_bits_per_sample
	.extern _cl_wavread_close
	.extern _cl_wavread_frame_rate
	.extern _cl_wavread_getnchannels
	.extern _cl_wavread_number_of_frames
	.extern _cl_wavread_open
	.extern _cl_wavread_recvsample
	.extern _cl_wavwrite_close
	.extern _cl_wavwrite_open
	.extern _cl_wavwrite_sendsample
	.public _main
	.extern _printf
	.public _saturation
	.extern _signalProcessing
	.extern _strcpy
	.extern __div
	.xdata_ovly
_enabled
	.dw  (0x1)
	.xdata_ovly
_limiterThreshold
	.dw  (0x7fdf3b64)
	.xdata_ovly
_outputMode
	.dw  (0x1)
	.ydata_ovly
_sampleBuffer
	.bss (0x80)
	.xdata_ovly
_string_const_0
	.dw  (0x45)
	.dw  (0x72)
	.dw  (0x72)
	.dw  (0x6f)
	.dw  (0x72)
	.dw  (0x3a)
	.dw  (0x20)
	.dw  (0x43)
	.dw  (0x6f)
	.dw  (0x75)
	.dw  (0x6c)
	.dw  (0x64)
	.dw  (0x20)
	.dw  (0x6e)
	.dw  (0x6f)
	.dw  (0x74)
	.dw  (0x20)
	.dw  (0x6f)
	.dw  (0x70)
	.dw  (0x65)
	.dw  (0x6e)
	.dw  (0x20)
	.dw  (0x77)
	.dw  (0x61)
	.dw  (0x76)
	.dw  (0x65)
	.dw  (0x66)
	.dw  (0x69)
	.dw  (0x6c)
	.dw  (0x65)
	.dw  (0x2e)
	.dw  (0xa)
	.dw  (0x0)
	.xdata_ovly
_string_const_1
	.dw  (0x45)
	.dw  (0x72)
	.dw  (0x72)
	.dw  (0x6f)
	.dw  (0x72)
	.dw  (0x3a)
	.dw  (0x20)
	.dw  (0x43)
	.dw  (0x6f)
	.dw  (0x75)
	.dw  (0x6c)
	.dw  (0x64)
	.dw  (0x20)
	.dw  (0x6e)
	.dw  (0x6f)
	.dw  (0x74)
	.dw  (0x20)
	.dw  (0x6f)
	.dw  (0x70)
	.dw  (0x65)
	.dw  (0x6e)
	.dw  (0x20)
	.dw  (0x77)
	.dw  (0x61)
	.dw  (0x76)
	.dw  (0x65)
	.dw  (0x66)
	.dw  (0x69)
	.dw  (0x6c)
	.dw  (0x65)
	.dw  (0x2e)
	.dw  (0xa)
	.dw  (0x0)
	.xdata_ovly
_string_const_2
	.dw  (0x25)
	.dw  (0x6c)
	.dw  (0x6c)
	.dw  (0x75)
	.dw  (0xa)
	.dw  (0x0)
	.xdata_ovly
_variablesGain
	.dw  (0x7ffff79d)
	.dw  (0x7ffff79d)
	.code_ovly



	# This construction should ensure linking of crt0 in case when target is a standalone program without the OS
	.if defined(_OVLY_)
		.if .strcmp('standalone',_OVLY_)=0
		.if .strcmp('crystal32',_TARGET_FAMILY_)=0
			.extern __start         # dummy use of __start to force linkage of crt0
dummy		.equ(__start)
		.else
			.extern __intvec         # dummy use of __intvec to force linkage of intvec
dummy		.equ(__intvec)
		.endif
		.endif
	.endif

_main:			/* LN: 134 | CYCLE: 0 | RULES: () */ 
	xmem[i7] = a2g; i7 += 1			# LN: 134, 134 | 
	xmem[i7] = a2h; i7 += 1			# LN: 134, 134 | 
	xmem[i7] = a2l; i7 += 1			# LN: 134, 134 | 
	xmem[i7] = i2; i7 += 1			# LN: 134, 134 | 
	xmem[i7] = a3g; i7 += 1			# LN: 134, 134 | 
	xmem[i7] = a3h; i7 += 1			# LN: 134, 134 | 
	xmem[i7] = a3l; i7 += 1			# LN: 134, 134 | 
	xmem[i7] = i3; i7 += 1			# LN: 134, 134 | 
	xmem[i7] = b2g; i7 += 1			# LN: 134, 134 | 
	xmem[i7] = b2h; i7 += 1			# LN: 134, 134 | 
	xmem[i7] = b2l; i7 += 1			# LN: 134, 134 | 
	xmem[i7] = b3g; i7 += 1			# LN: 134, 134 | 
	xmem[i7] = b3h; i7 += 1			# LN: 134, 134 | 
	xmem[i7] = b3l; i7 += 1			# LN: 134, 134 | 
	i7 = i7 + (0x202)			# LN: 134 | 
	i4 = i0			# LN: 134 | 
	a0 = 0			# LN: 155 | 
	do (0x8), label_end_93			# LN: 155 | 
label_begin_93:			/* LN: 155 | CYCLE: 0 | RULES: () */ 
	a1 = 0			# LN: 156 | 
	do (0x10), label_end_92			# LN: 156 | 
label_begin_92:			/* LN: 156 | CYCLE: 0 | RULES: () */ 
	b0 =+ a0			# LN: 157 | 
	b0 = b0 << 4			# LN: 157 | 
	i0 = b0; b0 = 0			# LN: 157, 157 | 
	uhalfword(b1) = (0x1)			# LN: 156 | 
	i0 = i0 + (_sampleBuffer + 0)			# LN: 157 | 
	a2 = i0			# LN: 157 | 
	a2 = a2 + a1			# LN: 157 | 
	AnyReg(i0, a2h)			# LN: 157 | 
	a1 = a1 + b1			# LN: 156 | 
label_end_92:			# LN: 156 | CYCLE: 9 | RULES: ()
	ymem[i0] = b0h			# LN: 157 | 
init_latch_label_1:			/* LN: 157 | CYCLE: 0 | RULES: () */ 
for_end_1:			/* LN: 156 | CYCLE: 0 | RULES: () */ 
	uhalfword(a1) = (0x1)			# LN: 155 | 
label_end_93:			# LN: 155 | CYCLE: 1 | RULES: ()
	a0 = a0 + a1			# LN: 155 | 
for_end_0:			/* LN: 155 | CYCLE: 0 | RULES: () */ 
	i1 = xmem[i4]			# LN: 161 | 
	i0 = i7 - (256 - 0)			# LN: 161 | 
	call (_strcpy)			# LN: 161 | 
	i0 = i7 - (256 - 0)			# LN: 162 | 
	call (_cl_wavread_open)			# LN: 162 | 
	AnyReg(i2, a0h)			# LN: 162 | 
	a0 = i2			# LN: 163 | 
	a0 & a0			# LN: 163 | 
	if (a != 0) jmp (else_2)			# LN: 163 | 
	i0 = (0) + (_string_const_0)			# LN: 165 | 
	call (_printf)			# LN: 165 | 
	halfword(a0) = (0xffff)			# LN: 166 | 
	jmp (__epilogue_238)			# LN: 166 | 
endif_2:			/* LN: 163 | CYCLE: 0 | RULES: () */ 
else_2:			/* LN: 163 | CYCLE: 0 | RULES: () */ 
	i0 = i2			# LN: 172 | 
	call (_cl_wavread_getnchannels)			# LN: 172 | 
	i4 += 1; a2 =+ a0			# LN: 180, 172 | 
	i0 = i2			# LN: 173 | 
	call (_cl_wavread_bits_per_sample)			# LN: 173 | 
	a1 =+ a0			# LN: 173 | 
	i0 = i2			# LN: 174 | 
	call (_cl_wavread_frame_rate)			# LN: 174 | 
	b0 =+ a0			# LN: 174 | 
	i0 = i2			# LN: 175 | 
	call (_cl_wavread_number_of_frames)			# LN: 175 | 
	i0 = i7 - (512 - 0)			# LN: 180 | 
	i1 = xmem[i4]			# LN: 180 | 
	b1 =+ a0			# LN: 175 | 
	call (_strcpy)			# LN: 180 | 
	a0 = xmem[_outputMode + 0]			# LN: 182 | 
	a0 & a0			# LN: 182 | 
	if (a != 0) jmp (elsecondition_0)			# LN: 182 | 
	uhalfword(a3) = (0x2)			# LN: 182 | 
	jmp (endcondition_0)			# LN: 182 | 
elsecondition_0:			/* LN: 182 | CYCLE: 0 | RULES: () */ 
	uhalfword(a3) = (0x5)			# LN: 182 | 
endcondition_0:			/* LN: 182 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (512 - 0)			# LN: 183 | 
	a0 =+ a1			# LN: 183 | 
	a1 =+ a3			# LN: 183 | 
	call (_cl_wavwrite_open)			# LN: 183 | 
	AnyReg(i3, a0h)			# LN: 183 | 
	a0 = i3			# LN: 184 | 
	a0 & a0			# LN: 184 | 
	if (a != 0) jmp (else_3)			# LN: 184 | 
	i0 = (0) + (_string_const_1)			# LN: 186 | 
	call (_printf)			# LN: 186 | 
	halfword(a0) = (0xffff)			# LN: 187 | 
	jmp (__epilogue_238)			# LN: 187 | 
endif_3:			/* LN: 184 | CYCLE: 0 | RULES: () */ 
else_3:			/* LN: 184 | CYCLE: 0 | RULES: () */ 
	uhalfword(a1) = (0x10)			# LN: 202 | 
	a0 =+ b1			# LN: 202 | 
	call (__div)			# LN: 202 | 
	AnyReg(i0, a0h)			# LN: 202 | 
	nop #empty cycle
	do (i0), label_end_98			# LN: 202 | 
label_begin_98:			/* LN: 202 | CYCLE: 0 | RULES: () */ 
	b2 = 0			# LN: 204 | 
	do (0x10), label_end_95			# LN: 204 | 
label_begin_95:			/* LN: 204 | CYCLE: 0 | RULES: () */ 
	AnyReg(i0, a2h)			# LN: 206 | 
	b0 = 0			# LN: 206 | 
	do (i0), label_end_94			# LN: 206 | 
label_begin_94:			/* LN: 206 | CYCLE: 0 | RULES: () */ 
	i0 = i2			# LN: 208 | 
	call (_cl_wavread_recvsample)			# LN: 208 | 
	a1 =+ a0; b1 =+ b0			# LN: 208, 209 | 
	b1 = b1 << 4			# LN: 209 | 
	i0 = b1; a0 =+ a1			# LN: 209, 209 | 
	uhalfword(a1) = (0x1)			# LN: 206 | 
	i0 = i0 + (_sampleBuffer + 0)			# LN: 209 | 
	b1 = i0; b0 = b0 + a1			# LN: 209, 206 | 
	a1 =+ b1			# LN: 209 | 
	a1 = a1 + b2			# LN: 209 | 
	AnyReg(i0, a1h)			# LN: 209 | 
	nop #empty cycle
label_end_94:			# LN: 206 | CYCLE: 12 | RULES: ()
	ymem[i0] = a0h			# LN: 209 | 
init_latch_label_3:			/* LN: 211 | CYCLE: 0 | RULES: () */ 
for_end_4:			/* LN: 206 | CYCLE: 0 | RULES: () */ 
	uhalfword(a0) = (0x1)			# LN: 204 | 
label_end_95:			# LN: 204 | CYCLE: 1 | RULES: ()
	b2 = b2 + a0			# LN: 204 | 
for_end_3:			/* LN: 204 | CYCLE: 0 | RULES: () */ 
	a0 = xmem[_enabled + 0]			# LN: 214 | 
	a0 & a0			# LN: 214 | 
	if (a == 0) jmp (else_4)			# LN: 214 | 
	call (_cl_get_cycle_count)			# LN: 216 | 
	i0 = (0) + (_sampleBuffer)			# LN: 217 | 
	i4 = i7 - (0x202)			# LN: 216 | 
	xmem[i4] = a0h; i4 += 1			# LN: 216, 216 | 
	xmem[i4] = a0l			# LN: 216 | 
	i1 = (0) + (_sampleBuffer)			# LN: 217 | 
	call (_signalProcessing)			# LN: 217 | 
	call (_cl_get_cycle_count)			# LN: 218 | 
	b3 =+ a0			# LN: 218 | 
endif_4:			/* LN: 214 | CYCLE: 0 | RULES: () */ 
else_4:			/* LN: 214 | CYCLE: 0 | RULES: () */ 
	i0 = i7 - (0x202)			# LN: 220 | 
	i1 = (0) + (_string_const_2)			# LN: 221 | 
	a0 = xmem[i0]; i0 += 1			# LN: 220, 220 | 
	a0l = xmem[i0]			# LN: 220 | 
	a0 = a0 - b3			# LN: 220 | 
	a0 =- a0			# LN: 220 | 
	i0 = i1			# LN: 221 | 
	call (_printf)			# LN: 221 | 
	a1 = 0			# LN: 224 | 
	do (0x10), label_end_97			# LN: 224 | 
label_begin_97:			/* LN: 224 | CYCLE: 0 | RULES: () */ 
	AnyReg(i0, a3h)			# LN: 226 | 
	b0 = 0			# LN: 226 | 
	do (i0), label_end_96			# LN: 226 | 
label_begin_96:			/* LN: 226 | CYCLE: 0 | RULES: () */ 
	a0 =+ b0			# LN: 228 | 
	a0 = a0 << 4			# LN: 228 | 
	i1 = a0			# LN: 228 | 
	i0 = i3			# LN: 229 | 
	i1 = i1 + (_sampleBuffer + 0)			# LN: 228 | 
	a0 = i1			# LN: 228 | 
	a0 = a0 + a1			# LN: 228 | 
	AnyReg(i1, a0h)			# LN: 228 | 
	uhalfword(a0) = (0x1)			# LN: 226 | 
	b1 = ymem[i1]; b0 = b0 + a0			# LN: 228, 226 | 
	a0 =+ b1			# LN: 228 | 
label_end_96:			# LN: 226 | CYCLE: 11 | RULES: ()
	call (_cl_wavwrite_sendsample)			# LN: 229 | 
init_latch_label_5:			/* LN: 231 | CYCLE: 0 | RULES: () */ 
for_end_6:			/* LN: 226 | CYCLE: 0 | RULES: () */ 
	uhalfword(a0) = (0x1)			# LN: 224 | 
label_end_97:			# LN: 224 | CYCLE: 1 | RULES: ()
	a1 = a1 + a0			# LN: 224 | 
init_latch_label_6:			/* LN: 232 | CYCLE: 0 | RULES: () */ 
for_end_5:			/* LN: 224 | CYCLE: 0 | RULES: () */ 
label_end_98:			# LN: 202 | CYCLE: 0 | RULES: ()
	nop #empty cycle
for_end_2:			/* LN: 202 | CYCLE: 0 | RULES: () */ 
	i0 = i2			# LN: 237 | 
	call (_cl_wavread_close)			# LN: 237 | 
	i0 = i3			# LN: 238 | 
	call (_cl_wavwrite_close)			# LN: 238 | 
	a0 = 0			# LN: 241 | 
__epilogue_238:			/* LN: 242 | CYCLE: 0 | RULES: () */ 
	i7 = i7 - (0x202)			# LN: 242 | 
	i7 -= 1			# LN: 242 | 
	b3l = xmem[i7]; i7 -= 1			# LN: 242, 242 | 
	b3h = xmem[i7]; i7 -= 1			# LN: 242, 242 | 
	b3g = xmem[i7]; i7 -= 1			# LN: 242, 242 | 
	b2l = xmem[i7]; i7 -= 1			# LN: 242, 242 | 
	b2h = xmem[i7]; i7 -= 1			# LN: 242, 242 | 
	b2g = xmem[i7]; i7 -= 1			# LN: 242, 242 | 
	i3 = xmem[i7]; i7 -= 1			# LN: 242, 242 | 
	a3l = xmem[i7]; i7 -= 1			# LN: 242, 242 | 
	a3h = xmem[i7]; i7 -= 1			# LN: 242, 242 | 
	a3g = xmem[i7]; i7 -= 1			# LN: 242, 242 | 
	i2 = xmem[i7]; i7 -= 1			# LN: 242, 242 | 
	a2l = xmem[i7]; i7 -= 1			# LN: 242, 242 | 
	a2h = xmem[i7]; i7 -= 1			# LN: 242, 242 | 
	a2g = xmem[i7]			# LN: 242 | 
	ret			# LN: 242 | 



_saturation:			/* LN: 23 | CYCLE: 0 | RULES: () */ 
	a1 =+ a0			# LN: 23 | 
	b1 = xmem[_limiterThreshold + 0]			# LN: 25 | 
	a0 =+ b1; b0 =+ a1			# LN: 25, 25 | 
	a0 = a0 >> 1; b0 = b0 >> 1			# LN: 25, 25 | 
	b0 - a0			# LN: 25 | 
	if (b <= 0) jmp (else_0)			# LN: 25 | 
	a0 = xmem[_limiterThreshold + 0]			# LN: 27 | 
	jmp (__epilogue_234)			# LN: 27 | 
else_0:			/* LN: 25 | CYCLE: 0 | RULES: () */ 
	a0 = xmem[_limiterThreshold + 0]			# LN: 29 | 
	b1 =- a0			# LN: 29 | 
	a0 =+ b1; b0 =+ a1			# LN: 29, 29 | 
	a0 = a0 >> 1; b0 = b0 >> 1			# LN: 29, 29 | 
	b0 - a0			# LN: 29 | 
	if (b >= 0) jmp (else_1)			# LN: 29 | 
	a0 = xmem[_limiterThreshold + 0]			# LN: 31 | 
	a0 =- a0			# LN: 31 | 
	jmp (__epilogue_234)			# LN: 31 | 
endif_0:			/* LN: 25 | CYCLE: 0 | RULES: () */ 
endif_1:			/* LN: 29 | CYCLE: 0 | RULES: () */ 
else_1:			/* LN: 29 | CYCLE: 0 | RULES: () */ 
	a0 = a1			# LN: 33 | 
__epilogue_234:			/* LN: 34 | CYCLE: 0 | RULES: () */ 
	ret			# LN: 34 | 
