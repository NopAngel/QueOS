CC = i686-elf-gcc
LD = i686-elf-ld
AS = nasm
CFLAGS = -ffreestanding -m32 -g
ASFLAGS = -f elf32 -g

export PREFIX=$(HOME)/Documents/Tools/CrossCompiler
export PATH=$(PREFIX)/bin:/usr/bin:/bin:$$PATH

.PHONY: always all boot kernel link grub2 run clean apps debug

all: always run


always:
	@echo "Building OS..."
	@echo "Creating build directories..."
	mkdir -p $(BUILD_DIR)/obj
	mkdir -p $(BUILD_DIR)/iso/boot/grub

ISO_DIR = $(BUILD_DIR)/iso


#--------------------#
# Create kernel OBJs #
#--------------------#

SRC_DIR = src
BUILD_DIR = build
BUILD_DIR_OBJ = $(BUILD_DIR)/obj

C_SRCS = $(shell find $(SRC_DIR) -type f -name "*.c")
ASM_SRCS = $(shell find $(SRC_DIR) -type f -name "*.asm")

C_OBJS   := $(patsubst %.c, $(BUILD_DIR_OBJ)/%.o, $(subst /,_,$(C_SRCS)))
ASM_OBJS := $(patsubst %.asm, $(BUILD_DIR_OBJ)/%.o, $(subst /,_,$(ASM_SRCS)))

OBJS     := $(C_OBJS) $(ASM_OBJS)

build : always
	@echo "Building object files..."
	
	@$(foreach file, $(C_SRCS), \
		obj=$(BUILD_DIR_OBJ)/$(subst /,_,$(basename $(file)))_c.o; \
		$(CC) $(CFLAGS) -c $(file) -o $$obj; \
	)
	
	@$(foreach file, $(ASM_SRCS), \
		obj=$(BUILD_DIR_OBJ)/$(subst /,_,$(basename $(file)))_asm.o; \
		$(AS) $(ASFLAGS) $(file) -o $$obj; \
	)

	@echo "All object files built successfully."
	@echo ""




#------------------#
#     User Apps    #
#------------------#

APP_SRC_DIR = src_apps
APP_BUILD_DIR = $(BUILD_DIR)/apps

APP_DIRS = $(wildcard $(APP_SRC_DIR)/*)
APP_ELFS = $(patsubst $(APP_SRC_DIR)/%, $(ISO_DIR)/boot/%.elf, $(APP_DIRS))

# Build all apps
apps: $(APP_ELFS)

# Compile each app separately
$(ISO_DIR)/boot/%.elf: $(APP_SRC_DIR)/%/*.c
	mkdir -p $(APP_BUILD_DIR)/$*
	$(CC) $(CFLAGS) -c $< -o $(APP_BUILD_DIR)/$*/app.o
	$(LD) -m elf_i386 -T applinker.ld -o $@ $(APP_BUILD_DIR)/$*/app.o
	@echo "Built user app: $@"





	

#------------------#
#    Link & ISO    #
#------------------#

link: build apps
	$(LD) -m elf_i386 -T linker.ld -o $(ISO_DIR)/boot/kernel.elf $(BUILD_DIR_OBJ)/*.o 


grub2: link
	mkdir -p $(ISO_DIR)/boot/grub
	cp ./grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub2-mkrescue -o $(BUILD_DIR)/queos.iso $(ISO_DIR)




#------------------#
#      Run it      #
#------------------#

run: always link grub2
	qemu-system-i386 -cdrom $(BUILD_DIR)/queos.iso 


debug: always link grub2
	qemu-system-i386 -cdrom $(BUILD_DIR)/queos.iso -s -S


clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR)/boot/kernel.elf