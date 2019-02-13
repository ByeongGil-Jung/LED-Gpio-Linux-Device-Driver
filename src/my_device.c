/*
 * Created by IntelliJ IDEA.
 * Project: LED-Gpio-Linux-Device-Driver
 * ===========================================
 * User: ByeongGil Jung
 * Date: 2019-02-11
 * Time: 오후 5:15
 */

/*
< GPIO input / ouput 설정 >
GPIO 입력설정 : gpio_direction_input(gpio_num);
GPIO 입력 : gpio_get_value(gpio_num);
GPIO 출력설정 : gpio_direction_ouput(gpio_num,  init_val); (nit_val 는 초기값)
GPIO 출력 : gpio_set_value(gpio_num, val); (val 는 0, 1)
*/

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/slab.h>

#include <linux/gpio.h>

#define LED_ADDRESS 171  // gpio number
#define BUFF_SIZE 1024
#define MAJOR_NUMBER 100

static const char device_name[] = "my_device";
static int *buffer = NULL;

static int my_open(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "[MyDevice] my_device open function called\n");

    return 0;
}

static int my_close(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "[MyDevice] my_device release function called\n");

    return 0;
}

static ssize_t my_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
    printk(KERN_ALERT "[MyDevice] my_device write function called\n");

    // Change mode to 'output mode'
    gpio_direction_output(LED_ADDRESS, 0);

    switch (buf[0]) {
        case '0':
            gpio_set_value(LED_ADDRESS, 0);
            break;
        case '1':
            gpio_set_value(LED_ADDRESS, 1);
            break;
        default:
            printk(KERN_ALERT "[MyDevice] It's wrong ops\n");
            break;
    }

    return count;
}

static ssize_t my_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
    printk(KERN_ALERT "[MyDevice] my_device read function called\n");
    int led_value, result;

    // Change mode to 'input mode'
    gpio_direction_input(LED_ADDRESS);

    led_value = gpio_get_value(LED_ADDRESS);
    printk(KERN_ALERT "[MyDevice] LED value : %d", led_value);
    buffer[0] = led_value;

    result = copy_to_user(buf, buffer, BUFF_SIZE);
    if (result < 0)
        return result;  // return error

    return count;
}

static struct file_operations my_ops = {
    .read = my_read,
    .write = my_write,
    .open = my_open,
    .release = my_close
};

int __init my_device_init(void)
{
    int result = 0;

    printk(KERN_ALERT "[MyDevice] my_device init function is called\n");

    // Register my_deivce (character device)
    result = register_chrdev(MAJOR_NUMBER, device_name, &my_ops);
    if (result < 0) {
        printk(KERN_ALERT "[MyDevice] my_device init failed\n");
        return result;
    }

    // Request GPIO_ADDRESS
    // (해당 gpio 를 사용하겠다는 의미. -> 나중에 gpio_free(num) 으로 풀어주어야 함)
    if (gpio_request(LED_ADDRESS, "my_device_led") != 0)
        printk(KERN_ALERT "[MyDevice] gpio_request LED error\n");

    // Set buffer memory
    buffer = (int*) kmalloc(BUFF_SIZE, GFP_KERNEL); // ??
    if (buffer != NULL)
        memset(buffer, 0, BUFF_SIZE);

    printk(KERN_ALERT "[MyDevice] my_device init successful with result : %d", result);
    return 0;
}

void __exit my_device_exit(void)
{
    printk(KERN_ALERT "[MyDevice] my_device exit function is called\n");

    gpio_free(LED_ADDRESS);

    unregister_chrdev(MAJOR_NUMBER, device_name);
    printk(KERN_ALERT "[MyDevice] my_device cleanup successful\n");

    kfree(buffer);
}

module_init(my_device_init);
module_exit(my_device_exit);
MODULE_LICENSE("GPL");
