// 
// nullgl.h from Erik Wilson -- Boston University
//  This file can be used with OpenGl is not available
//
#ifndef NULLGL_H
#define NULLGL_H

typedef unsigned int   GLenum;
typedef unsigned char  GLboolean;
typedef unsigned int   GLbitfield;
typedef void           GLvoid;
typedef signed char    GLbyte;         /* 1-byte signed */
typedef short          GLshort;        /* 2-byte signed */
typedef int            GLint;          /* 4-byte signed */
typedef unsigned char  GLubyte;        /* 1-byte unsigned */
typedef unsigned short GLushort;       /* 2-byte unsigned */
typedef unsigned int   GLuint;         /* 4-byte unsigned */
typedef int            GLsizei;        /* 4-byte signed */
typedef float          GLfloat;        /* single precision float */
typedef float          GLclampf;       /* single precision float in [0,1] */
typedef double         GLdouble;       /* double precision float */
typedef double         GLclampd;       /* double precision float in [0,1] */

#define GL_VERSION_1_1   1
#define GL_VERSION_1_2   1
#define GL_VERSION_1_3   1
#define GL_ARB_imaging   1
#define GL_FALSE                               0x0
#define GL_TRUE                                        0x1
#define GL_BYTE                                        0x1400
#define GL_UNSIGNED_BYTE                       0x1401
#define GL_SHORT                               0x1402
#define GL_UNSIGNED_SHORT                      0x1403
#define GL_INT                                 0x1404
#define GL_UNSIGNED_INT                                0x1405
#define GL_FLOAT                               0x1406
#define GL_2_BYTES                             0x1407
#define GL_3_BYTES                             0x1408
#define GL_4_BYTES                             0x1409
#define GL_DOUBLE                              0x140A
#define GL_POINTS                              0x0000
#define GL_LINES                               0x0001
#define GL_LINE_LOOP                           0x0002
#define GL_LINE_STRIP                          0x0003
#define GL_TRIANGLES                           0x0004
#define GL_TRIANGLE_STRIP                      0x0005
#define GL_TRIANGLE_FAN                                0x0006
#define GL_QUADS                               0x0007
#define GL_QUAD_STRIP                          0x0008
#define GL_POLYGON                             0x0009
#define GL_VERTEX_ARRAY                                0x8074
#define GL_NORMAL_ARRAY                                0x8075
#define GL_COLOR_ARRAY                         0x8076
#define GL_INDEX_ARRAY                         0x8077
#define GL_TEXTURE_COORD_ARRAY                 0x8078
#define GL_EDGE_FLAG_ARRAY                     0x8079
#define GL_VERTEX_ARRAY_SIZE                   0x807A
#define GL_VERTEX_ARRAY_TYPE                   0x807B
#define GL_VERTEX_ARRAY_STRIDE                 0x807C
#define GL_NORMAL_ARRAY_TYPE                   0x807E
#define GL_NORMAL_ARRAY_STRIDE                 0x807F
#define GL_COLOR_ARRAY_SIZE                    0x8081
#define GL_COLOR_ARRAY_TYPE                    0x8082
#define GL_COLOR_ARRAY_STRIDE                  0x8083
#define GL_INDEX_ARRAY_TYPE                    0x8085
#define GL_INDEX_ARRAY_STRIDE                  0x8086
#define GL_TEXTURE_COORD_ARRAY_SIZE            0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE            0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE          0x808A
#define GL_EDGE_FLAG_ARRAY_STRIDE              0x808C
#define GL_VERTEX_ARRAY_POINTER                        0x808E
#define GL_NORMAL_ARRAY_POINTER                        0x808F
#define GL_COLOR_ARRAY_POINTER                 0x8090
#define GL_INDEX_ARRAY_POINTER                 0x8091
#define GL_TEXTURE_COORD_ARRAY_POINTER         0x8092
#define GL_EDGE_FLAG_ARRAY_POINTER             0x8093
#define GL_V2F                                 0x2A20
#define GL_V3F                                 0x2A21
#define GL_C4UB_V2F                            0x2A22
#define GL_C4UB_V3F                            0x2A23
#define GL_C3F_V3F                             0x2A24
#define GL_N3F_V3F                             0x2A25
#define GL_C4F_N3F_V3F                         0x2A26
#define GL_T2F_V3F                             0x2A27
#define GL_T4F_V4F                             0x2A28
#define GL_T2F_C4UB_V3F                                0x2A29
#define GL_T2F_C3F_V3F                         0x2A2A
#define GL_T2F_N3F_V3F                         0x2A2B
#define GL_T2F_C4F_N3F_V3F                     0x2A2C
#define GL_T4F_C4F_N3F_V4F                     0x2A2D
#define GL_MATRIX_MODE                         0x0BA0
#define GL_MODELVIEW                           0x1700
#define GL_PROJECTION                          0x1701
#define GL_TEXTURE                             0x1702
#define GL_POINT_SMOOTH                                0x0B10
#define GL_POINT_SIZE                          0x0B11
#define GL_POINT_SIZE_GRANULARITY              0x0B13
#define GL_POINT_SIZE_RANGE                    0x0B12
#define GL_LINE_SMOOTH                         0x0B20
#define GL_LINE_STIPPLE                                0x0B24
#define GL_LINE_STIPPLE_PATTERN                        0x0B25
#define GL_LINE_STIPPLE_REPEAT                 0x0B26
#define GL_LINE_WIDTH                          0x0B21
#define GL_LINE_WIDTH_GRANULARITY              0x0B23
#define GL_LINE_WIDTH_RANGE                    0x0B22
#define GL_POINT                               0x1B00
#define GL_LINE                                        0x1B01
#define GL_FILL                                        0x1B02
#define GL_CW                                  0x0900
#define GL_CCW                                 0x0901
#define GL_FRONT                               0x0404
#define GL_BACK                                        0x0405
#define GL_POLYGON_MODE                                0x0B40
#define GL_POLYGON_SMOOTH                      0x0B41
#define GL_POLYGON_STIPPLE                     0x0B42
#define GL_EDGE_FLAG                           0x0B43
#define GL_CULL_FACE                           0x0B44
#define GL_CULL_FACE_MODE                      0x0B45
#define GL_FRONT_FACE                          0x0B46
#define GL_POLYGON_OFFSET_FACTOR               0x8038
#define GL_POLYGON_OFFSET_UNITS                        0x2A00
#define GL_POLYGON_OFFSET_POINT                        0x2A01
#define GL_POLYGON_OFFSET_LINE                 0x2A02
#define GL_POLYGON_OFFSET_FILL                 0x8037
#define GL_COMPILE                             0x1300
#define GL_COMPILE_AND_EXECUTE                 0x1301
#define GL_LIST_BASE                           0x0B32
#define GL_LIST_INDEX                          0x0B33
#define GL_LIST_MODE                           0x0B30
#define GL_NEVER                               0x0200
#define GL_LESS                                        0x0201
#define GL_EQUAL                               0x0202
#define GL_LEQUAL                              0x0203
#define GL_GREATER                             0x0204
#define GL_NOTEQUAL                            0x0205
#define GL_GEQUAL                              0x0206
#define GL_ALWAYS                              0x0207
#define GL_DEPTH_TEST                          0x0B71
#define GL_DEPTH_BITS                          0x0D56
#define GL_DEPTH_CLEAR_VALUE                   0x0B73
#define GL_DEPTH_FUNC                          0x0B74
#define GL_DEPTH_RANGE                         0x0B70
#define GL_DEPTH_WRITEMASK                     0x0B72
#define GL_DEPTH_COMPONENT                     0x1902
#define GL_LIGHTING                            0x0B50
#define GL_LIGHT0                              0x4000
#define GL_LIGHT1                              0x4001
#define GL_LIGHT2                              0x4002
#define GL_LIGHT3                              0x4003
#define GL_LIGHT4                              0x4004
#define GL_LIGHT5                              0x4005
#define GL_LIGHT6                              0x4006
#define GL_LIGHT7                              0x4007
#define GL_SPOT_EXPONENT                       0x1205
#define GL_SPOT_CUTOFF                         0x1206
#define GL_CONSTANT_ATTENUATION                        0x1207
#define GL_LINEAR_ATTENUATION                  0x1208
#define GL_QUADRATIC_ATTENUATION               0x1209
#define GL_AMBIENT                             0x1200
#define GL_DIFFUSE                             0x1201
#define GL_SPECULAR                            0x1202
#define GL_SHININESS                           0x1601
#define GL_EMISSION                            0x1600
#define GL_POSITION                            0x1203
#define GL_SPOT_DIRECTION                      0x1204
#define GL_AMBIENT_AND_DIFFUSE                 0x1602
#define GL_COLOR_INDEXES                       0x1603
#define GL_LIGHT_MODEL_TWO_SIDE                        0x0B52
#define GL_LIGHT_MODEL_LOCAL_VIEWER            0x0B51
#define GL_LIGHT_MODEL_AMBIENT                 0x0B53
#define GL_FRONT_AND_BACK                      0x0408
#define GL_SHADE_MODEL                         0x0B54
#define GL_FLAT                                        0x1D00
#define GL_SMOOTH                              0x1D01
#define GL_COLOR_MATERIAL                      0x0B57
#define GL_COLOR_MATERIAL_FACE                 0x0B55
#define GL_COLOR_MATERIAL_PARAMETER            0x0B56
#define GL_NORMALIZE                           0x0BA1
#define GL_CLIP_PLANE0                         0x3000
#define GL_CLIP_PLANE1                         0x3001
#define GL_CLIP_PLANE2                         0x3002
#define GL_CLIP_PLANE3                         0x3003
#define GL_CLIP_PLANE4                         0x3004
#define GL_CLIP_PLANE5                         0x3005
#define GL_ACCUM_RED_BITS                      0x0D58
#define GL_ACCUM_GREEN_BITS                    0x0D59
#define GL_ACCUM_BLUE_BITS                     0x0D5A
#define GL_ACCUM_ALPHA_BITS                    0x0D5B
#define GL_ACCUM_CLEAR_VALUE                   0x0B80
#define GL_ACCUM                               0x0100
#define GL_ADD                                 0x0104
#define GL_LOAD                                        0x0101
#define GL_MULT                                        0x0103
#define GL_RETURN                              0x0102
#define GL_ALPHA_TEST                          0x0BC0
#define GL_ALPHA_TEST_REF                      0x0BC2
#define GL_ALPHA_TEST_FUNC                     0x0BC1
#define GL_BLEND                               0x0BE2
#define GL_BLEND_SRC                           0x0BE1
#define GL_BLEND_DST                           0x0BE0
#define GL_ZERO                                        0x0
#define GL_ONE                                 0x1
#define GL_SRC_COLOR                           0x0300
#define GL_ONE_MINUS_SRC_COLOR                 0x0301
#define GL_SRC_ALPHA                           0x0302
#define GL_ONE_MINUS_SRC_ALPHA                 0x0303
#define GL_DST_ALPHA                           0x0304
#define GL_ONE_MINUS_DST_ALPHA                 0x0305
#define GL_DST_COLOR                           0x0306
#define GL_ONE_MINUS_DST_COLOR                 0x0307
#define GL_SRC_ALPHA_SATURATE                  0x0308
#define GL_FEEDBACK                            0x1C01
#define GL_RENDER                              0x1C00
#define GL_SELECT                              0x1C02
#define GL_2D                                  0x0600
#define GL_3D                                  0x0601
#define GL_3D_COLOR                            0x0602
#define GL_3D_COLOR_TEXTURE                    0x0603
#define GL_4D_COLOR_TEXTURE                    0x0604
#define GL_POINT_TOKEN                         0x0701
#define GL_LINE_TOKEN                          0x0702
#define GL_LINE_RESET_TOKEN                    0x0707
#define GL_POLYGON_TOKEN                       0x0703
#define GL_BITMAP_TOKEN                                0x0704
#define GL_DRAW_PIXEL_TOKEN                    0x0705
#define GL_COPY_PIXEL_TOKEN                    0x0706
#define GL_PASS_THROUGH_TOKEN                  0x0700
#define GL_FEEDBACK_BUFFER_POINTER             0x0DF0
#define GL_FEEDBACK_BUFFER_SIZE                        0x0DF1
#define GL_FEEDBACK_BUFFER_TYPE                        0x0DF2
#define GL_SELECTION_BUFFER_POINTER            0x0DF3
#define GL_SELECTION_BUFFER_SIZE               0x0DF4
#define GL_FOG                                 0x0B60
#define GL_FOG_MODE                            0x0B65
#define GL_FOG_DENSITY                         0x0B62
#define GL_FOG_COLOR                           0x0B66
#define GL_FOG_INDEX                           0x0B61
#define GL_FOG_START                           0x0B63
#define GL_FOG_END                             0x0B64
#define GL_LINEAR                              0x2601
#define GL_EXP                                 0x0800
#define GL_EXP2                                        0x0801
#define GL_LOGIC_OP                            0x0BF1
#define GL_INDEX_LOGIC_OP                      0x0BF1
#define GL_COLOR_LOGIC_OP                      0x0BF2
#define GL_LOGIC_OP_MODE                       0x0BF0
#define GL_CLEAR                               0x1500
#define GL_SET                                 0x150F
#define GL_COPY                                        0x1503
#define GL_COPY_INVERTED                       0x150C
#define GL_NOOP                                        0x1505
#define GL_INVERT                              0x150A
#define GL_AND                                 0x1501
#define GL_NAND                                        0x150E
#define GL_OR                                  0x1507
#define GL_NOR                                 0x1508
#define GL_XOR                                 0x1506
#define GL_EQUIV                               0x1509
#define GL_AND_REVERSE                         0x1502
#define GL_AND_INVERTED                                0x1504
#define GL_OR_REVERSE                          0x150B
#define GL_OR_INVERTED                         0x150D
#define GL_STENCIL_BITS                                0x0D57
#define GL_STENCIL_TEST                                0x0B90
#define GL_STENCIL_CLEAR_VALUE                 0x0B91
#define GL_STENCIL_FUNC                                0x0B92
#define GL_STENCIL_VALUE_MASK                  0x0B93
#define GL_STENCIL_FAIL                                0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL             0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS             0x0B96
#define GL_STENCIL_REF                         0x0B97
#define GL_STENCIL_WRITEMASK                   0x0B98
#define GL_STENCIL_INDEX                       0x1901
#define GL_KEEP                                        0x1E00
#define GL_REPLACE                             0x1E01
#define GL_INCR                                        0x1E02
#define GL_DECR                                        0x1E03
#define GL_NONE                                        0x0
#define GL_LEFT                                        0x0406
#define GL_RIGHT                               0x0407
#define GL_FRONT_LEFT                          0x0400
#define GL_FRONT_RIGHT                         0x0401
#define GL_BACK_LEFT                           0x0402
#define GL_BACK_RIGHT                          0x0403
#define GL_AUX0                                        0x0409
#define GL_AUX1                                        0x040A
#define GL_AUX2                                        0x040B
#define GL_AUX3                                        0x040C
#define GL_COLOR_INDEX                         0x1900
#define GL_RED                                 0x1903
#define GL_GREEN                               0x1904
#define GL_BLUE                                        0x1905
#define GL_ALPHA                               0x1906
#define GL_LUMINANCE                           0x1909
#define GL_LUMINANCE_ALPHA                     0x190A
#define GL_ALPHA_BITS                          0x0D55
#define GL_RED_BITS                            0x0D52
#define GL_GREEN_BITS                          0x0D53
#define GL_BLUE_BITS                           0x0D54
#define GL_INDEX_BITS                          0x0D51
#define GL_SUBPIXEL_BITS                       0x0D50
#define GL_AUX_BUFFERS                         0x0C00
#define GL_READ_BUFFER                         0x0C02
#define GL_DRAW_BUFFER                         0x0C01
#define GL_DOUBLEBUFFER                                0x0C32
#define GL_STEREO                              0x0C33
#define GL_BITMAP                              0x1A00
#define GL_COLOR                               0x1800
#define GL_DEPTH                               0x1801
#define GL_STENCIL                             0x1802
#define GL_DITHER                              0x0BD0
#define GL_RGB                                 0x1907
#define GL_RGBA                                        0x1908
#define GL_MAX_LIST_NESTING                    0x0B31
#define GL_MAX_EVAL_ORDER                      0x0D30
#define GL_MAX_LIGHTS                          0x0D31
#define GL_MAX_CLIP_PLANES                     0x0D32
#define GL_MAX_TEXTURE_SIZE                    0x0D33
#define GL_MAX_PIXEL_MAP_TABLE                 0x0D34
#define GL_MAX_ATTRIB_STACK_DEPTH              0x0D35
#define GL_MAX_MODELVIEW_STACK_DEPTH           0x0D36
#define GL_MAX_NAME_STACK_DEPTH                        0x0D37
#define GL_MAX_PROJECTION_STACK_DEPTH          0x0D38
#define GL_MAX_TEXTURE_STACK_DEPTH             0x0D39
#define GL_MAX_VIEWPORT_DIMS                   0x0D3A
#define GL_MAX_CLIENT_ATTRIB_STACK_DEPTH       0x0D3B
#define GL_ATTRIB_STACK_DEPTH                  0x0BB0
#define GL_CLIENT_ATTRIB_STACK_DEPTH           0x0BB1
#define GL_COLOR_CLEAR_VALUE                   0x0C22
#define GL_COLOR_WRITEMASK                     0x0C23
#define GL_CURRENT_INDEX                       0x0B01
#define GL_CURRENT_COLOR                       0x0B00
#define GL_CURRENT_NORMAL                      0x0B02
#define GL_CURRENT_RASTER_COLOR                        0x0B04
#define GL_CURRENT_RASTER_DISTANCE             0x0B09
#define GL_CURRENT_RASTER_INDEX                        0x0B05
#define GL_CURRENT_RASTER_POSITION             0x0B07
#define GL_CURRENT_RASTER_TEXTURE_COORDS       0x0B06
#define GL_CURRENT_RASTER_POSITION_VALID       0x0B08
#define GL_CURRENT_TEXTURE_COORDS              0x0B03
#define GL_INDEX_CLEAR_VALUE                   0x0C20
#define GL_INDEX_MODE                          0x0C30
#define GL_INDEX_WRITEMASK                     0x0C21
#define GL_MODELVIEW_MATRIX                    0x0BA6
#define GL_MODELVIEW_STACK_DEPTH               0x0BA3
#define GL_NAME_STACK_DEPTH                    0x0D70
#define GL_PROJECTION_MATRIX                   0x0BA7
#define GL_PROJECTION_STACK_DEPTH              0x0BA4
#define GL_RENDER_MODE                         0x0C40
#define GL_RGBA_MODE                           0x0C31
#define GL_TEXTURE_MATRIX                      0x0BA8
#define GL_TEXTURE_STACK_DEPTH                 0x0BA5
#define GL_VIEWPORT                            0x0BA2
#define GL_AUTO_NORMAL                         0x0D80
#define GL_MAP1_COLOR_4                                0x0D90
#define GL_MAP1_INDEX                          0x0D91
#define GL_MAP1_NORMAL                         0x0D92
#define GL_MAP1_TEXTURE_COORD_1                        0x0D93
#define GL_MAP1_TEXTURE_COORD_2                        0x0D94
#define GL_MAP1_TEXTURE_COORD_3                        0x0D95
#define GL_MAP1_TEXTURE_COORD_4                        0x0D96
#define GL_MAP1_VERTEX_3                       0x0D97
#define GL_MAP1_VERTEX_4                       0x0D98
#define GL_MAP2_COLOR_4                                0x0DB0
#define GL_MAP2_INDEX                          0x0DB1
#define GL_MAP2_NORMAL                         0x0DB2
#define GL_MAP2_TEXTURE_COORD_1                        0x0DB3
#define GL_MAP2_TEXTURE_COORD_2                        0x0DB4
#define GL_MAP2_TEXTURE_COORD_3                        0x0DB5
#define GL_MAP2_TEXTURE_COORD_4                        0x0DB6
#define GL_MAP2_VERTEX_3                       0x0DB7
#define GL_MAP2_VERTEX_4                       0x0DB8
#define GL_MAP1_GRID_DOMAIN                    0x0DD0
#define GL_MAP1_GRID_SEGMENTS                  0x0DD1
#define GL_MAP2_GRID_DOMAIN                    0x0DD2
#define GL_MAP2_GRID_SEGMENTS                  0x0DD3
#define GL_COEFF                               0x0A00
#define GL_ORDER                               0x0A01
#define GL_DOMAIN                              0x0A02
#define GL_PERSPECTIVE_CORRECTION_HINT         0x0C50
#define GL_POINT_SMOOTH_HINT                   0x0C51
#define GL_LINE_SMOOTH_HINT                    0x0C52
#define GL_POLYGON_SMOOTH_HINT                 0x0C53
#define GL_FOG_HINT                            0x0C54
#define GL_DONT_CARE                           0x1100
#define GL_FASTEST                             0x1101
#define GL_NICEST                              0x1102
#define GL_SCISSOR_BOX                         0x0C10
#define GL_SCISSOR_TEST                                0x0C11
#define GL_MAP_COLOR                           0x0D10
#define GL_MAP_STENCIL                         0x0D11
#define GL_INDEX_SHIFT                         0x0D12
#define GL_INDEX_OFFSET                                0x0D13
#define GL_RED_SCALE                           0x0D14
#define GL_RED_BIAS                            0x0D15
#define GL_GREEN_SCALE                         0x0D18
#define GL_GREEN_BIAS                          0x0D19
#define GL_BLUE_SCALE                          0x0D1A
#define GL_BLUE_BIAS                           0x0D1B
#define GL_ALPHA_SCALE                         0x0D1C
#define GL_ALPHA_BIAS                          0x0D1D
#define GL_DEPTH_SCALE                         0x0D1E
#define GL_DEPTH_BIAS                          0x0D1F
#define GL_PIXEL_MAP_S_TO_S_SIZE               0x0CB1
#define GL_PIXEL_MAP_I_TO_I_SIZE               0x0CB0
#define GL_PIXEL_MAP_I_TO_R_SIZE               0x0CB2
#define GL_PIXEL_MAP_I_TO_G_SIZE               0x0CB3
#define GL_PIXEL_MAP_I_TO_B_SIZE               0x0CB4
#define GL_PIXEL_MAP_I_TO_A_SIZE               0x0CB5
#define GL_PIXEL_MAP_R_TO_R_SIZE               0x0CB6
#define GL_PIXEL_MAP_G_TO_G_SIZE               0x0CB7
#define GL_PIXEL_MAP_B_TO_B_SIZE               0x0CB8
#define GL_PIXEL_MAP_A_TO_A_SIZE               0x0CB9
#define GL_PIXEL_MAP_S_TO_S                    0x0C71
#define GL_PIXEL_MAP_I_TO_I                    0x0C70
#define GL_PIXEL_MAP_I_TO_R                    0x0C72
#define GL_PIXEL_MAP_I_TO_G                    0x0C73
#define GL_PIXEL_MAP_I_TO_B                    0x0C74
#define GL_PIXEL_MAP_I_TO_A                    0x0C75
#define GL_PIXEL_MAP_R_TO_R                    0x0C76
#define GL_PIXEL_MAP_G_TO_G                    0x0C77
#define GL_PIXEL_MAP_B_TO_B                    0x0C78
#define GL_PIXEL_MAP_A_TO_A                    0x0C79
#define GL_PACK_ALIGNMENT                      0x0D05
#define GL_PACK_LSB_FIRST                      0x0D01
#define GL_PACK_ROW_LENGTH                     0x0D02
#define GL_PACK_SKIP_PIXELS                    0x0D04
#define GL_PACK_SKIP_ROWS                      0x0D03
#define GL_PACK_SWAP_BYTES                     0x0D00
#define GL_UNPACK_ALIGNMENT                    0x0CF5
#define GL_UNPACK_LSB_FIRST                    0x0CF1
#define GL_UNPACK_ROW_LENGTH                   0x0CF2
#define GL_UNPACK_SKIP_PIXELS                  0x0CF4
#define GL_UNPACK_SKIP_ROWS                    0x0CF3
#define GL_UNPACK_SWAP_BYTES                   0x0CF0
#define GL_ZOOM_X                              0x0D16
#define GL_ZOOM_Y                              0x0D17
#define GL_TEXTURE_ENV                         0x2300
#define GL_TEXTURE_ENV_MODE                    0x2200
#define GL_TEXTURE_1D                          0x0DE0
#define GL_TEXTURE_2D                          0x0DE1
#define GL_TEXTURE_WRAP_S                      0x2802
#define GL_TEXTURE_WRAP_T                      0x2803
#define GL_TEXTURE_MAG_FILTER                  0x2800
#define GL_TEXTURE_MIN_FILTER                  0x2801
#define GL_TEXTURE_ENV_COLOR                   0x2201
#define GL_TEXTURE_GEN_S                       0x0C60
#define GL_TEXTURE_GEN_T                       0x0C61
#define GL_TEXTURE_GEN_MODE                    0x2500
#define GL_TEXTURE_BORDER_COLOR                        0x1004
#define GL_TEXTURE_WIDTH                       0x1000
#define GL_TEXTURE_HEIGHT                      0x1001
#define GL_TEXTURE_BORDER                      0x1005
#define GL_TEXTURE_COMPONENTS                  0x1003
#define GL_TEXTURE_RED_SIZE                    0x805C
#define GL_TEXTURE_GREEN_SIZE                  0x805D
#define GL_TEXTURE_BLUE_SIZE                   0x805E
#define GL_TEXTURE_ALPHA_SIZE                  0x805F
#define GL_TEXTURE_LUMINANCE_SIZE              0x8060
#define GL_TEXTURE_INTENSITY_SIZE              0x8061
#define GL_NEAREST_MIPMAP_NEAREST              0x2700
#define GL_NEAREST_MIPMAP_LINEAR               0x2702
#define GL_LINEAR_MIPMAP_NEAREST               0x2701
#define GL_LINEAR_MIPMAP_LINEAR                        0x2703
#define GL_OBJECT_LINEAR                       0x2401
#define GL_OBJECT_PLANE                                0x2501
#define GL_EYE_LINEAR                          0x2400
#define GL_EYE_PLANE                           0x2502
#define GL_SPHERE_MAP                          0x2402
#define GL_DECAL                               0x2101
#define GL_MODULATE                            0x2100
#define GL_NEAREST                             0x2600
#define GL_REPEAT                              0x2901
#define GL_CLAMP                               0x2900
#define GL_S                                   0x2000
#define GL_T                                   0x2001
#define GL_R                                   0x2002
#define GL_Q                                   0x2003
#define GL_TEXTURE_GEN_R                       0x0C62
#define GL_TEXTURE_GEN_Q                       0x0C63
#define GL_VENDOR                              0x1F00
#define GL_RENDERER                            0x1F01
#define GL_VERSION                             0x1F02
#define GL_EXTENSIONS                          0x1F03
#define GL_NO_ERROR                            0x0
#define GL_INVALID_ENUM                                0x0500
#define GL_INVALID_VALUE                       0x0501
#define GL_INVALID_OPERATION                   0x0502
#define GL_STACK_OVERFLOW                      0x0503
#define GL_STACK_UNDERFLOW                     0x0504
#define GL_OUT_OF_MEMORY                       0x0505
#define GL_CURRENT_BIT                         0x00000001
#define GL_POINT_BIT                           0x00000002
#define GL_LINE_BIT                            0x00000004
#define GL_POLYGON_BIT                         0x00000008
#define GL_POLYGON_STIPPLE_BIT                 0x00000010
#define GL_PIXEL_MODE_BIT                      0x00000020
#define GL_LIGHTING_BIT                                0x00000040
#define GL_FOG_BIT                             0x00000080
#define GL_DEPTH_BUFFER_BIT                    0x00000100
#define GL_ACCUM_BUFFER_BIT                    0x00000200
#define GL_STENCIL_BUFFER_BIT                  0x00000400
#define GL_VIEWPORT_BIT                                0x00000800
#define GL_TRANSFORM_BIT                       0x00001000
#define GL_ENABLE_BIT                          0x00002000
#define GL_COLOR_BUFFER_BIT                    0x00004000
#define GL_HINT_BIT                            0x00008000
#define GL_EVAL_BIT                            0x00010000
#define GL_LIST_BIT                            0x00020000
#define GL_TEXTURE_BIT                         0x00040000
#define GL_SCISSOR_BIT                         0x00080000
#define GL_ALL_ATTRIB_BITS                     0x000FFFFF
#define GL_PROXY_TEXTURE_1D                    0x8063
#define GL_PROXY_TEXTURE_2D                    0x8064
#define GL_TEXTURE_PRIORITY                    0x8066
#define GL_TEXTURE_RESIDENT                    0x8067
#define GL_TEXTURE_BINDING_1D                  0x8068
#define GL_TEXTURE_BINDING_2D                  0x8069
#define GL_TEXTURE_INTERNAL_FORMAT             0x1003
#define GL_ALPHA4                              0x803B
#define GL_ALPHA8                              0x803C
#define GL_ALPHA12                             0x803D
#define GL_ALPHA16                             0x803E
#define GL_LUMINANCE4                          0x803F
#define GL_LUMINANCE8                          0x8040
#define GL_LUMINANCE12                         0x8041
#define GL_LUMINANCE16                         0x8042
#define GL_LUMINANCE4_ALPHA4                   0x8043
#define GL_LUMINANCE6_ALPHA2                   0x8044
#define GL_LUMINANCE8_ALPHA8                   0x8045
#define GL_LUMINANCE12_ALPHA4                  0x8046
#define GL_LUMINANCE12_ALPHA12                 0x8047
#define GL_LUMINANCE16_ALPHA16                 0x8048
#define GL_INTENSITY                           0x8049
#define GL_INTENSITY4                          0x804A
#define GL_INTENSITY8                          0x804B
#define GL_INTENSITY12                         0x804C
#define GL_INTENSITY16                         0x804D
#define GL_R3_G3_B2                            0x2A10
#define GL_RGB4                                        0x804F
#define GL_RGB5                                        0x8050
#define GL_RGB8                                        0x8051
#define GL_RGB10                               0x8052
#define GL_RGB12                               0x8053
#define GL_RGB16                               0x8054
#define GL_RGBA2                               0x8055
#define GL_RGBA4                               0x8056
#define GL_RGB5_A1                             0x8057
#define GL_RGBA8                               0x8058
#define GL_RGB10_A2                            0x8059
#define GL_RGBA12                              0x805A
#define GL_RGBA16                              0x805B
#define GL_CLIENT_PIXEL_STORE_BIT              0x00000001
#define GL_CLIENT_VERTEX_ARRAY_BIT             0x00000002
#define GL_ALL_CLIENT_ATTRIB_BITS              0xFFFFFFFF
#define GL_CLIENT_ALL_ATTRIB_BITS              0xFFFFFFFF
#define GL_RESCALE_NORMAL                      0x803A
#define GL_CLAMP_TO_EDGE                       0x812F
#define GL_MAX_ELEMENTS_VERTICES               0x80E8
#define GL_MAX_ELEMENTS_INDICES                        0x80E9
#define GL_BGR                                 0x80E0
#define GL_BGRA                                        0x80E1
#define GL_UNSIGNED_BYTE_3_3_2                 0x8032
#define GL_UNSIGNED_BYTE_2_3_3_REV             0x8362
#define GL_UNSIGNED_SHORT_5_6_5                        0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV            0x8364
#define GL_UNSIGNED_SHORT_4_4_4_4              0x8033
#define GL_UNSIGNED_SHORT_4_4_4_4_REV          0x8365
#define GL_UNSIGNED_SHORT_5_5_5_1              0x8034
#define GL_UNSIGNED_SHORT_1_5_5_5_REV          0x8366
#define GL_UNSIGNED_INT_8_8_8_8                        0x8035
#define GL_UNSIGNED_INT_8_8_8_8_REV            0x8367
#define GL_UNSIGNED_INT_10_10_10_2             0x8036
#define GL_UNSIGNED_INT_2_10_10_10_REV         0x8368
#define GL_LIGHT_MODEL_COLOR_CONTROL           0x81F8
#define GL_SINGLE_COLOR                                0x81F9
#define GL_SEPARATE_SPECULAR_COLOR             0x81FA
#define GL_TEXTURE_MIN_LOD                     0x813A
#define GL_TEXTURE_MAX_LOD                     0x813B
#define GL_TEXTURE_BASE_LEVEL                  0x813C
#define GL_TEXTURE_MAX_LEVEL                   0x813D
#define GL_SMOOTH_POINT_SIZE_RANGE             0x0B12
#define GL_SMOOTH_POINT_SIZE_GRANULARITY       0x0B13
#define GL_SMOOTH_LINE_WIDTH_RANGE             0x0B22
#define GL_SMOOTH_LINE_WIDTH_GRANULARITY       0x0B23
#define GL_ALIASED_POINT_SIZE_RANGE            0x846D
#define GL_ALIASED_LINE_WIDTH_RANGE            0x846E
#define GL_PACK_SKIP_IMAGES                    0x806B
#define GL_PACK_IMAGE_HEIGHT                   0x806C
#define GL_UNPACK_SKIP_IMAGES                  0x806D
#define GL_UNPACK_IMAGE_HEIGHT                 0x806E
#define GL_TEXTURE_3D                          0x806F
#define GL_PROXY_TEXTURE_3D                    0x8070
#define GL_TEXTURE_DEPTH                       0x8071
#define GL_TEXTURE_WRAP_R                      0x8072
#define GL_MAX_3D_TEXTURE_SIZE                 0x8073
#define GL_TEXTURE_BINDING_3D                  0x806A
#define GL_CONSTANT_COLOR                      0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR            0x8002
#define GL_CONSTANT_ALPHA                      0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA            0x8004
#define GL_COLOR_TABLE                         0x80D0
#define GL_POST_CONVOLUTION_COLOR_TABLE                0x80D1
#define GL_POST_COLOR_MATRIX_COLOR_TABLE       0x80D2
#define GL_PROXY_COLOR_TABLE                   0x80D3
#define GL_PROXY_POST_CONVOLUTION_COLOR_TABLE  0x80D4
#define GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE 0x80D5
#define GL_COLOR_TABLE_SCALE                   0x80D6
#define GL_COLOR_TABLE_BIAS                    0x80D7
#define GL_COLOR_TABLE_FORMAT                  0x80D8
#define GL_COLOR_TABLE_WIDTH                   0x80D9
#define GL_COLOR_TABLE_RED_SIZE                        0x80DA
#define GL_COLOR_TABLE_GREEN_SIZE              0x80DB
#define GL_COLOR_TABLE_BLUE_SIZE               0x80DC
#define GL_COLOR_TABLE_ALPHA_SIZE              0x80DD
#define GL_COLOR_TABLE_LUMINANCE_SIZE          0x80DE
#define GL_COLOR_TABLE_INTENSITY_SIZE          0x80DF
#define GL_CONVOLUTION_1D                      0x8010
#define GL_CONVOLUTION_2D                      0x8011
#define GL_SEPARABLE_2D                                0x8012
#define GL_CONVOLUTION_BORDER_MODE             0x8013
#define GL_CONVOLUTION_FILTER_SCALE            0x8014
#define GL_CONVOLUTION_FILTER_BIAS             0x8015
#define GL_REDUCE                              0x8016
#define GL_CONVOLUTION_FORMAT                  0x8017
#define GL_CONVOLUTION_WIDTH                   0x8018
#define GL_CONVOLUTION_HEIGHT                  0x8019
#define GL_MAX_CONVOLUTION_WIDTH               0x801A
#define GL_MAX_CONVOLUTION_HEIGHT              0x801B
#define GL_POST_CONVOLUTION_RED_SCALE          0x801C
#define GL_POST_CONVOLUTION_GREEN_SCALE                0x801D
#define GL_POST_CONVOLUTION_BLUE_SCALE         0x801E
#define GL_POST_CONVOLUTION_ALPHA_SCALE                0x801F
#define GL_POST_CONVOLUTION_RED_BIAS           0x8020
#define GL_POST_CONVOLUTION_GREEN_BIAS         0x8021
#define GL_POST_CONVOLUTION_BLUE_BIAS          0x8022
#define GL_POST_CONVOLUTION_ALPHA_BIAS         0x8023
#define GL_CONSTANT_BORDER                     0x8151
#define GL_REPLICATE_BORDER                    0x8153
#define GL_CONVOLUTION_BORDER_COLOR            0x8154
#define GL_COLOR_MATRIX                                0x80B1
#define GL_COLOR_MATRIX_STACK_DEPTH            0x80B2
#define GL_MAX_COLOR_MATRIX_STACK_DEPTH                0x80B3
#define GL_POST_COLOR_MATRIX_RED_SCALE         0x80B4
#define GL_POST_COLOR_MATRIX_GREEN_SCALE       0x80B5
#define GL_POST_COLOR_MATRIX_BLUE_SCALE                0x80B6
#define GL_POST_COLOR_MATRIX_ALPHA_SCALE       0x80B7
#define GL_POST_COLOR_MATRIX_RED_BIAS          0x80B8
#define GL_POST_COLOR_MATRIX_GREEN_BIAS                0x80B9
#define GL_POST_COLOR_MATRIX_BLUE_BIAS         0x80BA
#define GL_POST_COLOR_MATRIX_ALPHA_BIAS                0x80BB
#define GL_HISTOGRAM                           0x8024
#define GL_PROXY_HISTOGRAM                     0x8025
#define GL_HISTOGRAM_WIDTH                     0x8026
#define GL_HISTOGRAM_FORMAT                    0x8027
#define GL_HISTOGRAM_RED_SIZE                  0x8028
#define GL_HISTOGRAM_GREEN_SIZE                        0x8029
#define GL_HISTOGRAM_BLUE_SIZE                 0x802A
#define GL_HISTOGRAM_ALPHA_SIZE                        0x802B
#define GL_HISTOGRAM_LUMINANCE_SIZE            0x802C
#define GL_HISTOGRAM_SINK                      0x802D
#define GL_MINMAX                              0x802E
#define GL_MINMAX_FORMAT                       0x802F
#define GL_MINMAX_SINK                         0x8030
#define GL_TABLE_TOO_LARGE                     0x8031
#define GL_BLEND_EQUATION                      0x8009
#define GL_MIN                                 0x8007
#define GL_MAX                                 0x8008
#define GL_FUNC_ADD                            0x8006
#define GL_FUNC_SUBTRACT                       0x800A
#define GL_FUNC_REVERSE_SUBTRACT               0x800B
#define GL_BLEND_COLOR                         0x8005
#define GL_TEXTURE0                            0x84C0
#define GL_TEXTURE1                            0x84C1
#define GL_TEXTURE2                            0x84C2
#define GL_TEXTURE3                            0x84C3
#define GL_TEXTURE4                            0x84C4
#define GL_TEXTURE5                            0x84C5
#define GL_TEXTURE6                            0x84C6
#define GL_TEXTURE7                            0x84C7
#define GL_TEXTURE8                            0x84C8
#define GL_TEXTURE9                            0x84C9
#define GL_TEXTURE10                           0x84CA
#define GL_TEXTURE11                           0x84CB
#define GL_TEXTURE12                           0x84CC
#define GL_TEXTURE13                           0x84CD
#define GL_TEXTURE14                           0x84CE
#define GL_TEXTURE15                           0x84CF
#define GL_TEXTURE16                           0x84D0
#define GL_TEXTURE17                           0x84D1
#define GL_TEXTURE18                           0x84D2
#define GL_TEXTURE19                           0x84D3
#define GL_TEXTURE20                           0x84D4
#define GL_TEXTURE21                           0x84D5
#define GL_TEXTURE22                           0x84D6
#define GL_TEXTURE23                           0x84D7
#define GL_TEXTURE24                           0x84D8
#define GL_TEXTURE25                           0x84D9
#define GL_TEXTURE26                           0x84DA
#define GL_TEXTURE27                           0x84DB
#define GL_TEXTURE28                           0x84DC
#define GL_TEXTURE29                           0x84DD
#define GL_TEXTURE30                           0x84DE
#define GL_TEXTURE31                           0x84DF
#define GL_ACTIVE_TEXTURE                      0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE               0x84E1
#define GL_MAX_TEXTURE_UNITS                   0x84E2
#define GL_NORMAL_MAP                          0x8511
#define GL_REFLECTION_MAP                      0x8512
#define GL_TEXTURE_CUBE_MAP                    0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP            0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X         0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X         0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y         0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y         0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z         0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z         0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP              0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE           0x851C
#define GL_COMPRESSED_ALPHA                    0x84E9
#define GL_COMPRESSED_LUMINANCE                        0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA          0x84EB
#define GL_COMPRESSED_INTENSITY                        0x84EC
#define GL_COMPRESSED_RGB                      0x84ED
#define GL_COMPRESSED_RGBA                     0x84EE
#define GL_TEXTURE_COMPRESSION_HINT            0x84EF
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE       0x86A0
#define GL_TEXTURE_COMPRESSED                  0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS      0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS          0x86A3
#define GL_MULTISAMPLE                         0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE            0x809E
#define GL_SAMPLE_ALPHA_TO_ONE                 0x809F
#define GL_SAMPLE_COVERAGE                     0x80A0
#define GL_SAMPLE_BUFFERS                      0x80A8
#define GL_SAMPLES                             0x80A9
#define GL_SAMPLE_COVERAGE_VALUE               0x80AA
#define GL_SAMPLE_COVERAGE_INVERT              0x80AB
#define GL_MULTISAMPLE_BIT                     0x20000000
#define GL_TRANSPOSE_MODELVIEW_MATRIX          0x84E3
#define GL_TRANSPOSE_PROJECTION_MATRIX         0x84E4
#define GL_TRANSPOSE_TEXTURE_MATRIX            0x84E5
#define GL_TRANSPOSE_COLOR_MATRIX              0x84E6
#define GL_COMBINE                             0x8570
#define GL_COMBINE_RGB                         0x8571
#define GL_COMBINE_ALPHA                       0x8572
#define GL_SOURCE0_RGB                         0x8580
#define GL_SOURCE1_RGB                         0x8581
#define GL_SOURCE2_RGB                         0x8582
#define GL_SOURCE0_ALPHA                       0x8588
#define GL_SOURCE1_ALPHA                       0x8589
#define GL_SOURCE2_ALPHA                       0x858A
#define GL_OPERAND0_RGB                                0x8590
#define GL_OPERAND1_RGB                                0x8591
#define GL_OPERAND2_RGB                                0x8592
#define GL_OPERAND0_ALPHA                      0x8598
#define GL_OPERAND1_ALPHA                      0x8599
#define GL_OPERAND2_ALPHA                      0x859A
#define GL_RGB_SCALE                           0x8573
#define GL_ADD_SIGNED                          0x8574
#define GL_INTERPOLATE                         0x8575
#define GL_SUBTRACT                            0x84E7
#define GL_CONSTANT                            0x8576
#define GL_PRIMARY_COLOR                       0x8577
#define GL_PREVIOUS                            0x8578
#define GL_DOT3_RGB                            0x86AE
#define GL_DOT3_RGBA                           0x86AF
#define GL_CLAMP_TO_BORDER                     0x812D
#define GL_ARB_multitexture 1
#define GL_TEXTURE0_ARB                                0x84C0
#define GL_TEXTURE1_ARB                                0x84C1
#define GL_TEXTURE2_ARB                                0x84C2
#define GL_TEXTURE3_ARB                                0x84C3
#define GL_TEXTURE4_ARB                                0x84C4
#define GL_TEXTURE5_ARB                                0x84C5
#define GL_TEXTURE6_ARB                                0x84C6
#define GL_TEXTURE7_ARB                                0x84C7
#define GL_TEXTURE8_ARB                                0x84C8
#define GL_TEXTURE9_ARB                                0x84C9
#define GL_TEXTURE10_ARB                       0x84CA
#define GL_TEXTURE11_ARB                       0x84CB
#define GL_TEXTURE12_ARB                       0x84CC
#define GL_TEXTURE13_ARB                       0x84CD
#define GL_TEXTURE14_ARB                       0x84CE
#define GL_TEXTURE15_ARB                       0x84CF
#define GL_TEXTURE16_ARB                       0x84D0
#define GL_TEXTURE17_ARB                       0x84D1
#define GL_TEXTURE18_ARB                       0x84D2
#define GL_TEXTURE19_ARB                       0x84D3
#define GL_TEXTURE20_ARB                       0x84D4
#define GL_TEXTURE21_ARB                       0x84D5
#define GL_TEXTURE22_ARB                       0x84D6
#define GL_TEXTURE23_ARB                       0x84D7
#define GL_TEXTURE24_ARB                       0x84D8
#define GL_TEXTURE25_ARB                       0x84D9
#define GL_TEXTURE26_ARB                       0x84DA
#define GL_TEXTURE27_ARB                       0x84DB
#define GL_TEXTURE28_ARB                       0x84DC
#define GL_TEXTURE29_ARB                       0x84DD
#define GL_TEXTURE30_ARB                       0x84DE
#define GL_TEXTURE31_ARB                       0x84DF
#define GL_ACTIVE_TEXTURE_ARB                  0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE_ARB           0x84E1
#define GL_MAX_TEXTURE_UNITS_ARB               0x84E2
#define GL_MESA_shader_debug 1
#define GL_DEBUG_OBJECT_MESA              0x8759
#define GL_DEBUG_PRINT_MESA               0x875A
#define GL_DEBUG_ASSERT_MESA              0x875B
#define GL_MESA_packed_depth_stencil 1
#define GL_DEPTH_STENCIL_MESA                  0x8750
#define GL_UNSIGNED_INT_24_8_MESA              0x8751
#define GL_UNSIGNED_INT_8_24_REV_MESA          0x8752
#define GL_UNSIGNED_SHORT_15_1_MESA            0x8753
#define GL_UNSIGNED_SHORT_1_15_REV_MESA                0x8754
#define GL_MESA_program_debug 1
#define GL_FRAGMENT_PROGRAM_POSITION_MESA       0x8bb0
#define GL_FRAGMENT_PROGRAM_CALLBACK_MESA       0x8bb1
#define GL_FRAGMENT_PROGRAM_CALLBACK_FUNC_MESA  0x8bb2
#define GL_FRAGMENT_PROGRAM_CALLBACK_DATA_MESA  0x8bb3
#define GL_VERTEX_PROGRAM_POSITION_MESA         0x8bb4
#define GL_VERTEX_PROGRAM_CALLBACK_MESA         0x8bb5
#define GL_VERTEX_PROGRAM_CALLBACK_FUNC_MESA    0x8bb6
#define GL_VERTEX_PROGRAM_CALLBACK_DATA_MESA    0x8bb7
#define GL_ATI_blend_equation_separate 1
#define GL_ALPHA_BLEND_EQUATION_ATI            0x883D

