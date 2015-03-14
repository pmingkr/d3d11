#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 9.29.952.3111
//
//
//   fxc /nologo /Emain /T vs_4_0_level_9_1 /Fh vs_skinned.h /Vn s_vs_skinned
//    vs_skinned.hlsl
//
//
// Buffer Definitions: 
//
// cbuffer CB_VSSkinned
// {
//
//   float4x4 mVP;                      // Offset:    0 Size:    64
//
// }
//
// cbuffer CB_VSSkinned
// {
//
//   float4x3 mWorlds[80];              // Offset:    0 Size:  3840
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim Slot Elements
// ------------------------------ ---------- ------- ----------- ---- --------
// CB_VSSkinned                      cbuffer      NA          NA    0        1
// CB_VSSkinned                      cbuffer      NA          NA    1        1
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// POSITION                 0   xyzw        0     NONE  float   xyzw
// NORMAL                   0   xyz         1     NONE  float   xyz 
// TEXCOORD                 0   xy          2     NONE  float   xy  
// BLENDINDICES             0   xyzw        3     NONE   uint   xyzw
// BLENDWEIGHT              0   xyzw        4     NONE  float   xyzw
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// SV_POSITION              0   xyzw        0      POS  float   xyzw
// COLOR                    0   xyzw        1     NONE  float   xyzw
// TEXCOORD                 0   xy          2     NONE  float   xy  
//
//
// Constant buffer to DX9 shader constant mappings:
//
// Target Reg Buffer  Start Reg # of Regs        Data Conversion
// ---------- ------- --------- --------- ----------------------
// c0         cb1             0       240  ( FLT, FLT, FLT, FLT)
// c241       cb0             0         4  ( FLT, FLT, FLT, FLT)
//
//
// Runtime generated constant mappings:
//
// Target Reg                               Constant Description
// ---------- --------------------------------------------------
// c240                            Vertex Shader position offset
//
//
// Level9 shader bytecode:
//
    vs_2_0
    def c245, 3, 1, 0, 0
    dcl_texcoord v0
    dcl_texcoord1 v1
    dcl_texcoord2 v2
    dcl_texcoord3 v3
    dcl_texcoord4 v4
    mul r0, v3, c245.x
    mova a0, r0.yxzw
    dp4 r1.x, v0, c0[a0.x]
    dp4 r1.y, v0, c1[a0.x]
    dp4 r1.z, v0, c2[a0.x]
    mul r1.xyz, r1, v4.y
    dp4 r2.x, v0, c0[a0.y]
    dp4 r2.y, v0, c1[a0.y]
    dp4 r2.z, v0, c2[a0.y]
    mad r1.xyz, r2, v4.x, r1
    dp4 r0.x, v0, c0[a0.z]
    dp4 r0.y, v0, c1[a0.z]
    dp4 r0.z, v0, c2[a0.z]
    mad r0.xyz, r0, v4.z, r1
    dp4 r1.x, v0, c0[a0.w]
    dp4 r1.y, v0, c1[a0.w]
    dp4 r1.z, v0, c2[a0.w]
    mad r0.xyz, r1, v4.w, r0
    mov r0.w, c245.y
    dp4 oPos.z, r0, c243
    dp3 r1.x, v1, c0[a0.x]
    dp3 r1.y, v1, c1[a0.x]
    dp3 r1.z, v1, c2[a0.x]
    mul r1.xyz, r1, v4.y
    dp3 r2.x, v1, c0[a0.y]
    dp3 r2.y, v1, c1[a0.y]
    dp3 r2.z, v1, c2[a0.y]
    mad r1.xyz, r2, v4.x, r1
    dp3 r2.x, v1, c0[a0.z]
    dp3 r2.y, v1, c1[a0.z]
    dp3 r2.z, v1, c2[a0.z]
    mad r1.xyz, r2, v4.z, r1
    dp3 r2.x, v1, c0[a0.w]
    dp3 r2.y, v1, c1[a0.w]
    dp3 r2.z, v1, c2[a0.w]
    mad r1.xyz, r2, v4.w, r1
    dp3 r1.x, r1, r1
    rsq r1.x, r1.x
    mul r1.x, r1.x, r1.y
    max r1.x, r1.x, c245.z
    min oT0, r1.x, c245.y
    dp4 r1.x, r0, c241
    dp4 r1.y, r0, c242
    dp4 r0.x, r0, c244
    mad oPos.xy, r0.x, c240, r1
    mov oPos.w, r0.x
    mov oT1.xy, v2

