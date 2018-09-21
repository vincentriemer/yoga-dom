import commonjs from "rollup-plugin-commonjs";
import builtins from "rollup-plugin-node-builtins";
import fileAsBlob from "rollup-plugin-file-as-blob";

export default {
  input: "src/index.js",
  context: "window",
  plugins: [
    commonjs(),
    builtins(),
    fileAsBlob({
      include: "**/pthread-main.js",
    }),
  ],
};
