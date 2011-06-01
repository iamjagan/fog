// [Fog-Core]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_CORE_TOOLS_STRINGFILTER_H
#define _FOG_CORE_TOOLS_STRINGFILTER_H

// [Dependencies]
#include <Fog/Core/Global/Assert.h>
#include <Fog/Core/Global/Class.h>
#include <Fog/Core/Tools/String.h>

namespace Fog {

//! @addtogroup Fog_Core_Tools
//! @{

// ============================================================================
// [Fog::StringFilter]
// ============================================================================

struct FOG_API StringFilter
{
  // [Construction / Destruction]

  StringFilter();
  virtual ~StringFilter();

  // [Public]

  virtual Range indexOf(const Char* str, sysuint_t slen, uint cs = CASE_SENSITIVE, const Range& range = Range(0)) const;
  virtual Range lastIndexOf(const Char* str, sysuint_t slen, uint cs = CASE_SENSITIVE, const Range& range = Range(0)) const;

  // [Filter Implementation]

  virtual sysuint_t getLength() const;
  virtual Range match(const Char* str, sysuint_t slen, uint cs, const Range& range) const = 0;

private:
  _FOG_CLASS_NO_COPY(StringFilter)
};

//! @}

} // Fog namespace

// [Guard]
#endif // _FOG_CORE_TOOLS_STRINGFILTER_H