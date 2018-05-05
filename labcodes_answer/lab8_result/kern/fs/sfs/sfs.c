#include <defs.h>
#include <sfs.h>
#include <error.h>
#include <assert.h>

// From Rust
extern int sfs_do_mount(struct device *dev, struct fs **fs_store);

int
sfs_mount(const char *devname) {
    return vfs_mount(devname, sfs_do_mount);
}

/*
 * sfs_init - mount sfs on disk0
 *
 * CALL GRAPH:
 *   kern_init-->fs_init-->sfs_init
 */
void
sfs_init(void) {
    int ret;
    if ((ret = sfs_mount("disk0")) != 0) {
        panic("failed: sfs: sfs_mount: %e.\n", ret);
    }
}

