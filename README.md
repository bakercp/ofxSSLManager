ofxSSLManager
=============

A system for managing SSL certificates via Poco.

## Description

A system for managing SSL certificates via Poco.

Typically, clients, such as those in ofxHTTP work best with up-to-date CA Certificate bundles.  An fairly recent example is included in example folder. For up-to-date certificates (or if you just want to generate your own for security reasons), check out [this page](http://curl.haxx.se/docs/caextract.html) for more info on creating your own Certificate Authority PEM (e.g. `ca.pem`).

## Getting Started

To get started, generate the example project files using the openFrameworks [Project Generator](http://openframeworks.cc/learning/01_basics/how_to_add_addon_to_project/).

## Documentation

API documentation can be found here.

## Build Status

Linux, macOS [![Build Status](https://travis-ci.org/bakercp/ofxSSLManager.svg?branch=master)](https://travis-ci.org/bakercp/ofxSSLManager)

Visual Studio, MSYS [![Build status](https://ci.appveyor.com/api/projects/status/i6m2u8s8jqrqkyea/branch/master?svg=true)](https://ci.appveyor.com/project/bakercp/ofxsslmanager/branch/master)

## Compatibility

The `stable` branch of this repository is meant to be compatible with the openFrameworks [stable branch](https://github.com/openframeworks/openFrameworks/tree/stable), which corresponds to the latest official openFrameworks release.

The `master` branch of this repository is meant to be compatible with the openFrameworks [master branch](https://github.com/openframeworks/openFrameworks/tree/master).

Some past openFrameworks releases are supported via tagged versions, but only `stable` and `master` branches are actively supported.

## Versioning

This project uses Semantic Versioning, although strict adherence will only come into effect at version 1.0.0.

## Licensing

See `LICENSE.md`.

## Contributing

Pull Requests are always welcome, so if you make any improvements please feel free to float them back upstream :)

1. Fork this repository.
2. Create your feature branch (`git checkout -b my-new-feature`).
3. Commit your changes (`git commit -am 'Add some feature'`).
4. Push to the branch (`git push origin my-new-feature`).
5. Create new Pull Request.
