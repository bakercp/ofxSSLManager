// =============================================================================
//
// Copyright (c) 2013-2016 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#include "ofSSLManager.h"


const std::string ofSSLManager::DEFAULT_CA_LOCATION       = "ssl/cacert.pem";
const std::string ofSSLManager::DEFAULT_PRIVATE_KEY_FILE  = "ssl/privateKey.pem";
const std::string ofSSLManager::DEFAULT_CERTIFICATE_FILE  = "ssl/certificate.pem";


ofSSLManager::ofSSLManager()
{
    Poco::Net::initializeSSL();
}


ofSSLManager::~ofSSLManager()
{
    Poco::Net::uninitializeSSL();
}


Poco::Net::Context::Ptr ofSSLManager::getDefaultServerContext()
{
    initializeServer(nullptr); // make sure it's initialized with something
    return Poco::Net::SSLManager::instance().defaultServerContext();
}


Poco::Net::Context::Ptr ofSSLManager::getDefaultClientContext()
{
    initializeClient(nullptr); // make sure it's initialized with something
    return Poco::Net::SSLManager::instance().defaultClientContext();
}


void ofSSLManager::initializeClient(Poco::Net::Context::Ptr pContext)
{
    ofSSLManager& manager = ofSSLManager::instance();

    if (!pContext.isNull())
    {
        Poco::Net::SSLManager::instance().initializeClient(nullptr,
                                                           nullptr,
                                                           pContext);
        manager._clientContextInitialized = true;
    }
    else if (!manager._clientContextInitialized)
    {
        std::string caLocation = ofToDataPath(DEFAULT_CA_LOCATION, true);

        Poco::File caLocationFile(caLocation);

        if (!caLocationFile.exists())
        {
            ofLogWarning("ofSSLManager::initializeClient") << "CA File not found.";
            caLocation = "";
        }

        Poco::Net::Context::Ptr _pContext = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE,
                                                                   caLocation);

        Poco::Net::SSLManager::instance().initializeClient(nullptr,
                                                           nullptr,
                                                           _pContext);
        manager._clientContextInitialized = true;
    }
    else
    {
        ofLogVerbose("ofSSLManager::initializeClient") << "pContext exists or the manager has already been initialized.";
    }
}


void ofSSLManager::initializeServer(Poco::Net::Context::Ptr pContext)
{
    ofSSLManager& manager = ofSSLManager::instance();

    if (!pContext.isNull())
    {
        Poco::Net::SSLManager::instance().initializeServer(nullptr,
                                                           nullptr,
                                                           pContext);
        manager._serverContextInitialized = true;
    }
    else if (!manager._serverContextInitialized)
    {
        std::string privateKeyFile = ofToDataPath(DEFAULT_PRIVATE_KEY_FILE, true);
        std::string certificateFile = ofToDataPath(DEFAULT_CERTIFICATE_FILE, true);
        std::string caLocation = ofToDataPath(DEFAULT_CA_LOCATION, true);

        Poco::File caLocationFile(caLocation);

        if (!caLocationFile.exists())
        {
            ofLogWarning("ofSSLManager::initializeClient") << "CA File not found.";
            caLocation = "";
        }

        Poco::Net::Context::Ptr _pContext = new Poco::Net::Context(Poco::Net::Context::SERVER_USE,
                                                                   privateKeyFile,
                                                                   certificateFile,
                                                                   caLocation);

        Poco::Net::SSLManager::instance().initializeServer(nullptr,
                                                           nullptr,
                                                           _pContext);
        manager._serverContextInitialized = true;
    }
    else
    {
        ofLogVerbose("ofSSLManager::initializeServer") << "pContext exists or the manager has already been initialized.";
    }
}


ofSSLManager& ofSSLManager::instance()
{
    static ofSSLManager manager;
    return manager;
}


std::string ofSSLManager::verificationModeToString(Poco::Net::Context::VerificationMode mode)
{
    switch (mode)
    {
        case Poco::Net::Context::VERIFY_NONE:
            return "VERIFY_NONE";
        case Poco::Net::Context::VERIFY_RELAXED:
            return "VERIFY_RELAXED";
        case Poco::Net::Context::VERIFY_STRICT:
            return "VERIFY_STRICT";
        case Poco::Net::Context::VERIFY_ONCE:
            return "VERIFY_ONCE";
    }

    return "UNKNOWN";
}


Poco::Net::Context::VerificationMode ofSSLManager::verificationModeFromString(const std::string& mode)
{
    if (mode == "VERIFY_NONE")
    {
        return Poco::Net::Context::VERIFY_NONE;
    }
    else if (mode == "VERIFY_RELAXED")
    {
        return Poco::Net::Context::VERIFY_RELAXED;
    }
    else if (mode == "VERIFY_STRICT")
    {
        return Poco::Net::Context::VERIFY_STRICT;
    }
    else if (mode == "VERIFY_ONCE")
    {
        return Poco::Net::Context::VERIFY_ONCE;
    }
    else
    {
        ofLogWarning("ofSSLManager::verificationModeFromString") << "Unrecognized verification mode: " << mode;
        return Poco::Net::Context::VERIFY_STRICT;
    }
}
