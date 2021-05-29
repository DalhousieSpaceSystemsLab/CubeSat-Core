NAME=run_Arducam_Demo
NAME_ARM=run_Arducam_Demo_arm
CC=gcc
CC_ARM=arm-none-linux-gnueabihf-gcc
CFLAGS=-Wall -pthread 
ODIR=obj
ODIR_ARM=obj_arm
SDIR=src

_DEPS=ArduCAM.h bcm283x_board_driver.h sccb_bus.h ov2640_regs.h ov5640_regs.h ov5642_regs.h
DEPS=$(patsubst %,$(SDIR)/%,$(_DEPS))

_OBJ=ArducamDemo.o ArduCAM.o bcm283x_board_driver.o sccb_bus.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))
OBJ_ARM=$(patsubst %,$(ODIR_ARM)/%,$(_OBJ))

.PHONY: all 
.DEFAULT: all
all: $(NAME) $(NAME_ARM)

$(ODIR)/%.o: src/%.c $(DEPS)
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR_ARM)/%.o: src/%.c $(DEPS)
	@mkdir -p $(@D)
	$(CC_ARM) -c -o $@ $< $(CFLAGS)

$(NAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(NAME_ARM): $(OBJ_ARM)
	$(CC_ARM) -o $@ $^ $(CFLAGS)

.PHONY:	run

run:	$(NAME)
	sudo ./$(NAME)

.PHONY:	clean
clean:	
	rm -r $(NAME) $(ODIR)
	rm -r $(NAME_ARM) $(ODIR_ARM)