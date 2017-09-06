.POSIX:

include config.mk

SRC = baiji.c
OBJ = $(SRC:.c=.o)

all: options baiji

options:
	@echo baiji build options:
	@echo "CFLAGS  = $(BAIJI_CFLAGS)"
	@echo "LDFLAGS = $(BAIJI_LDFLAGS)"
	@echo "CC = $(CC)"

.c.o:
	@echo CC -c $<
	@$(CC) $(BAIJI_CFLAGS) -c $<

$(OBJ): config.mk

baiji: $(OBJ)
	@echo CC -o $@
	@$(CC) $(BAIJI_CFLAGS) -o $@ $(OBJ) $(BAIJI_LDFLAGS)

test:
	@echo Testing...
	@baiji "Travis CI"

clean:
	@echo cleaning
	@rm -f baiji $(OBJ)

.PHONY: all options test clean
