#include <defs.h>
#include <sfs.h>
#include <error.h>
#include <assert.h>
#include <vfs.h>
#include <inode.h>

// Define useless symbols to prevent link error
void __udivdi3() {}
void __udivti3() {}
void __floatundisf() {}
void __floatundidf() {}

struct fs_ops {
    int (*fs_sync)(struct fs *fs);                 // Flush all dirty buffers to disk
    struct inode *(*fs_get_root)(struct fs *fs);   // Return root inode of filesystem.
    int (*fs_unmount)(struct fs *fs);              // Attempt unmount of filesystem.
    void (*fs_cleanup)(struct fs *fs);             // Cleanup of filesystem.???
};

// Export to Rust
struct fs*
create_fs_for_sfs(struct fs_ops* ops) {
    struct fs* fs = alloc_fs(sfs);
    assert(fs != NULL);
    fs->fs_sync = ops->fs_sync;
    fs->fs_get_root = ops->fs_get_root;
    fs->fs_unmount = ops->fs_unmount;
    fs->fs_cleanup = ops->fs_cleanup;
    return fs;
}

struct inode*
create_inode_for_sfs(const struct inode_ops* ops, struct fs* fs) {
    struct inode* inode = alloc_inode(sfs_inode);
    assert(inode != NULL);
    inode_init(inode, ops, fs);
    return inode;
}

// From Rust
extern int
sfs_do_mount(struct device *dev, struct fs **fs_store);

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

