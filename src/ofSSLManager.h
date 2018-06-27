//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "Poco/BasicEvent.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Delegate.h"
#include "Poco/File.h"
#include "Poco/SharedPtr.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/NetSSL.h"
#include "Poco/Net/PrivateKeyPassphraseHandler.h"
#include "Poco/Net/InvalidCertificateHandler.h"
#include "Poco/Net/SSLManager.h"
#include "ofUtils.h"
#include "ofEvents.h"


/// \brief A class to simplify client and server SSL Context management.
///
/// Default OpenSSL security contexts are represented by the
/// Poco::Net:Context class and managed by the managed by the
/// Poco::Net::SSLManager class.  Poco Sockets, Streams and Sessions
/// can be explicity instantiated with a Poco::Net::Context or
/// they can (more conveniently) employ a default SSL Client or
/// Server Context.  For applications that implement
/// Poco::Util::Application, default Context setup and configuration
/// trivially done via config files. But, since ofBaseApp does not
/// extend Poco::Util::Application, special care must be taken when
/// creating and using Poco's default Client and Server Contexts.
///
/// ofSSLManager adds a layer between the openFrameworks user and
/// the Poco::Net::SSLManager to ensure that the default contexts
/// are configured in an openFrameworks-friendly way (e.g. providing
/// a default location for certificate authority files, SSL
/// certificate and Private Key files, the default security settings
/// and mechanisms for registering SSL verification event callbacks.
///
/// ofSSLManager allows the user to either accept the default
/// ofSSLManager Client and Server Contexts or provide a custom
/// Context during by calling ofSSLManager::initializeClient(...) or
/// ofSSLManager::initializeServer(...).  The user must take care
/// to interact with the default SSL Context management system via
/// the ofSSLManager singleton, rather than calling
/// Poco::Net::SSLManager directly.
///
/// Finally, in order to simplify the certificate verification
/// process, ofSSLManager configures Contexts to seek their
/// verification and private key passwords via an event callback.
/// Thus, users wishing to manually verify Client and Server private
/// key passwords, SSL certificates that are not automatically
/// accepted based on the certificate authority settings, etc
/// must add listeners for SSL verification callbacks BEFORE
/// any calls to any of the following methods:
///
/// ~~~{.cpp}
///    ofSSLManager::defaultServerContext();
///    ofSSLManager::defaultServerContext();
///    ofSSLManager::initializeServer(...);
///    ofSSLManager::initializeClient(...);
/// ~~~
///
/// Further information about registering these event callbacks can
/// be found in the ofSSLManager::registerAllEvents(),
/// ofSSLManager::registerClientEvents(), and
/// ofSSLManager::registerServerEvents() method documentation.
class ofSSLManager
{
public:
    /// \brief Get the default Server Context via the ofSSLManager.
    /// \returns A pointer to the default Server Context.
    /// \note This is the same context that is returned via
    ///        Poco::Net::SSLManager::instance().defaultServerContext();
    ///        but unlike that call, ofSSLManager::defaultServerContext();
    ///        calls ensures that ofSSLManager::initializeServer(); is
    ///        called prior to the Poco::Net::SSLManager call.  This
    ///        ensures that the default Server Context has been configured
    ///        first by ofSSLManager, rather than Poco::Net::SSLManager.
    static Poco::Net::Context::Ptr getDefaultServerContext();

    /// \brief Get the default Client Context via the ofSSLManager.
    /// \returns A pointer to the default Client Context.
    /// \note This is the same context that is returned via
    ///        Poco::Net::SSLManager::instance().defaultClientContext();
    ///        but unlike that call, ofSSLManager::defaultClientContext();
    ///        calls ensures that ofSSLManager::initializeClient(); is
    ///        called prior to the Poco::Net::SSLManager call.  This
    ///        ensures that the default Client Context has been configured
    ///        first by ofSSLManager, rather than Poco::Net::SSLManager.
    static Poco::Net::Context::Ptr getDefaultClientContext();

    /// \brief Initialize a SSL Server Context.
    /// \param pContext A Poco::Net::Context::Ptr to set as the default.
    /// \note  If the user wishes to use non-default Context (see
    ///        Poco::Net::Context for a list of options), the
    ///        user must create a Context and initialize it with this
    ///        function immediately in the ofBaseApp::setup() function.
    ///        Otherwise, ofSSLManager::initializeSlient() will be
    ///        configured with the ofSSLManager default settings.
    static void initializeServer(Poco::Net::Context::Ptr pContext = nullptr);

    /// \brief Initialize a SSL Client Context.
    /// \param pContext A Poco::Net::Context::Ptr to set as the default.
    /// \note  If the user wishes to use non-default Context (see
    ///        Poco::Net::Context for a list of options), the
    ///        user must create a Context and initialize it with this
    ///        function immediately in the ofBaseApp::setup() function.
    ///        Otherwise, ofSSLManager::initializeClient() will be
    ///        configured with the ofSSLManager default settings.
    static void initializeClient(Poco::Net::Context::Ptr pContext = nullptr);

