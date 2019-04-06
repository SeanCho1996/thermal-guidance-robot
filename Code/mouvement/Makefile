cc = "/sec/buildroot/output/host/usr/bin/arm-linux-gcc"

/targetfs/usr/sbin/test_mouvement:	test_mouvement
	cp	test_mouvement	/targetfs/usr/sbin
test_mouvement:	test_mouvement.c	mouvement.c
	$(cc)	-g -o	test_mouvement test_mouvement.c	mouvement.c 