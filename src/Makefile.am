## Makefile.am for kopyright

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = kopyright kopyright_client

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

# the library search path. 
kopyright_LDFLAGS = $(KDE_RPATH) $(all_libraries)

# the libraries to link against.
kopyright_LDADD = -ldvdread $(LIB_KFILE) $(LIB_KDEPRINT)

# which sources should be compiled for kopyright
kopyright_SOURCES = main.cpp kopyright.cpp kopyrightview.cpp 		 pref.cpp \
		kopyrightiface.skel kopyrightviewbase.ui dvdaudio.cpp dvdaudio.h dvdcell.cpp dvdcell.h \
		dvdchapter.cpp dvdchapter.h dvdparsing.cpp dvdparsing.h dvdstream.cpp dvdstream.h \
		dvdsubpicture.cpp dvdsubpicture.h dvdtitle.cpp dvdtitle.h dvdvideo.cpp dvdvideo.h \
		dvdvisualice.cpp dvdvisualice.h qfillstatus.cpp

# these are the headers for your project
noinst_HEADERS = kopyright.h kopyrightview.h pref.h qfillstatus.h

# client stuff
kopyright_client_LDFLAGS = $(KDE_RPATH) $(all_libraries)
kopyright_client_LDADD = -ldvdread $(LIB_KDECORE)
kopyright_client_SOURCES = kopyright_client.cpp

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(EXTRACTRC) `find . -name \*.ui -o -name \*.rc` > rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kopyright.pot

KDE_ICON = AUTO

# this is where the kdelnk file will go 
kdelnkdir   = $(kde_appsdir)/Utilities
kdelnk_DATA = kopyright.desktop

# this is where the XML-GUI resource file goes
rcdir = $(kde_datadir)/kopyright
rc_DATA = kopyrightui.rc
