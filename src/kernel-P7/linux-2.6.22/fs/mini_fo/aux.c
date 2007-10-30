/*
 * Copyright (c) 1997-2003 Erez Zadok
 * Copyright (c) 2001-2003 Stony Brook University
 *
 * For specific licensing information, see the COPYING file distributed with
 * this package, or get one from ftp://ftp.filesystems.org/pub/fist/COPYING.
 *
 * This Copyright notice must be kept intact and distributed with all
 * fistgen sources INCLUDING sources generated by fistgen.
 */
/*
 * Copyright (C) 2004, 2005 Markus Klotzbuecher <mk@creamnet.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */
/*
 *  $Id$
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "fist.h"
#include "mini_fo.h"

/* check if file exists in storage  */
int exists_in_storage(dentry_t *dentry)
{
	check_mini_fo_dentry(dentry);
	if(dtost(dentry) == MODIFIED || dtost(dentry) == CREATED || dtost(dentry) == DEL_REWRITTEN)
		return 1;
	return 0;	
}

/* check if dentry is in an existing state */
int is_mini_fo_existant(dentry_t *dentry) 
{
	check_mini_fo_dentry(dentry);

	if(dtost(dentry) == DELETED || dtost(dentry) == NON_EXISTANT)
		return 0;
	else
		return 1;
}

/* 
 * This function will create a negative storage dentry for 
 * dentry, what is required for many create like options.
 * It will create the storage structure if necessary.
 */
int get_neg_sto_dentry(dentry_t *dentry) 
{
	int err = 0;
	unsigned int len;
	const unsigned char *name;

	if(!dentry ||
	   !dtopd(dentry) ||
	   !(dtost(dentry) == UNMODIFIED ||
	     dtost(dentry) == NON_EXISTANT ||
	     dtost(dentry) == DELETED)) {
		printk(KERN_CRIT "mini_fo: get_neg_sto_dentry: invalid dentry passed.\n");
		err = -1;
		goto out;
	}
	/* Have we got a neg. dentry already? */
	if(dtohd2(dentry)) {
		err = 0;
		goto out;
	}
	if(dtost(dentry->d_parent) == UNMODIFIED) {
		/* build sto struct */
		err = build_sto_structure(dentry->d_parent->d_parent, dentry->d_parent);
		if(err || 
		   dtost(dentry->d_parent) != MODIFIED) {
			printk(KERN_CRIT "mini_fo: get_neg_sto_dentry: ERROR building sto structure.\n");
			err = -1;
			goto out;
		}		
	}

	len = dentry->d_name.len;
	name = dentry->d_name.name;
	 
	dtohd2(dentry) = 
		lookup_one_len(name, dtohd2(dentry->d_parent), len);

 out:
	return err;
}

int check_mini_fo_dentry(dentry_t *dentry)
{
 	ASSERT(dentry != NULL);
	ASSERT(dtopd(dentry) != NULL);
	ASSERT((dtohd(dentry) != NULL) || (dtohd2(dentry) != NULL));
	       
/* 	if(dtost(dentry) == MODIFIED) { */
/* 		ASSERT(dentry->d_inode != NULL); */
/* 		ASSERT(dtohd(dentry) != NULL); */
/* 		ASSERT(dtohd(dentry)->d_inode != NULL); */
/* 		ASSERT(dtohd2(dentry) != NULL); */
/* 		ASSERT(dtohd2(dentry)->d_inode != NULL); */
/* 	} */
/* 	else if(dtost(dentry) == UNMODIFIED) { */
/* 		ASSERT(dentry->d_inode != NULL); */
/* 		ASSERT( */
/* 	} */
	return 0;	       
}

int check_mini_fo_file(file_t *file)
{
	ASSERT(file != NULL);
	ASSERT(ftopd(file) != NULL);
	ASSERT(file->f_dentry != NULL);
	
	/* violent checking, check depending of state and type 
	 *	if(S_ISDIR(file->f_dentry->d_inode->i_mode)) {}
	 */
	ASSERT((ftohf(file) != NULL) || (ftohf2(file) != NULL));
	return 0;
}

int check_mini_fo_inode(inode_t *inode)
{
	ASSERT(inode != NULL);
	ASSERT(itopd(inode) != NULL);
	ASSERT((itohi(inode) != NULL) || (itohi2(inode) != NULL));
	return 0;
}

/* 
 * will walk a base path as provided by get_mini_fo_bpath and return
 * the (hopefully ;-) ) positive dentry of the renamed base dir.
 *
 * This does some work of path_init.
 */