    /// \brief Register the listener class for all Client and Server SSL events.
    /// \param listener A pointer to the class containing all callbacks.
    /// \note Applications that do not implement these callbacks will not be
    ///         given the opportunity to manually approve SSL cert errors.
    ///         Listening classes must have callbacks in the form:
    ///
    /// ~~~{.cpp}
    ///     void onSSLServerVerificationError(Poco::Net::VerificationErrorArgs& args);
    ///     void onSSLClientVerificationError(Poco::Net::VerificationErrorArgs& args);
    ///     void onSSLPrivateKeyPassphraseRequired(std::string& args);
    /// ~~~
    ///
    ///        A listener might call this class in the ofBaseApp::setup()
    ///        method like this:
    ///
    /// ~~~{.cpp}
    ///    ofSSLManager::registerAllEvents(this);
    /// ~~~
    template <class ListenerClass>
    static void registerAllEvents(ListenerClass* listener);

    /// \brief Unregister the listener class for all Client and Server SSL events.
    /// \param listener A pointer to the class containing all callbacks.
    template <class ListenerClass>
    static void unregisterAllEvents(ListenerClass* listener);

    /// \brief Register the listener class for all Client SSL events.
    /// \param listener A pointer to the class containing client callbacks.
    /// \note Applications that do not implement these callbacks will not be
    ///         given the opportunity to manually approve SSL cert errors.
    ///         Listening classes must have callbacks in the form:
    ///
    /// ~~~{.cpp}
    ///     void onSSLClientVerificationError(Poco::Net::VerificationErrorArgs& args);
    ///     void onSSLPrivateKeyPassphraseRequired(std::string& args);
    /// ~~~
    ///        A listener might call this class in the ofBaseApp::setup()
    ///        method like this:
    ///
    /// ~~~{.cpp}
    ///    ofSSLManager::registerClientEvents(this);
    /// ~~~
    template <class ListenerClass>
    static void registerClientEvents(ListenerClass* listener);

    /// \brief Unregister the listener class for all Client SSL events.
    /// \param listener A pointer to the class containing all callbacks.
    template <class ListenerClass>
    static void unregisterClientEvents(ListenerClass* listener);

    /// \brief Register the listener class for all Server SSL events.
    /// \param listener A pointer to the class containing client callbacks.
    /// \note Applications that do not implement these callbacks will not be
    ///         given the opportunity to manually approve SSL cert errors.
    ///         Listening classes must have callbacks in the form:
    ///
    /// ~~~{.cpp}
    ///     void onSSLServerVerificationError(Poco::Net::VerificationErrorArgs& args);
    ///     void onSSLPrivateKeyPassphraseRequired(std::string& args);
    /// ~~~
    ///
    ///        A listener might call this class in the ofBaseApp::setup()
    ///        method like this:
    ///
    /// ~~~{.cpp}
    ///     ofSSLManager::registerServerEvents(this);
    /// ~~~
    template <class ListenerClass>
    static void registerServerEvents(ListenerClass* listener);

    /// \brief Unregister the listener class for all Server SSL events.
    /// \param listener A pointer to the class containing all callbacks.
    template <class ListenerClass>
    static void unregisterServerEvents(ListenerClass* listener);

    /// \brief The default location of the certificate authority bundle.
    ///
    /// The certificate authority bundle can be extracted from
    /// The cURL website. More information is here:
    /// http://curl.haxx.se/docs/caextract.html
    static const std::string DEFAULT_CA_LOCATION;

    /// \brief The default location of the private key pem file.
    ///
    /// SSL Certificate files and their private key counterparts
    /// can be purchased or self-signed.  More information on
    /// generating self-signed certificates is here:
    /// https://devcenter.heroku.com/articles/ssl-certificate-self
    static const std::string DEFAULT_PRIVATE_KEY_FILE;

    /// \brief The default location of the certificate pem file.
    ///
    /// SSL Certificate files and their private key counterparts
    /// can be purchased or self-signed.  More information on
    /// generating self-signed certificates is here:
    /// https://devcenter.heroku.com/articles/ssl-certificate-self
    static const std::string DEFAULT_CERTIFICATE_FILE;

    /// \brief Get the string representation of a verification mode.
    /// \param mode The mode to convert.
    /// \returns The string representation.  Returns "UNKNOWN" if unknown.
    static std::string to_string(Poco::Net::Context::VerificationMode mode);

    /// \brief Get the verification mode from a string.
    /// \param mode The mode string to convert.
    /// \returns The verification mode. Returns VERIFY_STRICT if unknown.
    static Poco::Net::Context::VerificationMode from_string(const std::string& mode);

private:
    ofSSLManager();
    ofSSLManager(const ofSSLManager&) = delete;
    ofSSLManager& operator = (const ofSSLManager&) = delete;

    /// \brief Destroys the ofSSLManager.
    ~ofSSLManager();

