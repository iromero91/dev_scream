/*
Character device driver:/dev/scream like the /dev/zero but printing only setted bits

*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");

static int device_file_major_number = 0;
static const char device_name[] = "scream";

static dev_t first; // Global variable for the first device number (major and minor)
static const unsigned int first_minor = 1; // The first minor number we want to request (Needs to be between 1 ans 254)
static const unsigned int device_count = 8; // The number of devices we want to support. Make sure that first_minor + device_count <= 254.
static struct cdev c_dev; // Global variable for the character device structure
static struct class *cl; // Global variable for the device class
static const char alphabet[] = "aaAAhH";

static ssize_t device_file_read(
        struct file *file_ptr,
        char __user *user_buffer,
        size_t count,
        loff_t *position) {

    // Allocate Kernel buffer
    char* ptr = (char*) vmalloc(count);

    // Fill it with scream, byte per byte
    // -- Note that byte is the smallest accesible data unit
    int rand = 0;
    for (int i=0; i < count; i++){
        get_random_bytes(&rand, sizeof(int));
        memset(ptr+i, alphabet[rand % sizeof(alphabet)], 1);
    }

    char res = copy_to_user(user_buffer, ptr, count);
    if (res != 0){ return -EFAULT; }

    // Return number of byte read
    return count;
}

static struct file_operations simple_driver_fops = {
    .owner   = THIS_MODULE,
    .read    = device_file_read,
};

int register_device(void) {
    int res = 0;
    // Hi
    printk( KERN_NOTICE "scream: Function register_device is called.\n" );

    // Allocate memory
    // This used to be as follows before auto-createdevice (#8)
    // --  res = register_chrdev( 0, device_name, &simple_driver_fops )
    res = alloc_chrdev_region(&first, first_minor, device_count, device_name);
    if( res != 0 ) {
        printk( KERN_WARNING "scream: Can\'t register character device with error code = %i\n", res );
        return res;
    }
    device_file_major_number = MAJOR(first);
    printk( KERN_NOTICE "scream: Has registered character device with major number = %i and minor numbers %i..%i\n", device_file_major_number, first_minor, first_minor+device_count );

    // Create sysfs information:
    printk( KERN_NOTICE "scream: Creating Device class\n" );
    if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL) {
        printk( KERN_ALERT "scream: Device class creation failed\n" );
        unregister_chrdev_region(first, 1);
        return -1;
    }

    // Create device
    printk( KERN_NOTICE "scream: Creating Device\n" );
    if (device_create(cl, NULL, first, NULL, "scream") == NULL) {
        printk( KERN_ALERT "scream: Device creation failed\n" );
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return -1;
    }

    // Init device
    printk( KERN_NOTICE "scream: Initialising device\n" );
    cdev_init(&c_dev, &simple_driver_fops);

    // Register device
    printk( KERN_ALERT "scream: Adding device\n" );
    if (cdev_add(&c_dev, first, 1) == -1) {
        printk( KERN_ALERT "scream: Device addition failed\n" );
        device_destroy(cl, first);
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return -1;
    }

    // Bye
    printk( KERN_NOTICE "scream: Function register_device is returning\n" );
    return 0;
}

void unregister_device(void) {
    printk( KERN_NOTICE "scream: Function unregister_device is called\n" );

    // Destroy sysfs information:
    printk( KERN_NOTICE "scream: Deleting device\n" );
    cdev_del(&c_dev);
    printk( KERN_NOTICE "scream: Destroying device\n" );
    device_destroy(cl, first);
    printk( KERN_NOTICE "scream: Destroying device class\n" );
    class_destroy(cl);

    if(device_file_major_number != 0) {
        unregister_chrdev_region(first, device_count);
    }

    printk( KERN_NOTICE "scream: Function unregister_device is returning\n" );
}

static int my_init(void) {
    register_device();
    return 0;
}

static void my_exit(void) {
    unregister_device();
    return;
}

// Declare register and unregister command
module_init(my_init);
module_exit(my_exit);
