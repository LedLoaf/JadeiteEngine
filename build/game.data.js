
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
    loadPackage({"files": [{"filename": "/assets/fonts/pixel/OFL.txt", "start": 0, "end": 4488}, {"filename": "/assets/fonts/pixel/README.txt", "start": 4488, "end": 6741}, {"filename": "/assets/fonts/pixel/pixel.ttf", "start": 6741, "end": 58053}, {"filename": "/assets/scripts/defs/assetDefs.lua", "start": 58053, "end": 59724}, {"filename": "/assets/scripts/defs/characterDefs.lua", "start": 59724, "end": 60958}, {"filename": "/assets/scripts/defs/maps/level1.lua", "start": 60958, "end": 114547}, {"filename": "/assets/scripts/defs/maps/level2.lua", "start": 114547, "end": 146111}, {"filename": "/assets/scripts/main - Copy.lua", "start": 146111, "end": 154767}, {"filename": "/assets/scripts/main.lua", "start": 154767, "end": 157234}, {"filename": "/assets/scripts/utilities.lua", "start": 157234, "end": 168320}, {"filename": "/assets/sounds/Attribution.txt", "start": 168320, "end": 168430}, {"filename": "/assets/sounds/menu_accept.ogg", "start": 168430, "end": 178582}, {"filename": "/assets/sounds/music/Attribution.txt", "start": 178582, "end": 178834}, {"filename": "/assets/sounds/music/game_over.mp3", "start": 178834, "end": 660137}, {"filename": "/assets/sounds/music/main_theme.ogg", "start": 660137, "end": 767959}, {"filename": "/assets/sounds/soundfx/Attribution.txt", "start": 767959, "end": 768106}, {"filename": "/assets/sounds/soundfx/bump.wav", "start": 768106, "end": 780648}, {"filename": "/assets/sounds/soundfx/death.wav", "start": 780648, "end": 920116}, {"filename": "/assets/sounds/soundfx/finish_row.wav", "start": 920116, "end": 965672}, {"filename": "/assets/textures/Attributions.txt", "start": 965672, "end": 965811}, {"filename": "/assets/textures/characters.png", "start": 965811, "end": 978565}, {"filename": "/assets/textures/tilesets/tileset1.png", "start": 978565, "end": 990680}], "remote_package_size": 990680});

  })();
