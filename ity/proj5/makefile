NAME=proj5

all: clean pdf

do: $(NAME).tex
	latex $(NAME).tex
	bibtex $(NAME) || true
	latex $(NAME).tex
	latex $(NAME).tex
	dvips $(NAME).dvi
	ps2pdf $(NAME).ps

pdf: $(NAME).tex
	xelatex $(NAME).tex
	xelatex $(NAME).tex

clean:
	rm -f $(NAME).dvi $(NAME).log $(NAME).aux $(NAME).bbl $(NAME).blg $(NAME).ps $(NAME).pdf *-eps-converted-to.pdf

