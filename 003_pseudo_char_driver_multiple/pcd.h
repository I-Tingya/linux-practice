#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/kdev_t.h>
#include<linux/uaccess.h>


#undef pr_fmt
#define pr_fmt(fmt) "%s : " fmt,__func__

#define DEV_MEM_SIZE_DEV1 512
#define DEV_MEM_SIZE_DEV2 128
#define TOTAL_DEVICES 2

/* pseudo device's memory */
char device_buffer_dev1[DEV_MEM_SIZE_DEV1];
char device_buffer_dev2[DEV_MEM_SIZE_DEV2];


struct pcd_private_data { unsigned size; char *buffer; const char *serial_number; struct cdev cdev;};
struct pcd_private_data pcd_dev[2] = {
									  {.size=DEV_MEM_SIZE_DEV1, .buffer= device_buffer_dev1, .serial_number="PCDEV111"},
									  {.size=DEV_MEM_SIZE_DEV2, .buffer= device_buffer_dev2, .serial_number="PCDEV222"}
									 };

/* This holds the device number */
dev_t device_number;

/* Cdev variable */
//struct cdev pcd_cdev;

/*holds the class pointer */
struct class *class_pcd;

struct device *device_pcd;
