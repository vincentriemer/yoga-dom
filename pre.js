// import PthreadMain from "../build/pthread-main";

var FS = {};

Module["locateFile"] = function() {
  const file = (() => {
    try {
      return PthreadMain;
    } catch (err) {
      return "";
    }
  })();
  return file;
};

Module["mainScriptUrlOrBlob"] =
  location.origin + "/node_modules/yoga-dom/dist/yoga.worker.js";
