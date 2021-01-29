/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <algorithm>
#include <emscripten/bind.h>

#include "../yoga/Yoga.h"

#include "./Node.hh"
#include "./Layout.hh"
#include "./Size.hh"
#include "./Config.hh"

static YGSize globalMeasureFunc(
    YGNodeRef nodeRef,
    float width,
    YGMeasureMode widthMode,
    float height,
    YGMeasureMode heightMode) {
  Node const& node = *reinterpret_cast<Node const*>(YGNodeGetContext(nodeRef));

  Size size = node.callMeasureFunc(width, widthMode, height, heightMode);
  YGSize ygSize = {static_cast<float>(size.width),
                   static_cast<float>(size.height)};

  return ygSize;
}

// static void globalDirtiedFunc(YGNodeRef nodeRef) {
//   Node const& node = *reinterpret_cast<Node const*>(YGNodeGetContext(nodeRef));

//   node.callDirtiedFunc();
// }

/* static */ Node* Node::createDefault(void) {
  return new Node(nullptr);
}

/* static */ Node* Node::createWithConfig(Config* config) {
  return new Node(config);
}

/* static */ void Node::destroy(Node* node) {
  delete node;
}

/* static */ Node* Node::fromYGNode(YGNodeRef nodeRef) {
  return reinterpret_cast<Node*>(YGNodeGetContext(nodeRef));
}

Node::Node(Config* config)
    : m_node(
          config != nullptr ? YGNodeNewWithConfig(config->m_config)
                            : YGNodeNew()),
      m_measureFunc(nullptr)
      //,m_dirtiedFunc(nullptr) 
{
  YGNodeSetContext(m_node, reinterpret_cast<void*>(this));
}

Node::~Node(void) {
  YGNodeFree(m_node);
}

void Node::reset(void) {
  m_measureFunc.reset(nullptr);
  // m_dirtiedFunc.reset(nullptr);

  YGNodeReset(m_node);
}

void Node::copyStyle(Node const& other) {
  YGNodeCopyStyle(m_node, other.m_node);
}

// void Node::setPositionType(int positionType) {
//   YGNodeStyleSetPositionType(m_node, static_cast<YGPositionType>(positionType));
// }

// void Node::setPosition(int edge, double position) {
//   YGNodeStyleSetPosition(m_node, static_cast<YGEdge>(edge), position);
// }

// void Node::setPositionPercent(int edge, double position) {
//   YGNodeStyleSetPositionPercent(m_node, static_cast<YGEdge>(edge), position);
// }

// void Node::setAlignContent(int alignContent) {
//   YGNodeStyleSetAlignContent(m_node, static_cast<YGAlign>(alignContent));
// }

// void Node::setAlignItems(int alignItems) {
//   YGNodeStyleSetAlignItems(m_node, static_cast<YGAlign>(alignItems));
// }

// void Node::setAlignSelf(int alignSelf) {
//   YGNodeStyleSetAlignSelf(m_node, static_cast<YGAlign>(alignSelf));
// }

// void Node::setFlexDirection(int flexDirection) {
//   YGNodeStyleSetFlexDirection(
//       m_node, static_cast<YGFlexDirection>(flexDirection));
// }

// void Node::setFlexWrap(int flexWrap) {
//   YGNodeStyleSetFlexWrap(m_node, static_cast<YGWrap>(flexWrap));
// }

// void Node::setJustifyContent(int justifyContent) {
//   YGNodeStyleSetJustifyContent(m_node, static_cast<YGJustify>(justifyContent));
// }

// void Node::setMargin(int edge, double margin) {
//   YGNodeStyleSetMargin(m_node, static_cast<YGEdge>(edge), margin);
// }

// void Node::setMarginPercent(int edge, double margin) {
//   YGNodeStyleSetMarginPercent(m_node, static_cast<YGEdge>(edge), margin);
// }

// void Node::setMarginAuto(int edge) {
//   YGNodeStyleSetMarginAuto(m_node, static_cast<YGEdge>(edge));
// }

// void Node::setOverflow(int overflow) {
//   YGNodeStyleSetOverflow(m_node, static_cast<YGOverflow>(overflow));
// }

// void Node::setDisplay(int display) {
//   YGNodeStyleSetDisplay(m_node, static_cast<YGDisplay>(display));
// }

// void Node::setFlex(double flex) {
//   YGNodeStyleSetFlex(m_node, flex);
// }

// void Node::setFlexBasis(double flexBasis) {
//   YGNodeStyleSetFlexBasis(m_node, flexBasis);
// }

