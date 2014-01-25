###-----------------------------------------------------------------------------
##  This file is part of
### ---               Timothy Budd's Kamin Interpreters in C++
###-----------------------------------------------------------------------------
### Title: Makefile for LaTeX documentation
###  Description:
##    Builds both the PDF and HTML versions using pdfLaTeX and TeX4HT
##    respectively
###-----------------------------------------------------------------------------
DOCUMENT = BuddKaminInterpreters

###-----------------------------------------------------------------------------
### Source files
###-----------------------------------------------------------------------------
TEX_FILES = \
    chap1.tex chap2.tex chap3.tex chap4.tex chap5.tex chap6.tex \
    chap7.tex chap8.tex future.tex listingLines.tex

SOURCE_FILES = $(DOCUMENT).tex $(TEX_FILES) Makefile

C_FILES = \
    environment.h  expression.h  function.h  lisp.h  list.h  reader.h \
    basicLisp.C lisp.C apl.C scheme.C sasl.C clu.C smalltalk.C prolog.C \
    environment.C expression.C function.C lispPrimitives.C list.C main.C reader.C

HTML_DIR = html
HTML1_DIR = html1

###-----------------------------------------------------------------------------
### Generate the PDF file
###-----------------------------------------------------------------------------
.PHONY: all
all: pdf html

.PHONY: all1
all1: pdf html html1

.PHONY: pdf
pdf: $(DOCUMENT).pdf
$(DOCUMENT).pdf: $(SOURCE_FILES)
	$(MAKE) clean
	pdflatex $(DOCUMENT)
	pdflatex $(DOCUMENT)

.PHONY: html
html: $(HTML_DIR)/$(DOCUMENT).html
$(HTML_DIR)/$(DOCUMENT).html: $(SOURCE_FILES) $(4HT_FILES)
	@mkdir -p $(HTML_DIR)
	xhlatex $(DOCUMENT) "listing,html,1,next,noannex" "" "-d./$(HTML_DIR)/"
	xhlatex $(DOCUMENT) "listing,html,1,next,noannex" "" "-d./$(HTML_DIR)/"

.PHONY: html1
html1: $(HTML1_DIR)/$(DOCUMENT).html
$(HTML1_DIR)/$(DOCUMENT).html: $(SOURCE_FILES) $(4HT_FILES)
	@mkdir -p $(HTML1_DIR)
	xhlatex $(DOCUMENT) "listing,html" "" "-d./$(HTML1_DIR)/"
	xhlatex $(DOCUMENT) "listing,html" "" "-d./$(HTML1_DIR)/"

###-----------------------------------------------------------------------------
### Scan for first and last line numbers of the source listings
###-----------------------------------------------------------------------------
listingLines.tex: ${C_FILES:%=../Src/%}
	@rm -f listingLines.tex
	@touch listingLines.tex
	@$(foreach cfile, $(C_FILES), \
		echo "Scanning " $(cfile); \
		cat ../Src/$(cfile) --number | grep "/// " | \
		egrep -v -e "/// ---|Title|Description|Forward|Member" | \
		awk '{printf "\\newcommand*{\\%sfirstline}{%d}\n", $$3, $$1+1}' \
			>> listingLines.tex;\
		cat ../Src/$(cfile) --number | grep "///- " | \
		awk '{printf "\\newcommand*{\\%slastline}{%d}\n", $$3, $$1-1}' \
			>> listingLines.tex;)

###-----------------------------------------------------------------------------
.PHONY: clean
clean:
	@rm -f *.{aux,bbl,blg,dvi,idx,ilg,ind,lof,log,lot,lol,toc,4ct,4tc,idv,in}
	@rm -f *.{lg,tmp,out,xref,html,css,fls,cut}
	@rm -f *.{png,4dx,4ix}

.PHONY: distclean
distclean: clean
	@rm -f $(DOCUMENT).pdf
	@rm -rf $(HTML_DIR) $(HTML1_DIR)

###-----------------------------------------------------------------------------
