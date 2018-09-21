import Yoga from "../build/yoga.module";

function patch(prototype, name, fn) {
  var original = prototype[name];

  prototype[name] = function() {
    const originalArgs = Array.prototype.slice.call(arguments);
    return fn.apply(this, [original].concat(originalArgs));
  };
}

// Emscripten exports a "promise-like" function which, if
// you try to resolve via a mechanism that uses native promises,
// causes an infinite loop, so we manually resolve/wrap the call in
// a native promise
export default new Promise(function(resolve) {
  const Module = Yoga;
  Module["onRuntimeInitialized"] = function() {
    patch(Module.YGNode.prototype, "free", function() {
      this.delete();
    });

    patch(Module.YGNode.prototype, "freeRecursive", function() {
      for (var t = 0, T = this.getChildCount(); t < T; ++t)
        this.getChild(0).freeRecursive();
      this.free();
    });

    function wrapMeasureFunction(measureFunction) {
      return Module.MeasureCallback.implement({ measure: measureFunction });
    }

    patch(Module.YGNode.prototype, "setMeasureFunc", function(
      original,
      measureFunc
    ) {
      original.call(this, wrapMeasureFunction(measureFunc));
    });

    patch(Module.YGNode.prototype, "calculateLayout", function(
      original,
      width = Module.YGUndefined,
      height = Module.YGUndefined,
      direction = Module.YGDirection.ltr
    ) {
      return original.call(this, width, height, direction);
    });

    resolve({
      Node: Module.YGNode,
      Config: Module.YGConfig,
      Constants: {
        align: Module.YGAlign,
        dimension: Module.YGDimension,
        direction: Module.YGDirection,
        display: Module.YGDisplay,
        // edge: Module.YGEdge,
        flexDirection: Module.YGFlexDirection,
        justify: Module.YGJustify,
        measureMode: Module.YGMeasureMode,
        // nodeType: Module.YGNodeType,
        overflow: Module.YGOverflow,
        position: Module.YGPositionType,
        unit: Module.YGUnit,
        wrap: Module.YGWrap,
        undefinedValue: Module.YGUndefined,
        // autoValue: Module.YGValueAuto,
      },
    });
  };
});