// void Node::setFlexBasisPercent(double flexBasis) {
//   YGNodeStyleSetFlexBasisPercent(m_node, flexBasis);
// }

// void Node::setFlexGrow(double flexGrow) {
//   YGNodeStyleSetFlexGrow(m_node, flexGrow);
// }

// void Node::setFlexShrink(double flexShrink) {
//   YGNodeStyleSetFlexShrink(m_node, flexShrink);
// }

// void Node::setWidth(double width) {
//   YGNodeStyleSetWidth(m_node, width);
// }

// void Node::setWidthPercent(double width) {
//   YGNodeStyleSetWidthPercent(m_node, width);
// }

// void Node::setWidthAuto() {
//   YGNodeStyleSetWidthAuto(m_node);
// }

// void Node::setHeight(double height) {
//   YGNodeStyleSetHeight(m_node, height);
// }

// void Node::setHeightPercent(double height) {
//   YGNodeStyleSetHeightPercent(m_node, height);
// }

// void Node::setHeightAuto() {
//   YGNodeStyleSetHeightAuto(m_node);
// }

// void Node::setMinWidth(double minWidth) {
//   YGNodeStyleSetMinWidth(m_node, minWidth);
// }

// void Node::setMinWidthPercent(double minWidth) {
//   YGNodeStyleSetMinWidthPercent(m_node, minWidth);
// }

// void Node::setMinHeight(double minHeight) {
//   YGNodeStyleSetMinHeight(m_node, minHeight);
// }

// void Node::setMinHeightPercent(double minHeight) {
//   YGNodeStyleSetMinHeightPercent(m_node, minHeight);
// }

// void Node::setMaxWidth(double maxWidth) {
//   YGNodeStyleSetMaxWidth(m_node, maxWidth);
// }

// void Node::setMaxWidthPercent(double maxWidth) {
//   YGNodeStyleSetMaxWidthPercent(m_node, maxWidth);
// }

// void Node::setMaxHeight(double maxHeight) {
//   YGNodeStyleSetMaxHeight(m_node, maxHeight);
// }

// void Node::setMaxHeightPercent(double maxHeight) {
//   YGNodeStyleSetMaxHeightPercent(m_node, maxHeight);
// }

// void Node::setAspectRatio(double aspectRatio) {
//   YGNodeStyleSetAspectRatio(m_node, aspectRatio);
// }

// void Node::setBorder(int edge, double border) {
//   YGNodeStyleSetBorder(m_node, static_cast<YGEdge>(edge), border);
// }

// void Node::setPadding(int edge, double padding) {
//   YGNodeStyleSetPadding(m_node, static_cast<YGEdge>(edge), padding);
// }

// void Node::setPaddingPercent(int edge, double padding) {
//   YGNodeStyleSetPaddingPercent(m_node, static_cast<YGEdge>(edge), padding);
// }

// void Node::setIsReferenceBaseline(bool isReferenceBaseline) {
//   YGNodeSetIsReferenceBaseline(m_node, isReferenceBaseline);
// }

// int Node::getPositionType(void) const {
//   return YGNodeStyleGetPositionType(m_node);
// }

// Value Node::getPosition(int edge) const {
//   return Value::fromYGValue(
//       YGNodeStyleGetPosition(m_node, static_cast<YGEdge>(edge)));
// }

// int Node::getAlignContent(void) const {
//   return YGNodeStyleGetAlignContent(m_node);
// }

// int Node::getAlignItems(void) const {
//   return YGNodeStyleGetAlignItems(m_node);
// }

// int Node::getAlignSelf(void) const {
//   return YGNodeStyleGetAlignSelf(m_node);
// }

// int Node::getFlexDirection(void) const {
//   return YGNodeStyleGetFlexDirection(m_node);
// }

// int Node::getFlexWrap(void) const {
//   return YGNodeStyleGetFlexWrap(m_node);
// }

// int Node::getJustifyContent(void) const {
//   return YGNodeStyleGetJustifyContent(m_node);
// }

// Value Node::getMargin(int edge) const {
//   return Value::fromYGValue(
//       YGNodeStyleGetMargin(m_node, static_cast<YGEdge>(edge)));
// }

// int Node::getOverflow(void) const {
//   return YGNodeStyleGetOverflow(m_node);
// }

// int Node::getDisplay(void) const {
//   return YGNodeStyleGetDisplay(m_node);
// }

