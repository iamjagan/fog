// [Fog-G2d]
//
// [License]
// MIT, See COPYING file in package

// [Precompiled Headers]
#if defined(FOG_PRECOMP)
#include FOG_PRECOMP
#endif // FOG_PRECOMP

// [Dependencies]
#include <Fog/Core/Global/Global.h>

#include <Fog/G2d/Painting/RasterApi_p.h>
#include <Fog/G2d/Painting/RasterConstants_p.h>
#include <Fog/G2d/Painting/RasterInit_p.h>

#include <Fog/G2d/Painting/RasterOps_C/BaseAccess_p.h>
#include <Fog/G2d/Painting/RasterOps_C/BaseConvert_p.h>
#include <Fog/G2d/Painting/RasterOps_C/BaseDefs_p.h>
#include <Fog/G2d/Painting/RasterOps_C/BaseHelpers_p.h>

#include <Fog/G2d/Painting/RasterOps_C/CompositeBase_p.h>
#include <Fog/G2d/Painting/RasterOps_C/CompositeClear_p.h>
#include <Fog/G2d/Painting/RasterOps_C/CompositeNop_p.h>
#include <Fog/G2d/Painting/RasterOps_C/CompositeSrc_p.h>
#include <Fog/G2d/Painting/RasterOps_C/CompositeSrcOver_p.h>

#include <Fog/G2d/Painting/RasterOps_C/GradientBase_p.h>
#include <Fog/G2d/Painting/RasterOps_C/GradientConical_p.h>
#include <Fog/G2d/Painting/RasterOps_C/GradientLinear_p.h>
#include <Fog/G2d/Painting/RasterOps_C/GradientRadial_p.h>
#include <Fog/G2d/Painting/RasterOps_C/GradientRectangular_p.h>

#include <Fog/G2d/Painting/RasterOps_C/TextureBase_p.h>
#include <Fog/G2d/Painting/RasterOps_C/TextureAffine_p.h>
#include <Fog/G2d/Painting/RasterOps_C/TextureProjection_p.h>
#include <Fog/G2d/Painting/RasterOps_C/TextureScale_p.h>
#include <Fog/G2d/Painting/RasterOps_C/TextureSimple_p.h>

// C implementation can be disabled when hardcoding for SSE2.
#if !defined(FOG_HARDCODE_SSE2)
# define FOG_RASTER_INIT_C
#endif // FOG_HARDCODE_SSE2

// TODO:
# define FOG_RASTER_INIT_C

