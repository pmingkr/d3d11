#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 9.29.952.3111
//
//
//   fxc /nologo /Emain /T vs_4_0_level_9_1 /Fh vshader.h /Vn s_vshader /Od
//    vshader.hlsl
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// POSITION                 0   xyzw        0     NONE  float   xyzw
// TEXCOORD                 0   xy          1     NONE  float   xy  
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// SV_POSITION              0   xyzw        0      POS  float   xyzw
// TEXCOORD                 0   xy          1     NONE  float   xy  
//
//
// Runtime generated constant mappings:
//
// Target Reg                               Constant Description
// ---------- --------------------------------------------------
// c0                              Vertex Shader position offset
//
//
// Level9 shader bytecode:
//
    vs_2_0
    dcl_texcoord v0
    dcl_texcoord1 v1
    mov r0, v0
    mov r1.xy, v1
    mov r0.xy, r0
    mov r0.zw, r0
    mov oT0.xy, r1
    mul r1.xy, r0.w, c0
    add oPos.xy, r0, r1
    mov oPos.zw, r0

// approximately 8 instruction slots used
vs_4_0
dcl_input v0.xyzw
dcl_input v1.xy
dcl_output_siv o0.xyzw, position
dcl_output o1.xy
dcl_temps 2
mov r0.xyzw, v0.xyzw
mov r1.xy, v1.xyxx
mov o0.xyzw, r0.xyzw
mov o1.xy, r1.xyxx
ret 
// Approximately 5 instruction slots used
#endif

const BYTE s_vshader[] =
{
     68,  88,  66,  67,  84, 196, 
    252, 179, 215, 235,  57,  68, 
     35, 198, 226,   3, 132, 202, 
    186, 112,   1,   0,   0,   0, 
     16,   3,   0,   0,   6,   0, 
      0,   0,  56,   0,   0,   0, 
    240,   0,   0,   0, 144,   1, 
      0,   0,  12,   2,   0,   0, 
    100,   2,   0,   0, 184,   2, 
      0,   0,  65, 111, 110,  57, 
    176,   0,   0,   0, 176,   0, 
      0,   0,   0,   2, 254, 255, 
    136,   0,   0,   0,  40,   0, 
      0,   0,   0,   0,  36,   0, 
      0,   0,  36,   0,   0,   0, 
     36,   0,   0,   0,  36,   0, 
      1,   0,  36,   0,   0,   0, 
      0,   0,   0,   2, 254, 255, 
     31,   0,   0,   2,   5,   0, 
      0, 128,   0,   0,  15, 144, 
     31,   0,   0,   2,   5,   0, 
      1, 128,   1,   0,  15, 144, 
      1,   0,   0,   2,   0,   0, 
     15, 128,   0,   0, 228, 144, 
      1,   0,   0,   2,   1,   0, 
      3, 128,   1,   0, 228, 144, 
      1,   0,   0,   2,   0,   0, 
      3, 128,   0,   0, 228, 128, 
      1,   0,   0,   2,   0,   0, 
     12, 128,   0,   0, 228, 128, 
      1,   0,   0,   2,   0,   0, 
      3, 224,   1,   0, 228, 128, 
      5,   0,   0,   3,   1,   0, 
      3, 128,   0,   0, 255, 128, 
      0,   0, 228, 160,   2,   0, 
      0,   3,   0,   0,   3, 192, 
      0,   0, 228, 128,   1,   0, 
    228, 128,   1,   0,   0,   2, 
      0,   0,  12, 192,   0,   0, 
    228, 128, 255, 255,   0,   0, 
     83,  72,  68,  82, 152,   0, 
      0,   0,  64,   0,   1,   0, 
     38,   0,   0,   0,  95,   0, 
      0,   3, 242,  16,  16,   0, 
      0,   0,   0,   0,  95,   0, 
      0,   3,  50,  16,  16,   0, 
      1,   0,   0,   0, 103,   0, 
      0,   4, 242,  32,  16,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0, 101,   0,   0,   3, 
     50,  32,  16,   0,   1,   0, 
      0,   0, 104,   0,   0,   2, 
      2,   0,   0,   0,  54,   0, 
      0,   5, 242,   0,  16,   0, 
      0,   0,   0,   0,  70,  30, 
     16,   0,   0,   0,   0,   0, 
     54,   0,   0,   5,  50,   0, 
     16,   0,   1,   0,   0,   0, 
     70,  16,  16,   0,   1,   0, 
      0,   0,  54,   0,   0,   5, 
    242,  32,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   5,  50,  32,  16,   0, 
      1,   0,   0,   0,  70,   0, 
     16,   0,   1,   0,   0,   0, 
     62,   0,   0,   1,  83,  84, 
     65,  84, 116,   0,   0,   0, 
      5,   0,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  82,  68,  69,  70, 
     80,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  28,   0, 
      0,   0,   0,   4, 254, 255, 
      4,   1,   0,   0,  28,   0, 
      0,   0,  77, 105,  99, 114, 
    111, 115, 111, 102, 116,  32, 
     40,  82,  41,  32,  72,  76, 
     83,  76,  32,  83, 104,  97, 
    100, 101, 114,  32,  67, 111, 
    109, 112, 105, 108, 101, 114, 
     32,  57,  46,  50,  57,  46, 
     57,  53,  50,  46,  51,  49, 
     49,  49,   0, 171, 171, 171, 
     73,  83,  71,  78,  76,   0, 
      0,   0,   2,   0,   0,   0, 
      8,   0,   0,   0,  56,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,  15,   0,   0,  65,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
      3,   3,   0,   0,  80,  79, 
     83,  73,  84,  73,  79,  78, 
      0,  84,  69,  88,  67,  79, 
     79,  82,  68,   0, 171, 171, 
     79,  83,  71,  78,  80,   0, 
      0,   0,   2,   0,   0,   0, 
      8,   0,   0,   0,  56,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   0,   0,   0,  68,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
      3,  12,   0,   0,  83,  86, 
     95,  80,  79,  83,  73,  84, 
     73,  79,  78,   0,  84,  69, 
     88,  67,  79,  79,  82,  68, 
      0, 171, 171, 171
};