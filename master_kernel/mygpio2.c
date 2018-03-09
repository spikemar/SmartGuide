#ifndef __KERNEL__
#define __KERNEL__
#endif

#ifndef MODULE
#define MODULE
#endif

#include <linux/timer.h>
#include <linux/jiffies.h>

//this is from proc_gpio.c
#include <linux/init.h>
//#include <asm/hardware.h>
#include <asm/uaccess.h>
//this is from proc_gpio.c

#define HZ 100

#include <linux/module.h>
#include <linux/interrupt.h>
#include <asm/arch/pxa-regs.h>
#include <asm-arm/arch/hardware.h>
#include <linux/slab.h> /* kmalloc() */
#include <linux/kernel.h> /* printk() */
#include <linux/module.h>
#include <linux/fs.h> /* everything... */
#include <asm/uaccess.h> /* copy_from/to_user */
#include <linux/proc_fs.h>
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/ktime.h>
#include <linux/ktime.h>

#define ECHO 31 //this is gpio input, ECHO
#define TRIG 17 //this is gpio output, TRG
//#define ECHO2 31 //this is gpio input, ECHO
//#define TRIG2 17 //this is gpio output, TRG
//static int mygpio_major = 61;
//static int sensor_fasync(int fd, struct file *filp, int mode);
static int sensor_open(struct inode *inode, struct file *filp);
static int sensor_release(struct inode *inode, struct file *filp);
//static ssize_t sensor_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
//static ssize_t fasync_example_read(struct file *filp, const char *buf, size_t count, loff_t *f_pos);

static unsigned long pulse_duration;
static unsigned long jiffy1;
static unsigned long jiffy2;
static struct timer_list * sensor_timer;
static struct timer_list * trig1_timer;
static struct timeval tstart1, tend1;
static int proc_read =1;

static struct proc_dir_entry *proc_entry;
static int sensor_read( char *page, char **start, off_t off,
                   int count, int *eof, void *data);

struct file_operations sensor_fops = {
  //write: sensor_write,
  //read: sensor,
 open: sensor_open,
 release: sensor_release,
 //fasync: sensor_fasync
};

void do_gettimeofday(struct timeval *tv);
static void timer_handler(unsigned long data) {
  //printk(KERN_ALERT "initially echo is %d\n",pxa_gpio_get_value(ECHO));
	pxa_gpio_set_value(TRIG, 0);

	pxa_gpio_set_value(TRIG, 1);
	jiffy2=jiffies;

	//	printk(KERN_ALERT " echo is %d when triggered\n",pxa_gpio_get_value(ECHO));
	while (jiffies-jiffy2<msecs_to_jiffies(0.01));
	pxa_gpio_set_value(TRIG, 0);
}

static void trig1_timer_handler(unsigned long data) {

	pxa_gpio_set_value(TRIG, 1);
	jiffy1=jiffies;
	//printk(KERN_ALERT "trig1 while before\n");
	printk("");
	while (jiffies-jiffy1<msecs_to_jiffies(0.01));
	//printk(KERN_ALERT "trig1 while after\n");
	printk("");
	pxa_gpio_set_value(TRIG, 0);

}

irqreturn_t gpio_irq_1(int irq, void *dev_id, struct pt_regs *regs)
{

	if(pxa_gpio_get_value(ECHO)!=0){
		do_gettimeofday(&tstart1);
        	return IRQ_HANDLED;
	}
	else{
		pxa_gpio_set_value(TRIG, 0);
		do_gettimeofday(&tend1);
		pulse_duration = 1000000 * (tend1.tv_sec - tstart1.tv_sec) +
			(tend1.tv_usec - tstart1.tv_usec);
		printk("time taken echo: %ld microsec\n",

		1000000 * (tend1.tv_sec - tstart1.tv_sec) +

		(tend1.tv_usec - tstart1.tv_usec) );
		setup_timer(trig1_timer, trig1_timer_handler, 0);
		mod_timer(trig1_timer, jiffies + msecs_to_jiffies(1000));
        	return IRQ_HANDLED;
	}
}

static int my_init_module(void)
{
	int result;
	int irq = IRQ_GPIO(ECHO);
	/* Registering device */


	  proc_entry = create_proc_entry( "sensor2", 0644, NULL );
	  if (proc_entry == NULL) {
	    printk(KERN_INFO "sensor2: Couldn't create proc entry\n");
	    
	  } else {
	    proc_entry->read_proc = sensor_read;
	    //proc_entry->write_proc = mytimer_write;
	    proc_entry->owner = THIS_MODULE;
	    printk(KERN_INFO "sensor2 proc entry created.\n");
	  }
	
	  sensor_timer = (struct timer_list *) kmalloc(sizeof(struct timer_list), GFP_KERNEL);
	  trig1_timer = (struct timer_list *) kmalloc(sizeof(struct timer_list), GFP_KERNEL);

	/* Check if all right */
	if (!sensor_timer)
	  { 
	    printk(KERN_ALERT "Insufficient kernel memory\n"); 
	    result = -ENOMEM;
	    //goto fail;
	  }

	pxa_gpio_mode(TRIG | GPIO_OUT);
	pxa_gpio_mode(ECHO | GPIO_IN);
	irq = IRQ_GPIO(ECHO);
	if (request_irq(irq, &gpio_irq_1, SA_INTERRUPT | SA_TRIGGER_RISING | SA_TRIGGER_FALLING,
				"mygpio", NULL) != 0 ) {
                printk ( "irq not acquired \n" );
                return -1;
        }
	setup_timer(sensor_timer, timer_handler, 0);
	mod_timer(sensor_timer, jiffies + msecs_to_jiffies(1));
    
	return 0;
}

static void my_cleanup_module(void)
{
	/* Freeing the major number */
	/* Freeing buffer memory */
	remove_proc_entry("sensor2", &proc_root);
	free_irq(IRQ_GPIO(ECHO), NULL);

	if (sensor_timer){
	  del_timer(sensor_timer);
	  kfree(sensor_timer);
	}
	if (trig1_timer){
	  del_timer(trig1_timer);
	  kfree(trig1_timer);
	}
	
}

static int sensor_open(struct inode *inode, struct file *filp) {
	return 0;
}

static int sensor_release(struct inode *inode, struct file *filp) {
	return 0;
}

static int sensor_read( char *page, char **start, off_t off,
                   int count, int *eof, void *data )
{
  int len;
  
  if(proc_read==0){
	proc_read=1;
	return 0;
  }
  if (off > 0) {
    *eof = 1;
    return 0;
  }
	if(pulse_duration < 2500)
	  len=sprintf(page,"2");
	else if(pulse_duration < 5000)
	  len=sprintf(page,"1");
	else
	  len=sprintf(page,"0");
  proc_read=0;
  return len;

}
module_init(my_init_module);
module_exit(my_cleanup_module);