// Value Node::getFlexBasis(void) const {
//   return Value::fromYGValue(YGNodeStyleGetFlexBasis(m_node));
// }

// double Node::getFlexGrow(void) const {
//   return YGNodeStyleGetFlexGrow(m_node);
// }

// double Node::getFlexShrink(void) const {
//   return YGNodeStyleGetFlexShrink(m_node);
// }

// Value Node::getWidth(void) const {
//   return Value::fromYGValue(YGNodeStyleGetWidth(m_node));
// }

// Value Node::getHeight(void) const {
//   return Value::fromYGValue(YGNodeStyleGetHeight(m_node));
// }

// Value Node::getMinWidth(void) const {
//   return Value::fromYGValue(YGNodeStyleGetMinWidth(m_node));
// }

// Value Node::getMinHeight(void) const {
//   return Value::fromYGValue(YGNodeStyleGetMinHeight(m_node));
// }

// Value Node::getMaxWidth(void) const {
//   return Value::fromYGValue(YGNodeStyleGetMaxWidth(m_node));
// }

// Value Node::getMaxHeight(void) const {
//   return Value::fromYGValue(YGNodeStyleGetMaxHeight(m_node));
// }

// double Node::getAspectRatio(void) const {
//   return YGNodeStyleGetAspectRatio(m_node);
// }

// double Node::getBorder(int edge) const {
//   return YGNodeStyleGetBorder(m_node, static_cast<YGEdge>(edge));
// }

// Value Node::getPadding(int edge) const {
//   return Value::fromYGValue(
//       YGNodeStyleGetPadding(m_node, static_cast<YGEdge>(edge)));
// }

bool Node::isReferenceBaseline() {
  return YGNodeIsReferenceBaseline(m_node);
}

void Node::insertChild(Node* child, unsigned index) {
  YGNodeInsertChild(m_node, child->m_node, index);
}

void Node::removeChild(Node* child) {
  YGNodeRemoveChild(m_node, child->m_node);
}

unsigned Node::getChildCount(void) const {
  return YGNodeGetChildCount(m_node);
}

Node* Node::getParent(void) {
  auto nodePtr = YGNodeGetParent(m_node);

  if (nodePtr == nullptr)
    return nullptr;

  return Node::fromYGNode(nodePtr);
}

Node* Node::getChild(unsigned index) {
  auto nodePtr = YGNodeGetChild(m_node, index);

  if (nodePtr == nullptr)
    return nullptr;

  return Node::fromYGNode(nodePtr);
}

void Node::setMeasureFunc(MeasureCallback *measureFunc) {
  m_measureFunc.reset(measureFunc);

  YGNodeSetMeasureFunc(m_node, &globalMeasureFunc);
}

void Node::unsetMeasureFunc(void) {
  m_measureFunc.reset(nullptr);

  YGNodeSetMeasureFunc(m_node, nullptr);
}

Size Node::callMeasureFunc(
    float width,
    YGMeasureMode widthMode,
    float height,
    YGMeasureMode heightMode) const {
  return m_measureFunc->measure(width, widthMode, height, heightMode);
}

// void Node::setDirtiedFunc(nbind::cbFunction& dirtiedFunc) {
//   m_dirtiedFunc.reset(new nbind::cbFunction(dirtiedFunc));

//   YGNodeSetDirtiedFunc(m_node, &globalDirtiedFunc);
// }

// void Node::unsetDirtiedFunc(void) {
//   m_dirtiedFunc.reset(nullptr);

//   YGNodeSetDirtiedFunc(m_node, nullptr);
// }

// void Node::callDirtiedFunc(void) const {
//   m_dirtiedFunc->call<void>();
// }

void Node::markDirty(void) {
  YGNodeMarkDirty(m_node);
}

bool Node::isDirty(void) const {
  return YGNodeIsDirty(m_node);
}

void Node::calculateLayout(float width, float height, YGDirection direction) {
  YGNodeCalculateLayout(
      m_node, width, height, direction);
      // m_node, width, height, static_cast<YGDirection>(direction));
}

// double Node::getComputedLeft(void) const {
//   return YGNodeLayoutGetLeft(m_node);
// }

// double Node::getComputedRight(void) const {
//   return YGNodeLayoutGetRight(m_node);
// }

// double Node::getComputedTop(void) const {
//   return YGNodeLayoutGetTop(m_node);
// }

// double Node::getComputedBottom(void) const {
//   return YGNodeLayoutGetBottom(m_node);
// }

// double Node::getComputedWidth(void) const {
//   return YGNodeLayoutGetWidth(m_node);
// }

