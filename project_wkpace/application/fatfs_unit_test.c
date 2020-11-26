#include <zephyr.h>
#include <device.h>
#include <disk/disk_access.h>
#include <logging/log.h>
#include <fs/fs.h>
#include <ff.h>

#include <string.h>

LOG_MODULE_REGISTER(fstest);


static int fat_fs_mount(const char *volume, const char *mp)
{
    static FATFS fat_fs;
    static struct fs_mount_t mount_info = {
    	.type = FS_FATFS,
    	.fs_data = &fat_fs,
    };
    int err;

    mount_info.mnt_point = mp;
    err = disk_access_init(volume);
    if (err) {
        LOG_ERR("%s is not exist\n", volume);
        return err;
    }

    err = fs_mount(&mount_info);
    if (err != FR_OK)
        LOG_ERR("Mounting %s to %s failed with error: %d\n", volume, mp, err);

    return err;
}

static void fs_test_thread(void)
{
    char text[] = {"Hello, Zephyr!"};
    char buffer[sizeof(text) + 1];
    struct fs_file_t fd;
    int ret;

    ret = fat_fs_mount(CONFIG_DISK_FLASH_VOLUME_NAME, "/home");
    if (ret)
        goto _exit;
    
    ret = fs_open(&fd, "/home/hello.txt", FS_O_CREATE|FS_O_WRITE);
    if (ret)
        goto _exit;

    ret = fs_write(&fd, text, sizeof(text));
    if (ret)
        goto _close;

    ret = fs_close(&fd);
    if (ret)
        goto _exit;

    k_sleep(K_MSEC(2000));

    ret = fs_open(&fd, "/home/hello.txt", FS_O_READ);
    if (ret)
        goto _exit;

    memset(buffer, 0, sizeof(buffer));
    ret = fs_read(&fd, buffer, sizeof(text));
    if (ret)
        goto _close;

    if (memcmp(text, buffer, sizeof(text))) {
        LOG_ERR("File system test failed!!!!!!!");
        goto _close;
    }

    LOG_INF("File system test successful\n");

_close:
    fs_close(&fd);
_exit:
    LOG_INF("fs_test thread is exit now\n");
    k_thread_abort(k_current_get());
}

K_THREAD_DEFINE(fs_test, 2048, 
                fs_test_thread, NULL, NULL, NULL,
                10, 0, 0);

