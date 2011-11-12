/*
 * Copyright (c) 2010-2011 Thilo Cestonaro
 * Licensed under the simplified BSD license.
 * See Documentation/Licenses/BSD-simplified.txt for more information.
 */

#pragma once

#include <Swiften/Network/ConnectionFactory.h>
#include <Swiften/Network/HostAddressPort.h>
#include <Swiften/Base/SafeString.h>

namespace Swift {
	class HTTPConnectProxiedConnectionFactory : public ConnectionFactory {
		public:
			HTTPConnectProxiedConnectionFactory(ConnectionFactory* connectionFactory, const HostAddressPort& proxy);
			HTTPConnectProxiedConnectionFactory(ConnectionFactory* connectionFactory, const HostAddressPort& proxy, const SafeString& authID, const SafeString& authPassword);

			virtual boost::shared_ptr<Connection> createConnection();

		private:
			ConnectionFactory* connectionFactory_;
			HostAddressPort proxy_;
			SafeString authID_;
			SafeString authPassword_;
	};
}
