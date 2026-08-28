
  if (!Module['expectedDataFileDownloads']) Module['expectedDataFileDownloads'] = 0;
  Module['expectedDataFileDownloads']++;
  (() => {
    // Do not attempt to redownload the virtual filesystem data when in a pthread or a Wasm Worker context.
    var isPthread = typeof ENVIRONMENT_IS_PTHREAD != 'undefined' && ENVIRONMENT_IS_PTHREAD;
    var isWasmWorker = typeof ENVIRONMENT_IS_WASM_WORKER != 'undefined' && ENVIRONMENT_IS_WASM_WORKER;
    if (isPthread || isWasmWorker) return;
    var isNode = globalThis.process && globalThis.process.versions && globalThis.process.versions.node && globalThis.process.type != 'renderer';
    async function loadPackage(metadata) {

      var PACKAGE_PATH = '';
      if (typeof window === 'object') {
        PACKAGE_PATH = window['encodeURIComponent'](window.location.pathname.substring(0, window.location.pathname.lastIndexOf('/')) + '/');
      } else if (typeof process === 'undefined' && typeof location !== 'undefined') {
        // web worker
        PACKAGE_PATH = encodeURIComponent(location.pathname.substring(0, location.pathname.lastIndexOf('/')) + '/');
      }
      var PACKAGE_NAME = 'game.data';
      var REMOTE_PACKAGE_BASE = 'game.data';
      var REMOTE_PACKAGE_NAME = Module['locateFile'] ? Module['locateFile'](REMOTE_PACKAGE_BASE, '') : REMOTE_PACKAGE_BASE;
      var REMOTE_PACKAGE_SIZE = metadata['remote_package_size'];

      async function fetchRemotePackage(packageName, packageSize) {
        if (isNode) {
          var contents = require('fs').readFileSync(packageName);
          return new Uint8Array(contents).buffer;
        }
        if (!Module['dataFileDownloads']) Module['dataFileDownloads'] = {};
        try {
          var response = await fetch(packageName);
        } catch (e) {
          throw new Error(`Network Error: ${packageName}`, {e});
        }
        if (!response.ok) {
          throw new Error(`${response.status}: ${response.url}`);
        }

        const chunks = [];
        const headers = response.headers;
        const total = Number(headers.get('Content-Length') || packageSize);
        let loaded = 0;

        Module['setStatus'] && Module['setStatus']('Downloading data...');
        const reader = response.body.getReader();

        while (1) {
          var {done, value} = await reader.read();
          if (done) break;
          chunks.push(value);
          loaded += value.length;
          Module['dataFileDownloads'][packageName] = {loaded, total};

          let totalLoaded = 0;
          let totalSize = 0;

          for (const download of Object.values(Module['dataFileDownloads'])) {
            totalLoaded += download.loaded;
            totalSize += download.total;
          }

          Module['setStatus'] && Module['setStatus'](`Downloading data... (${totalLoaded}/${totalSize})`);
        }

        const packageData = new Uint8Array(chunks.map((c) => c.length).reduce((a, b) => a + b, 0));
        let offset = 0;
        for (const chunk of chunks) {
          packageData.set(chunk, offset);
          offset += chunk.length;
        }
        return packageData.buffer;
      }

      var fetchPromise;
      var fetched = Module['getPreloadedPackage'] && Module['getPreloadedPackage'](REMOTE_PACKAGE_NAME, REMOTE_PACKAGE_SIZE);

      if (!fetched) {
        // Note that we don't use await here because we want to execute the
        // the rest of this function immediately.
        fetchPromise = fetchRemotePackage(REMOTE_PACKAGE_NAME, REMOTE_PACKAGE_SIZE);
      }

    async function runWithFS(Module) {

      function assert(check, msg) {
        if (!check) throw new Error(msg);
      }
Module['FS_createPath']("/", "assets", true, true);
Module['FS_createPath']("/assets", "fonts", true, true);
Module['FS_createPath']("/assets/fonts", "pixel", true, true);
Module['FS_createPath']("/assets", "scripts", true, true);
Module['FS_createPath']("/assets/scripts", "defs", true, true);
Module['FS_createPath']("/assets/scripts/defs", "maps", true, true);
Module['FS_createPath']("/assets", "sounds", true, true);
Module['FS_createPath']("/assets/sounds", "music", true, true);
Module['FS_createPath']("/assets/sounds", "soundfx", true, true);
Module['FS_createPath']("/assets", "textures", true, true);
Module['FS_createPath']("/assets/textures", "tilesets", true, true);

    for (var file of metadata['files']) {
      var name = file['filename']
      Module['addRunDependency'](`fp ${name}`);
    }

      async function processPackageData(arrayBuffer) {
        assert(arrayBuffer, 'Loading data file failed.');
        assert(arrayBuffer.constructor.name === ArrayBuffer.name, 'bad input to processPackageData ' + arrayBuffer.constructor.name);
        var byteArray = new Uint8Array(arrayBuffer);
        var curr;
        // Reuse the bytearray from the XHR as the source for file reads.
          for (var file of metadata['files']) {
            var name = file['filename'];
            var data = byteArray.subarray(file['start'], file['end']);
            // canOwn this data in the filesystem, it is a slice into the heap that will never change
        Module['FS_createDataFile'](name, null, data, true, true, true);
        Module['removeRunDependency'](`fp ${name}`);
          }
          Module['removeRunDependency']('datafile_game.data');
      }
      Module['addRunDependency']('datafile_game.data');

      if (!Module['preloadResults']) Module['preloadResults'] = {};

      Module['preloadResults'][PACKAGE_NAME] = {fromCache: false};
      if (!fetched) {
        fetched = await fetchPromise;
      }
      processPackageData(fetched);

    }
    if (Module['calledRun']) {
      runWithFS(Module);
    } else {
      if (!Module['preRun']) Module['preRun'] = [];
      Module['preRun'].push(runWithFS); // FS is not initialized yet, wait for it
    }

    }
    loadPackage({"files": [{"filename": "/assets/fonts/pixel/OFL.txt", "start": 0, "end": 4488}, {"filename": "/assets/fonts/pixel/README.txt", "start": 4488, "end": 6741}, {"filename": "/assets/fonts/pixel/pixel.ttf", "start": 6741, "end": 58053}, {"filename": "/assets/scripts/defs/assetDefs.lua", "start": 58053, "end": 59724}, {"filename": "/assets/scripts/defs/characterDefs.lua", "start": 59724, "end": 60958}, {"filename": "/assets/scripts/defs/maps/level1.lua", "start": 60958, "end": 114512}, {"filename": "/assets/scripts/defs/maps/level2.lua", "start": 114512, "end": 146041}, {"filename": "/assets/scripts/main - Copy.lua", "start": 146041, "end": 154697}, {"filename": "/assets/scripts/main.lua", "start": 154697, "end": 157132}, {"filename": "/assets/scripts/utilities.lua", "start": 157132, "end": 168554}, {"filename": "/assets/sounds/Attribution.txt", "start": 168554, "end": 168664}, {"filename": "/assets/sounds/menu_accept.ogg", "start": 168664, "end": 178816}, {"filename": "/assets/sounds/music/Attribution.txt", "start": 178816, "end": 179068}, {"filename": "/assets/sounds/music/game_over.mp3", "start": 179068, "end": 660371}, {"filename": "/assets/sounds/music/main_theme.ogg", "start": 660371, "end": 768193}, {"filename": "/assets/sounds/soundfx/Attribution.txt", "start": 768193, "end": 768340}, {"filename": "/assets/sounds/soundfx/bump.wav", "start": 768340, "end": 780882}, {"filename": "/assets/sounds/soundfx/death.wav", "start": 780882, "end": 920350}, {"filename": "/assets/sounds/soundfx/finish_row.wav", "start": 920350, "end": 965906}, {"filename": "/assets/textures/Attributions.txt", "start": 965906, "end": 966045}, {"filename": "/assets/textures/characters.png", "start": 966045, "end": 978799}, {"filename": "/assets/textures/tilesets/tileset1.png", "start": 978799, "end": 990914}], "remote_package_size": 990914});

  })();
