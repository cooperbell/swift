/*
 * Copyright (c) 2011 Kevin Smith
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */


#pragma once

#include <vector>

#include <Swiften/Base/SafeString.h>
#include <Swiften/Network/BOSHConnectionFactory.h>
#include <Swiften/Network/BOSHConnection.h>

namespace Swift {
	class HTTPConnectProxiedConnectionFactory;
	class TLSConnectionFactory;
	class BOSHConnectionPool : public boost::bsignals::trackable {
		public:
			BOSHConnectionPool(boost::shared_ptr<BOSHConnectionFactory> factory, const std::string& to, long initialRID, const URL& boshHTTPConnectProxyURL, const SafeString& boshHTTPConnectProxyAuthID, const SafeString& boshHTTPConnectProxyAuthPassword);
			~BOSHConnectionPool();
			void write(const SafeByteArray& data);
			void writeFooter();
			void close();
			void restartStream();

			boost::signal<void (BOSHError::ref)> onSessionTerminated;
			boost::signal<void ()> onSessionStarted;
			boost::signal<void (const SafeByteArray&)> onXMPPDataRead;
			boost::signal<void (const SafeByteArray&)> onBOSHDataRead;
			boost::signal<void (const SafeByteArray&)> onBOSHDataWritten;

		private:
			void handleDataRead(const SafeByteArray& data);
			void handleSessionStarted(const std::string& sid, size_t requests);
			void handleBOSHDataRead(const SafeByteArray& data);
			void handleBOSHDataWritten(const SafeByteArray& data);
			void handleSessionTerminated(BOSHError::ref condition);
			void handleConnectFinished(bool, BOSHConnection::ref connection);
			void handleConnectionDisconnected(const boost::optional<Connection::Error>& error, BOSHConnection::ref connection);
			void handleHTTPError(const std::string& errorCode);

		private:
			BOSHConnection::ref createConnection();
			void destroyConnection(BOSHConnection::ref connection);
			void tryToSendQueuedData();
			BOSHConnection::ref getSuitableConnection();

		private:
			boost::shared_ptr<BOSHConnectionFactory> connectionFactory;
			std::vector<BOSHConnection::ref> connections;
			std::string sid;
			unsigned long rid;
			std::vector<SafeByteArray> dataQueue;
			bool pendingTerminate;
			std::string to;
			size_t requestLimit;
			int restartCount;
			bool pendingRestart;
			HTTPConnectProxiedConnectionFactory* connectProxyFactory;
			TLSConnectionFactory* tlsConnectionFactory;
	};
}