    /// \brief An instance of the ofSSLManager.
    ///
    /// All static methods access the singleton via this method.
    /// Usually there is no need for a user to call this method.
    ///
    /// \returns A reference to the singleton.
    static ofSSLManager& instance();

    /// \brief True iff ofSSLManager initialized its own Client Context.
    bool _clientContextInitialized = false;

    /// \brief True iff ofSSLManager initialized its own Server Context.
    bool _serverContextInitialized = false;

};


template <class ListenerClass>
void ofSSLManager::registerAllEvents(ListenerClass* listener)
{
    ofSSLManager::instance();
    Poco::Net::SSLManager& manager = Poco::Net::SSLManager::instance();
    manager.ServerVerificationError      += Poco::delegate(listener, &ListenerClass::onSSLServerVerificationError);
    manager.ClientVerificationError      += Poco::delegate(listener, &ListenerClass::onSSLClientVerificationError);
    manager.PrivateKeyPassphraseRequired += Poco::delegate(listener, &ListenerClass::onSSLPrivateKeyPassphraseRequired);
}


template <class ListenerClass>
void ofSSLManager::unregisterAllEvents(ListenerClass* listener)
{
    ofSSLManager::instance();
    Poco::Net::SSLManager& manager = Poco::Net::SSLManager::instance();
    manager.ServerVerificationError      -= Poco::delegate(listener, &ListenerClass::onSSLServerVerificationError);
    manager.ClientVerificationError      -= Poco::delegate(listener, &ListenerClass::onSSLClientVerificationError);
    manager.PrivateKeyPassphraseRequired -= Poco::delegate(listener, &ListenerClass::onSSLPrivateKeyPassphraseRequired);
}


template <class ListenerClass>
void ofSSLManager::registerClientEvents(ListenerClass* listener)
{
    ofSSLManager::instance();
    Poco::Net::SSLManager& manager = Poco::Net::SSLManager::instance();
    manager.ClientVerificationError      += Poco::delegate(listener, &ListenerClass::onSSLClientVerificationError);
    manager.PrivateKeyPassphraseRequired += Poco::delegate(listener, &ListenerClass::onSSLPrivateKeyPassphraseRequired);
}


template <class ListenerClass>
void ofSSLManager::unregisterClientEvents(ListenerClass* listener)
{
    ofSSLManager::instance();
    Poco::Net::SSLManager& manager = Poco::Net::SSLManager::instance();
    manager.ClientVerificationError      -= Poco::delegate(listener, &ListenerClass::onSSLClientVerificationError);
    manager.PrivateKeyPassphraseRequired -= Poco::delegate(listener, &ListenerClass::onSSLPrivateKeyPassphraseRequired);
}


template <class ListenerClass>
void ofSSLManager::registerServerEvents(ListenerClass* listener)
{
    ofSSLManager::instance();
    Poco::Net::SSLManager& manager = Poco::Net::SSLManager::instance();
    manager.ServerVerificationError      += Poco::delegate(listener, &ListenerClass::onSSLServerVerificationError);
    manager.PrivateKeyPassphraseRequired += Poco::delegate(listener, &ListenerClass::onSSLPrivateKeyPassphraseRequired);
}


template <class ListenerClass>
void ofSSLManager::unregisterServerEvents(ListenerClass* listener)
{
    ofSSLManager::instance();
    Poco::Net::SSLManager& manager = Poco::Net::SSLManager::instance();
    manager.ServerVerificationError      -= Poco::delegate(listener, &ListenerClass::onSSLServerVerificationError);
    manager.PrivateKeyPassphraseRequired -= Poco::delegate(listener, &ListenerClass::onSSLPrivateKeyPassphraseRequired);
}


/// \brief Allows users to easily view the contents of Poco::Net::VerificationErrorArgs.
template <>
inline std::string ofToString(const Poco::Net::VerificationErrorArgs& args)
{
    std::stringstream os;

    static int width = 15;
    static std::string fmt = "%dd %H:%M:%S.%i";

    os << "======================================================" << std::endl;
    os << "Error: " << std::endl;
    os << std::setw(width) << "Message: " << args.errorMessage() << std::endl;
    os << std::setw(width) << "#: " << args.errorNumber() << std::endl;
    os << std::setw(width) << "Depth: " << args.errorDepth() << std::endl;
    os << "======================================================" << std::endl;
    const Poco::Net::X509Certificate& cert = args.certificate();
    os << "Certificate: " << std::endl;
    os << std::setw(width) << "Issued By: " << cert.issuerName() << std::endl;
    os << std::setw(width) << "Subject Name: " << cert.subjectName() << std::endl;
    os << std::setw(width) << "Common Name: " << cert.commonName() << std::endl;
    os << std::setw(width) << "Valid From: " << Poco::DateTimeFormatter::format(cert.validFrom(), fmt) << std::endl;
    os << std::setw(width) << "Expires On: " << Poco::DateTimeFormatter::format(cert.expiresOn(), fmt) << std::endl;
    os << "======================================================" << std::endl;

    return os.str();
}
