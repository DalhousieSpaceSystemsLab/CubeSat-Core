/*
 * RecipientIdentifiers.h
 *
 *  Created on: Jul 2, 2019
 *      Author: andrew
 */

#ifndef MESSAGE_IDENTIFIERS_RECIPIENTIDENTIFIERS_H_
#define MESSAGE_IDENTIFIERS_RECIPIENTIDENTIFIERS_H_


struct RecipientIdentifiers {
	static constexpr unsigned int power_subsystem = 8578128;
	static constexpr unsigned int comms_subsystem = 2038403;
	static constexpr unsigned int adcs_subsystem = 9188409;
	static constexpr unsigned int payload_subsystem = 4589013;
	static constexpr unsigned int gps_subsystem = 6858902;
	static constexpr unsigned int power_repository = 3025893;
	static constexpr unsigned int comms_repository = 1309382;
	static constexpr unsigned int adcs_repository = 5890745;
	static constexpr unsigned int payload_repository = 4132438;
	static constexpr unsigned int gps_repository = 1097346;
};


#endif /* MESSAGE_IDENTIFIERS_RECIPIENTIDENTIFIERS_H_ */
