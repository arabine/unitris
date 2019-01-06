# *******************************************************************************
# Main makefile project
# This makefile calls all the modules defined in the config.mk file
# *******************************************************************************

# *******************************************************************************
# DEFAULT DEFINITIONS
# These definitions can be overloaded from the command line
# *******************************************************************************
PROJECT		?= example
TARGET 		?= release
BOARD 		?= host
OPTIM 		?= high
ENABLE_DEP 	?= true
ARCH		?= host-none-gcc



TOPDIR		:= ./
OUTDIR		:= $(TOPDIR)build/output/


# Export them to be sure that they are available in sub-makefiles
export PROJECT
export TARGET
export BOARD
export DEFINES
export OPTIM
export ENABLE_DEP
export ARCH
export TOPDIR
export OUTDIR

# *******************************************************************************
# APPLICATION DEFINITIONS
# List of modules and extra libraries needed to generate project targets
# *******************************************************************************
LIB_UNITRIS			:= components/unitris
LIB_SYS_PRINTF			:= sys_printf

export LIB_TERIS
export LIB_SYS_PRINTF


# *******************************************************************************
# TETRIS COMMAND LINE TARGET
# *******************************************************************************
ifeq ($(MAKECMDGOALS), unitris_cli)

APP_MODULES 	:= projects/unitris_cli $(LIB_UNITRIS) $(LIB_SYS_PRINTF)
APP_LIBPATH 	:= 
APP_LIBS 	:= 

endif


# *******************************************************************************
# BUILD ENGINE
# *******************************************************************************
include build/Main.mk

unitris_cli: $(OBJECTS)
	$(call linker, $(OBJECTS), $(APP_LIBS), unitris_cli)

	
clean:
	@echo "Cleaning generated files..."
	$(VERBOSE) $(RM) -rf $(OUTDIR)/*.o $(OUTDIR)/*.d $(OUTDIR)/*.gcov $(OUTDIR)/*.gcov.htm


# *******************************************************************************
# END OF MAKEFILE
# *******************************************************************************
