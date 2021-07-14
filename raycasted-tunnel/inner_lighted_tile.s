  @.globl TileLightedSpecialization 
  
@ struct InnerParams
@ {
@   unsigned int* texture;        // 0
@   unsigned int* current_pixel;  // 4
@   int u_fx16;                   // 8
@   int v_fx16;                   // 12
@   int u_step_value_fx16;        // 16
@   int v_step_value_fx16;        // 20
@ };

@ void TileLightedSpecialization(struct InnerParams* params);
@ r0 = InnerParams* params

TileLightedSpecialization:
  stmdb sp!,{r4,r5,r6,r7,r8,r9,r10,r11}

  ldr   r1, [r0]                 @ r1 = texture ptr
  ldr   r2, [r0, #4]             @ r2 = current_pixel ptr
  ldr   r5, [r0, #16]            @ r5 = u_step_value_fx16
  ldr   r6, [r0, #20]            @ r6 = v_step_value_fx16
  mov   r11, #511
  sub   r11, r11, #1
  mov   r11, r11, lsl #9         @ r11 = v mask
  add   r2, r2, #64

  mov   r10, #64
  ldr   r3, [r0, #8]             @ r3 = u_fx16
  ldr   r4, [r0, #12]            @ r4 = v_fx16

loop:

  ubfx  r8, r3, #16, #9          @ ((u >> 16) &= 511)
  and   r9, r11, r4, asr #7      @ ((v >> 16) &= 511) * 512
  
  orr   r8, r8, r9               @ u + v
  add   r3, r3, r5               @ u += params->u_step_fx16
  ldr   r7, [r1, r8, lsl#2]      @ color = params->texture[u + (v<<9)]
  add   r4, r4, r6               @ v += params->v_step_fx16
  str   r7, [r2, -r10]
  subs  r10, r10, #4
  bgt   loop

  ldmia sp!,{r4,r5,r6,r7,r8,r9,r10,r11}
  bx lr