// approximately 47 instruction slots used
vs_4_0
dcl_constantbuffer cb0[4], immediateIndexed
dcl_constantbuffer cb1[240], dynamicIndexed
dcl_input v0.xyzw
dcl_input v1.xyz
dcl_input v2.xy
dcl_input v3.xyzw
dcl_input v4.xyzw
dcl_output_siv o0.xyzw, position
dcl_output o1.xyzw
dcl_output o2.xy
dcl_temps 3
imul null, r0.xyzw, v3.xyzw, l(3, 3, 3, 3)
dp4 r1.x, v0.xyzw, cb1[r0.y + 0].xyzw
dp4 r1.y, v0.xyzw, cb1[r0.y + 1].xyzw
dp4 r1.z, v0.xyzw, cb1[r0.y + 2].xyzw
mul r1.xyz, r1.xyzx, v4.yyyy
dp4 r2.x, v0.xyzw, cb1[r0.x + 0].xyzw
dp4 r2.y, v0.xyzw, cb1[r0.x + 1].xyzw
dp4 r2.z, v0.xyzw, cb1[r0.x + 2].xyzw
mad r1.xyz, r2.xyzx, v4.xxxx, r1.xyzx
dp4 r2.x, v0.xyzw, cb1[r0.z + 0].xyzw
dp4 r2.y, v0.xyzw, cb1[r0.z + 1].xyzw
dp4 r2.z, v0.xyzw, cb1[r0.z + 2].xyzw
mad r1.xyz, r2.xyzx, v4.zzzz, r1.xyzx
dp4 r2.x, v0.xyzw, cb1[r0.w + 0].xyzw
dp4 r2.y, v0.xyzw, cb1[r0.w + 1].xyzw
dp4 r2.z, v0.xyzw, cb1[r0.w + 2].xyzw
mad r1.xyz, r2.xyzx, v4.wwww, r1.xyzx
mov r1.w, l(1.000000)
dp4 o0.x, r1.xyzw, cb0[0].xyzw
dp4 o0.y, r1.xyzw, cb0[1].xyzw
dp4 o0.z, r1.xyzw, cb0[2].xyzw
dp4 o0.w, r1.xyzw, cb0[3].xyzw
dp3 r1.x, v1.xyzx, cb1[r0.y + 0].xyzx
dp3 r1.y, v1.xyzx, cb1[r0.y + 1].xyzx
dp3 r1.z, v1.xyzx, cb1[r0.y + 2].xyzx
mul r1.xyz, r1.xyzx, v4.yyyy
dp3 r2.x, v1.xyzx, cb1[r0.x + 0].xyzx
dp3 r2.y, v1.xyzx, cb1[r0.x + 1].xyzx
dp3 r2.z, v1.xyzx, cb1[r0.x + 2].xyzx
mad r1.xyz, r2.xyzx, v4.xxxx, r1.xyzx
dp3 r2.x, v1.xyzx, cb1[r0.z + 0].xyzx
dp3 r2.y, v1.xyzx, cb1[r0.z + 1].xyzx
dp3 r2.z, v1.xyzx, cb1[r0.z + 2].xyzx
mad r0.xyz, r2.xyzx, v4.zzzz, r1.xyzx
dp3 r1.x, v1.xyzx, cb1[r0.w + 0].xyzx
dp3 r1.y, v1.xyzx, cb1[r0.w + 1].xyzx
dp3 r1.z, v1.xyzx, cb1[r0.w + 2].xyzx
mad r0.xyz, r1.xyzx, v4.wwww, r0.xyzx
dp3 r0.x, r0.xyzx, r0.xyzx
rsq r0.x, r0.x
mul r0.x, r0.x, r0.y
mov_sat o1.xyzw, r0.xxxx
mov o2.xy, v2.xyxx
ret 
// Approximately 44 instruction slots used
#endif

