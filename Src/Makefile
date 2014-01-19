###-----------------------------------------------------------------------------
##  This file is part of
### ---               Timothy Budd's Kamin Interpreters in C++
###-----------------------------------------------------------------------------
### Title: Makefile
###   make
###    Build all interpreters for the default target
###   make TARGET=opt
###    Build optimised
###   make TARGET=debug
###    Build debug
###-----------------------------------------------------------------------------
PROJECT_DIR := ..
include $(PROJECT_DIR)/Make/Makefile.config

###-----------------------------------------------------------------------------
### Set the default targets
###-----------------------------------------------------------------------------
PROGRAMS = basicLisp lisp apl scheme sasl clu smalltalk prolog

###-----------------------------------------------------------------------------
### Source files
###-----------------------------------------------------------------------------
SOURCES= main.C reader.C expression.C list.C function.C environment.C \
    lispPrimitives.C

INCLUDES= environment.h  expression.h  function.h  lisp.h  list.h  reader.h

###-----------------------------------------------------------------------------
### Build rules
###-----------------------------------------------------------------------------
.PHONY: all $(PROGRAMS)
all: $(PROGRAMS:%=$(OBJDIR)/%)

define ruletemp
    $(1): $(OBJDIR)/$(1)
endef

$(foreach prog,$(PROGRAMS),$(eval $(call ruletemp,$(prog))))

include $(PROJECT_DIR)/Make/Makefile.build

###-----------------------------------------------------------------------------
### Miscellaneous commands
###-----------------------------------------------------------------------------

.PHONY: clean distclean
clean distclean:
	$H rm -rf $(OBJDIR)

###-----------------------------------------------------------------------------
