// [Fog/UI Library - C++ API]
//
// [Licence] 
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_UI_WIDGET_H
#define _FOG_UI_WIDGET_H

// [Dependencies]
#include <Fog/Core/Event.h>
#include <Fog/Core/Hash.h>
#include <Fog/Core/List.h>
#include <Fog/Core/Lock.h>
#include <Fog/Core/Misc.h>
#include <Fog/Core/Object.h>
#include <Fog/Core/Thread.h>
#include <Fog/Core/Time.h>
#include <Fog/Core/Vector.h>
#include <Fog/Graphics/Font.h>
#include <Fog/Graphics/Geometry.h>
#include <Fog/Graphics/Painter.h>
#include <Fog/UI/Event.h>
#include <Fog/UI/LayoutItem.h>

namespace Fog {

// ============================================================================
// [Forward Declarations]
// ============================================================================

struct Layout;
struct UISystem;
struct UIWindow;
struct Widget;
struct Window;

// ============================================================================
// [Fog::Widget]
// ============================================================================

//! @brief Base class for creating visual components that can be painted on the 
//! screen and that can catch events generated by windowing system.
//!
//! @c Fog::Widget is based on @c Fog::LayoutItem and layout item is based 
//! on @c Fog::Object. Layout item allows to very easy set or calculate layout 
//! metrics for each widget. And @c Fog::Object allows to use event mechanism
//! and object introspection that's implemented in Core library.
//!
//! @c Fog::Widget event base is a lot increased compared to @c Fog::Object. It 
//! contains these new event handlers:
//!  - <code>virtual void onStateChange(Wde_StateEvent* e)</code>
//!  - <code>virtual void onVisibilityChange(Wde_VisibilityEvent* e)</code>
//!  - <code>virtual void onConfigure(Wde_ConfigureEvent* e)</code>
//!  - <code>virtual void onFocus(Wde_FocusEvent* e)</code>
//!  - <code>virtual void onKey(Wde_KeyEvent* e)</code>
//!  - <code>virtual void onMouse(Wde_MouseEvent* e)</code>
//!  - <code>virtual void onNcPaint(Wde_PaintEvent* e)</code>
//!  - <code>virtual void onPaint(Wde_PaintEvent* e)</code>
//!  - <code>virtual void onClose(Wde_CloseEvent* e)</code>
//!
//! Widget state:
//!
//! Widget visibility is controled by @c StateEvent and can be changed 
//! via @c setState(), @c enable() and @c disable() methods. To check 
//! if widget is enabled use @c state() method.
//!
//! Widget visibility:
//!
//! Widget visibility is controled by @c VisibilityEvent and can be changed 
//! via @c setVisibility(), @c show() and @c hide() methods. To check if widget
//! is visible use @c visibility() method.
//!
//! Widget geometry:
//!
//! Widget geometry is most important thing for widgets. Widget position and size
//! is represented by @c Fog::Rect structure and can be retrieved by @c rect() method.
//! Coordinates that's retrieved represents widget position that's relative to it's
//! parent. If widget parent is @c NULL then this position is relative to the screen
//! coordinates (desktop window).
struct FOG_API Widget : public LayoutItem
{
  FOG_DECLARE_OBJECT(Widget, LayoutItem)

  // [Construction / Destruction]

  Widget(uint32_t createFlags = 0);
  virtual ~Widget();

  // [Hierarchy]

  //! @brief Returns @c true if element is root.
  FOG_INLINE bool isRoot() const
  { return _parent == NULL; }

  //! @brief Returns @c true if element has parent.
  FOG_INLINE bool hasParent() const
  { return _parent != NULL; }

  //! @brief Returns @c true if element has children.
  FOG_INLINE bool hasChildren() const
  { return !_children.isEmpty(); }

  FOG_INLINE Widget* parent() const
  { return _parent; }

  FOG_INLINE Vector<Widget*> children() const
  { return _children; }

  bool setParent(Widget* p);
  bool add(Widget* w);
  bool remove(Widget* w);

  virtual bool _add(sysuint_t index, Widget* w);
  virtual bool _remove(sysuint_t index, Widget* w);

  // [UIWindow]

  FOG_INLINE bool isUIWindow() const { return _uiWindow != NULL; }
  FOG_INLINE UIWindow* uiWindow() const { return _uiWindow; }

  UIWindow* closestUIWindow() const;

  err_t createWindow(uint32_t createFlags = 0);
  err_t destroyWindow();

  String32 windowTitle() const;
  Image windowIcon() const;
  Point windowGranularity() const;

  err_t setWindowTitle(const String32& title);
  err_t setWindowIcon(const Image& icon);
  err_t setWindowGranularity(const Point& pt);

  // [Geometry]

