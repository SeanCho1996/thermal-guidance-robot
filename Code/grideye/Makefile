CC = "/sec/buildroot/output/host/usr/bin/arm-linux-gcc"

/targetfs/usr/sbin/i2c:	i2c
			cp i2c /targetfs/usr/sbin

i2c:	i2c.c
	$(CC) -g -o i2c i2c.c