dentry_t *bpath_walk(super_block_t *sb, char *bpath) 
{
	int err;
	struct nameidata nd;

	/* be paranoid */
	if(!bpath || bpath[0] != '/') {
		printk(KERN_CRIT "mini_fo: bpath_walk: Invalid string.\n");
		return NULL;
	}
	if(!sb || !stopd(sb)) {
		printk(KERN_CRIT "mini_fo: bpath_walk: Invalid sb.\n");
		return NULL;
	}
	
	/* setup nd as path_init does */
	nd.last_type = LAST_ROOT; /* if there are only slashes... */
	nd.flags = LOOKUP_FOLLOW;
	/* fix this: how do I reach this lock? 
	 * read_lock(&current->fs->lock); */
	nd.mnt = mntget(stopd(sb)->hidden_mnt);
	nd.dentry = dget(stopd(sb)->base_dir_dentry);
	/* read_unlock(&current->fs->lock); */
	
	err = path_walk(bpath+1, &nd);

	/* validate */
	if (err || !nd.dentry || !nd.dentry->d_inode) {
		printk(KERN_CRIT "mini_fo: bpath_walk: path_walk failed.\n");
		return NULL;
	}
	return nd.dentry;
}


/* returns the full path of the basefile incl. its name */
int get_mini_fo_bpath(dentry_t *dentry, char **bpath, int *bpath_len)
{
	char *buf_walker;
	int len = 0;
	dentry_t *sky_walker;
	
	if(!dentry || !dtohd(dentry)) {
		printk(KERN_CRIT "mini_fo: get_mini_fo_bpath: invalid dentry passed.\n");
		return -1;
	}
	sky_walker = dtohd(dentry);

	do {
		len += sky_walker->d_name.len + 1 ; /* 1 for '/' */
		sky_walker = sky_walker->d_parent;
	} while(sky_walker != stopd(dentry->d_inode->i_sb)->base_dir_dentry);

	/* 1 to oil the loop */
	*bpath = (char*)  kmalloc(len + 1, GFP_KERNEL);
	if(!*bpath) {
		printk(KERN_CRIT "mini_fo: get_mini_fo_bpath: out of mem.\n");
		return -1;
	}
	buf_walker = *bpath+len; /* put it on last char */
	*buf_walker = '\n';
	sky_walker = dtohd(dentry);
	
	do {
		buf_walker -= sky_walker->d_name.len;
		strncpy(buf_walker, 
			sky_walker->d_name.name, 
			sky_walker->d_name.len);
		*(--buf_walker) = '/';
		sky_walker = sky_walker->d_parent;
	} while(sky_walker != stopd(dentry->d_inode->i_sb)->base_dir_dentry);

	/* bpath_len doesn't count newline! */
	*bpath_len = len;
 	return 0;
}

int mini_fo_cp_cont(dentry_t *tgt_dentry, struct vfsmount *tgt_mnt,
		    dentry_t *src_dentry, struct vfsmount *src_mnt)
{
	void *buf;
	mm_segment_t old_fs;
	file_t *tgt_file;
	file_t *src_file;
	int bytes, len, tmp, err;
	err = 0;

	if(!(tgt_dentry->d_inode && src_dentry->d_inode)) {
		printk(KERN_CRIT "mini_fo_cp_cont: ERROR, neg. dentry passed.\n");
		err = -EINVAL;
		goto out;
	}

	dget(tgt_dentry);
	dget(src_dentry);
	mntget(tgt_mnt);
	mntget(src_mnt);

	/* open file write only */
	tgt_file = dentry_open(tgt_dentry, tgt_mnt, 0x1);
	if(!tgt_file || IS_ERR(tgt_file)) {
		printk(KERN_CRIT "mini_fo_cp_cont: ERROR opening target file.\n");
		err = PTR_ERR(tgt_file);
		goto out_err;
	}

	/* open file read only */
	src_file = dentry_open(src_dentry, src_mnt, 0x0);
	if(!src_file || IS_ERR(src_file)) {
		printk(KERN_CRIT "mini_fo_cp_cont: ERROR opening source file.\n");
		err = PTR_ERR(src_file);

		/* close target file */
		fput(tgt_file);
		goto out_err;
	}

	/* check if the filesystem(s) support read respective write */
	if(!src_file->f_op->read || !tgt_file->f_op->write) {
		printk(KERN_CRIT "mini_fo_cp_cont: ERROR, no fs read or write support.\n");
		err = -EPERM;
		goto out_close;
	}

	/* allocate a page for transfering the data */
	buf = (void *) __get_free_page(GFP_KERNEL);
	if(!buf) {
		printk(KERN_CRIT "mini_fo_cp_cont: ERROR, out of kernel mem.\n");
		goto out_err;
	}

	tgt_file->f_pos = 0;
	src_file->f_pos = 0;

	old_fs = get_fs();
	set_fs(KERNEL_DS);

	/* Doing this I assume that a read operation will return a full
	 * buffer while there is still data to read, and a less than
	 * full buffer when all data has been read.
	 */
	bytes = len = PAGE_SIZE;
	while(bytes == len) {
		bytes = src_file->f_op->read(src_file, buf, len, 
					     &src_file->f_pos);
		tmp = tgt_file->f_op->write(tgt_file, buf, bytes, 
					    &tgt_file->f_pos);
		if(tmp != bytes) {
			printk(KERN_CRIT "mini_fo_cp_cont: ERROR writing.\n");
			goto out_close_unset;
		}
	}

	free_page((unsigned long) buf);
	set_fs(old_fs);
	fput(tgt_file);
	fput(src_file);
	goto out;

 out_close_unset:
	free_page((unsigned long) buf);
	set_fs(old_fs);

 out_close:
	fput(tgt_file);
	fput(src_file);

 out_err:
	dput(tgt_dentry);
	dput(src_dentry);

	/* mk: not sure if this need to be done */
	mntput(tgt_mnt);
	mntput(src_mnt);

 out:
	return err;
}

