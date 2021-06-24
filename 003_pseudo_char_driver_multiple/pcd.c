#include "pcd.h"
#include "pcd_fops.h"

/* file operations of the driver */
struct file_operations pcd_fops=
{
	.open = pcd_open,
	.release = pcd_release,
	.read = pcd_read,
	.write = pcd_write,
	.owner = THIS_MODULE
};


static int __init pcd_driver_init(void)
{
	int ret,i;

	/*1. Dynamically allocate a device number */
	ret = alloc_chrdev_region(&device_number,0,TOTAL_DEVICES,"pcd_devices");
	if(ret < 0){
		pr_err("Alloc chrdev failed\n");
		goto out;
	}
	
	/*4. create device class under /sys/class/ */
		class_pcd = class_create(THIS_MODULE,"pcd_class");
		if(IS_ERR(class_pcd)){
			pr_err("Class creation failed\n");
			ret = PTR_ERR(class_pcd);
			goto unreg_chrdev;
		}


	for(i=0;i<TOTAL_DEVICES;i++)
	{
		pr_info("Device number <major>:<minor> = %d:%d\n",MAJOR(device_number+i),MINOR(device_number+i));

		/*2. Initialize the cdev structure with fops*/
		cdev_init(&pcd_dev[i].cdev,&pcd_fops);

		/* 3. Register a device (cdev structure) with VFS */
		pcd_dev[i].cdev.owner = THIS_MODULE;
		ret = cdev_add(&pcd_dev[i].cdev,device_number+i,1);
		if(ret < 0){
			pr_err("Cdev add failed\n");
			goto class_del;
		}

		

		/*5.  populate the sysfs with device information */
		device_pcd = device_create(class_pcd,NULL,device_number+i,NULL,"pcdev-%d",i+1);
		if(IS_ERR(device_pcd)){
			pr_err("Device create failed\n");
			ret = PTR_ERR(device_pcd);
			goto cdev_del;
		}
	}
	pr_info("Module init was successful\n");

	return 0;


cdev_del:
	cdev_del(&pcd_dev[i].cdev);
class_del:
	class_destroy(class_pcd);
unreg_chrdev:
	unregister_chrdev_region(device_number+i,1);
out:
	pr_info("Module insertion failed\n");
	return ret;
}



static void __exit pcd_driver_cleanup(void)
{	int i;
	for(i=0;i<TOTAL_DEVICES;i++)
	{
		device_destroy(class_pcd,device_number+i);
		cdev_del(&pcd_dev[i].cdev);
	}
	class_destroy(class_pcd);
	
	unregister_chrdev_region(device_number,TOTAL_DEVICES);
	pr_info("module unloaded\n");
}


module_init(pcd_driver_init);
module_exit(pcd_driver_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kiran Nayak");
MODULE_DESCRIPTION("A pseudo character driver");
