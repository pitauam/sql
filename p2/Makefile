#makefile para correr todos los scripts de pruebas automaticamente. usar pruebas
.PHONY: menu pruebas clean

menu:
	$(MAKE) -C Menu
	cp Menu/menu ./menu

pruebas: menu
	@set -e; for script in *.sh; do \
		echo "===== $$script ====="; \
		expect "./$$script"; \
	done

clean:
	$(MAKE) -C Menu clean
	rm -f ./menu