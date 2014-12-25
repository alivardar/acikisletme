## Process this file with automake to produce Makefile.in

## Created by Anjuta - will be overwritten
## If you don't want it to overwrite it,
## 	Please disable it in the Anjuta project configuration

INCLUDES =\
	 -I/usr/include -I/usr/local/include -I/usr/include/glib-1.2 -I/usr/lib/glib/include

AM_CFLAGS =\
	 -w -Wall\
	 -g

bin_PROGRAMS = acikisletme

acikisletme_SOURCES = \
	main.c

acikisletme_LDFLAGS = \
	 -L/usr/lib -L/usr/local/lib

acikisletme_LDADD = \
	 -lcrypt -lncurses -lpq -lglib -lm -lmenu -lform