namespace Fog {

// ============================================================================
// [Init / Fini]
// ============================================================================

static void fog_rasterengine_set_nops(ApiRaster::_CompositeExt* funcs)
{
  funcs->cblit_line[RASTER_CBLIT_PRGB ] = RasterOps_C::CompositeNop::nop_cblit_line;
  funcs->cblit_span[RASTER_CBLIT_PRGB ] = RasterOps_C::CompositeNop::nop_cblit_span;
  funcs->cblit_line[RASTER_CBLIT_XRGB ] = RasterOps_C::CompositeNop::nop_cblit_line;
  funcs->cblit_span[RASTER_CBLIT_XRGB ] = RasterOps_C::CompositeNop::nop_cblit_span;

  uint i;
  for (i = 0; i < RASTER_VBLIT_COUNT; i++) funcs->vblit_line[i] = RasterOps_C::CompositeNop::nop_vblit_line;
  for (i = 0; i < RASTER_VBLIT_COUNT; i++) funcs->vblit_span[i] = RasterOps_C::CompositeNop::nop_vblit_span;
}

FOG_NO_EXPORT void RasterOps_init_C(void)
{
  ApiRaster& api = _api_raster;

#if defined(FOG_RASTER_INIT_C)

  // --------------------------------------------------------------------------
  // [RasterOps - Convert - API]
  // --------------------------------------------------------------------------

  ApiRaster::_Convert& convert = api.convert;

  convert.init = RasterOps_C::Convert::init;

  // --------------------------------------------------------------------------
  // [RasterOps - Convert - Copy]
  // --------------------------------------------------------------------------

  convert.copy[RASTER_COPY_8] = (ImageConverterBlitLineFunc)RasterOps_C::Convert::copy_8;
  convert.copy[RASTER_COPY_16] = (ImageConverterBlitLineFunc)RasterOps_C::Convert::copy_16;
  convert.copy[RASTER_COPY_24] = (ImageConverterBlitLineFunc)RasterOps_C::Convert::copy_24;
  convert.copy[RASTER_COPY_32] = (ImageConverterBlitLineFunc)RasterOps_C::Convert::copy_32;
  convert.copy[RASTER_COPY_48] = (ImageConverterBlitLineFunc)RasterOps_C::Convert::copy_48;
  convert.copy[RASTER_COPY_64] = (ImageConverterBlitLineFunc)RasterOps_C::Convert::copy_64;

  // --------------------------------------------------------------------------
  // [RasterOps - Convert - BSwap]
  // --------------------------------------------------------------------------

  convert.bswap[RASTER_BSWAP_16] = (ImageConverterBlitLineFunc)RasterOps_C::Convert::bswap_16;
  convert.bswap[RASTER_BSWAP_24] = (ImageConverterBlitLineFunc)RasterOps_C::Convert::bswap_24;
  convert.bswap[RASTER_BSWAP_32] = (ImageConverterBlitLineFunc)RasterOps_C::Convert::bswap_32;
  convert.bswap[RASTER_BSWAP_48] = (ImageConverterBlitLineFunc)RasterOps_C::Convert::bswap_48;
  convert.bswap[RASTER_BSWAP_64] = (ImageConverterBlitLineFunc)RasterOps_C::Convert::bswap_64;

  // --------------------------------------------------------------------------
  // [RasterOps - Convert - Premultiply / Demultiply]
  // --------------------------------------------------------------------------

  convert.prgb32_from_argb32 = (ImageConverterBlitLineFunc)RasterOps_C::Convert::prgb32_from_argb32;
  convert.argb32_from_prgb32 = (ImageConverterBlitLineFunc)RasterOps_C::Convert::argb32_from_prgb32;

  // --------------------------------------------------------------------------
  // [RasterOps - Convert - A8 <-> Extended]
  // --------------------------------------------------------------------------

//convert.a8_from_custom    [RASTER_FORMAT_A8                 ] = SKIP;
//convert.a8_from_custom    [RASTER_FORMAT_A16                ] = SKIP;
//convert.a8_from_custom    [RASTER_FORMAT_A16_BS             ] = RasterOps_C::Convert::a8_native_from_a16_bs;

//convert.custom_from_a8    [RASTER_FORMAT_A8                 ] = SKIP;
//convert.custom_from_a8    [RASTER_FORMAT_A16                ] = SKIP;
//convert.custom_from_a8    [RASTER_FORMAT_A16_BS             ] = SKIP;

  // --------------------------------------------------------------------------
  //[RasterOps - Convert - A16 <-> Extended]
  // --------------------------------------------------------------------------

//convert.a16_from_custom   [RASTER_FORMAT_A8                 ] = SKIP;
//convert.a16_from_custom   [RASTER_FORMAT_A16                ] = SKIP;
//convert.a16_from_custom   [RASTER_FORMAT_A16_BS             ] = SKIP;

//convert.custom_from_a16   [RASTER_FORMAT_A8                 ] = SKIP;
//convert.custom_from_a16   [RASTER_FORMAT_A16                ] = SKIP;
//convert.custom_from_a16   [RASTER_FORMAT_A16_BS             ] = SKIP;

  // --------------------------------------------------------------------------
  //[RasterOps - Convert - ARGB32 <-> Extended]
  // --------------------------------------------------------------------------

//convert.argb32_from_custom[RASTER_FORMAT_RGB16_555          ] = SKIP;
  convert.argb32_from_custom[RASTER_FORMAT_RGB16_555_BS       ] = RasterOps_C::Convert::argb32_native_from_rgb16_555_bs;
//convert.argb32_from_custom[RASTER_FORMAT_RGB16_565          ] = SKIP;
  convert.argb32_from_custom[RASTER_FORMAT_RGB16_565_BS       ] = RasterOps_C::Convert::argb32_native_from_rgb16_565_bs;
//convert.argb32_from_custom[RASTER_FORMAT_RGB24_888          ] = SKIP;
  convert.argb32_from_custom[RASTER_FORMAT_RGB24_888_BS       ] = RasterOps_C::Convert::argb32_native_from_rgb24_888_bs;
//convert.argb32_from_custom[RASTER_FORMAT_RGB32_888          ] = SKIP;
  convert.argb32_from_custom[RASTER_FORMAT_RGB32_888_BS       ] = RasterOps_C::Convert::argb32_native_from_rgb32_888_bs;
//convert.argb32_from_custom[RASTER_FORMAT_RGB48_161616       ] = SKIP;
  convert.argb32_from_custom[RASTER_FORMAT_RGB48_161616_BS    ] = RasterOps_C::Convert::argb32_native_from_rgb48_161616_bs;
  convert.argb32_from_custom[RASTER_FORMAT_RGB48_CUSTOM       ] = RasterOps_C::Convert::argb32_native_from_rgb48_dib;
  convert.argb32_from_custom[RASTER_FORMAT_RGB48_CUSTOM_BS    ] = RasterOps_C::Convert::argb32_native_from_rgb48_dib_bs;

  convert.argb32_from_custom[RASTER_FORMAT_ARGB16_4444        ] = RasterOps_C::Convert::argb32_native_from_argb16_4444;
  convert.argb32_from_custom[RASTER_FORMAT_ARGB16_4444_BS     ] = RasterOps_C::Convert::argb32_native_from_argb16_4444_bs;
  convert.argb32_from_custom[RASTER_FORMAT_ARGB16_CUSTOM      ] = RasterOps_C::Convert::argb32_native_from_argb16_dib;
  convert.argb32_from_custom[RASTER_FORMAT_ARGB16_CUSTOM_BS   ] = RasterOps_C::Convert::argb32_native_from_argb16_dib_bs;
  convert.argb32_from_custom[RASTER_FORMAT_ARGB24_CUSTOM      ] = RasterOps_C::Convert::argb32_native_from_argb24_dib;
  convert.argb32_from_custom[RASTER_FORMAT_ARGB24_CUSTOM_BS   ] = RasterOps_C::Convert::argb32_native_from_argb24_dib_bs;
//convert.argb32_from_custom[RASTER_FORMAT_ARGB32_8888        ] = SKIP;
  convert.argb32_from_custom[RASTER_FORMAT_ARGB32_8888_BS     ] = RasterOps_C::Convert::argb32_native_from_argb32_8888_bs;
  convert.argb32_from_custom[RASTER_FORMAT_ARGB32_CUSTOM      ] = RasterOps_C::Convert::argb32_native_from_argb32_dib;
  convert.argb32_from_custom[RASTER_FORMAT_ARGB32_CUSTOM_BS   ] = RasterOps_C::Convert::argb32_native_from_argb32_dib_bs;
  convert.argb32_from_custom[RASTER_FORMAT_ARGB48_CUSTOM      ] = RasterOps_C::Convert::argb32_native_from_argb48_dib;
  convert.argb32_from_custom[RASTER_FORMAT_ARGB48_CUSTOM_BS   ] = RasterOps_C::Convert::argb32_native_from_argb48_dib_bs;
//convert.argb32_from_custom[RASTER_FORMAT_ARGB64_16161616    ] = SKIP;
  convert.argb32_from_custom[RASTER_FORMAT_ARGB64_16161616_BS ] = RasterOps_C::Convert::argb32_native_from_argb64_16161616_bs;
  convert.argb32_from_custom[RASTER_FORMAT_ARGB64_CUSTOM      ] = RasterOps_C::Convert::argb32_native_from_argb64_dib;
  convert.argb32_from_custom[RASTER_FORMAT_ARGB64_CUSTOM_BS   ] = RasterOps_C::Convert::argb32_native_from_argb64_dib_bs;

//convert.argb32_from_custom[RASTER_FORMAT_I8                 ];

//convert.custom_from_argb32[RASTER_FORMAT_RGB24_888          ] = RasterOps_C::CompositeSrc::rgb24_vblit_xrgb32_line;

  // TODO:

  // --------------------------------------------------------------------------
  //[RasterOps - Convert - ARGB64 <-> Extended]
  // --------------------------------------------------------------------------

//convert.argb64_from_custom[RASTER_FORMAT_RGB16_555          ] = SKIP;
  convert.argb64_from_custom[RASTER_FORMAT_RGB16_555_BS       ] = RasterOps_C::Convert::argb64_native_from_rgb16_555_bs;
//convert.argb64_from_custom[RASTER_FORMAT_RGB16_565          ] = SKIP;
  convert.argb64_from_custom[RASTER_FORMAT_RGB16_565_BS       ] = RasterOps_C::Convert::argb64_native_from_rgb16_565_bs;
//convert.argb64_from_custom[RASTER_FORMAT_RGB24_888          ] = SKIP;
  convert.argb64_from_custom[RASTER_FORMAT_RGB24_888_BS       ] = RasterOps_C::Convert::argb64_native_from_rgb24_888_bs;
//convert.argb64_from_custom[RASTER_FORMAT_RGB32_888          ] = SKIP;
  convert.argb64_from_custom[RASTER_FORMAT_RGB32_888_BS       ] = RasterOps_C::Convert::argb64_native_from_rgb32_888_bs;
//convert.argb64_from_custom[RASTER_FORMAT_RGB48_161616       ] = SKIP;
  convert.argb64_from_custom[RASTER_FORMAT_RGB48_161616_BS    ] = RasterOps_C::Convert::argb64_native_from_rgb48_161616_bs;
  convert.argb64_from_custom[RASTER_FORMAT_RGB48_CUSTOM       ] = RasterOps_C::Convert::argb64_native_from_rgb48_dib;
  convert.argb64_from_custom[RASTER_FORMAT_RGB48_CUSTOM_BS    ] = RasterOps_C::Convert::argb64_native_from_rgb48_dib_bs;

  convert.argb64_from_custom[RASTER_FORMAT_ARGB16_4444        ] = RasterOps_C::Convert::argb64_native_from_argb16_4444;
  convert.argb64_from_custom[RASTER_FORMAT_ARGB16_4444_BS     ] = RasterOps_C::Convert::argb64_native_from_argb16_4444_bs;
  convert.argb64_from_custom[RASTER_FORMAT_ARGB16_CUSTOM      ] = RasterOps_C::Convert::argb64_native_from_argb16_dib;
  convert.argb64_from_custom[RASTER_FORMAT_ARGB16_CUSTOM_BS   ] = RasterOps_C::Convert::argb64_native_from_argb16_dib_bs;
  convert.argb64_from_custom[RASTER_FORMAT_ARGB24_CUSTOM      ] = RasterOps_C::Convert::argb64_native_from_argb24_dib;
  convert.argb64_from_custom[RASTER_FORMAT_ARGB24_CUSTOM_BS   ] = RasterOps_C::Convert::argb64_native_from_argb24_dib_bs;
//convert.argb64_from_custom[RASTER_FORMAT_ARGB32_8888        ] = SKIP;
  convert.argb64_from_custom[RASTER_FORMAT_ARGB32_8888_BS     ] = RasterOps_C::Convert::argb64_native_from_argb32_8888_bs;
  convert.argb64_from_custom[RASTER_FORMAT_ARGB32_CUSTOM      ] = RasterOps_C::Convert::argb64_native_from_argb32_dib;
  convert.argb64_from_custom[RASTER_FORMAT_ARGB32_CUSTOM_BS   ] = RasterOps_C::Convert::argb64_native_from_argb32_dib_bs;
  convert.argb64_from_custom[RASTER_FORMAT_ARGB48_CUSTOM      ] = RasterOps_C::Convert::argb64_native_from_argb48_dib;
  convert.argb64_from_custom[RASTER_FORMAT_ARGB48_CUSTOM_BS   ] = RasterOps_C::Convert::argb64_native_from_argb48_dib_bs;
//convert.argb64_from_custom[RASTER_FORMAT_ARGB64_16161616    ] = SKIP;
  convert.argb64_from_custom[RASTER_FORMAT_ARGB64_16161616_BS ] = RasterOps_C::Convert::argb64_native_from_argb64_16161616_bs;
  convert.argb64_from_custom[RASTER_FORMAT_ARGB64_CUSTOM      ] = RasterOps_C::Convert::argb64_native_from_argb64_dib;
  convert.argb64_from_custom[RASTER_FORMAT_ARGB64_CUSTOM_BS   ] = RasterOps_C::Convert::argb64_native_from_argb64_dib_bs;

//convert.argb64_from_custom[RASTER_FORMAT_I8                 ];

  // TODO:

  // --------------------------------------------------------------------------
  // [RasterOps - Composite - Src - PRGB32]
  // --------------------------------------------------------------------------

  {
    ApiRaster::_CompositeCore& funcs = api.compositeCore[IMAGE_FORMAT_PRGB32][RASTER_COMPOSITE_CORE_SRC];

    FOG_RASTER_INIT(cblit_line[RASTER_CBLIT_PRGB     ], RasterOps_C::CompositeSrc::prgb32_cblit_prgb32_line);
    FOG_RASTER_SKIP(cblit_line[RASTER_CBLIT_XRGB     ]);

    FOG_RASTER_INIT(cblit_span[RASTER_CBLIT_PRGB     ], RasterOps_C::CompositeSrc::prgb32_cblit_prgb32_span);
    FOG_RASTER_SKIP(cblit_span[RASTER_CBLIT_XRGB     ]);

    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_PRGB32   ], RasterOps_C::Convert::copy_32);
    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_XRGB32   ], RasterOps_C::CompositeSrc::frgb32_vblit_xrgb32_line);
    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_RGB24    ], RasterOps_C::CompositeSrc::frgb32_vblit_rgb24_line);
    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_A8       ], RasterOps_C::CompositeSrc::comp8_vblit_a8_line        <RasterOps_C::AccessPRGB32>);
    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_I8       ], RasterOps_C::CompositeSrc::comp8_vblit_i8_line        <RasterOps_C::AccessPRGB32>);
    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_PRGB64   ], RasterOps_C::CompositeSrc::comp8_vblit_prgb64_line    <RasterOps_C::AccessPRGB32>);
    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_RGB48    ], RasterOps_C::CompositeSrc::comp8_vblit_rgb48_line     <RasterOps_C::AccessPRGB32>);
    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_A16      ], RasterOps_C::CompositeSrc::comp8_vblit_a16_line       <RasterOps_C::AccessPRGB32>);

    FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_PRGB32   ], RasterOps_C::CompositeSrc::prgb32_vblit_prgb32_span);
    FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_XRGB32   ], RasterOps_C::CompositeSrc::frgb32_vblit_xrgb32_span);
    FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_RGB24    ], RasterOps_C::CompositeSrc::frgb32_vblit_rgb24_span);
    FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_A8       ], RasterOps_C::CompositeSrc::comp8_vblit_a8_span        <RasterOps_C::AccessPRGB32>);
    FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_I8       ], RasterOps_C::CompositeSrc::comp8_vblit_i8_span        <RasterOps_C::AccessPRGB32>);
    FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_PRGB64   ], RasterOps_C::CompositeSrc::comp8_vblit_prgb64_span    <RasterOps_C::AccessPRGB32>);
    FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_RGB48    ], RasterOps_C::CompositeSrc::comp8_vblit_rgb48_span     <RasterOps_C::AccessPRGB32>);
    FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_A16      ], RasterOps_C::CompositeSrc::comp8_vblit_a16_span       <RasterOps_C::AccessPRGB32>);
  }

  // --------------------------------------------------------------------------
  // [RasterOps - Composite - Src - XRGB32]
  // --------------------------------------------------------------------------

  {
    ApiRaster::_CompositeCore& funcs = api.compositeCore[IMAGE_FORMAT_XRGB32][RASTER_COMPOSITE_CORE_SRC];

    FOG_RASTER_INIT(cblit_line[RASTER_CBLIT_PRGB     ], RasterOps_C::CompositeSrc::xrgb32_cblit_prgb32_line);
    FOG_RASTER_SKIP(cblit_line[RASTER_CBLIT_XRGB     ]);

    FOG_RASTER_INIT(cblit_span[RASTER_CBLIT_PRGB     ], RasterOps_C::CompositeSrc::xrgb32_cblit_prgb32_span);
    FOG_RASTER_SKIP(cblit_span[RASTER_CBLIT_XRGB     ]);

    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_PRGB32   ], RasterOps_C::CompositeSrc::frgb32_vblit_xrgb32_line);
    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_XRGB32   ], RasterOps_C::Convert::copy_32);
    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_RGB24    ], RasterOps_C::CompositeSrc::frgb32_vblit_rgb24_line);
    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_A8       ], RasterOps_C::CompositeSrc::comp8_vblit_a8_line        <RasterOps_C::AccessXRGB32>);
    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_I8       ], RasterOps_C::CompositeSrc::comp8_vblit_i8_line        <RasterOps_C::AccessXRGB32>);
    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_PRGB64   ], RasterOps_C::CompositeSrc::comp8_vblit_prgb64_line    <RasterOps_C::AccessXRGB32>);
    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_RGB48    ], RasterOps_C::CompositeSrc::comp8_vblit_rgb48_line     <RasterOps_C::AccessXRGB32>);
    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_A16      ], RasterOps_C::CompositeSrc::comp8_vblit_a16_line       <RasterOps_C::AccessXRGB32>);

    FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_PRGB32   ], RasterOps_C::CompositeSrc::xrgb32_vblit_prgb32_span);
    FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_XRGB32   ], RasterOps_C::CompositeSrc::frgb32_vblit_xrgb32_span);
    FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_RGB24    ], RasterOps_C::CompositeSrc::frgb32_vblit_rgb24_span);
    FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_A8       ], RasterOps_C::CompositeSrc::comp8_vblit_a8_span        <RasterOps_C::AccessXRGB32>);
    FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_I8       ], RasterOps_C::CompositeSrc::comp8_vblit_i8_span        <RasterOps_C::AccessXRGB32>);
    FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_PRGB64   ], RasterOps_C::CompositeSrc::comp8_vblit_prgb64_span    <RasterOps_C::AccessXRGB32>);
    FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_RGB48    ], RasterOps_C::CompositeSrc::comp8_vblit_rgb48_span     <RasterOps_C::AccessXRGB32>);
    FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_A16      ], RasterOps_C::CompositeSrc::comp8_vblit_a16_span       <RasterOps_C::AccessXRGB32>);
  }

  // --------------------------------------------------------------------------
  // [RasterOps - Composite - Src - RGB24]
  // --------------------------------------------------------------------------

  {
    ApiRaster::_CompositeCore& funcs = api.compositeCore[IMAGE_FORMAT_RGB24][RASTER_COMPOSITE_CORE_SRC];

    FOG_RASTER_INIT(cblit_line[RASTER_CBLIT_PRGB     ], RasterOps_C::CompositeSrc::rgb24_cblit_prgb32_line);
    FOG_RASTER_SKIP(cblit_line[RASTER_CBLIT_XRGB     ]);

    FOG_RASTER_INIT(cblit_span[RASTER_CBLIT_PRGB     ], RasterOps_C::CompositeSrc::rgb24_cblit_prgb32_span);
    FOG_RASTER_SKIP(cblit_span[RASTER_CBLIT_XRGB     ]);

    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_PRGB32   ], RasterOps_C::CompositeSrc::rgb24_vblit_xrgb32_line);
    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_XRGB32   ], RasterOps_C::CompositeSrc::rgb24_vblit_xrgb32_line);
    FOG_RASTER_SKIP(vblit_line[IMAGE_FORMAT_RGB24    ]);
    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_A8       ], RasterOps_C::CompositeSrc::comp8_vblit_a8_line        <RasterOps_C::AccessRGB24>);
    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_I8       ], RasterOps_C::CompositeSrc::comp8_vblit_i8_line        <RasterOps_C::AccessRGB24>);
    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_PRGB64   ], RasterOps_C::CompositeSrc::comp8_vblit_prgb64_line    <RasterOps_C::AccessRGB24>);
    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_RGB48    ], RasterOps_C::CompositeSrc::comp8_vblit_rgb48_line     <RasterOps_C::AccessRGB24>);
    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_A16      ], RasterOps_C::CompositeSrc::comp8_vblit_a16_line       <RasterOps_C::AccessRGB24>);

    FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_PRGB32   ], RasterOps_C::CompositeSrc::rgb24_vblit_xrgb32_span);
    FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_XRGB32   ], RasterOps_C::CompositeSrc::rgb24_vblit_xrgb32_span);
  //FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_RGB24    ], RasterOps_C::CompositeSrc::rgb24_vblit_rgb24_span); // TODO
    FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_A8       ], RasterOps_C::CompositeSrc::comp8_vblit_a8_span        <RasterOps_C::AccessRGB24>);
    FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_I8       ], RasterOps_C::CompositeSrc::comp8_vblit_i8_span        <RasterOps_C::AccessRGB24>);
    FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_PRGB64   ], RasterOps_C::CompositeSrc::comp8_vblit_prgb64_span    <RasterOps_C::AccessRGB24>);
    FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_RGB48    ], RasterOps_C::CompositeSrc::comp8_vblit_rgb48_span     <RasterOps_C::AccessRGB24>);
    FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_A16      ], RasterOps_C::CompositeSrc::comp8_vblit_a16_span       <RasterOps_C::AccessRGB24>);
  }

  // --------------------------------------------------------------------------
  // [RasterOps - Composite - SrcOver - PRGB32]
  // --------------------------------------------------------------------------

  {
    ApiRaster::_CompositeCore& funcs = api.compositeCore[IMAGE_FORMAT_PRGB32][RASTER_COMPOSITE_CORE_SRC_OVER];

    FOG_RASTER_INIT(cblit_line[RASTER_CBLIT_PRGB     ], RasterOps_C::CompositeSrcOver::prgb32_cblit_prgb32_line);
    FOG_RASTER_SKIP(cblit_line[RASTER_CBLIT_XRGB     ]);

    FOG_RASTER_INIT(cblit_span[RASTER_CBLIT_PRGB     ], RasterOps_C::CompositeSrcOver::prgb32_cblit_prgb32_span);
    FOG_RASTER_SKIP(cblit_span[RASTER_CBLIT_XRGB     ]);

    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_PRGB32   ], RasterOps_C::CompositeSrcOver::prgb32_vblit_prgb32_line);
    FOG_RASTER_SKIP(vblit_line[IMAGE_FORMAT_XRGB32   ]);
    FOG_RASTER_SKIP(vblit_line[IMAGE_FORMAT_RGB24    ]);
  //FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_A8       ], RasterOps_C::CompositeSrc::prgb32_vblit_a8_line);
    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_I8       ], RasterOps_C::CompositeSrcOver::prgb32_vblit_i8_line);
  //FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_PRGB64   ], RasterOps_C::CompositeSrc::prgb32_vblit_prgb64_line);
    FOG_RASTER_SKIP(vblit_line[IMAGE_FORMAT_RGB48    ]);
  //FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_A16      ], RasterOps_C::CompositeSrc::prgb32_vblit_a16_line);

    FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_PRGB32   ], RasterOps_C::CompositeSrcOver::prgb32_vblit_prgb32_span);
    FOG_RASTER_SKIP(vblit_span[IMAGE_FORMAT_XRGB32   ]);
    FOG_RASTER_SKIP(vblit_span[IMAGE_FORMAT_RGB24    ]);
  //FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_A8       ], RasterOps_C::CompositeSrc::prgb32_vblit_a8_span);
  //FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_I8       ], RasterOps_C::CompositeSrc::prgb32_vblit_i8_span);
  //FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_PRGB64   ], RasterOps_C::CompositeSrc::prgb32_vblit_prgb64_span);
    FOG_RASTER_SKIP(vblit_span[IMAGE_FORMAT_RGB48    ]);
  //FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_A16      ], RasterOps_C::CompositeSrc::prgb32_vblit_a16_span);
  }

  // --------------------------------------------------------------------------
  // [RasterOps - Composite - SrcOver - XRGB32]
  // --------------------------------------------------------------------------

  {
    ApiRaster::_CompositeCore& funcs = api.compositeCore[IMAGE_FORMAT_XRGB32][RASTER_COMPOSITE_CORE_SRC_OVER];

    FOG_RASTER_INIT(cblit_line[RASTER_CBLIT_PRGB     ], RasterOps_C::CompositeSrcOver::prgb32_cblit_prgb32_line);
    FOG_RASTER_SKIP(cblit_line[RASTER_CBLIT_XRGB     ]);

    FOG_RASTER_INIT(cblit_span[RASTER_CBLIT_PRGB     ], RasterOps_C::CompositeSrcOver::prgb32_cblit_prgb32_span);
    FOG_RASTER_SKIP(cblit_span[RASTER_CBLIT_XRGB     ]);

    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_PRGB32   ], RasterOps_C::CompositeSrcOver::prgb32_vblit_prgb32_line);
    FOG_RASTER_SKIP(vblit_line[IMAGE_FORMAT_XRGB32   ]);
    FOG_RASTER_SKIP(vblit_line[IMAGE_FORMAT_RGB24    ]);
  //FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_A8       ], RasterOps_C::CompositeSrc::prgb32_vblit_a8_line);
    FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_I8       ], RasterOps_C::CompositeSrcOver::prgb32_vblit_i8_line);
  //FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_PRGB64   ], RasterOps_C::CompositeSrc::prgb32_vblit_prgb64_line);
    FOG_RASTER_SKIP(vblit_line[IMAGE_FORMAT_RGB48    ]);
  //FOG_RASTER_INIT(vblit_line[IMAGE_FORMAT_A16      ], RasterOps_C::CompositeSrc::prgb32_vblit_a16_line);

    FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_PRGB32   ], RasterOps_C::CompositeSrcOver::prgb32_vblit_prgb32_span);
    FOG_RASTER_SKIP(vblit_span[IMAGE_FORMAT_XRGB32   ]);
    FOG_RASTER_SKIP(vblit_span[IMAGE_FORMAT_RGB24    ]);
  //FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_A8       ], RasterOps_C::CompositeSrc::prgb32_vblit_a8_span);
  //FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_I8       ], RasterOps_C::CompositeSrc::prgb32_vblit_i8_span);
  //FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_PRGB64   ], RasterOps_C::CompositeSrc::prgb32_vblit_prgb64_span);
    FOG_RASTER_SKIP(vblit_span[IMAGE_FORMAT_RGB48    ]);
  //FOG_RASTER_INIT(vblit_span[IMAGE_FORMAT_A16      ], RasterOps_C::CompositeSrc::prgb32_vblit_a16_span);
  }

  // --------------------------------------------------------------------------
  // [RasterOps - Composite - SrcOver - RGB24]
  // --------------------------------------------------------------------------

  // --------------------------------------------------------------------------
  // [RasterOps - Composite - Clear]
  // --------------------------------------------------------------------------

