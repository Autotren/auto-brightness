auto-brightness: auto-brightness.c
	clang auto-brightness.c -o auto-brightness

archive: auto-brightness
	zip -9 auto-brightness.zip auto-brightness
