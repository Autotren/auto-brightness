auto-brightness: auto-brightness.c
	gcc auto-brightness.c -o auto-brightness

archive: auto-brightness
	zip -9 auto-brightness.zip auto-brightness
