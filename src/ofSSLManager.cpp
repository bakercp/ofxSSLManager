//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofSSLManager.h"
#include "ofLog.h"


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
        std::string caLocation = "";
        
        std::filesystem::path localCACertPath = ofToDataPath(DEFAULT_CA_LOCATION, true);

        std::filesystem::path sharedCACertPath = std::filesystem::path(__FILE__).parent_path().parent_path();
        sharedCACertPath /= "shared";
        sharedCACertPath /= "data";
        sharedCACertPath /= DEFAULT_CA_LOCATION;

        if (std::filesystem::exists(localCACertPath))
        {
            caLocation = localCACertPath.string();
        }
        else if (std::filesystem::exists(sharedCACertPath))
        {
            ofLogWarning("ofSSLManager::initializeClient") << "CA File not found @ " << localCACertPath.string() << ". Using " << sharedCACertPath.string() << ".";
            caLocation = sharedCACertPath.string();
        }
        else
        {
            ofLogWarning("ofSSLManager::initializeClient") << "CA File not found. Please refer to the ofxSSLManager documentation.";
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


std::string ofSSLManager::to_string(Poco::Net::Context::VerificationMode mode)
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
    
    ofLogWarning("ofSSLManager::to_string") << "Unknown verification mode.";
    return "UNKNOWN";
}


Poco::Net::Context::VerificationMode ofSSLManager::from_string(const std::string& mode)
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
