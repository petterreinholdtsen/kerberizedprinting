DESTDIR =
prefix  = /usr
bindir  = $(prefix)/bin
man1dir = $(prefix)/share/man/man1
cupsbackenddir = $(prefix)/lib/cups/backend/
XSESSIONDIR = /etc/X11/Xsession.d
INSTALL = install -D
INSTALL_DATA = install -m644 -D

CFLAGS  = -W -Wall -g -O2

all: user_kprintd

install:
	# Backend need mode 700 and user root to be executed as root by CUPS
	$(INSTALL) -m700 ksmb $(DESTDIR)$(cupsbackenddir)/ksmb

	$(INSTALL) user_kprintd $(DESTDIR)$(bindir)/user_kprintd
	$(INSTALL_DATA) user_kprintd.1 $(DESTDIR)$(man1dir)/user_kprintd.1

	$(INSTALL) Xsession $(DESTDIR)$(XSESSIONDIR)/60kerberizedprinting
clean:
	$(RM) user_kprintd