#endif // FOG_RASTER_INIT_C

  // --------------------------------------------------------------------------
  // [RasterOps - Solid]
  // --------------------------------------------------------------------------

#if defined(FOG_RASTER_INIT_C)
  {
    ApiRaster::_Solid& solid = api.solid;

    solid.create = RasterOps_C::Helpers::p_solid_create_solid;
    solid.destroy = RasterOps_C::Helpers::p_solid_destroy;
    solid.prepare = RasterOps_C::Helpers::p_solid_prepare;

    solid.fetch[IMAGE_FORMAT_PRGB32] = RasterOps_C::Helpers::p_solid_fetch_prgb32_xrgb32;
    solid.fetch[IMAGE_FORMAT_XRGB32] = RasterOps_C::Helpers::p_solid_fetch_prgb32_xrgb32;
    solid.fetch[IMAGE_FORMAT_A8    ] = RasterOps_C::Helpers::p_solid_fetch_a8;

    solid.fetch[IMAGE_FORMAT_PRGB64] = RasterOps_C::Helpers::p_solid_fetch_prgb64;
    solid.fetch[IMAGE_FORMAT_RGB48 ] = RasterOps_C::Helpers::p_solid_fetch_rgb48;
    solid.fetch[IMAGE_FORMAT_A16   ] = RasterOps_C::Helpers::p_solid_fetch_a16;
  }
