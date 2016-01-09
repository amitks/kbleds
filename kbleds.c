#include <linux/module.h>
#include <linux/configfs.h>
#include <linux/init.h>
#include <linux/tty.h>          /* For fg_console, MAX_NR_CONSOLES */
#include <linux/kd.h>           /* For KDSETLED */
#include <linux/vt.h>
#include <linux/console_struct.h>       /* For vc_cons */
#include <linux/vt_kern.h>

MODULE_DESCRIPTION("SET Keyboard LED ON/OFF");
MODULE_LICENSE("GPL");

#define ALL_LEDS_ON   0x07
#define RESTORE_LEDS  0xFF

#ifndef CLOCK_TICK_RATE
#define CLOCK_TICK_RATE 1193180
#endif

#define DEFAULT_FREQ       440.0 /* Middle A */


struct tty_driver *my_driver;

static int __init kbleds_init(void)
{
	printk(KERN_INFO "kbleds: loading\n");	
	my_driver = vc_cons[fg_console].d->port.tty->driver;
        (my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED,
                            ALL_LEDS_ON);
        (my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KIOCSOUND,
                            CLOCK_TICK_RATE/DEFAULT_FREQ);
	return 0;
}
static void __exit kbleds_cleanup(void)
{
        printk(KERN_INFO "kbleds: unloading...\n");
        (my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED,
                            RESTORE_LEDS);
        (my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KIOCSOUND,
                            0x00);
}
module_init(kbleds_init);
module_exit(kbleds_cleanup);
