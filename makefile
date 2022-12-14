# VEXcode makefile 2019_03_26_01

# show compiler output
VERBOSE = 0

# include toolchain options
include vex/mkenv.mk

# location of the project source cpp and c files
SRC_C  = $(wildcard src/*.cpp) 
SRC_C += $(wildcard src/*/*.cpp) 
SRC_C += $(wildcard include/*.cpp)
SRC_C += $(wildcard include/*/*.cpp)
SRC_C += $(wildcard include/*/*/*.cpp)

OBJA = $(addprefix $(BUILD)/, $(addsuffix .o, $(basename $(SRC_C))) )
OBJ = $(addprefix $(BUILD)/, $(addsuffix .o, $(basename $(SRC_C))) )

# location of include files that c and cpp files depend on
SRC_H  = $(wildcard include/*.h)
SRC_H += $(wildcard include/*/*.h)
SRC_H += $(wildcard include/*/*/*.h)
SRC_H += $(wildcard include/*/*/*/*.h)
SRC_H += $(wildcard include/*/*/*/*/*.h)
SRC_H += $(wildcard include/*/*/*/*/*/*.h)
SRC_H += $(wildcard include/*/*/*/*/*/*/*.h)
SRC_H += $(wildcard include/*/*/*/*/*/*/*/*.h)


# additional dependancies
SRC_A  = makefile

# project header file locations
INC_F  = include . 

# headers needed to use library

LV_DST_H = $(addprefix $(BUILD)/include/)

$(BUILD)/include/%: %
	$(Q)$(MKDIR)
	$(Q) $(call copyfile,$^, $@)

vpath %.h include/

# override proj name
PROJECTLIB = libinertial

# build targets
all: $(BUILD)/$(PROJECT).bin $(BUILD)/$(PROJECTLIB).a

# include build rules
include vex/mkrules.mk