#endif

  // --------------------------------------------------------------------------
  // [RasterOps - Gradient - API]
  // --------------------------------------------------------------------------

  ApiRaster::_Gradient& gradient = api.gradient;

  // --------------------------------------------------------------------------
  // [RasterOps - Gradient - Interpolate]
  // --------------------------------------------------------------------------

#if defined(FOG_RASTER_INIT_C)
  gradient.interpolate[IMAGE_FORMAT_PRGB32] = RasterOps_C::PGradientBase::interpolate_prgb32;
  gradient.interpolate[IMAGE_FORMAT_XRGB32] = RasterOps_C::PGradientBase::interpolate_prgb32;
#endif

  // --------------------------------------------------------------------------
  // [RasterOps - Gradient - Linear]
  // --------------------------------------------------------------------------

  gradient.create[GRADIENT_TYPE_LINEAR] = RasterOps_C::PGradientLinear::create;

#if defined(FOG_RASTER_INIT_C)
  gradient.linear.fetch_simple_nearest[IMAGE_FORMAT_PRGB32][GRADIENT_SPREAD_PAD    ] = RasterOps_C::PGradientLinear::fetch_simple_nearest_pad_prgb32_xrgb32;
  gradient.linear.fetch_simple_nearest[IMAGE_FORMAT_XRGB32][GRADIENT_SPREAD_PAD    ] = RasterOps_C::PGradientLinear::fetch_simple_nearest_pad_prgb32_xrgb32;

  gradient.linear.fetch_simple_nearest[IMAGE_FORMAT_PRGB32][GRADIENT_SPREAD_REPEAT ] = RasterOps_C::PGradientLinear::fetch_simple_nearest_repeat_prgb32_xrgb32;
  gradient.linear.fetch_simple_nearest[IMAGE_FORMAT_XRGB32][GRADIENT_SPREAD_REPEAT ] = RasterOps_C::PGradientLinear::fetch_simple_nearest_repeat_prgb32_xrgb32;

  gradient.linear.fetch_simple_nearest[IMAGE_FORMAT_PRGB32][GRADIENT_SPREAD_REFLECT] = RasterOps_C::PGradientLinear::fetch_simple_nearest_reflect_prgb32_xrgb32;
  gradient.linear.fetch_simple_nearest[IMAGE_FORMAT_XRGB32][GRADIENT_SPREAD_REFLECT] = RasterOps_C::PGradientLinear::fetch_simple_nearest_reflect_prgb32_xrgb32;

  gradient.linear.fetch_proj_nearest  [IMAGE_FORMAT_PRGB32][GRADIENT_SPREAD_PAD    ] = RasterOps_C::PGradientLinear::fetch_proj_nearest<RasterOps_C::PGradientAccessor_PRGB32_Pad>;
  gradient.linear.fetch_proj_nearest  [IMAGE_FORMAT_XRGB32][GRADIENT_SPREAD_PAD    ] = RasterOps_C::PGradientLinear::fetch_proj_nearest<RasterOps_C::PGradientAccessor_PRGB32_Pad>;

  gradient.linear.fetch_proj_nearest  [IMAGE_FORMAT_PRGB32][GRADIENT_SPREAD_REPEAT ] = RasterOps_C::PGradientLinear::fetch_proj_nearest<RasterOps_C::PGradientAccessor_PRGB32_Repeat>;
  gradient.linear.fetch_proj_nearest  [IMAGE_FORMAT_XRGB32][GRADIENT_SPREAD_REPEAT ] = RasterOps_C::PGradientLinear::fetch_proj_nearest<RasterOps_C::PGradientAccessor_PRGB32_Repeat>;

  gradient.linear.fetch_proj_nearest  [IMAGE_FORMAT_PRGB32][GRADIENT_SPREAD_REFLECT] = RasterOps_C::PGradientLinear::fetch_proj_nearest<RasterOps_C::PGradientAccessor_PRGB32_Reflect>;
  gradient.linear.fetch_proj_nearest  [IMAGE_FORMAT_XRGB32][GRADIENT_SPREAD_REFLECT] = RasterOps_C::PGradientLinear::fetch_proj_nearest<RasterOps_C::PGradientAccessor_PRGB32_Reflect>;