// double Node::getComputedHeight(void) const {
//   return YGNodeLayoutGetHeight(m_node);
// }

Layout Node::getComputedLayout(void) const {
  Layout layout;

  layout.left = YGNodeLayoutGetLeft(m_node);
  // layout.right = YGNodeLayoutGetRight(m_node);

  layout.top = YGNodeLayoutGetTop(m_node);
  // layout.bottom = YGNodeLayoutGetBottom(m_node);

  layout.width = YGNodeLayoutGetWidth(m_node);
  layout.height = YGNodeLayoutGetHeight(m_node);

  return layout;
}

bool Node::hasNewLayout(void) const
{
  return YGNodeGetHasNewLayout(m_node);
}

void Node::setHasNewLayout(bool hasNewLayout)
{
  YGNodeSetHasNewLayout(m_node, hasNewLayout);
}

// double Node::getComputedMargin(int edge) const {
//   return YGNodeLayoutGetMargin(m_node, static_cast<YGEdge>(edge));
// }

// double Node::getComputedBorder(int edge) const {
//   return YGNodeLayoutGetBorder(m_node, static_cast<YGEdge>(edge));
// }

// double Node::getComputedPadding(int edge) const {
//   return YGNodeLayoutGetPadding(m_node, static_cast<YGEdge>(edge));
// }
#define NODE_STYLE_PROPERTY_SETTER_IMPL(type, name, paramName) \
  void Node::set##name(const type paramName)                   \
  {                                                            \
    YGNodeStyleSet##name(m_node, paramName);                   \
  }

#define NODE_STYLE_PROPERTY_IMPL(type, name, paramName)  \
  NODE_STYLE_PROPERTY_SETTER_IMPL(type, name, paramName) \
                                                         \
  type Node::get##name(void) const                       \
  {                                                      \
    return YGNodeStyleGet##name(m_node);                 \
  }

#define NODE_STYLE_PROPERTY_SETTER_UNIT_IMPL(name, paramName) \
  void Node::set##name(const YGValue paramName)               \
  {                                                           \
    switch (paramName.unit)                                   \
    {                                                         \
    case YGUnitPercent:                                       \
    {                                                         \
      YGNodeStyleSet##name##Percent(m_node, paramName.value); \
      break;                                                  \
    }                                                         \
    case YGUnitPoint:                                         \
    {                                                         \
      YGNodeStyleSet##name(m_node, paramName.value);          \
      break;                                                  \
    }                                                         \
    default:                                                  \
    {                                                         \
      break;                                                  \
    }                                                         \
    }                                                         \
  }

#define NODE_STYLE_PROPERTY_SETTER_UNIT_AUTO_IMPL(name, paramName) \
  void Node::set##name(const YGValue paramName)                    \
  {                                                                \
    switch (paramName.unit)                                        \
    {                                                              \
    case YGUnitAuto:                                               \
    {                                                              \
      YGNodeStyleSet##name##Auto(m_node);                          \
      break;                                                       \
    }                                                              \
    case YGUnitPercent:                                            \
    {                                                              \
      YGNodeStyleSet##name##Percent(m_node, paramName.value);      \
      break;                                                       \
    }                                                              \
    case YGUnitPoint:                                              \
    {                                                              \
      YGNodeStyleSet##name(m_node, paramName.value);               \
      break;                                                       \
    }                                                              \
    default:                                                       \
    {                                                              \
      break;                                                       \
    }                                                              \
    }                                                              \
  }

#define NODE_STYLE_PROPERTY_UNIT_IMPL(name, paramName)   \
  NODE_STYLE_PROPERTY_SETTER_UNIT_IMPL(name, paramName); \
                                                         \
  YGValue Node::get##name(void) const                    \
  {                                                      \
    return YGNodeStyleGet##name(m_node);                 \
  }

#define NODE_STYLE_PROPERTY_UNIT_AUTO_IMPL(name, paramName)   \
  NODE_STYLE_PROPERTY_SETTER_UNIT_AUTO_IMPL(name, paramName); \
                                                              \
  YGValue Node::get##name(void) const                         \
  {                                                           \
    return YGNodeStyleGet##name(m_node);                      \
  }

#define NODE_STYLE_INDV_EDGE_PROPERTY_IMPL(type, methodName, name, paramName, edge) \
  void Node::set##methodName(const type paramName)                   \
  {                                                                  \
    YGNodeStyleSet##name(m_node, edge, paramName);                   \
  }                                                                  \
  type Node::get##methodName(void) const                             \
  {                                                                  \
    return YGNodeStyleGet##name(m_node, edge);                       \
  }

