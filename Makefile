auto-brightness: auto-brightness.c
	gcc auto-brightness.c -o auto-brightness

archive: auto-brightness
	zip -9 auto-brightnes.zip auto-brightness