#endif

  // --------------------------------------------------------------------------
  // [RasterOps - Gradient - Radial]
  // --------------------------------------------------------------------------

  gradient.create[GRADIENT_TYPE_RADIAL] = RasterOps_C::PGradientRadial::create;

#if defined(FOG_RASTER_INIT_C)
  gradient.radial.fetch_simple_nearest[IMAGE_FORMAT_PRGB32][GRADIENT_SPREAD_PAD    ] = RasterOps_C::PGradientRadial::fetch_simple_nearest<RasterOps_C::PGradientAccessor_PRGB32_Pad>;
  gradient.radial.fetch_simple_nearest[IMAGE_FORMAT_XRGB32][GRADIENT_SPREAD_PAD    ] = RasterOps_C::PGradientRadial::fetch_simple_nearest<RasterOps_C::PGradientAccessor_PRGB32_Pad>;

  gradient.radial.fetch_simple_nearest[IMAGE_FORMAT_PRGB32][GRADIENT_SPREAD_REPEAT ] = RasterOps_C::PGradientRadial::fetch_simple_nearest<RasterOps_C::PGradientAccessor_PRGB32_Repeat>;
  gradient.radial.fetch_simple_nearest[IMAGE_FORMAT_XRGB32][GRADIENT_SPREAD_REPEAT ] = RasterOps_C::PGradientRadial::fetch_simple_nearest<RasterOps_C::PGradientAccessor_PRGB32_Repeat>;

  gradient.radial.fetch_simple_nearest[IMAGE_FORMAT_PRGB32][GRADIENT_SPREAD_REFLECT] = RasterOps_C::PGradientRadial::fetch_simple_nearest<RasterOps_C::PGradientAccessor_PRGB32_Reflect>;
  gradient.radial.fetch_simple_nearest[IMAGE_FORMAT_XRGB32][GRADIENT_SPREAD_REFLECT] = RasterOps_C::PGradientRadial::fetch_simple_nearest<RasterOps_C::PGradientAccessor_PRGB32_Reflect>;

  gradient.radial.fetch_proj_nearest  [IMAGE_FORMAT_PRGB32][GRADIENT_SPREAD_PAD    ] = RasterOps_C::PGradientRadial::fetch_proj_nearest<RasterOps_C::PGradientAccessor_PRGB32_Pad>;
  gradient.radial.fetch_proj_nearest  [IMAGE_FORMAT_XRGB32][GRADIENT_SPREAD_PAD    ] = RasterOps_C::PGradientRadial::fetch_proj_nearest<RasterOps_C::PGradientAccessor_PRGB32_Pad>;

  gradient.radial.fetch_proj_nearest  [IMAGE_FORMAT_PRGB32][GRADIENT_SPREAD_REPEAT ] = RasterOps_C::PGradientRadial::fetch_proj_nearest<RasterOps_C::PGradientAccessor_PRGB32_Repeat>;
  gradient.radial.fetch_proj_nearest  [IMAGE_FORMAT_XRGB32][GRADIENT_SPREAD_REPEAT ] = RasterOps_C::PGradientRadial::fetch_proj_nearest<RasterOps_C::PGradientAccessor_PRGB32_Repeat>;

  gradient.radial.fetch_proj_nearest  [IMAGE_FORMAT_PRGB32][GRADIENT_SPREAD_REFLECT] = RasterOps_C::PGradientRadial::fetch_proj_nearest<RasterOps_C::PGradientAccessor_PRGB32_Reflect>;
  gradient.radial.fetch_proj_nearest  [IMAGE_FORMAT_XRGB32][GRADIENT_SPREAD_REFLECT] = RasterOps_C::PGradientRadial::fetch_proj_nearest<RasterOps_C::PGradientAccessor_PRGB32_Reflect>;
#endif

  // --------------------------------------------------------------------------
  // [RasterOps - Gradient - Conical]
  // --------------------------------------------------------------------------

  gradient.create[GRADIENT_TYPE_CONICAL] = RasterOps_C::PGradientConical::create;

#if defined(FOG_RASTER_INIT_C)
  gradient.conical.fetch_simple_nearest[IMAGE_FORMAT_PRGB32] = RasterOps_C::PGradientConical::fetch_simple_nearest<RasterOps_C::PGradientAccessor_PRGB32_Base>;
  gradient.conical.fetch_simple_nearest[IMAGE_FORMAT_XRGB32] = RasterOps_C::PGradientConical::fetch_simple_nearest<RasterOps_C::PGradientAccessor_PRGB32_Base>;
#endif

  // --------------------------------------------------------------------------
  // [RasterOps - Gradient - Rectangular]
  // --------------------------------------------------------------------------

  gradient.create[GRADIENT_TYPE_RECTANGULAR] = RasterOps_C::PGradientRectangular::create;