#define NODE_LAYOUT_PROPERTY_IMPL(type, name) \
  type Node::getComputed##name(void) const    \
  {                                           \
    return YGNodeLayoutGet##name(m_node);     \
  }

#define NODE_LAYOUT_INDV_EDGE_PROPERTY_IMPL(type, methodName, name, edge) \
  type Node::getComputed##methodName(void) const                     \
  {                                                                  \
    return YGNodeLayoutGet##name(m_node, edge);                      \
  }

#define NODE_STYLE_INDV_EDGE_UNIT_PROPERTY_IMPL(methodName, name, paramName, edge) \
  void Node::set##methodName(const YGValue paramName)                              \
  {                                                                                \
    switch (paramName.unit)                                                        \
    {                                                                              \
      case YGUnitPercent:                                                          \
      {                                                                            \
        YGNodeStyleSet##name##Percent(m_node, edge, paramName.value);              \
        break;                                                                     \
      }                                                                            \
      case YGUnitPoint:                                                            \
      {                                                                            \
        YGNodeStyleSet##name(m_node, edge, paramName.value);                       \
        break;                                                                     \
      }                                                                            \
      default:                                                                     \
      {                                                                            \
        break;                                                                     \
      }                                                                            \
    }                                                                              \
  }                                                                                \
  YGValue Node::get##methodName(void) const                                        \
  {                                                                                \
    return YGNodeStyleGet##name(m_node, edge);                                     \
  }

#define NODE_STYLE_INDV_EDGE_UNIT_AUTO_PROPERTY_IMPL(methodName, name, paramName, edge) \
  void Node::set##methodName(const YGValue paramName)                                   \
  {                                                                                     \
    switch (paramName.unit)                                                             \
    {                                                                                   \
      case YGUnitAuto:                                                                  \
      {                                                                                 \
        YGNodeStyleSet##name##Auto(m_node, edge);                                       \
        break;                                                                          \
      }                                                                                 \
      case YGUnitPercent:                                                               \
      {                                                                                 \
        YGNodeStyleSet##name##Percent(m_node, edge, paramName.value);                   \
        break;                                                                          \
      }                                                                                 \
      case YGUnitPoint:                                                                 \
      {                                                                                 \
        YGNodeStyleSet##name(m_node, edge, paramName.value);                            \
        break;                                                                          \
      }                                                                                 \
      default:                                                                          \
      {                                                                                 \
        break;                                                                          \
      }                                                                                 \
    }                                                                                   \
  }                                                                                     \
  YGValue Node::get##methodName(void) const                                             \
  {                                                                                     \
    return YGNodeStyleGet##name(m_node, edge);                                          \
  }

