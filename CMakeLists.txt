CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(CACHE_SIM)

SET(CMAKE_CXX_STANDARD 11)

SET(SIM_SOURCES
        src/CacheSim.cpp
        src/Handler.cpp
)

SET(SIM_INCLUDES
        include/Op.h
        include/Utils.h
        include/defines.h
        include/Handler.h
        include/Reader.h
        include/Writer.h
        include/BlockPolicy.h
        include/Mapping/MappingPolicy.h
        include/Mapping/FullConnectMapping.h
        include/Replacement/ReplacementPolicy.h
        include/Replacement/RandomReplace.h
        include/WritemissPolicy.h
)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
FILE(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/log)

ADD_EXECUTABLE(${PROJECT_NAME} ${SIM_SOURCES} ${SIM_INCLUDES})
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE include)
