all: menu worstwall primechecker

menu: menu.c
	gcc -o menu menu.c

worstwall: worstwall.c
	gcc -o worstwall worstwall.c

primechecker: primechecker.c
	gcc -o primechecker primechecker.c

run1: menu
	./menu

run2: worstwall
	./worstwall

run3: primechecker
	./primechecker

.PHONY: clean

clean:
	rm menu
	rm worstwall
	rm primechecker
