

-include depend.mk



export CPP             := g++
export CPPFLAGS        := -std=c++11 -Wextra 
export INCLUDE_DIR     := include 
export AR              := ar 
export RANLIB          := ranlib
export MAKE            := make
export CP              := cp -rf
export RM              := rm -rf
export MKDIR           := mkdir -p
export INSTALL_LIB_DIR := /usr/local/lib
export INSTALL_HDR_DIR := /usr/local/include


.SUFFIXES: .out .c .cc .o .h 
.cc.o: 
	$(CPP) $(CPPFLAGS) -c $< -o $@  -I$(INCLUDE_DIR)




IO_HDR = \
	stream.h \
	util.h

IO_SRC = \
	src/io/stream.cc \
	src/io/util.cc

TYPES_SRC    = \
	src/data_container.cc

ARCH_SRC = \
	src/arch/arch.cc

CORE_SRC = \
	src/core/address.cc \
	src/core/header.cc \
	src/core/packet.cc \
	src/core/protocol/ethernet.cc \
	src/core/protocol/ip.cc
	
SRC = $(IO_SRC) $(TYPES_SRC) $(ARCH_SRC) $(CORE_SRC) 
OBJ = $(SRC:.cc=.o)




all: libpgen2.a

libpgen2.a: $(OBJ)
	@rm -f $@
	$(AR) rc $@ $(OBJ)
	$(RANLIB) $@



.PHONY: clean
clean:
	$(RM) libpgen2.a a.out
	$(RM) $(OBJ) 


.PHONY: depend
depend:
	@echo "Create Dependfile"
	$(CPP) -MM -MG $(SRC) $(CPPFLAGS) -I$(INCLUDE_DIR) > depend.mk

	


.PHONY: install
install:
	$(CP) libpgen2.a      $(INSTALL_LIB_DIR)
	$(CP) include/pgen2.h $(INSTALL_HDR_DIR)
	$(CP) include/pgen2   $(INSTALL_HDR_DIR)

.PHONY: uninstall
uninstall:
	$(RM) $(INSTALL_LIB_DIR)/libpgen2.a
	$(RM) $(INSTALL_HDR_DIR)/pgen2.h
	$(RM) $(INSTALL_HDR_DIR)/pgen2 



main:
	g++ -std=c++11 main.cc -lpgen2 -Iinclude -L./ -lpgen2
	./a.out