/* mk:
 * ndl (no-duplicate list) stuff
 * This is used in mini_fo_readdir, to save the storage directory contents
 * and later when reading base, match them against the list in order
 * to avoid duplicates.
 */

/* add a file specified by name and len to the ndl
 * Return values: 0 on success, <0 on failure.
 */
int ndl_add_entry(struct readdir_data *rd, const char *name, int len)
{
	struct ndl_entry *tmp_entry;

	tmp_entry = (struct ndl_entry *) 
		kmalloc(sizeof(struct ndl_entry), GFP_KERNEL);
	if(!tmp_entry) {
                printk(KERN_CRIT "mini_fo: ndl_add_entry: out of mem.\n");
                return -ENOMEM;
        }
        tmp_entry->name = (char*) kmalloc(len, GFP_KERNEL);
        if(!tmp_entry->name) {
                printk(KERN_CRIT "mini_fo: ndl_add_entry: out of mem.\n");
                return -ENOMEM;
        }
	strncpy(tmp_entry->name, name, len);
        tmp_entry->len = len;

        list_add(&tmp_entry->list, &rd->ndl_list);
        rd->ndl_size++;
        return 0;
}

/* delete all list entries and free memory */
void ndl_put_list(struct readdir_data *rd)
{
	struct list_head *tmp;
	struct ndl_entry *tmp_entry;

	if(rd->ndl_size <= 0)
		return;
	while(!list_empty(&rd->ndl_list)) {
		tmp = rd->ndl_list.next;
                list_del(tmp);
                tmp_entry = list_entry(tmp, struct ndl_entry, list);
		kfree(tmp_entry->name);
                kfree(tmp_entry);
        }
	rd->ndl_size = 0;
}

/* Check if a file specified by name and len is in the ndl
 * Return value: 0 if not in list, 1 if file is found in ndl.
 */
int ndl_check_entry(struct readdir_data *rd, const char *name, int len)
{
	struct list_head *tmp;
	struct ndl_entry *tmp_entry;

	if(rd->ndl_size <= 0)
		return 0;

	list_for_each(tmp, &rd->ndl_list) {
                tmp_entry = list_entry(tmp, struct ndl_entry, list);
                if(tmp_entry->len != len)
                        continue;
                if(!strncmp(tmp_entry->name, name, len))
                        return 1;
        }
        return 0;
}

/* mk:
 * Recursive function to create corresponding directorys in the storage fs.
 * The function will build the storage directorys up to dentry.
 */
int build_sto_structure(dentry_t *dir, dentry_t *dentry)
{
	int err;
	dentry_t *hidden_sto_dentry;
	dentry_t *hidden_sto_dir_dentry;

	if(dentry->d_parent != dir) {
		printk(KERN_CRIT "mini_fo: build_sto_structure: invalid parameter or meta data corruption [1].\n");
		return 1;
	}

       	if(dtost(dir) != MODIFIED) {
		err = build_sto_structure(dir->d_parent, dentry->d_parent);
		if(err)
			return err;
	}

	/* ok, coming back again. */
	check_mini_fo_dentry(dentry);
	hidden_sto_dentry = dtohd2(dentry);

	if(!hidden_sto_dentry) {
		/*
		 * This is the case after creating the first 
		 * hidden_sto_dentry.
		 * After one negative storage_dentry, all pointers to 
		 * hidden_storage dentries are set to NULL. We need to
		 * create the negative dentry before we create the storage
		 * file.
		 */
		unsigned int len;
		const unsigned char *name;
		len = dtohd(dentry)->d_name.len;
		name = dtohd(dentry)->d_name.name;
		hidden_sto_dentry = lookup_one_len(name, dtohd2(dir), len);
		dtohd2(dentry) = hidden_sto_dentry;
	}

	/* was:	hidden_sto_dir_dentry = lock_parent(hidden_sto_dentry); */
	hidden_sto_dir_dentry = dget(hidden_sto_dentry->d_parent);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,16)
	mutex_lock(&hidden_sto_dir_dentry->d_inode->i_mutex);
