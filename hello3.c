// SPDX-License-Identifier: Dual BSD/GPL
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/bug.h>

MODULE_AUTHOR("Pyrohov Pavlo <pyrohovpavlo05@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static uint hello_count = 1;
module_param(hello_count, uint, 0444);
MODULE_PARM_DESC(hello_count, "Number of times to print Hello, world!");

struct hello_entry {
	struct list_head list;
	ktime_t time;
};

static LIST_HEAD(hello_list);

static int __init hello_init(void)
{
	int i;

	if (hello_count == 0 || (hello_count >= 5 && hello_count <= 10))
		printk(KERN_WARNING "Warning: Invalid count (%u), is 0 or between 5 and 10\n",
		hello_count);

	BUG_ON(hello_count > 10);

	for (i = 0; i < hello_count; i++) {
		struct hello_entry *entry;

		// Примусова помилка для 5-го елементу
		if (i == 4)
			entry = NULL;
		else
			entry = kmalloc(sizeof(*entry), GFP_KERNEL);

		if (!entry)
			BUG_ON(1);

		entry->time = ktime_get();
		list_add_tail(&entry->list, &hello_list);
		printk(KERN_EMERG "Hello, world!\n");
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct hello_entry *entry, *tmp;

	list_for_each_entry_safe(entry, tmp, &hello_list, list) {
		printk(KERN_INFO "Time: %lld ns\n", ktime_to_ns(entry->time));
		list_del(&entry->list);
		kfree(entry);
	}
}

module_init(hello_init);
module_exit(hello_exit);