inline void glClearIndex( GLfloat c ){};
inline void glClearColor( GLclampf red, ... ){};
inline void glClear( GLbitfield mask ){};
inline void glIndexMask( GLuint mask ){};
inline void glColorMask( GLboolean red, ... ){};
inline void glAlphaFunc( GLenum func, ... ){};
inline void glBlendFunc( GLenum sfactor, ... ){};
inline void glLogicOp( GLenum opcode ){};
inline void glCullFace( GLenum mode ){};
inline void glFrontFace( GLenum mode ){};
inline void glPointSize( GLfloat size ){};
inline void glLineWidth( GLfloat width ){};
inline void glLineStipple( GLint factor, ... ){};
inline void glPolygonMode( GLenum face, ... ){};
inline void glPolygonOffset( GLfloat factor, ... ){};
inline void glPolygonStipple( const GLubyte *mask ){};
inline void glGetPolygonStipple( GLubyte *mask ){};
inline void glEdgeFlag( GLboolean flag ){};
inline void glEdgeFlagv( const GLboolean *flag ){};
inline void glScissor( GLint x, ... ){};
inline void glClipPlane( GLenum plane, ... ){};
inline void glGetClipPlane( GLenum plane, ... ){};
inline void glDrawBuffer( GLenum mode ){};
inline void glReadBuffer( GLenum mode ){};
inline void glEnable( GLenum cap ){};
inline void glDisable( GLenum cap ){};
inline GLboolean glIsEnabled( GLenum cap ){};
inline void glEnableClientState( GLenum cap ){};  /* 1.1 */
inline void glDisableClientState( GLenum cap ){};  /* 1.1 */
inline void glGetBooleanv( GLenum pname, ... ){};
inline void glGetDoublev( GLenum pname, ... ){};
inline void glGetFloatv( GLenum pname, ... ){};
inline void glGetIntegerv( GLenum pname, ... ){};
inline void glPushAttrib( GLbitfield mask ){};
inline void glPopAttrib( void ){};
inline void glPushClientAttrib( GLbitfield mask ){};  /* 1.1 */
inline void glPopClientAttrib( void ){};  /* 1.1 */
inline GLint glRenderMode( GLenum mode ){};
inline GLenum glGetError( void ){};
inline const GLubyte * glGetString( GLenum name ){};
inline void glFinish( void ){};
inline void glFlush( void ){};
inline void glHint( GLenum target, ... ){};
inline void glClearDepth( GLclampd depth ){};
inline void glDepthFunc( GLenum func ){};
inline void glDepthMask( GLboolean flag ){};
inline void glDepthRange( GLclampd near_val, ... ){};
inline void glClearAccum( GLfloat red, ... ){};
inline void glAccum( GLenum op, ... ){};
inline void glMatrixMode( GLenum mode ){};
inline void glOrtho( GLdouble left, ... ){};
inline void glFrustum( GLdouble left, ... ){};
inline void glViewport( GLint x, ... ){};
inline void glPushMatrix( void ){};
inline void glPopMatrix( void ){};
inline void glLoadIdentity( void ){};
inline void glLoadMatrixd( const GLdouble *m ){};
inline void glLoadMatrixf( const GLfloat *m ){};
inline void glMultMatrixd( const GLdouble *m ){};
inline void glMultMatrixf( const GLfloat *m ){};
inline void glRotated( GLdouble angle, ... ){};
inline void glRotatef( GLfloat angle, ... ){};
inline void glScaled( GLdouble x, ... ){};
inline void glScalef( GLfloat x, ... ){};
inline void glTranslated( GLdouble x, ... ){};
inline void glTranslatef( GLfloat x, ... ){};
inline GLboolean glIsList( GLuint list ){};
inline void glDeleteLists( GLuint list, ... ){};
inline GLuint glGenLists( GLsizei range ){};
inline void glNewList( GLuint list, ... ){};
inline void glEndList( void ){};
inline void glCallList( GLuint list ){};
inline void glCallLists( GLsizei n, ... ){};
inline void glListBase( GLuint base ){};
inline void glBegin( GLenum mode ){};
inline void glEnd( void ){};
inline void glVertex2d( GLdouble x, ... ){};
inline void glVertex2f( GLfloat x, ... ){};
inline void glVertex2i( GLint x, ... ){};
inline void glVertex2s( GLshort x, ... ){};
inline void glVertex3d( GLdouble x, ... ){};
inline void glVertex3f( GLfloat x, ... ){};
inline void glVertex3i( GLint x, ... ){};
inline void glVertex3s( GLshort x, ... ){};
inline void glVertex4d( GLdouble x, ... ){};
inline void glVertex4f( GLfloat x, ... ){};
inline void glVertex4i( GLint x, ... ){};
inline void glVertex4s( GLshort x, ... ){};
inline void glVertex2dv( const GLdouble *v ){};
inline void glVertex2fv( const GLfloat *v ){};
inline void glVertex2iv( const GLint *v ){};
inline void glVertex2sv( const GLshort *v ){};
inline void glVertex3dv( const GLdouble *v ){};
inline void glVertex3fv( const GLfloat *v ){};
inline void glVertex3iv( const GLint *v ){};
inline void glVertex3sv( const GLshort *v ){};
inline void glVertex4dv( const GLdouble *v ){};
inline void glVertex4fv( const GLfloat *v ){};
inline void glVertex4iv( const GLint *v ){};
inline void glVertex4sv( const GLshort *v ){};
inline void glNormal3b( GLbyte nx, ... ){};
inline void glNormal3d( GLdouble nx, ... ){};
inline void glNormal3f( GLfloat nx, ... ){};
inline void glNormal3i( GLint nx, ... ){};
inline void glNormal3s( GLshort nx, ... ){};
inline void glNormal3bv( const GLbyte *v ){};
inline void glNormal3dv( const GLdouble *v ){};
inline void glNormal3fv( const GLfloat *v ){};
inline void glNormal3iv( const GLint *v ){};
inline void glNormal3sv( const GLshort *v ){};
inline void glIndexd( GLdouble c ){};
inline void glIndexf( GLfloat c ){};
inline void glIndexi( GLint c ){};
inline void glIndexs( GLshort c ){};
inline void glIndexub( GLubyte c ){};  /* 1.1 */
inline void glIndexdv( const GLdouble *c ){};
inline void glIndexfv( const GLfloat *c ){};
inline void glIndexiv( const GLint *c ){};
inline void glIndexsv( const GLshort *c ){};
inline void glIndexubv( const GLubyte *c ){};  /* 1.1 */
inline void glColor3b( GLbyte red, ... ){};
inline void glColor3d( GLdouble red, ... ){};
inline void glColor3f( GLfloat red, ... ){};
inline void glColor3i( GLint red, ... ){};
inline void glColor3s( GLshort red, ... ){};
inline void glColor3ub( GLubyte red, ... ){};
inline void glColor3ui( GLuint red, ... ){};
inline void glColor3us( GLushort red, ... ){};
inline void glColor4b( GLbyte red, ... ){};
inline void glColor4d( GLdouble red, ... ){};
inline void glColor4f( GLfloat red, ... ){};
inline void glColor4i( GLint red, ... ){};
inline void glColor4s( GLshort red, ... ){};
inline void glColor4ub( GLubyte red, ... ){};
inline void glColor4ui( GLuint red, ... ){};
inline void glColor4us( GLushort red, ... ){};
inline void glColor3bv( const GLbyte *v ){};
inline void glColor3dv( const GLdouble *v ){};
inline void glColor3fv( const GLfloat *v ){};
inline void glColor3iv( const GLint *v ){};
inline void glColor3sv( const GLshort *v ){};
inline void glColor3ubv( const GLubyte *v ){};
inline void glColor3uiv( const GLuint *v ){};
inline void glColor3usv( const GLushort *v ){};
inline void glColor4bv( const GLbyte *v ){};
inline void glColor4dv( const GLdouble *v ){};
inline void glColor4fv( const GLfloat *v ){};
inline void glColor4iv( const GLint *v ){};
inline void glColor4sv( const GLshort *v ){};
inline void glColor4ubv( const GLubyte *v ){};
inline void glColor4uiv( const GLuint *v ){};
inline void glColor4usv( const GLushort *v ){};
inline void glTexCoord1d( GLdouble s ){};
inline void glTexCoord1f( GLfloat s ){};
inline void glTexCoord1i( GLint s ){};
inline void glTexCoord1s( GLshort s ){};
inline void glTexCoord2d( GLdouble s, ... ){};
inline void glTexCoord2f( GLfloat s, ... ){};
inline void glTexCoord2i( GLint s, ... ){};
inline void glTexCoord2s( GLshort s, ... ){};
inline void glTexCoord3d( GLdouble s, ... ){};
inline void glTexCoord3f( GLfloat s, ... ){};
inline void glTexCoord3i( GLint s, ... ){};
inline void glTexCoord3s( GLshort s, ... ){};
inline void glTexCoord4d( GLdouble s, ... ){};
inline void glTexCoord4f( GLfloat s, ... ){};
inline void glTexCoord4i( GLint s, ... ){};
inline void glTexCoord4s( GLshort s, ... ){};
inline void glTexCoord1dv( const GLdouble *v ){};
inline void glTexCoord1fv( const GLfloat *v ){};
inline void glTexCoord1iv( const GLint *v ){};
inline void glTexCoord1sv( const GLshort *v ){};
inline void glTexCoord2dv( const GLdouble *v ){};
inline void glTexCoord2fv( const GLfloat *v ){};
inline void glTexCoord2iv( const GLint *v ){};
inline void glTexCoord2sv( const GLshort *v ){};
inline void glTexCoord3dv( const GLdouble *v ){};
inline void glTexCoord3fv( const GLfloat *v ){};
inline void glTexCoord3iv( const GLint *v ){};
inline void glTexCoord3sv( const GLshort *v ){};
inline void glTexCoord4dv( const GLdouble *v ){};
inline void glTexCoord4fv( const GLfloat *v ){};
inline void glTexCoord4iv( const GLint *v ){};
inline void glTexCoord4sv( const GLshort *v ){};
inline void glRasterPos2d( GLdouble x, ... ){};
inline void glRasterPos2f( GLfloat x, ... ){};
inline void glRasterPos2i( GLint x, ... ){};
inline void glRasterPos2s( GLshort x, ... ){};
inline void glRasterPos3d( GLdouble x, ... ){};
inline void glRasterPos3f( GLfloat x, ... ){};
inline void glRasterPos3i( GLint x, ... ){};
inline void glRasterPos3s( GLshort x, ... ){};
inline void glRasterPos4d( GLdouble x, ... ){};
inline void glRasterPos4f( GLfloat x, ... ){};
inline void glRasterPos4i( GLint x, ... ){};
inline void glRasterPos4s( GLshort x, ... ){};
inline void glRasterPos2dv( const GLdouble *v ){};
inline void glRasterPos2fv( const GLfloat *v ){};
inline void glRasterPos2iv( const GLint *v ){};
inline void glRasterPos2sv( const GLshort *v ){};
inline void glRasterPos3dv( const GLdouble *v ){};
inline void glRasterPos3fv( const GLfloat *v ){};
inline void glRasterPos3iv( const GLint *v ){};
inline void glRasterPos3sv( const GLshort *v ){};
inline void glRasterPos4dv( const GLdouble *v ){};
inline void glRasterPos4fv( const GLfloat *v ){};
inline void glRasterPos4iv( const GLint *v ){};
inline void glRasterPos4sv( const GLshort *v ){};
inline void glRectd( GLdouble x1, ... ){};
inline void glRectf( GLfloat x1, ... ){};
inline void glRecti( GLint x1, ... ){};
inline void glRects( GLshort x1, ... ){};
inline void glRectdv( const GLdouble *v1, ... ){};
inline void glRectfv( const GLfloat *v1, ... ){};
inline void glRectiv( const GLint *v1, ... ){};
inline void glRectsv( const GLshort *v1, ... ){};
inline void glVertexPointer( GLint size, ... ){};
inline void glNormalPointer( GLenum type, ... ){};
inline void glColorPointer( GLint size, ... ){};
inline void glIndexPointer( GLenum type, ... ){};
inline void glTexCoordPointer( GLint size, ... ){};
inline void glEdgeFlagPointer( GLsizei stride, ... ){};
inline void glGetPointerv( GLenum pname, ... ){};
inline void glArrayElement( GLint i ){};
inline void glDrawArrays( GLenum mode, ... ){};
inline void glDrawElements( GLenum mode, ... ){};
inline void glInterleavedArrays( GLenum format, ... ){};
inline void glShadeModel( GLenum mode ){};
inline void glLightf( GLenum light, ... ){};
inline void glLighti( GLenum light, ... ){};
inline void glLightfv( GLenum light, ... ){};
inline void glLightiv( GLenum light, ... ){};
inline void glGetLightfv( GLenum light, ... ){};
inline void glGetLightiv( GLenum light, ... ){};
inline void glLightModelf( GLenum pname, ... ){};
inline void glLightModeli( GLenum pname, ... ){};
inline void glLightModelfv( GLenum pname, ... ){};
inline void glLightModeliv( GLenum pname, ... ){};
inline void glMaterialf( GLenum face, ... ){};
inline void glMateriali( GLenum face, ... ){};
inline void glMaterialfv( GLenum face, ... ){};
inline void glMaterialiv( GLenum face, ... ){};
inline void glGetMaterialfv( GLenum face, ... ){};
inline void glGetMaterialiv( GLenum face, ... ){};
inline void glColorMaterial( GLenum face, ... ){};
inline void glPixelZoom( GLfloat xfactor, ... ){};
inline void glPixelStoref( GLenum pname, ... ){};
inline void glPixelStorei( GLenum pname, ... ){};
inline void glPixelTransferf( GLenum pname, ... ){};
inline void glPixelTransferi( GLenum pname, ... ){};
inline void glPixelMapfv( GLenum map, ... ){};
inline void glPixelMapuiv( GLenum map, ... ){};
inline void glPixelMapusv( GLenum map, ... ){};
inline void glGetPixelMapfv( GLenum map, ... ){};
inline void glGetPixelMapuiv( GLenum map, ... ){};
inline void glGetPixelMapusv( GLenum map, ... ){};
inline void glBitmap( GLsizei width, ... ){};
inline void glReadPixels( GLint x, ... ){};
inline void glDrawPixels( GLsizei width, ... ){};
inline void glCopyPixels( GLint x, ... ){};
inline void glStencilFunc( GLenum func, ... ){};
inline void glStencilMask( GLuint mask ){};
inline void glStencilOp( GLenum fail, ... ){};
inline void glClearStencil( GLint s ){};
inline void glTexGend( GLenum coord, ... ){};
inline void glTexGenf( GLenum coord, ... ){};
inline void glTexGeni( GLenum coord, ... ){};
inline void glTexGendv( GLenum coord, ... ){};
inline void glTexGenfv( GLenum coord, ... ){};
inline void glTexGeniv( GLenum coord, ... ){};
inline void glGetTexGendv( GLenum coord, ... ){};
inline void glGetTexGenfv( GLenum coord, ... ){};
inline void glGetTexGeniv( GLenum coord, ... ){};
inline void glTexEnvf( GLenum target, ... ){};
inline void glTexEnvi( GLenum target, ... ){};
inline void glTexEnvfv( GLenum target, ... ){};
inline void glTexEnviv( GLenum target, ... ){};
inline void glGetTexEnvfv( GLenum target, ... ){};
inline void glGetTexEnviv( GLenum target, ... ){};
inline void glTexParameterf( GLenum target, ... ){};
inline void glTexParameteri( GLenum target, ... ){};
inline void glTexParameterfv( GLenum target, ... ){};
inline void glTexParameteriv( GLenum target, ... ){};
inline void glGetTexParameterfv( GLenum target, ... ){};
inline void glGetTexParameteriv( GLenum target, ... ){};
inline void glGetTexLevelParameterfv( GLenum target, ... ){};
inline void glGetTexLevelParameteriv( GLenum target, ... ){};
inline void glTexImage1D( GLenum target, ... ){};
inline void glTexImage2D( GLenum target, ... ){};
inline void glGetTexImage( GLenum target, ... ){};
inline void glGenTextures( GLsizei n, ... ){};
inline void glDeleteTextures( GLsizei n, ... ){};
inline void glBindTexture( GLenum target, ... ){};
inline void glPrioritizeTextures( GLsizei n, ... ){};
inline GLboolean glAreTexturesResident( GLsizei n, ... ){};
inline GLboolean glIsTexture( GLuint texture ){};
inline void glTexSubImage1D( GLenum target, ... ){};
inline void glTexSubImage2D( GLenum target, ... ){};
inline void glCopyTexImage1D( GLenum target, ... ){};
inline void glCopyTexImage2D( GLenum target, ... ){};
inline void glCopyTexSubImage1D( GLenum target, ... ){};
inline void glCopyTexSubImage2D( GLenum target, ... ){};
inline void glMap1d( GLenum target, ... ){};
inline void glMap1f( GLenum target, ... ){};
inline void glMap2d( GLenum target, ... ){};
inline void glMap2f( GLenum target, ... ){};
inline void glGetMapdv( GLenum target, ... ){};
inline void glGetMapfv( GLenum target, ... ){};
inline void glGetMapiv( GLenum target, ... ){};
inline void glEvalCoord1d( GLdouble u ){};
inline void glEvalCoord1f( GLfloat u ){};
inline void glEvalCoord1dv( const GLdouble *u ){};
inline void glEvalCoord1fv( const GLfloat *u ){};
inline void glEvalCoord2d( GLdouble u, ... ){};
inline void glEvalCoord2f( GLfloat u, ... ){};
inline void glEvalCoord2dv( const GLdouble *u ){};
inline void glEvalCoord2fv( const GLfloat *u ){};
inline void glMapGrid1d( GLint un, ... ){};
inline void glMapGrid1f( GLint un, ... ){};
inline void glMapGrid2d( GLint un, ... ){};
inline void glMapGrid2f( GLint un, ... ){};
inline void glEvalPoint1( GLint i ){};
inline void glEvalPoint2( GLint i, ... ){};
inline void glEvalMesh1( GLenum mode, ... ){};
inline void glEvalMesh2( GLenum mode, ... ){};
inline void glFogf( GLenum pname, ... ){};
inline void glFogi( GLenum pname, ... ){};
inline void glFogfv( GLenum pname, ... ){};
inline void glFogiv( GLenum pname, ... ){};
inline void glFeedbackBuffer( GLsizei size, ... ){};
inline void glPassThrough( GLfloat token ){};
inline void glSelectBuffer( GLsizei size, ... ){};
inline void glInitNames( void ){};
inline void glLoadName( GLuint name ){};
inline void glPushName( GLuint name ){};
inline void glPopName( void ){};
inline void glDrawRangeElements( GLenum mode, ... ){};
inline void glTexImage3D( GLenum target, ... ){};
inline void glTexSubImage3D( GLenum target, ... ){};
inline void glCopyTexSubImage3D( GLenum target, ... ){};
inline void glColorTable( GLenum target, ... ){};
inline void glColorSubTable( GLenum target, ... ){};
inline void glColorTableParameteriv(GLenum target, ... ){};
inline void glColorTableParameterfv(GLenum target, ... ){};
inline void glCopyColorSubTable( GLenum target, ... ){};
inline void glCopyColorTable( GLenum target, ... ){};
inline void glGetColorTable( GLenum target, ... ){};
inline void glGetColorTableParameterfv( GLenum target, ... ){};
inline void glGetColorTableParameteriv( GLenum target, ... ){};
inline void glBlendEquation( GLenum mode ){};
inline void glBlendColor( GLclampf red, ... ){};
inline void glHistogram( GLenum target, ... ){};
inline void glResetHistogram( GLenum target ){};
inline void glGetHistogram( GLenum target, ... ){};
inline void glGetHistogramParameterfv( GLenum target, ... ){};
inline void glGetHistogramParameteriv( GLenum target, ... ){};
inline void glMinmax( GLenum target, ... ){};
inline void glResetMinmax( GLenum target ){};
inline void glGetMinmax( GLenum target, ... ){};
inline void glGetMinmaxParameterfv( GLenum target, ... ){};
inline void glGetMinmaxParameteriv( GLenum target, ... ){};
inline void glConvolutionFilter1D( GLenum target, ... ){};
inline void glConvolutionFilter2D( GLenum target, ... ){};
inline void glConvolutionParameterf( GLenum target, ... ){};
inline void glConvolutionParameterfv( GLenum target, ... ){};
inline void glConvolutionParameteri( GLenum target, ... ){};
inline void glConvolutionParameteriv( GLenum target, ... ){};
inline void glCopyConvolutionFilter1D( GLenum target, ... ){};
inline void glCopyConvolutionFilter2D( GLenum target, ... ){};
inline void glGetConvolutionFilter( GLenum target, ... ){};
inline void glGetConvolutionParameterfv( GLenum target, ... ){};
inline void glGetConvolutionParameteriv( GLenum target, ... ){};
inline void glSeparableFilter2D( GLenum target, ... ){};
inline void glGetSeparableFilter( GLenum target, ... ){};
inline void glActiveTexture( GLenum texture ){};
inline void glClientActiveTexture( GLenum texture ){};
inline void glCompressedTexImage1D( GLenum target, ... ){};
inline void glCompressedTexImage2D( GLenum target, ... ){};
inline void glCompressedTexImage3D( GLenum target, ... ){};
inline void glCompressedTexSubImage1D( GLenum target, ... ){};
inline void glCompressedTexSubImage2D( GLenum target, ... ){};
inline void glCompressedTexSubImage3D( GLenum target, ... ){};
inline void glGetCompressedTexImage( GLenum target, ... ){};
inline void glMultiTexCoord1d( GLenum target, ... ){};
inline void glMultiTexCoord1dv( GLenum target, ... ){};
inline void glMultiTexCoord1f( GLenum target, ... ){};
inline void glMultiTexCoord1fv( GLenum target, ... ){};
inline void glMultiTexCoord1i( GLenum target, ... ){};
inline void glMultiTexCoord1iv( GLenum target, ... ){};
inline void glMultiTexCoord1s( GLenum target, ... ){};
inline void glMultiTexCoord1sv( GLenum target, ... ){};
inline void glMultiTexCoord2d( GLenum target, ... ){};
inline void glMultiTexCoord2dv( GLenum target, ... ){};
inline void glMultiTexCoord2f( GLenum target, ... ){};
inline void glMultiTexCoord2fv( GLenum target, ... ){};
inline void glMultiTexCoord2i( GLenum target, ... ){};
inline void glMultiTexCoord2iv( GLenum target, ... ){};
inline void glMultiTexCoord2s( GLenum target, ... ){};
inline void glMultiTexCoord2sv( GLenum target, ... ){};
inline void glMultiTexCoord3d( GLenum target, ... ){};
inline void glMultiTexCoord3dv( GLenum target, ... ){};
inline void glMultiTexCoord3f( GLenum target, ... ){};
inline void glMultiTexCoord3fv( GLenum target, ... ){};
inline void glMultiTexCoord3i( GLenum target, ... ){};
inline void glMultiTexCoord3iv( GLenum target, ... ){};
inline void glMultiTexCoord3s( GLenum target, ... ){};
inline void glMultiTexCoord3sv( GLenum target, ... ){};
inline void glMultiTexCoord4d( GLenum target, ... ){};
inline void glMultiTexCoord4dv( GLenum target, ... ){};
inline void glMultiTexCoord4f( GLenum target, ... ){};
inline void glMultiTexCoord4fv( GLenum target, ... ){};
inline void glMultiTexCoord4i( GLenum target, ... ){};
inline void glMultiTexCoord4iv( GLenum target, ... ){};
inline void glMultiTexCoord4s( GLenum target, ... ){};
inline void glMultiTexCoord4sv( GLenum target, ... ){};
inline void glLoadTransposeMatrixd( const GLdouble m[16] ){};
inline void glLoadTransposeMatrixf( const GLfloat m[16] ){};
inline void glMultTransposeMatrixd( const GLdouble m[16] ){};
inline void glMultTransposeMatrixf( const GLfloat m[16] ){};
inline void glSampleCoverage( GLclampf value, ... ){};
inline void glActiveTextureARB(GLenum texture){};
inline void glClientActiveTextureARB(GLenum texture){};
inline void glMultiTexCoord1dARB(GLenum target, ... ){};
inline void glMultiTexCoord1dvARB(GLenum target, ... ){};
inline void glMultiTexCoord1fARB(GLenum target, ... ){};
inline void glMultiTexCoord1fvARB(GLenum target, ... ){};
inline void glMultiTexCoord1iARB(GLenum target, ... ){};
inline void glMultiTexCoord1ivARB(GLenum target, ... ){};
inline void glMultiTexCoord1sARB(GLenum target, ... ){};
inline void glMultiTexCoord1svARB(GLenum target, ... ){};
inline void glMultiTexCoord2dARB(GLenum target, ... ){};
inline void glMultiTexCoord2dvARB(GLenum target, ... ){};
inline void glMultiTexCoord2fARB(GLenum target, ... ){};
inline void glMultiTexCoord2fvARB(GLenum target, ... ){};
inline void glMultiTexCoord2iARB(GLenum target, ... ){};
inline void glMultiTexCoord2ivARB(GLenum target, ... ){};
inline void glMultiTexCoord2sARB(GLenum target, ... ){};
inline void glMultiTexCoord2svARB(GLenum target, ... ){};
inline void glMultiTexCoord3dARB(GLenum target, ... ){};
inline void glMultiTexCoord3dvARB(GLenum target, ... ){};
inline void glMultiTexCoord3fARB(GLenum target, ... ){};
inline void glMultiTexCoord3fvARB(GLenum target, ... ){};
inline void glMultiTexCoord3iARB(GLenum target, ... ){};
inline void glMultiTexCoord3ivARB(GLenum target, ... ){};
inline void glMultiTexCoord3sARB(GLenum target, ... ){};
inline void glMultiTexCoord3svARB(GLenum target, ... ){};
inline void glMultiTexCoord4dARB(GLenum target, ... ){};
inline void glMultiTexCoord4dvARB(GLenum target, ... ){};
inline void glMultiTexCoord4fARB(GLenum target, ... ){};
inline void glMultiTexCoord4fvARB(GLenum target, ... ){};
inline void glMultiTexCoord4iARB(GLenum target, ... ){};
inline void glMultiTexCoord4ivARB(GLenum target, ... ){};
inline void glMultiTexCoord4sARB(GLenum target, ... ){};
inline void glMultiTexCoord4svARB(GLenum target, ... ){};
/*
GLhandleARB glCreateDebugObjectMESA (void){};
void glClearDebugLogMESA (GLhandleARB obj, ... ){};
void glGetDebugLogMESA (GLhandleARB obj, ... ){};
GLsizei glGetDebugLogLengthMESA (GLhandleARB obj, ... ){};
void glProgramCallbackMESA(GLenum target, ... ){};
void glGetProgramRegisterfvMESA(GLenum target, ... ){};
void glBlendEquationSeparateATI( GLenum modeRGB, ... ){};
*/
#endif // NULLGL_H
