# Имя вашего модуля
MODULE_NAME = list_kmod

# skip module verification
#CONFIG_MODULE_SIG=n

SRC_DIR = ./src


KERNEL_CC = /usr/bin/gcc

BUILD = /lib/modules/$(shell uname -r)/build
KSRC = /usr/src/linux-headers-$(shell uname -r)

EXTRA_CFLAGS += -I$(SRC_DIR)

KERNEL_MODFLAGS = -DMODULE -D__KERNEL__

SRCS = $(SRC_DIR)/$(MODULE_NAME).c

obj-m += $(MODULE_NAME).o
$(MODULE_NAME)-y := $(SRCS:.c=.o)

all:
	$(MAKE) -C $(BUILD) M=$(PWD) modules

genkey:
	openssl req -new -x509 -newkey rsa:2048 -keyout MOK.priv -outform DER -out MOK.der -days 36500 -subj "/CN=key sign" -nodes

importkey:
	sudo keyctl padd asymmetric "my desc" @u <MOK.der

sign: all genkey
	$(KSRC)/scripts/sign-file sha256 ./MOK.priv ./MOK.der $(MODULE_NAME).ko

insmod: all
	-sudo rmmod $(MODULE_NAME)
	sudo insmod ./$(MODULE_NAME).ko

clean:
	$(MAKE) -C $(BUILD) M=$(PWD) clean
