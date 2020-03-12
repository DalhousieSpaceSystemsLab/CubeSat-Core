CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT("emulation")

INCLUDE_DIRECTORIES(
  "AdcsEmulator/header"
  "AttitudeSensor/header"
  "batteries/header"
  "CommsEmulator/header"
  "HardwareEmulationManager/header"
  "power/header"
  "subsystem/header"
  "../../internal-messaging/message/header"
  "../../internal-messaging/message/identifiers"
  "../../internal-messaging/messaging-service/header"
  "../../internal-messaging/messenger/header"
  "../../internal-messaging/unixdomain/header"
)

SET( src-files-emulation
  "main.cc"
  "AdcsEmulator/cc/AdcsEmulator.cc"
  "AttitudeSensor/cc/AttitudeSensor.cc"
  "batteries/cc/Battery.cc"
  "batteries/cc/BatteryManager.cc"
  "CommsEmulator/cc/CommsEmulator.cc"
  "HardwareEmulationManager/cc/HardwareEmulationManager.cc"
  "power/cc/PowerEmulator.cc"
  "power/cc/Relay.cc"
  "subsystem/cc/SubsystemEmulator.cc"
)

ADD_EXECUTABLE(${PROJECT_NAME} ${src-files-emulation})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} "intmsg")
