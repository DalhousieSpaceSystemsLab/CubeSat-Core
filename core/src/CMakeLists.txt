CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT("core")

SET ( EXECUTABLE_OUTPUT_PATH 
  "${PROJECT_SOURCE_DIR}/../../build"
)

INCLUDE_DIRECTORIES(
  "camera/header"
  "repositories/header"
  "../../internal-messaging/message/header"
  "../../internal-messaging/message/identifiers"
  "../../internal-messaging/messaging-service/header"
  "../../internal-messaging/messenger/header"
  "../../internal-messaging/unixdomain/header"
)

SET ( src-files-core
  "main.cc"
  "camera/cc/Camera.cc"
  "camera/cc/CameraManager.cc"
  "repositories/cc/AdcsRepository.cc"
  "repositories/cc/CommsRepository.cc"
  "repositories/cc/GpsRepository.cc"
  "repositories/cc/PayloadRepository.cc"
  "repositories/cc/PowerRepository.cc"
  "repositories/cc/Repository.cc"
)

ADD_EXECUTABLE(${PROJECT_NAME} ${src-files-core})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} "intmsg")