  //! @brief Get widget bounding rectangle.
  FOG_INLINE Rect rect() const { return _rect; }
  //! @brief Get widget position relative to parent.
  FOG_INLINE Point position() const { return _rect.point(); }
  //! @brief Get widget size.
  FOG_INLINE Size size() const { return _rect.size(); }
  //! @brief Get widget origin.
  FOG_INLINE const Point origin() const { return _origin; }

  //! @brief Get widget left position, this method is equal to @c left().
  FOG_INLINE int x1() const { return _rect.x1(); }
  //! @brief Get widget top position, this method is equal to @c top().
  FOG_INLINE int y1() const { return _rect.y1(); }
  //! @brief Get widget right position, this method is equal to @c right().
  FOG_INLINE int x2() const { return _rect.x2(); }
  //! @brief Get widget bottom position, this method is equal to @c bottom().
  FOG_INLINE int y2() const { return _rect.y2(); }

  //! @brief Get widget left position, this method is equal to @c x1().
  FOG_INLINE int left() const { return _rect.x1(); }
  //! @brief Get widget top position, this method is equal to @c y1().
  FOG_INLINE int top() const { return _rect.y1(); }
  //! @brief Get widget right position, this method is equal to @c x2().
  FOG_INLINE int right() const { return _rect.x2(); }
  //! @brief Get widget bottom position, this method is equal to @c y2().
  FOG_INLINE int bottom() const { return _rect.y2(); }

  //! @brief Get widget width.
  FOG_INLINE int width() const { return _rect.width(); }
  //! @brief Get widget height.
  FOG_INLINE int height() const { return _rect.height(); }

  void setRect(const Rect& rect);
  void setPosition(const Point& pt);
  void setSize(const Size& sz);

  //! @brief Sets widget origin to @a pt.
  void setOrigin(const Point& pt);

  FOG_INLINE void move(const Point& pt) { setPosition(pt); }
  FOG_INLINE void resize(const Size& size) { setSize(size); }

  bool worldToClient(Point* coords) const;
  bool clientToWorld(Point* coords) const;
  static bool translateCoordinates(Widget* to, Widget* from, Point* coords);

  // [Hit Testing]

  Widget* hitTest(const Point& pt) const;
  Widget* widgetAt(const Point& pt) const;

  // [Layout]

  FOG_INLINE Layout* layout() const { return _layout; }

  void setLayout(Layout* layout);
  void removeLayout();

  // [Layout Item]

  virtual void invalidateLayout() const;
  virtual void setSizeHint(const Size& sizeHint);
  virtual void setMinimumSize(const Size& sizeHint);
  virtual void setMaximumSize(const Size& sizeHint);
  virtual int heightForWidth(int width) const;

  // [State]

  //! @brief Widget state.
  enum State
  {
    //! @brief Widget is disabled.
    Disabled = 0,
    //! @brief Widget is enabled, but parent is disabled and this makes control disabled.
    DisabledByParent = 1,
    //! @brief Widget is enabled and it parent too.
    Enabled = 2
  };

  //! @brief Returns widget state, see @c State.
  FOG_INLINE uint32_t state() const { return _state; }

  FOG_INLINE bool isEnabled() const { return state() == Enabled; }

  void setEnabled(bool val = true);
 
  FOG_INLINE void enable() { setEnabled(true); }
  FOG_INLINE void disable() { setEnabled(false); }

  // [Visibility]

  //! @brief Widget visibility
  enum Visibility
  {
    //! @brief Widget is hidden.
    Hidden = 0,
    //! @brief Widget is hidden by parent that's not visible.
    HiddenByParent = 1,
    //! @brief Widget is visible.
    Visible = 2,
  };

  //! @brief Returns widget visibility, see @c Visibility_Enum.
  FOG_INLINE uint32_t visibility() const { return _visibility; }

  FOG_INLINE bool isVisible() const { return visibility() == Visible; }

  void setVisible(bool val = true);
  
  FOG_INLINE void show() { setVisible(true); }
  FOG_INLINE void hide() { setVisible(false); }

  // [Orientation]

  //! @brief Returns widget orientation, see @c OrientationEnum.
  FOG_INLINE uint32_t orientation() const
  { return _orientation; }

  //! @brief Returns @c true if widget orientation is horizontal (default).
  FOG_INLINE bool isHorizontal() const
  { return orientation() == OrientationHorizontal; }

  //! @brief Returns @c true if widget orientation is vertical.
  FOG_INLINE bool isVertical() const
  { return orientation() == OrientationVertical; }

  //! @brief Sets widget orientation.
  void setOrientation(uint32_t val);

  //! @brief Sets widget orientation to horizontal (@c OrientationHorizontal).
  FOG_INLINE void setHorizontal()
  { setOrientation(OrientationHorizontal); }

