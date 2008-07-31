DESTDIR =
prefix  = /usr
bindir  = $(prefix)/sbin
man1dir = $(prefix)/share/man/man1
cupsbackenddir = $(prefix)/lib/cups/backend/
XSESSIONDIR = /etc/X11/Xsession.d
INSTALL = install -m755 -D
INSTALL_DATA = install -m644 -D

CFLAGS  = -W -Wall -g -O2

all: user_kprintd

install:
	$(INSTALL) ksmb $(DESTDIR)$(cupsbackenddir)/ksmb
	$(INSTALL) user_kprintd $(DESTDIR)$(bindir)/user_kprintd
	$(INSTALL_DATA) user_kprintd.1 $(DESTDIR)$(man1dir)/user_kprintd.1

	$(INSTALL) Xsession $(DESTDIR)$(XSESSIONDIR)/60kerberizedprinting
clean:
	$(RM) user_kprintd
