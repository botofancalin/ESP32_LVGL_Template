#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := lvgl_template

# Add new components (source folders)
# Must be before include $(IDF_PATH)/make/project.mk
# $(PROJECT_PATH)/xxx didn't work -> use $(abspath xxx) instead
EXTRA_COMPONENT_DIRS += $(abspath components/lvgl_gui)	\
						$(abspath components/drivers)	\
						$(abspath components/general)

include $(IDF_PATH)/make/project.mk