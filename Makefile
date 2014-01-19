###-----------------------------------------------------------------------------
##  This file is part of
### ---               Timothy Budd's Kamin Interpreters in C++
###-----------------------------------------------------------------------------
### Title: Top-level Makefile
###   make
###    Build for the default target
###   make TARGET=opt
###    Build optimised
###   make TARGET=debug
###    Build debug
###-----------------------------------------------------------------------------
PROJECT_DIR := .
include $(PROJECT_DIR)/Make/Makefile.config

###-----------------------------------------------------------------------------
### Default build
###-----------------------------------------------------------------------------
.PHONY: all
all:
	$V $(MAKE) -C Src

###-----------------------------------------------------------------------------
### Miscellaneous commands
###-----------------------------------------------------------------------------
include $(PROJECT_DIR)/Make/Makefile.build

README.org: index.org
	@sed 's%file:%http://henry.github.com/BuddKaminInterpreters/%' $< > $@

.PHONY: README
README: index.html README.org

.PHONY: TODO
TODO: TODO.html

.PHONY: doc
doc: README TODO
	@$(MAKE) -C Doc

.PHONY: clean distclean
clean distclean:
	$H $(MAKE) -C Src clean
	$H $(MAKE) -C Doc clean
	$H rm -rf platforms

###-----------------------------------------------------------------------------
