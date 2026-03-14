##
## EPITECH PROJECT, 2026
## Makefile
## File description:
## Makefile
##

NAME    =	PocketExplorer

SRC	=	$(shell find src/ -type f -name '*.c')

OBJ	=	$(SRC:.c=.o)

.PHONY	:	all clean fclean re

SHELL	:=	/bin/bash

COUNTER	:=	0

TO_COMPILE := $(shell for f in $(SRC); do \
	obj=$${f%.cpp}.o; \
	if [ ! -f $$obj ] || [ $$f -nt $$obj ]; then \
		echo $$f; \
	fi; \
done)


%.o : %.c
		@$(eval COUNTER=$(shell echo $$(($(COUNTER) + 1))))
		@gcc -c $< -o $@;
		@n=0 ; \
		for x in $(SRC); do \
			if [ -f $$x ]; then \
				n=$$((n+1)); \
			fi; \
		done; \
		bash -c '\
		n='"$$n"'; \
		if (( $(words $(TO_COMPILE)) == 0 )); then \
			CPT=$(COUNTER); \
		else \
			CPT=$$(( n - $(words $(TO_COMPILE)) + $(COUNTER) )); \
		fi; \
		cols=`tput cols`; \
		filled=$$((CPT * cols / n)); \
		text="Compiling sources: [$$CPT/$$n]"; \
		text_len=$${#text}; \
		if (( filled >= text_len )); then \
			inverted="$$text"; \
			normal=""; \
			for (( i=text_len; i<filled; i++ )); do \
				inverted+=" "; \
			done; \
		else \
			inverted="$${text:0:filled}"; \
			normal="$${text:filled}"; \
		fi; \
		echo -ne "\r\033[7m$$inverted\033[0m$$normal";'


all	:	$(NAME)

$(NAME)	:	$(OBJ)
		@echo ""
		@gcc -o $(NAME) $(OBJ) -lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio -lm;
		@echo -e "\033[1;32mCompilation successful\033[0m\033[1m!\033[0m"

clean:
		@n=0 ; \
		for x in $(OBJ); do \
			if [ -f $$x ] ; then \
				let "n+=1" ; \
			fi ; \
		done ; \
		if [ $$n -eq 0 ] ; then \
			echo "Nothing to be cleaned." ; \
			exit 0 ; \
		fi ; \
		echo "Removed $$n files.";
		@rm -f $(OBJ) ;

fclean	:	clean
		@if [ -f $(NAME) ] ; then \
			echo -e "Removed '\033[1m$(NAME)\033[0m' binary." ; \
			rm $(NAME) ; \
		fi ;

re	:	fclean all
