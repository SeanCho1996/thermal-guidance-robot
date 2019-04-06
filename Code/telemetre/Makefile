CC = "/sec/buildroot/output/host/usr/bin/arm-linux-gcc"

/targetfs/usr/sbin/telemetre:	telemetre
			cp telemetre /targetfs/usr/sbin

telemetre:	telemetre.c
	$(CC) -g -o telemetre telemetre.c