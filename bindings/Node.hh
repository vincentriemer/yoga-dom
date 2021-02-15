/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <memory>

//#include <nbind/api.h>
//#include <nbind/BindDefiner.h>
#include "../yoga/Yoga.h"

#include "./Layout.hh"
#include "./Size.hh"
#include "./Value.hh"
#include "./Config.hh"

#define NODE_STYLE_PROPERTY(TYPE, NAME, PARAM_NAME) \
  void set##NAME(TYPE PARAM_NAME);                  \
  TYPE get##NAME(void) const;

#define NODE_STYLE_EDGE_PROPERTY(TYPE, NAME, PARAM_NAME)               \
  NODE_STYLE_PROPERTY(TYPE, NAME##Left, PARAM_NAME##Left);             \
  NODE_STYLE_PROPERTY(TYPE, NAME##Right, PARAM_NAME##Right);           \
  NODE_STYLE_PROPERTY(TYPE, NAME##Top, PARAM_NAME##Top);               \
  NODE_STYLE_PROPERTY(TYPE, NAME##Bottom, PARAM_NAME##Bottom);         \
  NODE_STYLE_PROPERTY(TYPE, NAME##Start, PARAM_NAME##Start);           \
  NODE_STYLE_PROPERTY(TYPE, NAME##End, PARAM_NAME##End);               \
  NODE_STYLE_PROPERTY(TYPE, NAME##Horizontal, PARAM_NAME##Horizontal); \
  NODE_STYLE_PROPERTY(TYPE, NAME##Vertical, PARAM_NAME##Vertical);     \
  NODE_STYLE_PROPERTY(TYPE, NAME##All, PARAM_NAME##All);

#define NODE_LAYOUT_PROPERTY(TYPE, NAME) \
  TYPE getComputed##NAME(void) const;

#define NODE_LAYOUT_EDGE_PROPERTY(TYPE, NAME, PARAM_NAME) \
  NODE_LAYOUT_PROPERTY(TYPE, NAME##Left);                 \
  NODE_LAYOUT_PROPERTY(TYPE, NAME##Right);                \
  NODE_LAYOUT_PROPERTY(TYPE, NAME##Top);                  \
  NODE_LAYOUT_PROPERTY(TYPE, NAME##Bottom);

typedef Size (*JSMeasureFunction)(float width,
                                  YGMeasureMode widthMode,
                                  float height,
                                  YGMeasureMode heightMode);

struct MeasureCallback
{
  virtual Size measure(float width,
                         YGMeasureMode widthMode,
                         float height,
                         YGMeasureMode heightMode) = 0;
};

struct MeasureCallbackWrapper : public emscripten::wrapper<MeasureCallback>
{
  EMSCRIPTEN_WRAPPER(MeasureCallbackWrapper);
  Size measure(float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
  {
    return call<Size>("measure", width, widthMode, height, heightMode);
  }
};
class Node {

public:
  static Node* createDefault(void);
  static Node* createWithConfig(Config* config);

  static void destroy(Node* node);

public:
  static Node* fromYGNode(YGNodeRef nodeRef);

private:
  Node(Config* config);

public:
  ~Node(void);

public: // Prevent accidental copy
  Node(Node const&) = delete;

  Node const& operator=(Node const&) = delete;

public:
  void reset(void);

public: // Style setters
  void copyStyle(Node const& other);

  // void setPositionType(int positionType);
  // void setPosition(int edge, double position);
  // void setPositionPercent(int edge, double position);

  // void setAlignContent(int alignContent);
  // void setAlignItems(int alignItems);
  // void setAlignSelf(int alignSelf);
  // void setFlexDirection(int flexDirection);
  // void setFlexWrap(int flexWrap);
  // void setJustifyContent(int justifyContent);

  // void setMargin(int edge, double margin);
  // void setMarginPercent(int edge, double margin);
  // void setMarginAuto(int edge);

  // void setOverflow(int overflow);
  // void setDisplay(int display);

  // void setFlex(double flex);
  // void setFlexBasis(double flexBasis);
  // void setFlexBasisPercent(double flexBasis);
  // void setFlexBasisAuto();
  // void setFlexGrow(double flexGrow);
  // void setFlexShrink(double flexShrink);

  // void setWidth(double width);
  // void setWidthPercent(double width);
  // void setWidthAuto();
  // void setHeight(double height);
  // void setHeightPercent(double height);
  // void setHeightAuto();

  // void setMinWidth(double minWidth);
  // void setMinWidthPercent(double minWidth);
  // void setMinHeight(double minHeight);
  // void setMinHeightPercent(double minHeight);

  // void setMaxWidth(double maxWidth);
  // void setMaxWidthPercent(double maxWidth);
  // void setMaxHeight(double maxHeight);
  // void setMaxHeightPercent(double maxHeight);

  // void setAspectRatio(double aspectRatio);

  // void setBorder(int edge, double border);

  // void setPadding(int edge, double padding);
  // void setPaddingPercent(int edge, double padding);
  
public: // Style properties
  NODE_STYLE_PROPERTY(YGDirection, Direction, direction);
  NODE_STYLE_PROPERTY(YGFlexDirection, FlexDirection, flexDirection);
  NODE_STYLE_PROPERTY(YGJustify, JustifyContent, justifyContent);
  NODE_STYLE_PROPERTY(YGAlign, AlignContent, alignContent);
  NODE_STYLE_PROPERTY(YGAlign, AlignItems, alignItems);
  NODE_STYLE_PROPERTY(YGAlign, AlignSelf, alignSelf);
  NODE_STYLE_PROPERTY(YGPositionType, PositionType, positionType);
  NODE_STYLE_PROPERTY(YGWrap, FlexWrap, flexWrap);
  NODE_STYLE_PROPERTY(YGOverflow, Overflow, overflow);
  NODE_STYLE_PROPERTY(YGDisplay, Display, display);

  NODE_STYLE_PROPERTY(float, Flex, flex);
  NODE_STYLE_PROPERTY(float, FlexGrow, flexGrow);
  NODE_STYLE_PROPERTY(float, FlexShrink, flexShrink);
  NODE_STYLE_PROPERTY(YGValue, FlexBasis, flexBasis);

  NODE_STYLE_EDGE_PROPERTY(YGValue, Position, position);
  NODE_STYLE_EDGE_PROPERTY(YGValue, Margin, margin);
  NODE_STYLE_EDGE_PROPERTY(YGValue, Padding, padding);
  NODE_STYLE_EDGE_PROPERTY(float, Border, border);

  NODE_STYLE_PROPERTY(YGValue, Width, width);
  NODE_STYLE_PROPERTY(YGValue, Height, height);
  NODE_STYLE_PROPERTY(YGValue, MinWidth, minWidth);
  NODE_STYLE_PROPERTY(YGValue, MinHeight, minHeight);
  NODE_STYLE_PROPERTY(YGValue, MaxWidth, maxWidth);
  NODE_STYLE_PROPERTY(YGValue, MaxHeight, maxHeight);

  NODE_STYLE_PROPERTY(float, AspectRatio, aspectRatio);

// public: // Style getters
//   int getPositionType(void) const;
//   Value getPosition(int edge) const;

//   int getAlignContent(void) const;
//   int getAlignItems(void) const;
//   int getAlignSelf(void) const;
//   int getFlexDirection(void) const;
//   int getFlexWrap(void) const;
//   int getJustifyContent(void) const;

//   Value getMargin(int edge) const;

//   int getOverflow(void) const;
//   int getDisplay(void) const;

//   Value getFlexBasis(void) const;
//   double getFlexGrow(void) const;
//   double getFlexShrink(void) const;

//   Value getWidth(void) const;
//   Value getHeight(void) const;

//   Value getMinWidth(void) const;
//   Value getMinHeight(void) const;

//   Value getMaxWidth(void) const;
//   Value getMaxHeight(void) const;

//   double getAspectRatio(void) const;

//   double getBorder(int edge) const;

//   Value getPadding(int edge) const;

public: // Tree hierarchy mutators
  void insertChild(Node* child, unsigned index);
  void removeChild(Node* child);

public: // Tree hierarchy inspectors
  unsigned getChildCount(void) const;

  // The following functions cannot be const because they could discard const
  // qualifiers (ex: constNode->getChild(0)->getParent() wouldn't be const)

  Node* getParent(void);
  Node* getChild(unsigned index);

public: // Measure func mutators
  void setMeasureFunc(MeasureCallback *measureFunc);
  void unsetMeasureFunc(void);

public: // Measure func inspectors
  Size callMeasureFunc(
      float width,
      YGMeasureMode widthMode,
      float height,
      YGMeasureMode heightMode) const;

//Start: TODO: commented as nbind is throwing error
// public: // Dirtied func mutators
//   void setDirtiedFunc(nbind::cbFunction& dirtiedFunc);
//   void unsetDirtiedFunc(void);

// public: // Dirtied func inspectors
//   void callDirtiedFunc(void) const;
//End: TODO: commented as nbind is throwing error

public: // Dirtiness accessors
  void markDirty(void);
  bool isDirty(void) const;

public: // Layout mutators
  void calculateLayout(float width, float height, YGDirection direction);

public: // Layout inspectors
  // double getComputedLeft(void) const;
  // double getComputedRight(void) const;

  // double getComputedTop(void) const;
  // double getComputedBottom(void) const;

  // double getComputedWidth(void) const;
  // double getComputedHeight(void) const;

  Layout getComputedLayout(void) const;

  // double getComputedMargin(int edge) const;
  // double getComputedBorder(int edge) const;
  // double getComputedPadding(int edge) const;
public: // Layout getters

  NODE_LAYOUT_PROPERTY(float, Left);
  NODE_LAYOUT_PROPERTY(float, Top);
  NODE_LAYOUT_PROPERTY(float, Width);
  NODE_LAYOUT_PROPERTY(float, Height);

  NODE_LAYOUT_EDGE_PROPERTY(float, Border, border);
  NODE_LAYOUT_EDGE_PROPERTY(float, Padding, padding);

public: // HasNewLayout manipulators
  bool hasNewLayout(void) const;
  void setHasNewLayout(bool hasNewLayout);
public:
  void setIsReferenceBaseline(bool isReferenceBaseline);
  bool isReferenceBaseline();

  YGNodeRef m_node;

  std::unique_ptr<MeasureCallback> m_measureFunc;
  // std::unique_ptr<nbind::cbFunction> m_dirtiedFunc;
};
