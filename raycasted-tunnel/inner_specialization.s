  .globl TileLightedSpecialization

  @void TileLightedSpecialization(struct InnerParams* params);
  @struct UVLight
  @{
  @  struct Vector2 uv; // Vector2 {float x, y;}
  @  float light;
  @};

  @struct InnerParams
  @{
  @   0 - unsigned int* screen_pixels;
  @   4 - unsigned int* texture;
  @   8 - struct UVLight top_left;
  @  20 - struct UVLight top_right;
  @  32 - struct UVLight bottom_left;
  @  44 - struct UVLight bottom_right;
  @  56 - float step_value;
  @  60 - int grid_x;
  @  64 - int grid_y;
  @};
  
TileLightedSpecialization:
  stmdb   sp!,{r4,r5,r6,r7,r8,r9,r10,r11,r12,r14}
  fstmdbs sp!,{s16,s17}

  flds  s0, [r0, #8]    @ s0 = top_left.u 
  flds  s1, [r0, #12]   @ s1 = top_left.v 
  flds  s2, [r0, #20]   @ s2 = top_right.u 
  flds  s3, [r0, #24]   @ s3 = top_right.v 
  flds  s4, [r0, #32]   @ s4 = bottom_left.u 
  flds  s5, [r0, #36]   @ s5 = bottom_left.v 
  flds  s6, [r0, #44]   @ s6 = bottom_right.u 
  flds  s7, [r0, #48]   @ s7 = bottom_right.v 
  flds  s8, [r0, #56]   @ s8 = step_value
  ldr   r1, [r0]        @ r1  = screen_pixels
  ldr   r14,[r0, #4]    @ r14 = texture
  ldr   r2, [r0, #60]   @ r2  = grid_x 
  ldr   r3, [r0, #64]   @ r3  = grid_y

  vmov   s12, #1.0        @ s12 = 1.0
  fsubs  s11, s12, s12    @ s11 = step_y (no he encontrado forma mejor)
  vmov   s14, #16.0
  fdivs  s9, s12, s14     @ s9 = 1.0 / 16.0  (reciproca de TILE_SIZE) 

  mov   r9, #0x10000      @ 1 << 16
  vmov  s10, r9
  fsitos s10, s10         @ s10 = (float)(1 << 16)
  
  mov   r0, r3, lsl #13
  add   r0, r0, r2, lsl #4   @ r0 = current_pixel
  add   r0, r1, r0, lsl #2   @ r0 = first pixel of the tile (top-left)
 
  mov   r12, #511
  mov   r12, r12, lsl #9       @ r12 = 511 mask shifted
  
  mov   r11, #16               @ r11 = screen_y

loop_y:
  fsubs s13, s12, s11    @ s13 = (1.0f - step_y)

  fmuls s14, s13, s0     @ s14 = (1.0f - step_y) * top_left.uv.u
  fmuls s15, s13, s1     @ s15 = (1.0f - step_y) * top_left.uv.v

  fmuls s16, s13, s2     @ s16 = (1.0f - step_y) * top_right.uv.u
  fmuls s17, s13, s3     @ s17 = (1.0f - step_y) * top_right.uv.v

  fmacs s14, s11, s4     @ s14 = (1.0f - step_y) * top_left.uv.u + step_y * bottom_left.uv.u
  fmacs s15, s11, s5     @ s15 = (1.0f - step_y) * top_left.uv.v + step_y * bottom_left.uv.v

  fmacs s16, s11, s6     @ s16 = (1.0f - step_y) * top_right.uv.u + step_y * bottom_right.uv.u
  fmacs s17, s11, s7     @ s17 = (1.0f - step_y) * top_right.uv.v + step_y * bottom_right.uv.v

  fsubs s16, s16, s14    @ right.u - left.u
  fsubs s17, s17, s15    @ right.v - left.v
  fmuls s16, s16, s9     @ s16 /= TILE_SIZE
  fmuls s17, s17, s9     @ s17 /= TILE_SIZE

  fmuls  s16, s16, s10    @ s16 to fx16
  fmuls  s17, s17, s10    @ s17 to fx16
  ftosis s16, s16
  ftosis s17, s17
  vmov   r3, s16           @ r3 = u_step_value_fx16
  vmov   r4, s17           @ r4 = v_step_value_fx16

  fmuls  s14, s14, s10    @ s14 to fx16
  fmuls  s15, s15, s10    @ s15 to fx16
  ftosis s14, s14
  ftosis s15, s15
  vmov   r1, s14          @ r1 = u_fx16
  vmov   r2, s15          @ r2 = v_fx16

  mov   r10, #16         @ r10 = screen_x

loop_x:
  ubfx  r6, r1, #16, #9
  and   r7, r12, r2, asr #7
  add   r1, r1, r3           @ u_fx16 to next step
  add   r2, r2, r4           @ v_fx16 to next step
  orr   r6, r6, r7

  ubfx  r8, r1, #16, #9
  and   r9, r12, r2, asr #7
  add   r1, r1, r3
  add   r2, r2, r4
  orr   r8, r8, r9

  ldr   r5, [r14, r6, lsl #2]
  ldr   r7, [r14, r8, lsl #2]
  str   r5, [r0], #4
  subs  r10, r10, #2
  str   r7, [r0], #4
  bgt   loop_x

  fadds s11, s11, s8     @ step_y += step_value
  add   r0, r0, #1984    @ siguiente fila del tile (512 - 16) << 2 (4 bytes px)
  subs  r11, r11, #1
  bgt   loop_y

  fldmias sp!,{s16,s17}
  ldmia   sp!,{r4,r5,r6,r7,r8,r9,r10,r11,r12,r14}
  bx lr

