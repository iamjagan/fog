// [Fog/Svg Library - C++ API]
//
// [Licence]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_SVG_SVGUTIL_H
#define _FOG_SVG_SVGUTIL_H

// [Dependencies]
#include <Fog/Core/Char.h>
#include <Fog/Core/String.h>
#include <Fog/Core/Vector.h>
#include <Fog/Graphics/Matrix.h>
#include <Fog/Graphics/Rgba.h>
#include <Fog/Graphics/Path.h>
#include <Fog/Svg/SvgBase.h>

namespace Fog {

//! @addtogroup Fog_Svg
//! @{

// ============================================================================
// [Svg::Util]
// ============================================================================

namespace SvgUtil {

FOG_API SvgStatus parseColor(const String& str, Rgba* dst);
FOG_API SvgCoord parseCoord(const String& str);
FOG_API SvgStatus parseMatrix(const String& str, Matrix* dst);

FOG_API Vector<SvgStyleItem> parseStyles(const String& str);
FOG_API String joinStyles(const Vector<SvgStyleItem>& items);

FOG_API Path parsePoints(const String& str);
FOG_API Path parsePath(const String& str);

} // SvgUtil namespace

//! @}

} // Fog namespace

// [Guard]
#endif // _FOG_SVG_SVGUTIL_H