#else
	down(&hidden_sto_dir_dentry->d_inode->i_sem);
#endif
	/* lets be safe */
	if(dtohd2(dir) != hidden_sto_dir_dentry) {
		printk(KERN_CRIT "mini_fo: build_sto_structure: invalid parameter or meta data corruption [2].\n");
		return 1;
	}

	/* check for errors in lock_parent */
	err = PTR_ERR(hidden_sto_dir_dentry);
	if(IS_ERR(hidden_sto_dir_dentry)) {
		printk(KERN_CRIT "mini_fo: build_sto_structure: lock_parent failed.\n");
		return err;
	}

	err = vfs_mkdir(hidden_sto_dir_dentry->d_inode,
			hidden_sto_dentry,
			dir->d_inode->i_mode);

	if(err) {
		printk(KERN_CRIT "mini_fo: build_sto_structure: failed to create storage dir [1].\n");
		/* was: unlock_dir(dir); */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,16)
		mutex_unlock(&dir->d_inode->i_mutex);
#else
		up(&dir->d_inode->i_sem);
#endif
		dput(dir);
		return err;
	}
	
	/* everything ok! */
	if(!dtohd2(dentry)->d_inode) {
		printk(KERN_CRIT "mini_fo: build_sto_structure: failed to create storage dir [2].\n");
		/* was: unlock_dir(dir); */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,16)
		mutex_unlock(&dir->d_inode->i_mutex);
#else
		up(&dir->d_inode->i_sem);
#endif
		dput(dir);
		return 1;
	}

	/* interpose the new inode and set new state */
	itohi2(dentry->d_inode) = igrab(dtohd2(dentry)->d_inode);
	dtopd(dentry)->state = MODIFIED;

	/* initalize the wol list */
	itopd(dentry->d_inode)->deleted_list_size = -1;
	itopd(dentry->d_inode)->renamed_list_size = -1;
	meta_build_lists(dentry);
	
	fist_copy_attr_all(dentry->d_inode, itohi2(dentry->d_inode));
	fist_copy_attr_timesizes(dir->d_inode, 
				 hidden_sto_dir_dentry->d_inode);
	dir->d_inode->i_nlink++;
	/* was: unlock_dir(hidden_sto_dir_dentry); */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,16)
	mutex_unlock(&hidden_sto_dir_dentry->d_inode->i_mutex);
#else
	up(&hidden_sto_dir_dentry->d_inode->i_sem);
#endif
	dput(hidden_sto_dir_dentry);
	return 0;
}


#if 0 /* unused */

/*
 * Read "len" bytes from "filename" into "buf".
 * "buf" is in kernel space.
 */
int
mini_fo_read_file(const char *filename, void *buf, int len)
{
    file_t *filp;
    mm_segment_t oldfs;
    int bytes;
    /* Chroot? Maybe NULL isn't right here */
    filp = filp_open(filename, O_RDONLY, 0);
    if (!filp || IS_ERR(filp)) {
	printk("mini_fo_read_file err %d\n", (int) PTR_ERR(filp));
	return -1;  /* or do something else */
    }

    if (!filp->f_op->read)
	return -2;  /* file(system) doesn't allow reads */

    /* now read len bytes from offset 0 */
    filp->f_pos = 0;		/* start offset */
    oldfs = get_fs();
    set_fs(KERNEL_DS);
    bytes = filp->f_op->read(filp, buf, len, &filp->f_pos);
    set_fs(oldfs);

    /* close the file */
    fput(filp);

    return bytes;
}



/*
 * Write "len" bytes from "buf" to "filename"
 * "buf" is in kernel space.
 */
int
mini_fo_write_file(const char *filename, void *buf, int len)
{
    file_t *filp;
    mm_segment_t oldfs;
    int bytes;
				/* Chroot? Maybe NULL isn't right here */
    filp = filp_open(filename, O_RDWR|O_CREAT, 0640);
    if (!filp || IS_ERR(filp)) {
	printk("mini_fo_write_file err %d\n", (int) PTR_ERR(filp));
	return -1;  /* or do something else */
    }

    if (!filp->f_op->write)
	return -2;  /* file(system) doesn't allow writes */

    /* now write len bytes from offset 0 */
    filp->f_pos = 0;		/* start offset */
    oldfs = get_fs();
    set_fs(KERNEL_DS);
    bytes = filp->f_op->write(filp, buf, len, &filp->f_pos);
    set_fs(oldfs);

    /* close the file */
    fput(filp);

    return bytes;
}

#endif /* unused */

