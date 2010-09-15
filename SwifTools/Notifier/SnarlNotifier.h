/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#pragma once

#include <map>

#include "SwifTools/Notifier/Notifier.h"
#include "SnarlInterface.h"

namespace Swift {
	class Win32NotifierWindow;

	class SnarlNotifier : public Notifier {
		public:
			SnarlNotifier(const String& name, Win32NotifierWindow* window);
			~SnarlNotifier();

			virtual void showMessage(Type type, const String& subject, const String& description, const boost::filesystem::path& picture, boost::function<void()> callback);
		
		private:
			void handleMessageReceived(MSG* message);

		private:
			static const int timeout = 3;
			Snarl::SnarlInterface snarl;
			Win32NotifierWindow* window;
			typedef std::map<int, boost::function<void()> > NotificationsMap;
			NotificationsMap notifications;
	};
}
