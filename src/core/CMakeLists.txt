target_sources(${PROJECT_NAME}
        PRIVATE
        application.cpp
        timer.cpp
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include/core/application.h
        ${PROJECT_SOURCE_DIR}/include/core/timer.h
)
