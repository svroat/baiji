# buscador version
VERSION = 0.1

# include and libs
CURLINC = `pkg-config --cflags libcurl libxml-2.0`
CURLLIB = `pkg-config --libs libcurl libxml-2.0`

# flags
CFLAGS  = --std=c99 -Wall -pedantic $(CURLINC)
LDFLAGS = $(CURLLIB)

BAIJI_CFLAGS  = $(CFLAGS)
BAIJI_LDFLAGS = $(LDFLAGS)

# compiler and linker
#CC = cc
