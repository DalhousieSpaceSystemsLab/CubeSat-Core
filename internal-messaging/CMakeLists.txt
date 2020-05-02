CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT("intmsg")

SET (CMAKE_CXX_STANDARD 14)

INCLUDE_DIRECTORIES(
  "${PROJECT_SOURCE_DIR}/message/header"
  "${PROJECT_SOURCE_DIR}/message/identifiers"
  "${PROJECT_SOURCE_DIR}/messaging-service/header"
  "${PROJECT_SOURCE_DIR}/messenger/header"
  "${PROJECT_SOURCE_DIR}/unixdomain/header"
)

SET ( src-files-intmsg
  # Message folder
  "message/cc/CommandMessage.cc"
  "message/cc/DataMessage.cc"
  "message/cc/Key.cc"
  "message/cc/KeyFloatPair.cc"
  "message/cc/KeyIntPair.cc"
  "message/cc/KeyStringPair.cc"
  "message/cc/KeyValuePairContainer.cc"
  "message/cc/KeyValuePairContainerInterface.cc"
  "message/cc/Message.cc"
  "message/cc/MessageBuilder.cc"
  "message/cc/MessageBuilderInterface.cc"

  # Messaging service folder
  "messaging-service/cc/MessageRecievingService.cc"
  "messaging-service/cc/MessageRecipientInterface.cc"
  "messaging-service/cc/MessageSenderInterface.cc"
  "messaging-service/cc/MessageSendingService.cc"
  "messaging-service/cc/PhoneBook.cc"

  # Messenger folder
  "messenger/cc/LorisMessenger.cc"

  # Unix domain folder
  "unixdomain/cc/UnixDomainStreamSocket.cc"
  "unixdomain/cc/UnixDomainStreamSocketClient.cc"
  "unixdomain/cc/UnixDomainStreamSocketServer.cc"
)

ADD_LIBRARY(${PROJECT_NAME} STATIC ${src-files-intmsg})
