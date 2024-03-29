#!/usr/bin/env bun
import fs from "fs";
import path from "path";
import { program } from "commander";

program
  .argument("<watch-dir>", "The directory to be watched")
  .requiredOption("-b, --build-dir <directory>", "CMake build directory")
  .option("-s, --serve-dir <directory>", "Static assets directory", "public")
  .option("-v, --verbose");
program.parse();

const options = program.opts();
if (options.verbose) {
  console.log(`--build-dir: ${options.buildDir}, --serve-dir: ${options.serveDir}`);
}

const getValidWatchDir = (watchDirArg) => {
  try {
    if (fs.statSync(watchDirArg).isDirectory()) {
      return watchDirArg;
    }
  } catch {
    console.warn(`Warning: Unable to watch ${watchDirArg}! Falling back to watch the project directory`);
    return __dirname;
  }
}

const getContentType = (filePath) => {
  const ext = path.extname(filePath).toLowerCase();
  const contentTypes = {
    ".html": "text/html",
    ".css": "text/css",
    ".js": "text/javascript",
    ".json": "application/json",
    ".png": "image/png",
    ".jpg": "image/jpeg",
    ".jpeg": "image/jpeg",
    ".wasm": "application/wasm",
    ".dwp": "application/octet-stream",
  };
  return contentTypes[ext] || "text/plain";
};

const isSource = (filename) => {
  const ext = path.extname(filename).toLowerCase();
  return [".cpp", ".hpp", ".c", ".h"].includes(ext) || path.basename(filename) === "CMakeLists.txt" || path.basename(filename) === "vcpkg.json";
}

const isAsset = (filename) => {
  return path.dirname(filename) === "www";
}

const httpPort = 3000;
const wsPort = 3001;
const buildDir = path.join(__dirname, options.buildDir);
const serveDir = path.join(__dirname, options.serveDir);
const watchDir = getValidWatchDir(path.join(__dirname, program.args[0]));
if (options.verbose) {
  console.log(`Watching directory: ${watchDir}`);
}

fs.watch(watchDir, { recursive: true }, (event, filename) => {
  if (filename) {
    if (isSource(filename) || isAsset(filename)) {
      if (options.verbose) {
        console.log(`Detected ${event} in ${filename}`);
      }
      console.log("CMake is rebuilding...");
      const time = performance.now();
      Bun.spawnSync(["cmake", "--build", buildDir], {
        onExit: (proc, exitCode, signalCode, error) => {
          if (exitCode) {
            throw "CMake cannot rebuild!"
          }
        },
      });
      console.log(`CMake rebuilt successfully in ${(performance.now() - time) / 1000} sec.`);
    }
  }
});

Bun.serve({
  port: httpPort,
  fetch(req, server) {
    const url = new URL(req.url);
    const filePath = url.pathname === "/" ? `${serveDir}/index.html` : `${serveDir}${url.pathname}`;
    try {
      const file = Bun.file(filePath);
      return new Response(file, {
        headers: {
          "Content-Type": getContentType(filePath),
        },
      });
    } catch (error) {
      console.error(`Failed to read file ${filePath}`);
      return new Response("Not Found", { status: 404 });
    }
  },
});