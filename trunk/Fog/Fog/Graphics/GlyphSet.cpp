// [Fog/Graphics Library - C++ API]
//
// [Licence] 
// MIT, See COPYING file in package

// [Precompiled headers]
#ifdef FOG_PRECOMP
#include FOG_PRECOMP
#endif

// [Dependencies]
#include <Fog/Core/Atomic.h>
#include <Fog/Core/Memory.h>
#include <Fog/Core/Std.h>
#include <Fog/Graphics/GlyphSet.h>

namespace Fog {

// ============================================================================
// [Statics]
// ============================================================================

static void copyGlyphs(Glyph* dst, Glyph* src, sysuint_t count)
{
  for (sysuint_t i = 0; i < count; i++)
    dst[i]._d = src[i]._d->ref();
}

static void freeGlyphs(Glyph* data, sysuint_t count)
{
  for (sysuint_t i = 0; i < count; i++)
    data[i]._d->deref();
}

// ============================================================================
// [Fog::GlyphSet]
// ============================================================================

Static<GlyphSet::Data> GlyphSet::sharedNull;

GlyphSet::GlyphSet()
{
  _d = sharedNull->refAlways();
}

GlyphSet::GlyphSet(const GlyphSet& other)
{
  _d = other._d->ref();
}

GlyphSet::~GlyphSet()
{
  _d->deref();
}

void GlyphSet::clear()
{
  if (_d->length == 0) return;

  if (_d->refCount.get() > 1)
  {
    AtomicBase::ptr_setXchg(&_d, sharedNull->refAlways())->deref();
  }
  else
  {
    freeGlyphs(_d->glyphs(), _d->length);
    _d->length = 0;
    _d->extents.clear();
    _d->advance = 0;
  }
}

void GlyphSet::free()
{
  AtomicBase::ptr_setXchg(&_d, sharedNull->refAlways())->deref();
}

err_t GlyphSet::reserve(sysuint_t capacity)
{
  if (_d->refCount.get() > 1)
  {
    Data* newd = Data::alloc(capacity);
    if (!newd) return Error::OutOfMemory;

    newd->length = _d->length;
    newd->extents = _d->extents;
    newd->advance = _d->advance;
    copyGlyphs(newd->glyphs(), _d->glyphs(), _d->length);

    AtomicBase::ptr_setXchg(&_d, newd)->deref();
    return Error::Ok;
  }
  else
  {
    if (_d->capacity < capacity)
    {
      Data* newd = Data::realloc(_d, capacity);
      if (!newd) return Error::OutOfMemory;
      _d = newd;
    }
    return Error::Ok;
  }
}

err_t GlyphSet::grow(sysuint_t by)
{
  sysuint_t before = _d->length;
  sysuint_t after = before + by;

  if (_d->refCount.get() > 1)
  {
    sysuint_t optimalCapacity = 
      Std::calcOptimalCapacity(sizeof(Data), sizeof(Glyph), before, after);

    Data* newd = Data::alloc(optimalCapacity);
    if (!newd) return Error::OutOfMemory;

    newd->length = _d->length;
    newd->extents = _d->extents;
    newd->advance = _d->advance;
    copyGlyphs(newd->glyphs(), _d->glyphs(), _d->length);

    AtomicBase::ptr_setXchg(&_d, newd)->deref();
    return Error::Ok;
  }
  else if (_d->capacity < after)
  {
    sysuint_t optimalCapacity = 
      Std::calcOptimalCapacity(sizeof(Data), sizeof(Glyph), before, after);

    Data* newd = Data::realloc(_d, optimalCapacity);
    if (!newd) return Error::OutOfMemory;

    _d = newd;
  }

  return Error::Ok;
}

err_t GlyphSet::begin(sysuint_t howMuch)
{
  // 128 is good for most operations
  if (howMuch == 0) howMuch = 128;

  return grow(howMuch);
}

err_t GlyphSet::end()
{
  sysuint_t i, len = _d->length;

  int advanceX = 0;
  int advanceY = 0;

  int x1 = 0;
  int y1 = 0;
  int x2 = 0;
  int y2 = 0;

  for (i = 0; i < len; i++)
  {
    Glyph::Data* gd = _d->glyphs()[i]._d;

    int gx1 = advanceX + gd->bitmapX;
    int gy1 = advanceY + gd->bitmapY;
    int gx2 = gx1 + gd->bitmap.width();
    int gy2 = gy1 + gd->bitmap.height();

    if (x1 > gx1) x1 = gx1;
    if (y1 > gy1) y1 = gy1;
    if (x2 < gx2) x2 = gx2;
    if (y2 < gy2) y2 = gy2;

    advanceX += gd->advance;
  }

  _d->extents.set(x1, y1, x2 - x1, y2 - y1);
  _d->advance = advanceX;

  return Error::Ok;
}

GlyphSet& GlyphSet::operator=(const GlyphSet& other)
{
  AtomicBase::ptr_setXchg(&_d, other._d->ref())->deref();
  return *this;
}

// ============================================================================
// [Fog::GlyphSet::Data]
// ============================================================================

GlyphSet::Data* GlyphSet::Data::ref() const
{
  if (flags & IsSharable)
    return refAlways();
  else
    return copy(const_cast<Data*>(this));
}

void GlyphSet::Data::deref()
{
  if (refCount.deref())
  {
    freeGlyphs(glyphs(), length);
    if (flags & IsDynamic) free(this);
  }
}

GlyphSet::Data* GlyphSet::Data::adopt(void* data, sysuint_t capacity)
{
  Data* d = reinterpret_cast<Data*>(data);

  d->refCount.init(1);
  d->flags = 0;
  d->capacity = capacity;
  d->length = 0;
  d->extents.clear();
  d->advance = 0;

  return d;
}

GlyphSet::Data* GlyphSet::Data::alloc(sysuint_t capacity)
{
  sysuint_t dsize = sizeof(Data) + capacity * sizeof(Glyph);

  Data* d = reinterpret_cast<Data*>(Memory::alloc(dsize));
  if (!d) return NULL;

  d->refCount.init(1);
  d->flags = IsDynamic | IsSharable;
  d->capacity = capacity;
  d->length = 0;
  d->extents.clear();
  d->advance = 0;

  return d;
}

GlyphSet::Data* GlyphSet::Data::realloc(Data* d, sysuint_t capacity)
{
  sysuint_t dsize = sizeof(Data) + capacity * sizeof(Glyph);

  Data* newd = reinterpret_cast<Data*>(Memory::realloc(d, dsize));
  if (!newd) return NULL;

  newd->capacity = capacity;
  return newd;
}

GlyphSet::Data* GlyphSet::Data::copy(Data* d)
{
  if (d->length == 0) return sharedNull->refAlways();

  Data* newd = alloc(d->capacity);
  if (!newd) return NULL;

  newd->length = d->length;
  newd->extents = d->extents;
  newd->advance = d->advance;
  copyGlyphs(newd->glyphs(), d->glyphs(), d->length);

  return newd;
}

void GlyphSet::Data::free(Data* d)
{
  Memory::free(reinterpret_cast<void*>(d));
}

} // Fog namespace

// ============================================================================
// [Library Initializers]
// ============================================================================

FOG_INIT_DECLARE err_t fog_glyphset_init(void)
{
  Fog::GlyphSet::Data* d = Fog::GlyphSet::sharedNull.instancep();

  d->refCount.init(1);
  d->flags = Fog::GlyphSet::Data::IsNull | 
             Fog::GlyphSet::Data::IsSharable;
  d->capacity = 0;
  d->length = 0;
  d->extents.set(0, 0, 0, 0);
  memset(d->data, 0, sizeof(d->data));

  return Error::Ok;
}

FOG_INIT_DECLARE void fog_glyphset_shutdown(void)
{
  Fog::GlyphSet::Data* d = Fog::GlyphSet::sharedNull.instancep();

  d->refCount.dec();
}