  //! @brief Sets widget orientation to vertical (@c OrientationVertical).
  FOG_INLINE void setVertical() 
  { setOrientation(OrientationVertical); }

  // [Caret]

  // bool showCaret();
  // bool showCaret(const CaretProperties& properties);
  // bool hideCaret();

  // [Tab Order]

  //! @brief Returns widget tab order.
  FOG_INLINE int tabOrder() const { return _tabOrder; }

  //! @brief Sets widget tab order.
  void setTabOrder(int tabOrder);

  // [Focus]

  enum Focusable
  {
    NextFocusable = 0,
    PreviousFocusable = 1
  };

  //! @brief Focus masks used in @c FocusPolicy
  enum FocusMask
  {
    TabFocusMask     = (1 << 0),
    ClickFocusMask   = (1 << 1),
    WheelFocusMask   = (1 << 2)
  };

  //! @brief Focus policy
  enum FocusPolicy
  {
    NoFocus = 0,
    TabFocus = TabFocusMask,
    ClickFocus = ClickFocusMask,
    StrongFocus = TabFocus | ClickFocus,
    WheelFocus = StrongFocus | WheelFocusMask
  };

  FOG_INLINE uint32_t focusPolicy() const { return _focusPolicy; }

  void setFocusPolicy(uint32_t val);

  FOG_INLINE bool hasFocus() const { return _hasFocus; }

  virtual Widget* getFocusableWidget(uint32_t focusable);

  //! @brief Takes client focus for this widget.
  void takeFocus(uint32_t reason = FocusReasonNone);

  void giveFocusNext(uint32_t reason = FocusReasonNone);
  void giveFocusPrevious(uint32_t reason = FocusReasonNone);

  Widget* _findFocus() const;

  // [Font]

  //! @brief Get widget font.
  FOG_INLINE Font font() const { return _font; }

  //! @brief Set widget font.
  void setFont(const Font& font);

  // [Update]

  enum UFlags
  {
    //! @brief Generic unspecified update flag.
    UFlagUpdate = (1 << 0),
    //! @brief Some child needs update (has generic update flag set).
    UFlagUpdateChild = (1 << 1),
    //! @brief Specifies to update everything for me and all children.
    UFlagUpdateAll = (1 << 2),

    UFlagUpdateGeometry = (1 << 3),

    //! @brief Widget needs repaint.
    UFlagRepaintWidget = (1 << 4),
    //! @brief Caret needs repaint
    UFlagRepaintCaret = (1 << 5),

    //! @brief This flag is set if the widget will call @c Widget::paintParent().
    UFlagPaintParentRequired = (1 << 30),
    //! @Brief Internal flag used together with UFlagPaintParentRequired. Do not use.
    UFlagPaintParentDone = (1 << 31)
  };

  void update(uint32_t updateFlags);

  // [Repaint]

  enum RepaintFlags
  {
    RepaintWidget    = UFlagRepaintWidget,
    RepaintCaret     = UFlagRepaintCaret,
    RepaintAll       = RepaintWidget | RepaintCaret
  };

  void repaint(uint32_t repaintFlags);

  // [Events]

  //! @brief Child event handler.
  virtual void onChildAdd(ChildEvent* e);
  //! @brief Child event handler.
  virtual void onChildRemove(ChildEvent* e);
  //! @brief State event handler.
  virtual void onStateChange(StateEvent* e);
  //! @brief Visibility event handler.
  virtual void onVisibilityChange(VisibilityEvent* e);
  //! @brief Configure event handler.
  virtual void onConfigure(ConfigureEvent* e);
  //! @brief Focus event handler.
  virtual void onFocus(FocusEvent* e);
  //! @brief Keyboard event handler.
  virtual void onKey(KeyEvent* e);
  //! @brief Mouse event handler.
  virtual void onMouse(MouseEvent* e);
  //! @brief Selection event handler.
  virtual void onSelection(SelectionEvent* e);

  //! @brief Client paint handler.
  virtual void onPaint(PaintEvent* e);

  //! @brief Close event handler.
  virtual void onClose(CloseEvent* e);

  // [Event Map]

