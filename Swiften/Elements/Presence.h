/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#pragma once

#include "Swiften/Base/Shared.h"
#include "Swiften/Elements/Stanza.h"
#include "Swiften/Elements/Status.h"
#include "Swiften/Elements/StatusShow.h"
#include "Swiften/Elements/Priority.h"

namespace Swift {
	class Presence : public Stanza, public Shared<Presence> {
		public:
			enum Type { Available, Error, Probe, Subscribe, Subscribed, Unavailable, Unsubscribe, Unsubscribed };

			Presence() : type_(Available) /*, showType_(Online)*/ {}
			Presence(const String& status) : type_(Available) {
				setStatus(status);
			}

			Type getType() const { return type_; }
			void setType(Type type) { type_ = type; }

			StatusShow::Type getShow() const { 
				boost::shared_ptr<StatusShow> show(getPayload<StatusShow>());
				if (show) {
					return show->getType();
				}
				return type_ == Available ? StatusShow::Online : StatusShow::None;
			}

			void setShow(const StatusShow::Type &show) { 
				updatePayload(boost::shared_ptr<StatusShow>(new StatusShow(show)));
			}

			String getStatus() const { 
				boost::shared_ptr<Status> status(getPayload<Status>());
				if (status) {
					return status->getText();
				}
				return "";
			}

			void setStatus(const String& status) { 
				updatePayload(boost::shared_ptr<Status>(new Status(status)));
			}

			int getPriority() const {
				boost::shared_ptr<Priority> priority(getPayload<Priority>());
				return (priority ? priority->getPriority() : 0);
			}

			void setPriority(int priority) {
				updatePayload(boost::shared_ptr<Priority>(new Priority(priority)));
			}

			boost::shared_ptr<Presence> clone() const {
				return boost::shared_ptr<Presence>(new Presence(*this));
			}

			bool isAvailable() const {
				return type_ == Available;
			}

		private:
			Presence::Type type_;
	};
}
