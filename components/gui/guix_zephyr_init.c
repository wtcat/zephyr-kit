#include <stdio.h>
#include <errno.h>

#include <kernel.h>
#include <init.h>

#include "gx_api.h"

static struct guix_driver *guix_driver_list;

UINT __weak guix_main(UINT disp_id, struct guix_driver *drv)
{
    printk("Warnning***: Please define"
        " \"UINT guix_main(UINT disp_id, struct guix_driver *drv)\" "
        "to initialize GUI!\n");
    return GX_FAILURE;
}

#ifdef CONFIG_GUI_SPLIT_BINRES
UINT guix_binres_load(struct guix_driver *drv, INT theme_id, 
    struct GX_THEME_STRUCT **theme, struct GX_STRING_STRUCT ***language)
{
    UINT ret;
    
    /* Load gui resource */
    ret = gx_binres_language_table_load_ext(drv->map_base, language);
    if (ret != GX_SUCCESS) {
        printk("%s load gui language resource failed\n", __func__);
        goto out;
    }
	
    ret = gx_binres_theme_load(drv->map_base, theme_id, theme);
    if (ret != GX_SUCCESS) {
        printk("%s load gui theme resource failed\n", __func__);
        goto out;
    }

out:
    return ret;
}
#endif

int guix_driver_register(struct guix_driver *drv)
{
    struct guix_driver *iter = guix_driver_list;
    
    if (!drv || !drv->setup)
        return -EINVAL;

    while (iter) {
        if (iter->id == drv->id)
            return -EEXIST;
        iter = iter->next;
    }

    drv->next = guix_driver_list;
    guix_driver_list = drv;
    return 0;
}

static int guix_initialize(const struct device *dev __unused)
{
    struct guix_driver *drv = guix_driver_list;
    int ret = -EINVAL;
    
    if (!drv)
        return -EINVAL;

    for (; drv; drv = drv->next) {
    #ifdef CONFIG_GUI_SPLIT_BINRES
        drv->mmap();
    #endif
        ret = (int)guix_main(drv->id, drv);
        if (ret) {
        #ifdef CONFIG_GUI_SPLIT_BINRES
            drv->unmap();
        #endif
            break;
        }
    #ifdef CONFIG_GUI_SPLIT_BINRES
        drv->unmap();
    #endif
    }
    return ret;
}

SYS_INIT(guix_initialize, APPLICATION,
	CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);