  fog_event_begin()
    fog_event(EvChildAdd         , onChildAdd        , ChildEvent     , Override)
    fog_event(EvChildRemove      , onChildRemove     , ChildEvent     , Override)
    fog_event(EvEnable           , onStateChange     , StateEvent     , Override)
    fog_event(EvDisable          , onStateChange     , StateEvent     , Override)
    fog_event(EvDisableByParent  , onStateChange     , StateEvent     , Override)
    fog_event(EvShow             , onVisibilityChange, VisibilityEvent, Override)
    fog_event(EvHide             , onVisibilityChange, VisibilityEvent, Override)
    fog_event(EvHideByParent     , onVisibilityChange, VisibilityEvent, Override)
    fog_event(EvConfigure        , onConfigure       , ConfigureEvent , Override)
    fog_event(EvFocusIn          , onFocus           , FocusEvent     , Override)
    fog_event(EvFocusOut         , onFocus           , FocusEvent     , Override)
    fog_event(EvKeyPress         , onKey             , KeyEvent       , Override)
    fog_event(EvKeyRelease       , onKey             , KeyEvent       , Override)
    fog_event(EvMouseIn          , onMouse           , MouseEvent     , Override)
    fog_event(EvMouseMove        , onMouse           , MouseEvent     , Override)
    fog_event(EvOutsideMove      , onMouse           , MouseEvent     , Override)
    fog_event(EvMouseOut         , onMouse           , MouseEvent     , Override)
    fog_event(EvButtonPress      , onMouse           , MouseEvent     , Override)
    fog_event(EvRepButtonPress   , onMouse           , MouseEvent     , Override)
    fog_event(EvButtonRelease    , onMouse           , MouseEvent     , Override)
    fog_event(EvClick            , onMouse           , MouseEvent     , Override)
    fog_event(EvDoubleClick      , onMouse           , MouseEvent     , Override)
    fog_event(EvWheel            , onMouse           , MouseEvent     , Override)
    fog_event(EvClearSelection   , onSelection       , SelectionEvent , Override)
    fog_event(EvSelectionRequired, onSelection       , SelectionEvent , Override)
    fog_event(EvPaint            , onPaint           , PaintEvent     , Override)
    fog_event(EvClose            , onClose           , CloseEvent     , Override)
  fog_event_end()

  // [Imaging]

protected:
  Widget* _parent;
  Vector<Widget*> _children;

  //! @brief Native window data.
  UIWindow* _uiWindow;

  //! @brief Widget bounds.
  Rect _rect;
  //! @brief Widget origin.
  Point _origin;

  //! @brief Layout.
  Layout* _layout;

  //! @brief Link to child that was last focus.
  Widget* _lastFocus;
  Widget* _focusLink;

  uint32_t _uflags;

  //! @brief Widget state.
  uint32_t _state : 2;
  //! @brief Widget visibility.
  uint32_t _visibility : 2;
  //! @brief Widget focus policy
  uint32_t _focusPolicy : 4;
  //! @brief Focus.
  uint32_t _hasFocus : 1;
  //! @brief Widget orientation
  uint32_t _orientation : 1;
  //! @brief Reserved.
  uint32_t _reserved : 22;

  //! @brief Tab order.
  int _tabOrder;

  //! @brief Font (used to draw text in widget).
  Font _font;

private:
  friend struct Application;
  friend struct Window;
  friend struct UIWindow;
  friend struct UIWindowDefault;
  friend struct UISystem;
  friend struct UISystemDefault;
};

} // Fog namespace

// ============================================================================
// [Private Widget Iterator used in GUI library]
//
// These macros are used internally, so don't use them in production code,
// please
// ============================================================================

#define FOG_WIDGET_TREE_ITERATOR(__name__, __basewidget__, __conditional__, __before_traverse__, __after_traverse__) \
        FOG_WIDGET_TREE_ITERATOR_EX(__name__, __basewidget__, __conditional__, __before_traverse__, __after_traverse__, {}, {})

#define FOG_WIDGET_TREE_ITERATOR_EX(__name__, __basewidget__, __conditional__, __before_traverse__, __after_traverse__, __push__, __pop__) \
  if ((__basewidget__)->_children.length()) \
  { \
    Fog::LocalStack<512> stack; \
    Fog::Widget** childCur = (Fog::Widget** )( __basewidget__->_children.cData() ); \
    Fog::Widget** childEnd = childCur + ( __basewidget__->_children.length() ); \
    Fog::Widget* child; \
    \
    for (;;) \
    { \
      child = *childCur; \
      \
      __before_traverse__ \
      \
      if (child->_children.length() && (__conditional__)) \
      { \
        \
        __push__ \
        \
        stack.push(childCur); \
        stack.push(childEnd); \
        \
        childCur = (Fog::Widget** )child->_children.cData(); \
        childEnd = childCur + child->_children.length(); \
        \
        continue; \
      } \
      \
__name__##_after: \
      \
      __after_traverse__ \
      \
__name__##_next: \
      \
      childCur++; \
      \
      if (FOG_UNLIKELY(childCur == childEnd)) \
      { \
        if (FOG_UNLIKELY(stack.isEmpty())) break; \
        \
        stack.pop(childEnd); \
        stack.pop(childCur); \
        \
        __pop__ \
        \
        goto __name__##_after; \
      } \
    } \
  }

#define FOG_WIDGET_TREE_ITERATOR_NEXT(__name__) goto __name__##_next

// [Guard]
#endif // _FOG_UI_WIDGET_H