#if defined(FOG_RASTER_INIT_C)
  gradient.rectangular.fetch_simple_nearest[IMAGE_FORMAT_PRGB32][GRADIENT_SPREAD_PAD    ] = RasterOps_C::PGradientRectangular::fetch_simple_nearest<RasterOps_C::PGradientAccessor_PRGB32_Pad>;
  gradient.rectangular.fetch_simple_nearest[IMAGE_FORMAT_XRGB32][GRADIENT_SPREAD_PAD    ] = RasterOps_C::PGradientRectangular::fetch_simple_nearest<RasterOps_C::PGradientAccessor_PRGB32_Pad>;

  gradient.rectangular.fetch_simple_nearest[IMAGE_FORMAT_PRGB32][GRADIENT_SPREAD_REPEAT ] = RasterOps_C::PGradientRectangular::fetch_simple_nearest<RasterOps_C::PGradientAccessor_PRGB32_Repeat>;
  gradient.rectangular.fetch_simple_nearest[IMAGE_FORMAT_XRGB32][GRADIENT_SPREAD_REPEAT ] = RasterOps_C::PGradientRectangular::fetch_simple_nearest<RasterOps_C::PGradientAccessor_PRGB32_Repeat>;

  gradient.rectangular.fetch_simple_nearest[IMAGE_FORMAT_PRGB32][GRADIENT_SPREAD_REFLECT] = RasterOps_C::PGradientRectangular::fetch_simple_nearest<RasterOps_C::PGradientAccessor_PRGB32_Reflect>;
  gradient.rectangular.fetch_simple_nearest[IMAGE_FORMAT_XRGB32][GRADIENT_SPREAD_REFLECT] = RasterOps_C::PGradientRectangular::fetch_simple_nearest<RasterOps_C::PGradientAccessor_PRGB32_Reflect>;

  gradient.rectangular.fetch_proj_nearest  [IMAGE_FORMAT_PRGB32][GRADIENT_SPREAD_PAD    ] = RasterOps_C::PGradientRectangular::fetch_proj_nearest<RasterOps_C::PGradientAccessor_PRGB32_Pad>;
  gradient.rectangular.fetch_proj_nearest  [IMAGE_FORMAT_XRGB32][GRADIENT_SPREAD_PAD    ] = RasterOps_C::PGradientRectangular::fetch_proj_nearest<RasterOps_C::PGradientAccessor_PRGB32_Pad>;

  gradient.rectangular.fetch_proj_nearest  [IMAGE_FORMAT_PRGB32][GRADIENT_SPREAD_REPEAT ] = RasterOps_C::PGradientRectangular::fetch_proj_nearest<RasterOps_C::PGradientAccessor_PRGB32_Repeat>;
  gradient.rectangular.fetch_proj_nearest  [IMAGE_FORMAT_XRGB32][GRADIENT_SPREAD_REPEAT ] = RasterOps_C::PGradientRectangular::fetch_proj_nearest<RasterOps_C::PGradientAccessor_PRGB32_Repeat>;

  gradient.rectangular.fetch_proj_nearest  [IMAGE_FORMAT_PRGB32][GRADIENT_SPREAD_REFLECT] = RasterOps_C::PGradientRectangular::fetch_proj_nearest<RasterOps_C::PGradientAccessor_PRGB32_Reflect>;
  gradient.rectangular.fetch_proj_nearest  [IMAGE_FORMAT_XRGB32][GRADIENT_SPREAD_REFLECT] = RasterOps_C::PGradientRectangular::fetch_proj_nearest<RasterOps_C::PGradientAccessor_PRGB32_Reflect>;
#endif

  // --------------------------------------------------------------------------
  // [RasterOps - Texture - API]
  // --------------------------------------------------------------------------

  ApiRaster::_Texture& texture = api.texture;

  // --------------------------------------------------------------------------
  // [RasterOps - Texture - Simple]
  // --------------------------------------------------------------------------

  texture.create = RasterOps_C::PTextureBase::create;

