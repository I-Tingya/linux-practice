obj-m := pcd_drv.o
pcd_drv-objs += pcd.o pcd_fops.o

HOST_KERN_DIR = /lib/modules/$(shell uname -r)/build/


clean:
	make -C $(HOST_KERN_DIR) M=$(PWD) clean
	
host:
	make -C $(HOST_KERN_DIR) M=$(PWD) modules
