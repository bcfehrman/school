#  This is the Makefile for the SDSMT Thesis document.
#  Requirements:  Linux/Unix system with TeX installed.
#  Author:  Anthony Gervasi
#
#  Usage:  
#       to build - type 'make' on the command line
#       to clean - type 'make clean' on the command line
#
#  Note that this uses pdflatex.  If you need to include eps figures, 
#    you will have to use latex (with epsf) instead of pdflatex.

# The file to be built

all: thesis.pdf

thesis.pdf : $(SRC) bib123
	pdflatex -shell-escape thesis
	pdflatex -shell-escape thesis

bib123: thesis.bib
	touch bib123
	bibtex thesis

clean:
	rm -f *~
	rm -f bib123 thesis.aux thesis.bbl thesis.blg thesis.lof thesis.log thesis.lot thesis.pdf thesis.toc

.PHONY:  thesis.pdf

