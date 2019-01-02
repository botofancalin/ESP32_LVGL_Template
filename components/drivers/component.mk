#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)

COMPONENT_SRCDIRS := ./display/ILI9341 \
                    ./input/touch/XPT2046

COMPONENT_ADD_INCLUDEDIRS := . \
                    ./include \
                    ./display/ILI9341/include \
                    ./input/touch/XPT2046/include