#if defined(FOG_RASTER_INIT_C)
  texture.prgb32.fetch_simple_align[IMAGE_FORMAT_PRGB32][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureSimple::fetch_align_pad<RasterOps_C::PTextureAccessor_PRGB32_From_PRGB32>;
  texture.prgb32.fetch_simple_align[IMAGE_FORMAT_XRGB32][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureSimple::fetch_align_pad<RasterOps_C::PTextureAccessor_PRGB32_From_XRGB32>;
  texture.prgb32.fetch_simple_align[IMAGE_FORMAT_RGB24 ][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureSimple::fetch_align_pad<RasterOps_C::PTextureAccessor_PRGB32_From_RGB24 >;
  texture.prgb32.fetch_simple_align[IMAGE_FORMAT_A8    ][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureSimple::fetch_align_pad<RasterOps_C::PTextureAccessor_PRGB32_From_A8    >;
  texture.prgb32.fetch_simple_align[IMAGE_FORMAT_I8    ][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureSimple::fetch_align_pad<RasterOps_C::PTextureAccessor_PRGB32_From_I8    >;

  texture.prgb32.fetch_simple_subx0[IMAGE_FORMAT_PRGB32][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureSimple::fetch_subx0_pad<RasterOps_C::PTextureAccessor_PRGB32_From_PRGB32>;
  texture.prgb32.fetch_simple_subx0[IMAGE_FORMAT_XRGB32][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureSimple::fetch_subx0_pad<RasterOps_C::PTextureAccessor_PRGB32_From_XRGB32>;
  texture.prgb32.fetch_simple_subx0[IMAGE_FORMAT_RGB24 ][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureSimple::fetch_subx0_pad<RasterOps_C::PTextureAccessor_PRGB32_From_RGB24 >;
  texture.prgb32.fetch_simple_subx0[IMAGE_FORMAT_A8    ][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureSimple::fetch_subx0_pad<RasterOps_C::PTextureAccessor_PRGB32_From_A8    >;
  texture.prgb32.fetch_simple_subx0[IMAGE_FORMAT_I8    ][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureSimple::fetch_subx0_pad<RasterOps_C::PTextureAccessor_PRGB32_From_I8    >;

  texture.prgb32.fetch_simple_sub0y[IMAGE_FORMAT_PRGB32][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureSimple::fetch_sub0y_pad<RasterOps_C::PTextureAccessor_PRGB32_From_PRGB32>;
  texture.prgb32.fetch_simple_sub0y[IMAGE_FORMAT_XRGB32][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureSimple::fetch_sub0y_pad<RasterOps_C::PTextureAccessor_PRGB32_From_XRGB32>;
  texture.prgb32.fetch_simple_sub0y[IMAGE_FORMAT_RGB24 ][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureSimple::fetch_sub0y_pad<RasterOps_C::PTextureAccessor_PRGB32_From_RGB24 >;
  texture.prgb32.fetch_simple_sub0y[IMAGE_FORMAT_A8    ][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureSimple::fetch_sub0y_pad<RasterOps_C::PTextureAccessor_PRGB32_From_A8    >;
  texture.prgb32.fetch_simple_sub0y[IMAGE_FORMAT_I8    ][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureSimple::fetch_sub0y_pad<RasterOps_C::PTextureAccessor_PRGB32_From_I8    >;

  texture.prgb32.fetch_simple_subxy[IMAGE_FORMAT_PRGB32][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureSimple::fetch_subxy_pad<RasterOps_C::PTextureAccessor_PRGB32_From_PRGB32>;
  texture.prgb32.fetch_simple_subxy[IMAGE_FORMAT_XRGB32][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureSimple::fetch_subxy_pad<RasterOps_C::PTextureAccessor_PRGB32_From_XRGB32>;
  texture.prgb32.fetch_simple_subxy[IMAGE_FORMAT_RGB24 ][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureSimple::fetch_subxy_pad<RasterOps_C::PTextureAccessor_PRGB32_From_RGB24 >;
  texture.prgb32.fetch_simple_subxy[IMAGE_FORMAT_A8    ][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureSimple::fetch_subxy_pad<RasterOps_C::PTextureAccessor_PRGB32_From_A8    >;
  texture.prgb32.fetch_simple_subxy[IMAGE_FORMAT_I8    ][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureSimple::fetch_subxy_pad<RasterOps_C::PTextureAccessor_PRGB32_From_I8    >;

  texture.prgb32.fetch_simple_align[IMAGE_FORMAT_PRGB32][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureSimple::fetch_align_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_PRGB32>;
  texture.prgb32.fetch_simple_align[IMAGE_FORMAT_XRGB32][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureSimple::fetch_align_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_XRGB32>;
  texture.prgb32.fetch_simple_align[IMAGE_FORMAT_RGB24 ][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureSimple::fetch_align_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_RGB24 >;
  texture.prgb32.fetch_simple_align[IMAGE_FORMAT_A8    ][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureSimple::fetch_align_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_A8    >;
  texture.prgb32.fetch_simple_align[IMAGE_FORMAT_I8    ][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureSimple::fetch_align_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_I8    >;

  texture.prgb32.fetch_simple_subx0[IMAGE_FORMAT_PRGB32][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureSimple::fetch_subx0_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_PRGB32>;
  texture.prgb32.fetch_simple_subx0[IMAGE_FORMAT_XRGB32][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureSimple::fetch_subx0_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_XRGB32>;
  texture.prgb32.fetch_simple_subx0[IMAGE_FORMAT_RGB24 ][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureSimple::fetch_subx0_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_RGB24 >;
  texture.prgb32.fetch_simple_subx0[IMAGE_FORMAT_A8    ][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureSimple::fetch_subx0_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_A8    >;
  texture.prgb32.fetch_simple_subx0[IMAGE_FORMAT_I8    ][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureSimple::fetch_subx0_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_I8    >;

  texture.prgb32.fetch_simple_sub0y[IMAGE_FORMAT_PRGB32][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureSimple::fetch_sub0y_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_PRGB32>;
  texture.prgb32.fetch_simple_sub0y[IMAGE_FORMAT_XRGB32][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureSimple::fetch_sub0y_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_XRGB32>;
  texture.prgb32.fetch_simple_sub0y[IMAGE_FORMAT_RGB24 ][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureSimple::fetch_sub0y_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_RGB24 >;
  texture.prgb32.fetch_simple_sub0y[IMAGE_FORMAT_A8    ][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureSimple::fetch_sub0y_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_A8    >;
  texture.prgb32.fetch_simple_sub0y[IMAGE_FORMAT_I8    ][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureSimple::fetch_sub0y_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_I8    >;

  texture.prgb32.fetch_simple_subxy[IMAGE_FORMAT_PRGB32][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureSimple::fetch_subxy_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_PRGB32>;
  texture.prgb32.fetch_simple_subxy[IMAGE_FORMAT_XRGB32][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureSimple::fetch_subxy_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_XRGB32>;
  texture.prgb32.fetch_simple_subxy[IMAGE_FORMAT_RGB24 ][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureSimple::fetch_subxy_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_RGB24 >;
  texture.prgb32.fetch_simple_subxy[IMAGE_FORMAT_A8    ][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureSimple::fetch_subxy_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_A8    >;
  texture.prgb32.fetch_simple_subxy[IMAGE_FORMAT_I8    ][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureSimple::fetch_subxy_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_I8    >;

  texture.prgb32.fetch_simple_align[IMAGE_FORMAT_PRGB32][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureSimple::fetch_align_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_PRGB32>;
  texture.prgb32.fetch_simple_align[IMAGE_FORMAT_XRGB32][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureSimple::fetch_align_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_XRGB32>;
  texture.prgb32.fetch_simple_align[IMAGE_FORMAT_RGB24 ][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureSimple::fetch_align_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_RGB24 >;
  texture.prgb32.fetch_simple_align[IMAGE_FORMAT_A8    ][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureSimple::fetch_align_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_A8    >;
  texture.prgb32.fetch_simple_align[IMAGE_FORMAT_I8    ][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureSimple::fetch_align_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_I8    >;

  texture.prgb32.fetch_simple_subx0[IMAGE_FORMAT_PRGB32][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureSimple::fetch_subx0_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_PRGB32>;
  texture.prgb32.fetch_simple_subx0[IMAGE_FORMAT_XRGB32][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureSimple::fetch_subx0_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_XRGB32>;
  texture.prgb32.fetch_simple_subx0[IMAGE_FORMAT_RGB24 ][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureSimple::fetch_subx0_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_RGB24 >;
  texture.prgb32.fetch_simple_subx0[IMAGE_FORMAT_A8    ][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureSimple::fetch_subx0_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_A8    >;
  texture.prgb32.fetch_simple_subx0[IMAGE_FORMAT_I8    ][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureSimple::fetch_subx0_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_I8    >;

  texture.prgb32.fetch_simple_sub0y[IMAGE_FORMAT_PRGB32][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureSimple::fetch_sub0y_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_PRGB32>;
  texture.prgb32.fetch_simple_sub0y[IMAGE_FORMAT_XRGB32][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureSimple::fetch_sub0y_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_XRGB32>;
  texture.prgb32.fetch_simple_sub0y[IMAGE_FORMAT_RGB24 ][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureSimple::fetch_sub0y_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_RGB24 >;
  texture.prgb32.fetch_simple_sub0y[IMAGE_FORMAT_A8    ][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureSimple::fetch_sub0y_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_A8    >;
  texture.prgb32.fetch_simple_sub0y[IMAGE_FORMAT_I8    ][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureSimple::fetch_sub0y_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_I8    >;

  texture.prgb32.fetch_simple_subxy[IMAGE_FORMAT_PRGB32][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureSimple::fetch_subxy_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_PRGB32>;
  texture.prgb32.fetch_simple_subxy[IMAGE_FORMAT_XRGB32][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureSimple::fetch_subxy_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_XRGB32>;
  texture.prgb32.fetch_simple_subxy[IMAGE_FORMAT_RGB24 ][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureSimple::fetch_subxy_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_RGB24 >;
  texture.prgb32.fetch_simple_subxy[IMAGE_FORMAT_A8    ][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureSimple::fetch_subxy_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_A8    >;
  texture.prgb32.fetch_simple_subxy[IMAGE_FORMAT_I8    ][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureSimple::fetch_subxy_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_I8    >;

  texture.prgb32.fetch_simple_align[IMAGE_FORMAT_PRGB32][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureSimple::fetch_align_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_PRGB32>;
  texture.prgb32.fetch_simple_align[IMAGE_FORMAT_XRGB32][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureSimple::fetch_align_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_XRGB32>;
  texture.prgb32.fetch_simple_align[IMAGE_FORMAT_RGB24 ][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureSimple::fetch_align_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_RGB24 >;
  texture.prgb32.fetch_simple_align[IMAGE_FORMAT_A8    ][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureSimple::fetch_align_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_A8    >;
  texture.prgb32.fetch_simple_align[IMAGE_FORMAT_I8    ][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureSimple::fetch_align_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_I8    >;

  texture.prgb32.fetch_simple_subx0[IMAGE_FORMAT_PRGB32][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureSimple::fetch_subx0_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_PRGB32>;
  texture.prgb32.fetch_simple_subx0[IMAGE_FORMAT_XRGB32][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureSimple::fetch_subx0_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_XRGB32>;
  texture.prgb32.fetch_simple_subx0[IMAGE_FORMAT_RGB24 ][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureSimple::fetch_subx0_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_RGB24 >;
  texture.prgb32.fetch_simple_subx0[IMAGE_FORMAT_A8    ][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureSimple::fetch_subx0_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_A8    >;
  texture.prgb32.fetch_simple_subx0[IMAGE_FORMAT_I8    ][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureSimple::fetch_subx0_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_I8    >;

  texture.prgb32.fetch_simple_sub0y[IMAGE_FORMAT_PRGB32][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureSimple::fetch_sub0y_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_PRGB32>;
  texture.prgb32.fetch_simple_sub0y[IMAGE_FORMAT_XRGB32][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureSimple::fetch_sub0y_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_XRGB32>;
  texture.prgb32.fetch_simple_sub0y[IMAGE_FORMAT_RGB24 ][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureSimple::fetch_sub0y_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_RGB24 >;
  texture.prgb32.fetch_simple_sub0y[IMAGE_FORMAT_A8    ][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureSimple::fetch_sub0y_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_A8    >;
  texture.prgb32.fetch_simple_sub0y[IMAGE_FORMAT_I8    ][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureSimple::fetch_sub0y_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_I8    >;

  texture.prgb32.fetch_simple_subxy[IMAGE_FORMAT_PRGB32][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureSimple::fetch_subxy_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_PRGB32>;
  texture.prgb32.fetch_simple_subxy[IMAGE_FORMAT_XRGB32][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureSimple::fetch_subxy_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_XRGB32>;
  texture.prgb32.fetch_simple_subxy[IMAGE_FORMAT_RGB24 ][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureSimple::fetch_subxy_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_RGB24 >;
  texture.prgb32.fetch_simple_subxy[IMAGE_FORMAT_A8    ][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureSimple::fetch_subxy_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_A8    >;
  texture.prgb32.fetch_simple_subxy[IMAGE_FORMAT_I8    ][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureSimple::fetch_subxy_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_I8    >;

  // --------------------------------------------------------------------------
  // [RasterOps - Texture - Affine]
  // --------------------------------------------------------------------------

  texture.prgb32.fetch_affine_nearest [IMAGE_FORMAT_PRGB32][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureAffine::fetch_affine_nearest_pad<RasterOps_C::PTextureAccessor_PRGB32_From_PRGB32>;
  texture.prgb32.fetch_affine_nearest [IMAGE_FORMAT_XRGB32][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureAffine::fetch_affine_nearest_pad<RasterOps_C::PTextureAccessor_PRGB32_From_XRGB32>;
  texture.prgb32.fetch_affine_nearest [IMAGE_FORMAT_RGB24 ][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureAffine::fetch_affine_nearest_pad<RasterOps_C::PTextureAccessor_PRGB32_From_RGB24 >;
  texture.prgb32.fetch_affine_nearest [IMAGE_FORMAT_A8    ][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureAffine::fetch_affine_nearest_pad<RasterOps_C::PTextureAccessor_PRGB32_From_A8    >;
  texture.prgb32.fetch_affine_nearest [IMAGE_FORMAT_I8    ][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureAffine::fetch_affine_nearest_pad<RasterOps_C::PTextureAccessor_PRGB32_From_I8    >;

  texture.prgb32.fetch_affine_bilinear[IMAGE_FORMAT_PRGB32][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureAffine::fetch_affine_bilinear_pad<RasterOps_C::PTextureAccessor_PRGB32_From_PRGB32>;
  texture.prgb32.fetch_affine_bilinear[IMAGE_FORMAT_XRGB32][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureAffine::fetch_affine_bilinear_pad<RasterOps_C::PTextureAccessor_PRGB32_From_XRGB32>;
  texture.prgb32.fetch_affine_bilinear[IMAGE_FORMAT_RGB24 ][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureAffine::fetch_affine_bilinear_pad<RasterOps_C::PTextureAccessor_PRGB32_From_RGB24 >;
  texture.prgb32.fetch_affine_bilinear[IMAGE_FORMAT_A8    ][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureAffine::fetch_affine_bilinear_pad<RasterOps_C::PTextureAccessor_PRGB32_From_A8    >;
  texture.prgb32.fetch_affine_bilinear[IMAGE_FORMAT_I8    ][TEXTURE_TILE_PAD    ] = RasterOps_C::PTextureAffine::fetch_affine_bilinear_pad<RasterOps_C::PTextureAccessor_PRGB32_From_I8    >;

  texture.prgb32.fetch_affine_nearest [IMAGE_FORMAT_PRGB32][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureAffine::fetch_affine_nearest_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_PRGB32>;
  texture.prgb32.fetch_affine_nearest [IMAGE_FORMAT_XRGB32][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureAffine::fetch_affine_nearest_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_XRGB32>;
  texture.prgb32.fetch_affine_nearest [IMAGE_FORMAT_RGB24 ][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureAffine::fetch_affine_nearest_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_RGB24 >;
  texture.prgb32.fetch_affine_nearest [IMAGE_FORMAT_A8    ][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureAffine::fetch_affine_nearest_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_A8    >;
  texture.prgb32.fetch_affine_nearest [IMAGE_FORMAT_I8    ][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureAffine::fetch_affine_nearest_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_I8    >;

  texture.prgb32.fetch_affine_bilinear[IMAGE_FORMAT_PRGB32][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureAffine::fetch_affine_bilinear_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_PRGB32>;
  texture.prgb32.fetch_affine_bilinear[IMAGE_FORMAT_XRGB32][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureAffine::fetch_affine_bilinear_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_XRGB32>;
  texture.prgb32.fetch_affine_bilinear[IMAGE_FORMAT_RGB24 ][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureAffine::fetch_affine_bilinear_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_RGB24 >;
  texture.prgb32.fetch_affine_bilinear[IMAGE_FORMAT_A8    ][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureAffine::fetch_affine_bilinear_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_A8    >;
  texture.prgb32.fetch_affine_bilinear[IMAGE_FORMAT_I8    ][TEXTURE_TILE_REPEAT ] = RasterOps_C::PTextureAffine::fetch_affine_bilinear_repeat<RasterOps_C::PTextureAccessor_PRGB32_From_I8    >;

  texture.prgb32.fetch_affine_nearest [IMAGE_FORMAT_PRGB32][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureAffine::fetch_affine_nearest_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_PRGB32>;
  texture.prgb32.fetch_affine_nearest [IMAGE_FORMAT_XRGB32][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureAffine::fetch_affine_nearest_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_XRGB32>;
  texture.prgb32.fetch_affine_nearest [IMAGE_FORMAT_RGB24 ][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureAffine::fetch_affine_nearest_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_RGB24 >;
  texture.prgb32.fetch_affine_nearest [IMAGE_FORMAT_A8    ][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureAffine::fetch_affine_nearest_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_A8    >;
  texture.prgb32.fetch_affine_nearest [IMAGE_FORMAT_I8    ][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureAffine::fetch_affine_nearest_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_I8    >;

  texture.prgb32.fetch_affine_bilinear[IMAGE_FORMAT_PRGB32][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureAffine::fetch_affine_bilinear_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_PRGB32>;
  texture.prgb32.fetch_affine_bilinear[IMAGE_FORMAT_XRGB32][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureAffine::fetch_affine_bilinear_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_XRGB32>;
  texture.prgb32.fetch_affine_bilinear[IMAGE_FORMAT_RGB24 ][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureAffine::fetch_affine_bilinear_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_RGB24 >;
  texture.prgb32.fetch_affine_bilinear[IMAGE_FORMAT_A8    ][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureAffine::fetch_affine_bilinear_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_A8    >;
  texture.prgb32.fetch_affine_bilinear[IMAGE_FORMAT_I8    ][TEXTURE_TILE_REFLECT] = RasterOps_C::PTextureAffine::fetch_affine_bilinear_reflect<RasterOps_C::PTextureAccessor_PRGB32_From_I8    >;

  texture.prgb32.fetch_affine_nearest [IMAGE_FORMAT_PRGB32][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureAffine::fetch_affine_nearest_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_PRGB32>;
  texture.prgb32.fetch_affine_nearest [IMAGE_FORMAT_XRGB32][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureAffine::fetch_affine_nearest_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_XRGB32>;
  texture.prgb32.fetch_affine_nearest [IMAGE_FORMAT_RGB24 ][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureAffine::fetch_affine_nearest_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_RGB24 >;
  texture.prgb32.fetch_affine_nearest [IMAGE_FORMAT_A8    ][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureAffine::fetch_affine_nearest_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_A8    >;
  texture.prgb32.fetch_affine_nearest [IMAGE_FORMAT_I8    ][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureAffine::fetch_affine_nearest_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_I8    >;

  texture.prgb32.fetch_affine_bilinear[IMAGE_FORMAT_PRGB32][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureAffine::fetch_affine_bilinear_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_PRGB32>;
  texture.prgb32.fetch_affine_bilinear[IMAGE_FORMAT_XRGB32][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureAffine::fetch_affine_bilinear_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_XRGB32>;
  texture.prgb32.fetch_affine_bilinear[IMAGE_FORMAT_RGB24 ][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureAffine::fetch_affine_bilinear_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_RGB24 >;
  texture.prgb32.fetch_affine_bilinear[IMAGE_FORMAT_A8    ][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureAffine::fetch_affine_bilinear_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_A8    >;
  texture.prgb32.fetch_affine_bilinear[IMAGE_FORMAT_I8    ][TEXTURE_TILE_CLAMP  ] = RasterOps_C::PTextureAffine::fetch_affine_bilinear_clamp<RasterOps_C::PTextureAccessor_PRGB32_From_I8    >;

  // --------------------------------------------------------------------------
  // [RasterOps - Texture - Projection]
  // --------------------------------------------------------------------------

  // TODO:

#endif
}

} // Fog namespace