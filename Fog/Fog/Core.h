// [Fog/Core Library - C++ API]
//
// [Licence] 
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_CORE_H
#define _FOG_CORE_H

#if defined(_MSVC)
#pragma once
#endif // _MSVC

// [There is only documentation]
#if defined(_DOXYGEN)

/*!
  @mainpage Fog Library Documentation

  @section Main_Page_Intro Introduction

  Core library is high performance library written mainly in C++ 
  language that abstracts operating system dependencies and enables 
  multi platform development through single source compatibility. This 
  library was created as an abstraction layer between Windows and 
  Unix like operating systems. Core library is set of C functions wrapped 
  around C++ classes.
*/

/*!
  @defgroup Fog_Core Fog/Core library API.

  Set of Fog/Core library classes and their members.
*/

/*!
  @defgroup Fog_Core_Detail Fog/Core library implementation details.
  @ingroup Fog_Core
*/

/*!
  @defgroup Fog_Core_CAPI Fog/Core library C API.
  @ingroup Fog_Core

  Set of Fog/Core functions compatible with C calling convention.
*/

/*!
  @defgroup Fog_Core_Macros Fog/Core macros.
  @ingroup Fog_Core

  Very low level macros, but needed for compiling Core library and
  applications.
*/

// [Doxygen::]
/*! @brief Namespace for shared documentation between classes. */
namespace Doxygen {

/*! @brief Not real structure, used only for doxygen. */
struct Implicit
{
  // [Implicit Sharing]

  //! @brief Returns reference count of object data.
  sysuint_t refCount() const;

  //! @brief Returns @c true if object is not sharing data with another.
  //!
  //! This method will return @c true if reference count is equal to 1.
  bool isDetached() const;

  //! @brief Detaches object data if they are shared between another object.
  //!
  //! If data of this object are read only or reference count is larger
  //! than 1, the data will be detached.
  //!
  //! Detaching means creating exact copy of data.
  void detach();

  //! @brief Private detach function.
  void _detach();

  //! @brief Frees all memory allocated by this object.
  //!
  //! If object data is allocated by dynamic memory allocation,
  //! they will be freed and object will share null data.
  //!
  //! If object data is allocated statically by template or other
  //! construction, it will be set to empty, but still ready to use.
  void free();

  // [Flags]

  //! @brief Returns object flags.
  //!
  //! Object flags contains boolean informations about object data. There
  //! are always informations about object allocation, shareability and
  //! null.
  uint32_t flags() const;

  //! @brief Returns @c true if object data are allocated by dynamic memory
  //! allocation.
  //!
  //! Dynamic memory allocation is used for every dynamic container object,
  //! but there are usually choices to use static (stack) version for temporary
  //! object data.
  bool isDynamic() const;

  //! @brief Returns @c true if object data are shareable.
  //!
  //! Sharable data are usually allocated on the heap and in rare cases
  //! on the stack. Implicit sharing system allows to share equal data
  //! between more objects, so use @c isSharable() in cases that you
  //! want to know if the object data can be shared (And will be shared
  //! if assign operation will be called).
  bool isSharable() const;

  //! @brief Returns @c true if this is null object (no data).
  //!
  //! In real life there is no difference between null and empty
  //! objects. Null is just convenience for library, because null
  //! objects are allocated by library and shared between all
  //! null instances (empty instances).
  bool isNull() const;

  //! @brief Returns @c true if object data are alive.
  //!
  //! Alive data means, that assign operations will ever free these data
  //! and deep copy will be used instead.
  //!
  //! This flag is set only by template versions of original objects.
  //! For example @c Fog::TemporaryString<> templates sets this flag to 
  //! true, but @c Fog::String family classes never sets this flag to 
  //! @c true.
  bool isStrong() const;
};

// [Doxygen::]
}

#endif // _DOXYGEN

// [Fog/Core Include Files]
#include <Fog/Build/Build.h>

#include <Fog/Core/Algorithms.h>
#include <Fog/Core/Application.h>
#include <Fog/Core/Assert.h>
#include <Fog/Core/Atomic.h>
#include <Fog/Core/BitArray.h>
#include <Fog/Core/Char.h>
#include <Fog/Core/Class.h>
#include <Fog/Core/Constants.h>
#include <Fog/Core/CpuInfo.h>
#include <Fog/Core/Delegate.h>
#include <Fog/Core/DirIterator.h>
#include <Fog/Core/Event.h>
#include <Fog/Core/EventLoop.h>
#include <Fog/Core/FileSystem.h>
#include <Fog/Core/FileUtil.h>
#include <Fog/Core/Hash.h>
#include <Fog/Core/Lazy.h>
#include <Fog/Core/Library.h>
#include <Fog/Core/List.h>
#include <Fog/Core/Locale.h>
#include <Fog/Core/MapFile.h>
#include <Fog/Core/Math.h>
#include <Fog/Core/Memory.h>
#include <Fog/Core/Misc.h>
#include <Fog/Core/OS.h>
#include <Fog/Core/Object.h>
#include <Fog/Core/ObserverList.h>
#include <Fog/Core/Properties.h>
#include <Fog/Core/RefCounted.h>
#include <Fog/Core/RefData.h>
#include <Fog/Core/ScopedHandle.h>
#include <Fog/Core/ScopedPtr.h>
#include <Fog/Core/Sequence.h>
#include <Fog/Core/SequenceInfo.h>
#include <Fog/Core/Static.h>
#include <Fog/Core/Stream.h>
#include <Fog/Core/String.h>
#include <Fog/Core/StringCache.h>
#include <Fog/Core/StringUtil.h>
#include <Fog/Core/Strings.h>
#include <Fog/Core/TextCodec.h>
#include <Fog/Core/Thread.h>
#include <Fog/Core/ThreadCondition.h>
#include <Fog/Core/ThreadEvent.h>
#include <Fog/Core/ThreadLocalStorage.h>
#include <Fog/Core/ThreadPool.h>
#include <Fog/Core/Time.h>
#include <Fog/Core/Timer.h>
#include <Fog/Core/TypeInfo.h>
#include <Fog/Core/UserInfo.h>
#include <Fog/Core/Value.h>
#include <Fog/Core/Vector.h>

// [Guard]
#endif // _FOG_CORE_H