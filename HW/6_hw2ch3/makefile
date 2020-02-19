all: menu worstwall

menu: menu.c
	gcc -o menu menu.c

worstwall: worstwall.c
	gcc -o worstwall worstwall.c

run1: menu
	./menu

run2: worstwall
	./worstwall

.PHONY: clean

clean:
	rm menu
	rm worstwall
