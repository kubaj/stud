NAME=proj2

all: clean do

do: $(NAME).tex
	latex $(NAME).tex
	bibtex $(NAME) || true
	latex $(NAME).tex
	latex $(NAME).tex
	dvips -t a4 $(NAME).dvi
	ps2pdf $(NAME).ps

pdf: $(NAME).tex
	pdflatex $(NAME).tex
	pdflatex $(NAME).tex

clean:
	rm -f $(NAME).dvi $(NAME).log $(NAME).aux $(NAME).ps $(NAME).pdf

