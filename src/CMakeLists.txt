CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT("core")

# Set build directory
SET ( EXECUTABLE_OUTPUT_PATH
  "${PROJECT_SOURCE_DIR}/../build"
)
SET ( LIBRARY_OUTPUT_PATH
  ${EXECUTABLE_OUTPUT_PATH}
)

# Header files
INCLUDE_DIRECTORIES(
  "${PROJECT_SOURCE_DIR}/../include"
  "${PROJECT_SOURCE_DIR}/../include/cmocka"
)

# IPC library
SET("LIB_IPC" "ipc")
SET("SRC_LIB_IPC" 
  "libipc/conn_t/conn_t.c"
  "libipc/client_t/client_t.c"
  "libipc/client_api/client_api.c" 
  "libipc/client_api/client_api.cc"
  "libipc/util/serialize.cc"
)
ADD_LIBRARY(${LIB_IPC} ${SRC_LIB_IPC})
TARGET_COMPILE_OPTIONS(${LIB_IPC} PUBLIC "-g")

# CMocka library
SET("LIB_CMOCKA" "cmocka")
SET("SRC_CMOCKA" "cmocka/cmocka.c")
ADD_LIBRARY(${LIB_CMOCKA} STATIC ${SRC_CMOCKA})

# IPC daemon binary
SET("BIN_IPCD" "ipcd")
SET("SRC_IPCD"
  "ipcd/main.c"
  "ipcd/ipcd.c"
)
ADD_EXECUTABLE(${BIN_IPCD} ${SRC_IPCD})
TARGET_LINK_LIBRARIES(${BIN_IPCD}
  "pthread"
  ${LIB_IPC}
)
TARGET_COMPILE_OPTIONS(${BIN_IPCD} BEFORE PUBLIC "-g")

# IPC daemon old binary
SET("BIN_IPCD_OLD" "ipcd_old")
SET("SRC_IPCD_OLD"
  "ipcd_old/main.c"
  "ipcd_old/client_handler.c"
)
ADD_EXECUTABLE(${BIN_IPCD_OLD} ${SRC_IPCD_OLD})
TARGET_LINK_LIBRARIES(${BIN_IPCD_OLD}
  "pthread"
)
TARGET_COMPILE_OPTIONS(${BIN_IPCD_OLD} BEFORE PUBLIC "-g")

# Client binary
SET("BIN_CLI" "client")
SET("SRC_CLI" "ipc_client/client.c")
ADD_EXECUTABLE(${BIN_CLI} ${SRC_CLI})
TARGET_LINK_LIBRARIES(${BIN_CLI}
  "pthread"
  ${LIB_IPC}
)
TARGET_COMPILE_OPTIONS(${BIN_CLI} BEFORE PUBLIC "-g")

# Client binary (old)
SET("BIN_CLI_OLD" "client_old")
SET("SRC_CLI_OLD" "ipc_client_old/client_old.c")
ADD_EXECUTABLE(${BIN_CLI_OLD} ${SRC_CLI_OLD})
TARGET_LINK_LIBRARIES(${BIN_CLI_OLD}
  "pthread"
  ${LIB_IPC}
)
TARGET_COMPILE_OPTIONS(${BIN_CLI_OLD} BEFORE PUBLIC "-g")

# Client binary (C++)
SET("BIN_CLI_CPP" "client_cc")
SET("SRC_CLI_CPP" "ipc_client/client.cc")
ADD_EXECUTABLE(${BIN_CLI_CPP} ${SRC_CLI_CPP})
TARGET_LINK_LIBRARIES(${BIN_CLI_CPP}
  "pthread"
  ${LIB_IPC}
)
TARGET_COMPILE_OPTIONS(${BIN_CLI_CPP} BEFORE PUBLIC "-g")

# Functional style C - IPCD daemon
SET("F_BIN_IPCD" "f_ipcd")
SET("F_SRC_IPCD" "functional/immut.c")
ADD_EXECUTABLE(${F_BIN_IPCD} ${F_SRC_IPCD})
TARGET_LINK_LIBRARIES(${F_BIN_IPCD}
  "pthread"
  ${LIB_IPC}
)

# Subsystem Server Template 
# SET("BIN_SUBSYS_SERVER" "subsys_server")
# SET("SRC_SUBSYS_SERVER" "subsys_server/server.c")
# ADD_EXECUTABLE(${BIN_SUBSYS_SERVER} ${SRC_SUBSYS_SERVER})
# TARGET_LINK_LIBRARIES(${BIN_SUBSYS_SERVER}
#   "pthread"
#   ${LIB_IPC}
# )

# Testing binary
SET("BIN_TEST" "test")
SET("SRC_TEST" "test.c")
ADD_EXECUTABLE(${BIN_TEST} ${SRC_TEST})
TARGET_LINK_LIBRARIES(${BIN_TEST}
  ${LIB_IPC}
  "check"
 "-Wl,--wrap=read,--wrap=write"
)
