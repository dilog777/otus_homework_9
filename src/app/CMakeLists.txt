project(async_cli VERSION ${PROJECT_VERSION})

file(GLOB_RECURSE SRC *.h *.hpp *.cpp)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} PRIVATE
	async
)
