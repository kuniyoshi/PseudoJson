OUTPUTFILE = lib/libpseudojson.a
SRCDIR = src/PseudoJson
CPPFLAGS += -Iinclude

.PHONY: all
all: $(OUTPUTFILE)

$(OUTPUTFILE): $(SRCDIR)/Decoder.o $(SRCDIR)/Value.o $(SRCDIR)/Object.o $(SRCDIR)/Array.o $(SRCDIR)/Data.o
	ar ru $@ $^
	ranlib $@

$(SRCDIR)/Decoder.o: $(SRCDIR)/Decoder.cpp
$(SRCDIR)/Value.o: $(SRCDIR)/Value.cpp
$(SRCDIR)/Object.o: $(SRCDIR)/Object.cpp
$(SRCDIR)/Array.o: $(SRCDIR)/Array.cpp
$(SRCDIR)/Data.o: $(SRCDIR)/Data.cpp
