NAME = kilo-lisp

noengine: $(NAME).c
	cc -o $(NAME) $(NAME).c

jmclisp: $(NAME.c) jmclisp.c
	cc -DJMCLISP -o $(NAME) $(NAME).c jmclisp.c

gauche: $(NAME).c
	cc -DGAUCHE -Wall `gauche-config -I` -o $(NAME) $(NAME).c `gauche-config -l`

guile: $(NAME).c
	cc -DGUILE `pkg-config guile-3.0 --cflags` -o $(NAME) $(NAME).c `pkg-config guile-3.0 --libs`

chibi: $(NAME).c
	cc -DCHIBI -o $(NAME) $(NAME).c -lchibi-scheme -lm

ecl: $(NAME).c
	cc -DECL `ecl-config --cflags` -o $(NAME) $(NAME).c `ecl-config --ldflags` -lecl

clean:
	rm -f $(NAME)

