obj-m := mytimer_kernel_module.o

all:
	sudo make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	sudo make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
