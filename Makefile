CC = gcc
CFLAGS = -Wall -g
LIBS =

SRC = main.c student.c restaurant.c delivery.c admin.c order.c utils.c
OBJ = $(SRC:.c=.o)
TARGET = cfds

%:
	@$(CC) $(CFLAGS) $@.c -o $@ $(addprefix -l,$(LIBS))
	@./$@

all: $(TARGET)

$(TARGET): $(OBJ)
	@$(CC) $(CFLAGS) -o $@ $(OBJ) $(addprefix -l,$(LIBS))

%.o: %.c
	@$(CC) $(CFLAGS) -c $<

run: $(TARGET)
	@echo "Running $(TARGET)..."
	@./$(TARGET)

clean:
	@rm -f $(OBJ) $(TARGET)
