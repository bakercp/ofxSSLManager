# ofxSSLManager

An [openFrameworks](http://openframeworks.cc) addon for managing SSL connections.

## Description

Typically, clients, such as those in `ofxHTTP` and `ofxSMTP` work best with up-to-date CA Certificate bundles. An fairly recent example is can be found at `example/bin/data/ssl/cacert.pem` and `shared/data/ssl/cacert.pem`. For up-to-date certificates (or if you just want to generate your own for security reasons), check out [this page](http://curl.haxx.se/docs/caextract.html) for more info on creating your own Certificate Authority PEM (e.g. `cacert.pem`).

## Getting Started

To get started, generate the example project files using the openFrameworks [Project Generator](http://openframeworks.cc/learning/01_basics/how_to_add_addon_to_project/). The example included in this addon does not do much. TO see the addon in action, check out the examples in [ofxHTTP](https://github.com/bakercp/ofxHTTP) or [ofxSMTP](https://github.com/bakercp/ofxSMTP).

Project that use this addon should have ensure that the required certificates are included in their project data. For standard SSL communication, place a `cacert.pem` in the `bin/data/ssl/` folder of your openFrameworks project (see the example). Alternatively, if no `bin/data/ssl/cacert.pem` is found, the addon will search in `ofxSSLManager/shared/data/ssl/cacert.pem`. To set a custom location, you can initialize a custom SSL context:

```c++
void ofApp::setup()
{
  Poco::Net::Context::Ptr pContext = ... ;

  // Initialize the custom SSL context.
  // See https://pocoproject.org/docs/Poco.Net.Context.html

  ofxSSLManager::initializeClient(pContext);

  // ...
}
```

## Documentation

API documentation can be found here.

## Build Status

Linux, macOS [![Build Status](https://travis-ci.org/bakercp/ofxSSLManager.svg?branch=master)](https://travis-ci.org/bakercp/ofxSSLManager)

Visual Studio, MSYS [![Build status](https://ci.appveyor.com/api/projects/status/i6m2u8s8jqrqkyea/branch/master?svg=true)](https://ci.appveyor.com/project/bakercp/ofxsslmanager/branch/master)

## Compatibility

The `stable` branch of this repository is meant to be compatible with the openFrameworks [stable branch](https://github.com/openframeworks/openFrameworks/tree/stable), which corresponds to the latest official openFrameworks release.

The `master` branch of this repository is meant to be compatible with the openFrameworks [master branch](https://github.com/openframeworks/openFrameworks/tree/master).

Some past openFrameworks releases are supported via [releases](../../releases/), but only the [stable branch](../../tree/stable) and the [master branch](../../tree/master) are actively supported.

## Versioning

This project uses Semantic Versioning, although strict adherence will only come into effect at version 1.0.0.

## Licensing

See [LICENSE.md](LICENSE.md).

## Contributing

Pull Requests are always welcome, so if you make any improvements please feel free to float them back upstream :)

1.  Fork this repository.
2.  Create your feature branch (`git checkout -b my-new-feature`).
3.  Commit your changes (`git commit -am 'Add some feature'`).
4.  Push to the branch (`git push origin my-new-feature`).
5.  Create new Pull Request.
