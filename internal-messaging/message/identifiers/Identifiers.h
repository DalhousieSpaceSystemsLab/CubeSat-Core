//
// Created by Spencer Axford on 2019-05-15.
//

#ifndef LORIS_IDENTIFIERS_H
#define LORIS_IDENTIFIERS_H
//List of Identifiers for each subsystem used to indicate senders and receipts in message sending
struct Identifiers {
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

#endif //LORIS_IDENTIFIERS_H