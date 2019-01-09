set(COMPONENT_ADD_INCLUDEDIRS "include")

# Edit following two lines to set component requirements (see docs)
set(COMPONENT_REQUIRES driver)
set(COMPONENT_PRIV_REQUIRES freertos)

set(COMPONENT_SRCS ./param.c)

register_component()