#define NODE_STYLE_EDGE_PROPERTY_IMPL(type, name, paramName)                                                 \
  NODE_STYLE_INDV_EDGE_PROPERTY_IMPL(type, name##Left, name, paramName##Left, YGEdgeLeft);                   \
  NODE_STYLE_INDV_EDGE_PROPERTY_IMPL(type, name##Right, name, paramName##Right, YGEdgeRight);                \
  NODE_STYLE_INDV_EDGE_PROPERTY_IMPL(type, name##Top, name, paramName##Top, YGEdgeTop);                      \
  NODE_STYLE_INDV_EDGE_PROPERTY_IMPL(type, name##Bottom, name, paramName##Bottom, YGEdgeBottom);             \
  NODE_STYLE_INDV_EDGE_PROPERTY_IMPL(type, name##Start, name, paramName##Start, YGEdgeStart);                \
  NODE_STYLE_INDV_EDGE_PROPERTY_IMPL(type, name##End, name, paramName##End, YGEdgeEnd);                      \
  NODE_STYLE_INDV_EDGE_PROPERTY_IMPL(type, name##Horizontal, name, paramName##Horizontal, YGEdgeHorizontal); \
  NODE_STYLE_INDV_EDGE_PROPERTY_IMPL(type, name##Vertical, name, paramName##Vertical, YGEdgeVertical);       \
  NODE_STYLE_INDV_EDGE_PROPERTY_IMPL(type, name##All, name, paramName##All, YGEdgeAll);

#define NODE_STYLE_EDGE_PROPERTY_UNIT_IMPL(name, paramName)                                                 \
  NODE_STYLE_INDV_EDGE_UNIT_PROPERTY_IMPL(name##Left, name, paramName##Left, YGEdgeLeft);                   \
  NODE_STYLE_INDV_EDGE_UNIT_PROPERTY_IMPL(name##Right, name, paramName##Right, YGEdgeRight);                \
  NODE_STYLE_INDV_EDGE_UNIT_PROPERTY_IMPL(name##Top, name, paramName##Top, YGEdgeTop);                      \
  NODE_STYLE_INDV_EDGE_UNIT_PROPERTY_IMPL(name##Bottom, name, paramName##Bottom, YGEdgeBottom);             \
  NODE_STYLE_INDV_EDGE_UNIT_PROPERTY_IMPL(name##Start, name, paramName##Start, YGEdgeStart);                \
  NODE_STYLE_INDV_EDGE_UNIT_PROPERTY_IMPL(name##End, name, paramName##End, YGEdgeEnd);                      \
  NODE_STYLE_INDV_EDGE_UNIT_PROPERTY_IMPL(name##Horizontal, name, paramName##Horizontal, YGEdgeHorizontal); \
  NODE_STYLE_INDV_EDGE_UNIT_PROPERTY_IMPL(name##Vertical, name, paramName##Vertical, YGEdgeVertical);       \
  NODE_STYLE_INDV_EDGE_UNIT_PROPERTY_IMPL(name##All, name, paramName##All, YGEdgeAll);

#define NODE_STYLE_EDGE_PROPERTY_UNIT_AUTO_IMPL(name, paramName)                                                 \
  NODE_STYLE_INDV_EDGE_UNIT_AUTO_PROPERTY_IMPL(name##Left, name, paramName##Left, YGEdgeLeft);                   \
  NODE_STYLE_INDV_EDGE_UNIT_AUTO_PROPERTY_IMPL(name##Right, name, paramName##Right, YGEdgeRight);                \
  NODE_STYLE_INDV_EDGE_UNIT_AUTO_PROPERTY_IMPL(name##Top, name, paramName##Top, YGEdgeTop);                      \
  NODE_STYLE_INDV_EDGE_UNIT_AUTO_PROPERTY_IMPL(name##Bottom, name, paramName##Bottom, YGEdgeBottom);             \
  NODE_STYLE_INDV_EDGE_UNIT_AUTO_PROPERTY_IMPL(name##Start, name, paramName##Start, YGEdgeStart);                \
  NODE_STYLE_INDV_EDGE_UNIT_AUTO_PROPERTY_IMPL(name##End, name, paramName##End, YGEdgeEnd);                      \
  NODE_STYLE_INDV_EDGE_UNIT_AUTO_PROPERTY_IMPL(name##Horizontal, name, paramName##Horizontal, YGEdgeHorizontal); \
  NODE_STYLE_INDV_EDGE_UNIT_AUTO_PROPERTY_IMPL(name##Vertical, name, paramName##Vertical, YGEdgeVertical);       \
  NODE_STYLE_INDV_EDGE_UNIT_AUTO_PROPERTY_IMPL(name##All, name, paramName##All, YGEdgeAll);

#define NODE_LAYOUT_EDGE_PROPERTY_IMPL(type, name) \
  NODE_LAYOUT_INDV_EDGE_PROPERTY_IMPL(type, name##Left, name, YGEdgeLeft);     \
  NODE_LAYOUT_INDV_EDGE_PROPERTY_IMPL(type, name##Right, name, YGEdgeRight);   \
  NODE_LAYOUT_INDV_EDGE_PROPERTY_IMPL(type, name##Top, name, YGEdgeTop);       \
  NODE_LAYOUT_INDV_EDGE_PROPERTY_IMPL(type, name##Bottom, name, YGEdgeBottom);

NODE_STYLE_PROPERTY_IMPL(YGDirection, Direction, direction);
NODE_STYLE_PROPERTY_IMPL(YGFlexDirection, FlexDirection, flexDirection);
NODE_STYLE_PROPERTY_IMPL(YGJustify, JustifyContent, justifyContent);
NODE_STYLE_PROPERTY_IMPL(YGAlign, AlignContent, alignContent);
NODE_STYLE_PROPERTY_IMPL(YGAlign, AlignItems, alignItems);
NODE_STYLE_PROPERTY_IMPL(YGAlign, AlignSelf, alignSelf);
NODE_STYLE_PROPERTY_IMPL(YGPositionType, PositionType, positionType);
NODE_STYLE_PROPERTY_IMPL(YGWrap, FlexWrap, flexWrap);
NODE_STYLE_PROPERTY_IMPL(YGOverflow, Overflow, overflow);
NODE_STYLE_PROPERTY_IMPL(YGDisplay, Display, display);
NODE_STYLE_PROPERTY_IMPL(float, Flex, flex);

NODE_STYLE_EDGE_PROPERTY_UNIT_IMPL(Position, position);
NODE_STYLE_EDGE_PROPERTY_UNIT_AUTO_IMPL(Margin, margin);
NODE_STYLE_EDGE_PROPERTY_UNIT_IMPL(Padding, padding);
NODE_STYLE_EDGE_PROPERTY_IMPL(float, Border, border);

NODE_STYLE_PROPERTY_UNIT_AUTO_IMPL(Width, width);
NODE_STYLE_PROPERTY_UNIT_AUTO_IMPL(Height, height);
NODE_STYLE_PROPERTY_UNIT_IMPL(MinWidth, minWidth);
NODE_STYLE_PROPERTY_UNIT_IMPL(MinHeight, minHeight);
NODE_STYLE_PROPERTY_UNIT_IMPL(MaxWidth, maxWidth);
NODE_STYLE_PROPERTY_UNIT_IMPL(MaxHeight, maxHeight);

NODE_STYLE_PROPERTY_IMPL(float, AspectRatio, aspectRatio);

NODE_STYLE_PROPERTY_SETTER_IMPL(float, FlexGrow, flexGrow);
float Node::getFlexGrow() const { return NULL; } // yoga doesn't provide a getter
NODE_STYLE_PROPERTY_SETTER_IMPL(float, FlexShrink, flexShrink);
float Node::getFlexShrink() const { return NULL; } // yoga doesn't provide a getter
NODE_STYLE_PROPERTY_UNIT_AUTO_IMPL(FlexBasis, flexBasis);

NODE_LAYOUT_PROPERTY_IMPL(float, Left);
NODE_LAYOUT_PROPERTY_IMPL(float, Top);
NODE_LAYOUT_PROPERTY_IMPL(float, Width);
NODE_LAYOUT_PROPERTY_IMPL(float, Height);

NODE_LAYOUT_EDGE_PROPERTY_IMPL(float, Border);
NODE_LAYOUT_EDGE_PROPERTY_IMPL(float, Padding);

using namespace emscripten;

#define EMBIND_NODE_READONLY_PROP_BINDING(exposedName, propName) \
  .property(#exposedName, &Node::get##propName)

#define EMBIND_NODE_PROP_BINDING(exposedName, propName) \
  .property(#exposedName, &Node::get##propName, &Node::set##propName)

#define EMBIND_NODE_EDGE_PROP_BINDING(exposedName, propName)               \
  EMBIND_NODE_PROP_BINDING(exposedName##Left, propName##Left)              \
  EMBIND_NODE_PROP_BINDING(exposedName##Right, propName##Right)            \
  EMBIND_NODE_PROP_BINDING(exposedName##Top, propName##Top)                \
  EMBIND_NODE_PROP_BINDING(exposedName##Bottom, propName##Bottom)          \
  EMBIND_NODE_PROP_BINDING(exposedName##Start, propName##Start)            \
  EMBIND_NODE_PROP_BINDING(exposedName##End, propName##End)                \
  EMBIND_NODE_PROP_BINDING(exposedName##Horizontal, propName##Horizontal)  \
  EMBIND_NODE_PROP_BINDING(exposedName##Vertical, propName##Vertical)      \
  EMBIND_NODE_PROP_BINDING(exposedName, propName##All)

#define EMBIND_NODE_EDGE_LAYOUT_PROP_BINDING(exposedName, propName)        \
  EMBIND_NODE_READONLY_PROP_BINDING(exposedName##Left, propName##Left)     \
  EMBIND_NODE_READONLY_PROP_BINDING(exposedName##Right, propName##Right)   \
  EMBIND_NODE_READONLY_PROP_BINDING(exposedName##Top, propName##Top)       \
  EMBIND_NODE_READONLY_PROP_BINDING(exposedName##Bottom, propName##Bottom)

EMSCRIPTEN_BINDINGS(YGNode)
{
  class_<MeasureCallback>("MeasureCallback")
    .function("measure", &MeasureCallback::measure, pure_virtual())
    .allow_subclass<MeasureCallbackWrapper>("MeasureCallbackWrapper")
    ;

  value_object<Size>("Size")
    .field("width", &Size::width)
    .field("height", &Size::height)
    ;

  value_object<YGValue>("YGValue")
    .field("value", &YGValue::value)
    .field("unit", &YGValue::unit)
    ;

  value_object<Layout>("YGLayout")
    .field("left", &Layout::left)
    .field("top", &Layout::top)
    .field("width", &Layout::width)
    .field("height", &Layout::height)
    ;

  constant("YGUndefined", YGUndefined);
  // constant("YGValueAuto", YGValueAuto);

  class_<Node>("YGNode")
    .constructor<>(&Node::createDefault, allow_raw_pointers())

    .class_function("createWithConfig", &Node::createWithConfig, allow_raw_pointers())

    .function("insertChild", &Node::insertChild, allow_raw_pointers())
    .function("removeChild", &Node::removeChild, allow_raw_pointers())
    .function("getChildCount", &Node::getChildCount)
    .function("getParent", &Node::getParent, allow_raw_pointers())
    .function("getChild", &Node::getChild, allow_raw_pointers())

    .function("setMeasureFunc", &Node::setMeasureFunc, allow_raw_pointers())
    .function("unsetMeasureFunc", &Node::unsetMeasureFunc, allow_raw_pointers())

    .function("markDirty", &Node::markDirty)
    .function("isDirty", &Node::isDirty)

    .function("calculateLayout", &Node::calculateLayout)
    .function("getComputedLayout", &Node::getComputedLayout)

    .property("hasNewLayout", &Node::hasNewLayout, &Node::setHasNewLayout)

    EMBIND_NODE_PROP_BINDING(direction, Direction)
    EMBIND_NODE_PROP_BINDING(flexDirection, FlexDirection)
    EMBIND_NODE_PROP_BINDING(justifyContent, JustifyContent)
    EMBIND_NODE_PROP_BINDING(alignContent, AlignContent)
    EMBIND_NODE_PROP_BINDING(alignItems, AlignItems)
    EMBIND_NODE_PROP_BINDING(alignSelf, AlignSelf)
    EMBIND_NODE_PROP_BINDING(position, PositionType)
    EMBIND_NODE_PROP_BINDING(flexWrap, FlexWrap)
    EMBIND_NODE_PROP_BINDING(overflow, Overflow)
    EMBIND_NODE_PROP_BINDING(display, Display)
    EMBIND_NODE_PROP_BINDING(flex, Flex)
    EMBIND_NODE_PROP_BINDING(flexGrow, FlexGrow)
    EMBIND_NODE_PROP_BINDING(flexShrink, FlexShrink)
    EMBIND_NODE_PROP_BINDING(flexBasis, FlexBasis)

    EMBIND_NODE_PROP_BINDING(left, PositionLeft)
    EMBIND_NODE_PROP_BINDING(right, PositionRight)
    EMBIND_NODE_PROP_BINDING(top, PositionTop)
    EMBIND_NODE_PROP_BINDING(bottom, PositionBottom)
    EMBIND_NODE_PROP_BINDING(start, PositionStart)
    EMBIND_NODE_PROP_BINDING(end, PositionEnd)

    EMBIND_NODE_EDGE_PROP_BINDING(margin, Margin)
    EMBIND_NODE_EDGE_PROP_BINDING(padding, Padding)
    EMBIND_NODE_EDGE_PROP_BINDING(border, Border)

    EMBIND_NODE_PROP_BINDING(width, Width)
    EMBIND_NODE_PROP_BINDING(height, Height)
    EMBIND_NODE_PROP_BINDING(minWidth, MinWidth)
    EMBIND_NODE_PROP_BINDING(minHeight, MinHeight)
    EMBIND_NODE_PROP_BINDING(maxWidth, MaxWidth)
    EMBIND_NODE_PROP_BINDING(maxHeight, MaxHeight)

    EMBIND_NODE_PROP_BINDING(aspectRatio, AspectRatio)

    EMBIND_NODE_READONLY_PROP_BINDING(computedLeft, ComputedLeft)
    EMBIND_NODE_READONLY_PROP_BINDING(computedTopt, ComputedTop)
    EMBIND_NODE_READONLY_PROP_BINDING(computedWidth, ComputedWidth)
    EMBIND_NODE_READONLY_PROP_BINDING(computedHeight, ComputedHeight)

    EMBIND_NODE_EDGE_LAYOUT_PROP_BINDING(computedPadding, ComputedPadding)
    EMBIND_NODE_EDGE_LAYOUT_PROP_BINDING(computedBorder, ComputedBorder)
    ;
}
