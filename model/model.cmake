include(${CMAKE_CURRENT_LIST_DIR}/pairs/pairs.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/readers/readers.cmake)

set(MODEL_FILES
        ${CMAKE_CURRENT_LIST_DIR}/TradingPairs.h
        ${CMAKE_CURRENT_LIST_DIR}/TradingPairs.cpp
        ${CMAKE_CURRENT_LIST_DIR}/TemplateManager.h
        ${CMAKE_CURRENT_LIST_DIR}/TemplateManager.cpp
        ${PAIRS_FILES}
        ${READERS_FILES}
    )
