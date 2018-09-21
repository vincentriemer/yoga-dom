import baseConfig from "./rollup.config";

const IIFEConfig = Object.assign({}, baseConfig);

IIFEConfig.output = {
  name: "Yoga",
  format: "iife",
  file: "dist/Yoga.iife.js",
};

export default IIFEConfig;
