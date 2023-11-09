# Edit following two lines to set component requirements (see docs)
set(COMPONENT_REQUIRES)
set(COMPONENT_PRIV_REQUIRES driver)

list(APPEND COMPONENT_ADD_INCLUDEDIRS ./inc)
list(APPEND COMPONENT_SRCS "./src/fonts.c"
						   "./src/ssd1306.c"
						   "./src/ssd1306_hal.c")

register_component()