const BYTE s_vs_skinned[] =
{
     68,  88,  66,  67,  85,  42, 
    225, 165,  69,  12,  98,  17, 
     78, 173, 221, 132, 221, 172, 
    122,   6,   1,   0,   0,   0, 
    152,  13,   0,   0,   6,   0, 
      0,   0,  56,   0,   0,   0, 
     52,   4,   0,   0, 184,  10, 
      0,   0,  52,  11,   0,   0, 
    104,  12,   0,   0,  36,  13, 
      0,   0,  65, 111, 110,  57, 
    244,   3,   0,   0, 244,   3, 
      0,   0,   0,   2, 254, 255, 
    180,   3,   0,   0,  64,   0, 
      0,   0,   2,   0,  36,   0, 
      0,   0,  60,   0,   0,   0, 
     60,   0,   0,   0,  36,   0, 
      1,   0,  60,   0,   1,   0, 
      0,   0, 240,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   4,   0, 241,   0, 
      0,   0,   0,   0,   0,   0, 
    240,   0,   0,   2, 254, 255, 
     81,   0,   0,   5, 245,   0, 
     15, 160,   0,   0,  64,  64, 
      0,   0, 128,  63,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     31,   0,   0,   2,   5,   0, 
      0, 128,   0,   0,  15, 144, 
     31,   0,   0,   2,   5,   0, 
      1, 128,   1,   0,  15, 144, 
     31,   0,   0,   2,   5,   0, 
      2, 128,   2,   0,  15, 144, 
     31,   0,   0,   2,   5,   0, 
      3, 128,   3,   0,  15, 144, 
     31,   0,   0,   2,   5,   0, 
      4, 128,   4,   0,  15, 144, 
      5,   0,   0,   3,   0,   0, 
     15, 128,   3,   0, 228, 144, 
    245,   0,   0, 160,  46,   0, 
      0,   2,   0,   0,  15, 176, 
      0,   0, 225, 128,   9,   0, 
      0,   4,   1,   0,   1, 128, 
      0,   0, 228, 144,   0,  32, 
    228, 160,   0,   0,   0, 176, 
      9,   0,   0,   4,   1,   0, 
      2, 128,   0,   0, 228, 144, 
      1,  32, 228, 160,   0,   0, 
      0, 176,   9,   0,   0,   4, 
      1,   0,   4, 128,   0,   0, 
    228, 144,   2,  32, 228, 160, 
      0,   0,   0, 176,   5,   0, 
      0,   3,   1,   0,   7, 128, 
      1,   0, 228, 128,   4,   0, 
     85, 144,   9,   0,   0,   4, 
      2,   0,   1, 128,   0,   0, 
    228, 144,   0,  32, 228, 160, 
      0,   0,  85, 176,   9,   0, 
      0,   4,   2,   0,   2, 128, 
      0,   0, 228, 144,   1,  32, 
    228, 160,   0,   0,  85, 176, 
      9,   0,   0,   4,   2,   0, 
      4, 128,   0,   0, 228, 144, 
      2,  32, 228, 160,   0,   0, 
     85, 176,   4,   0,   0,   4, 
      1,   0,   7, 128,   2,   0, 
    228, 128,   4,   0,   0, 144, 
      1,   0, 228, 128,   9,   0, 
      0,   4,   0,   0,   1, 128, 
      0,   0, 228, 144,   0,  32, 
    228, 160,   0,   0, 170, 176, 
      9,   0,   0,   4,   0,   0, 
      2, 128,   0,   0, 228, 144, 
      1,  32, 228, 160,   0,   0, 
    170, 176,   9,   0,   0,   4, 
      0,   0,   4, 128,   0,   0, 
    228, 144,   2,  32, 228, 160, 
      0,   0, 170, 176,   4,   0, 
      0,   4,   0,   0,   7, 128, 
      0,   0, 228, 128,   4,   0, 
    170, 144,   1,   0, 228, 128, 
      9,   0,   0,   4,   1,   0, 
      1, 128,   0,   0, 228, 144, 
      0,  32, 228, 160,   0,   0, 
    255, 176,   9,   0,   0,   4, 
      1,   0,   2, 128,   0,   0, 
    228, 144,   1,  32, 228, 160, 
      0,   0, 255, 176,   9,   0, 
      0,   4,   1,   0,   4, 128, 
      0,   0, 228, 144,   2,  32, 
    228, 160,   0,   0, 255, 176, 
      4,   0,   0,   4,   0,   0, 
      7, 128,   1,   0, 228, 128, 
      4,   0, 255, 144,   0,   0, 
    228, 128,   1,   0,   0,   2, 
      0,   0,   8, 128, 245,   0, 
     85, 160,   9,   0,   0,   3, 
      0,   0,   4, 192,   0,   0, 
    228, 128, 243,   0, 228, 160, 
      8,   0,   0,   4,   1,   0, 
      1, 128,   1,   0, 228, 144, 
      0,  32, 228, 160,   0,   0, 
      0, 176,   8,   0,   0,   4, 
      1,   0,   2, 128,   1,   0, 
    228, 144,   1,  32, 228, 160, 
      0,   0,   0, 176,   8,   0, 
      0,   4,   1,   0,   4, 128, 
      1,   0, 228, 144,   2,  32, 
    228, 160,   0,   0,   0, 176, 
      5,   0,   0,   3,   1,   0, 
      7, 128,   1,   0, 228, 128, 
      4,   0,  85, 144,   8,   0, 
      0,   4,   2,   0,   1, 128, 
      1,   0, 228, 144,   0,  32, 
    228, 160,   0,   0,  85, 176, 
      8,   0,   0,   4,   2,   0, 
      2, 128,   1,   0, 228, 144, 
      1,  32, 228, 160,   0,   0, 
     85, 176,   8,   0,   0,   4, 
      2,   0,   4, 128,   1,   0, 
    228, 144,   2,  32, 228, 160, 
      0,   0,  85, 176,   4,   0, 
      0,   4,   1,   0,   7, 128, 
      2,   0, 228, 128,   4,   0, 
      0, 144,   1,   0, 228, 128, 
      8,   0,   0,   4,   2,   0, 
      1, 128,   1,   0, 228, 144, 
      0,  32, 228, 160,   0,   0, 
    170, 176,   8,   0,   0,   4, 
      2,   0,   2, 128,   1,   0, 
    228, 144,   1,  32, 228, 160, 
      0,   0, 170, 176,   8,   0, 
      0,   4,   2,   0,   4, 128, 
      1,   0, 228, 144,   2,  32, 
    228, 160,   0,   0, 170, 176, 
      4,   0,   0,   4,   1,   0, 
      7, 128,   2,   0, 228, 128, 
      4,   0, 170, 144,   1,   0, 
    228, 128,   8,   0,   0,   4, 
      2,   0,   1, 128,   1,   0, 
    228, 144,   0,  32, 228, 160, 
      0,   0, 255, 176,   8,   0, 
      0,   4,   2,   0,   2, 128, 
      1,   0, 228, 144,   1,  32, 
    228, 160,   0,   0, 255, 176, 
      8,   0,   0,   4,   2,   0, 
      4, 128,   1,   0, 228, 144, 
      2,  32, 228, 160,   0,   0, 
    255, 176,   4,   0,   0,   4, 
      1,   0,   7, 128,   2,   0, 
    228, 128,   4,   0, 255, 144, 
      1,   0, 228, 128,   8,   0, 
      0,   3,   1,   0,   1, 128, 
      1,   0, 228, 128,   1,   0, 
    228, 128,   7,   0,   0,   2, 
      1,   0,   1, 128,   1,   0, 
      0, 128,   5,   0,   0,   3, 
      1,   0,   1, 128,   1,   0, 
      0, 128,   1,   0,  85, 128, 
     11,   0,   0,   3,   1,   0, 
      1, 128,   1,   0,   0, 128, 
    245,   0, 170, 160,  10,   0, 
      0,   3,   0,   0,  15, 224, 
      1,   0,   0, 128, 245,   0, 
     85, 160,   9,   0,   0,   3, 
      1,   0,   1, 128,   0,   0, 
    228, 128, 241,   0, 228, 160, 
      9,   0,   0,   3,   1,   0, 
      2, 128,   0,   0, 228, 128, 
    242,   0, 228, 160,   9,   0, 
      0,   3,   0,   0,   1, 128, 
      0,   0, 228, 128, 244,   0, 
    228, 160,   4,   0,   0,   4, 
      0,   0,   3, 192,   0,   0, 
      0, 128, 240,   0, 228, 160, 
      1,   0, 228, 128,   1,   0, 
      0,   2,   0,   0,   8, 192, 
      0,   0,   0, 128,   1,   0, 
      0,   2,   1,   0,   3, 224, 
      2,   0, 228, 144, 255, 255, 
      0,   0,  83,  72,  68,  82, 
    124,   6,   0,   0,  64,   0, 
      1,   0, 159,   1,   0,   0, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,  89,   8, 
      0,   4,  70, 142,  32,   0, 
      1,   0,   0,   0, 240,   0, 
      0,   0,  95,   0,   0,   3, 
    242,  16,  16,   0,   0,   0, 
      0,   0,  95,   0,   0,   3, 
    114,  16,  16,   0,   1,   0, 
      0,   0,  95,   0,   0,   3, 
     50,  16,  16,   0,   2,   0, 
      0,   0,  95,   0,   0,   3, 
    242,  16,  16,   0,   3,   0, 
      0,   0,  95,   0,   0,   3, 
    242,  16,  16,   0,   4,   0, 
      0,   0, 103,   0,   0,   4, 
    242,  32,  16,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
    101,   0,   0,   3, 242,  32, 
     16,   0,   1,   0,   0,   0, 
    101,   0,   0,   3,  50,  32, 
     16,   0,   2,   0,   0,   0, 
    104,   0,   0,   2,   3,   0, 
      0,   0,  38,   0,   0,  11, 
      0, 208,   0,   0, 242,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  30,  16,   0,   3,   0, 
      0,   0,   2,  64,   0,   0, 
      3,   0,   0,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
      3,   0,   0,   0,  17,   0, 
      0,   9,  18,   0,  16,   0, 
      1,   0,   0,   0,  70,  30, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   4,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  17,   0, 
      0,  10,  34,   0,  16,   0, 
      1,   0,   0,   0,  70,  30, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   6,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  17,   0,   0,  10, 
     66,   0,  16,   0,   1,   0, 
      0,   0,  70,  30,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   6,   1,   0,   0,   0, 
      2,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   7, 114,   0, 
     16,   0,   1,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  86,  21,  16,   0, 
      4,   0,   0,   0,  17,   0, 
      0,   9,  18,   0,  16,   0, 
      2,   0,   0,   0,  70,  30, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   4,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  17,   0, 
      0,  10,  34,   0,  16,   0, 
      2,   0,   0,   0,  70,  30, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   6,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  17,   0,   0,  10, 
     66,   0,  16,   0,   2,   0, 
      0,   0,  70,  30,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   6,   1,   0,   0,   0, 
      2,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,   9, 114,   0, 
     16,   0,   1,   0,   0,   0, 
     70,   2,  16,   0,   2,   0, 
      0,   0,   6,  16,  16,   0, 
      4,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
     17,   0,   0,   9,  18,   0, 
     16,   0,   2,   0,   0,   0, 
     70,  30,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   4, 
      1,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     17,   0,   0,  10,  34,   0, 
     16,   0,   2,   0,   0,   0, 
     70,  30,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   6, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0,  17,   0, 
      0,  10,  66,   0,  16,   0, 
      2,   0,   0,   0,  70,  30, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   6,   1,   0, 
      0,   0,   2,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,   9, 
    114,   0,  16,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      2,   0,   0,   0, 166,  26, 
     16,   0,   4,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  17,   0,   0,   9, 
     18,   0,  16,   0,   2,   0, 
      0,   0,  70,  30,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   4,   1,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  17,   0,   0,  10, 
     34,   0,  16,   0,   2,   0, 
      0,   0,  70,  30,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   6,   1,   0,   0,   0, 
      1,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     17,   0,   0,  10,  66,   0, 
     16,   0,   2,   0,   0,   0, 
     70,  30,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   6, 
      1,   0,   0,   0,   2,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,   9, 114,   0,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   2,   0,   0,   0, 
    246,  31,  16,   0,   4,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  54,   0, 
      0,   5, 130,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 128,  63, 
     17,   0,   0,   8,  18,  32, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  17,   0,   0,   8, 
     34,  32,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,  17,   0, 
      0,   8,  66,  32,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
     17,   0,   0,   8, 130,  32, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,  16,   0,   0,   9, 
     18,   0,  16,   0,   1,   0, 
      0,   0,  70,  18,  16,   0, 
      1,   0,   0,   0,  70, 130, 
     32,   4,   1,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  16,   0,   0,  10, 
     34,   0,  16,   0,   1,   0, 
      0,   0,  70,  18,  16,   0, 
      1,   0,   0,   0,  70, 130, 
     32,   6,   1,   0,   0,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     16,   0,   0,  10,  66,   0, 
     16,   0,   1,   0,   0,   0, 
     70,  18,  16,   0,   1,   0, 
      0,   0,  70, 130,  32,   6, 
      1,   0,   0,   0,   2,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   7, 114,   0,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
     86,  21,  16,   0,   4,   0, 
      0,   0,  16,   0,   0,   9, 
     18,   0,  16,   0,   2,   0, 
      0,   0,  70,  18,  16,   0, 
      1,   0,   0,   0,  70, 130, 
     32,   4,   1,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  16,   0,   0,  10, 
     34,   0,  16,   0,   2,   0, 
      0,   0,  70,  18,  16,   0, 
      1,   0,   0,   0,  70, 130, 
     32,   6,   1,   0,   0,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     16,   0,   0,  10,  66,   0, 
     16,   0,   2,   0,   0,   0, 
     70,  18,  16,   0,   1,   0, 
      0,   0,  70, 130,  32,   6, 
      1,   0,   0,   0,   2,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,   9, 114,   0,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   2,   0,   0,   0, 
      6,  16,  16,   0,   4,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  16,   0, 
      0,   9,  18,   0,  16,   0, 
      2,   0,   0,   0,  70,  18, 
     16,   0,   1,   0,   0,   0, 
     70, 130,  32,   4,   1,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0,  16,   0, 
      0,  10,  34,   0,  16,   0, 
      2,   0,   0,   0,  70,  18, 
     16,   0,   1,   0,   0,   0, 
     70, 130,  32,   6,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0,  16,   0,   0,  10, 
     66,   0,  16,   0,   2,   0, 
      0,   0,  70,  18,  16,   0, 
      1,   0,   0,   0,  70, 130, 
     32,   6,   1,   0,   0,   0, 
      2,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,   9, 114,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   2,   0, 
      0,   0, 166,  26,  16,   0, 
      4,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
     16,   0,   0,   9,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     70,  18,  16,   0,   1,   0, 
      0,   0,  70, 130,  32,   4, 
      1,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     16,   0,   0,  10,  34,   0, 
     16,   0,   1,   0,   0,   0, 
     70,  18,  16,   0,   1,   0, 
      0,   0,  70, 130,  32,   6, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  16,   0, 
      0,  10,  66,   0,  16,   0, 
      1,   0,   0,   0,  70,  18, 
     16,   0,   1,   0,   0,   0, 
     70, 130,  32,   6,   1,   0, 
      0,   0,   2,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,   9, 
    114,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0, 246,  31, 
     16,   0,   4,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  16,   0,   0,   7, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     68,   0,   0,   5,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   7, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     54,  32,   0,   5, 242,  32, 
     16,   0,   1,   0,   0,   0, 
      6,   0,  16,   0,   0,   0, 
      0,   0,  54,   0,   0,   5, 
     50,  32,  16,   0,   2,   0, 
      0,   0,  70,  16,  16,   0, 
      2,   0,   0,   0,  62,   0, 
      0,   1,  83,  84,  65,  84, 
    116,   0,   0,   0,  44,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   8,   0, 
      0,   0,  33,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     82,  68,  69,  70,  44,   1, 
      0,   0,   2,   0,   0,   0, 
    108,   0,   0,   0,   2,   0, 
      0,   0,  28,   0,   0,   0, 
      0,   4, 254, 255,   0,   1, 
      0,   0, 248,   0,   0,   0, 
     92,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  92,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  67,  66, 
     95,  86,  83,  83, 107, 105, 
    110, 110, 101, 100,   0, 171, 
    171, 171,  92,   0,   0,   0, 
      1,   0,   0,   0, 156,   0, 
      0,   0,  64,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  92,   0,   0,   0, 
      1,   0,   0,   0, 200,   0, 
      0,   0,   0,  15,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 180,   0,   0,   0, 
      0,   0,   0,   0,  64,   0, 
      0,   0,   2,   0,   0,   0, 
    184,   0,   0,   0,   0,   0, 
      0,   0, 109,  86,  80,   0, 
      3,   0,   3,   0,   4,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 224,   0, 
      0,   0,   0,   0,   0,   0, 
      0,  15,   0,   0,   2,   0, 
      0,   0, 232,   0,   0,   0, 
      0,   0,   0,   0, 109,  87, 
    111, 114, 108, 100, 115,   0, 
      3,   0,   3,   0,   4,   0, 
      3,   0,  80,   0,   0,   0, 
      0,   0,   0,   0,  77, 105, 
     99, 114, 111, 115, 111, 102, 
    116,  32,  40,  82,  41,  32, 
     72,  76,  83,  76,  32,  83, 
    104,  97, 100, 101, 114,  32, 
     67, 111, 109, 112, 105, 108, 
    101, 114,  32,  57,  46,  50, 
     57,  46,  57,  53,  50,  46, 
     51,  49,  49,  49,   0, 171, 
    171, 171,  73,  83,  71,  78, 
    180,   0,   0,   0,   5,   0, 
      0,   0,   8,   0,   0,   0, 
    128,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,  15,   0,   0, 
    137,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,   7,   7,   0,   0, 
    144,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,   3,   3,   0,   0, 
    153,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,  15,  15,   0,   0, 
    166,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   4,   0, 
      0,   0,  15,  15,   0,   0, 
     80,  79,  83,  73,  84,  73, 
     79,  78,   0,  78,  79,  82, 
     77,  65,  76,   0,  84,  69, 
     88,  67,  79,  79,  82,  68, 
      0,  66,  76,  69,  78,  68, 
     73,  78,  68,  73,  67,  69, 
     83,   0,  66,  76,  69,  78, 
     68,  87,  69,  73,  71,  72, 
     84,   0, 171, 171,  79,  83, 
     71,  78, 108,   0,   0,   0, 
      3,   0,   0,   0,   8,   0, 
      0,   0,  80,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  15,   0, 
      0,   0,  92,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   0,   0,  15,   0, 
      0,   0,  98,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      2,   0,   0,   0,   3,  12, 
      0,   0,  83,  86,  95,  80, 
     79,  83,  73,  84,  73,  79, 
     78,   0,  67,  79,  76,  79, 
     82,   0,  84,  69,  88,  67, 
     79,  79,  82,  68,   0, 171
};
