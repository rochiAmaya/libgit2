/*
 * Copyright (C) the libgit2 contributors. All rights reserved.
 *
 * This file is part of libgit2, distributed under the GNU GPL v2 with
 * a Linking Exception. For full terms see the included COPYING file.
 */
#ifndef INCLUDE_git_types_h__
#define INCLUDE_git_types_h__

#include "common.h"

/**
 * @file git2/types.h
 * @brief libgit2 base & compatibility types
 * @ingroup Git
 * @{
 */
GIT_BEGIN_DECL

/**
 * Cross-platform compatibility types for off_t / time_t
 *
 * NOTE: This needs to be in a public header so that both the library
 * implementation and client applications both agree on the same types.
 * Otherwise we get undefined behavior.
 *
 * Use the "best" types that each platform provides. Currently we truncate
 * these intermediate representations for compatibility with the git ABI, but
 * if and when it changes to support 64 bit types, our code will naturally
 * adapt.
 * NOTE: These types should match those that are returned by our internal
 * stat() functions, for all platforms.
 */
#include <sys/types.h>
#ifdef __amigaos4__
#include <stdint.h>
#endif

#if defined(_MSC_VER)

typedef __int64 git_off_t;
typedef __time64_t git_time_t;

#elif defined(__MINGW32__)

typedef off64_t git_off_t;
typedef __time64_t git_time_t;

#elif defined(__HAIKU__)

typedef __haiku_std_int64 git_off_t;
typedef __haiku_std_int64 git_time_t;

#else /* POSIX */

/*
 * Note: Can't use off_t since if a client program includes <sys/types.h>
 * before us (directly or indirectly), they'll get 32 bit off_t in their client
 * app, even though /we/ define _FILE_OFFSET_BITS=64.
 */
typedef int64_t git_off_t;
typedef int64_t git_time_t;

#endif

/** Basic type (loose or packed) of any Git object. */
typedef enum {
	GIT_OBJ_ANY = -2,		/**< Object can be any of the following */
	GIT_OBJ_BAD = -1,		/**< Object is invalid. */
	GIT_OBJ__EXT1 = 0,		/**< Reserved for future use. */
	GIT_OBJ_COMMIT = 1,		/**< A commit object. */
	GIT_OBJ_TREE = 2,		/**< A tree (directory listing) object. */
	GIT_OBJ_BLOB = 3,		/**< A file revision object. */
	GIT_OBJ_TAG = 4,		/**< An annotated tag object. */
	GIT_OBJ__EXT2 = 5,		/**< Reserved for future use. */
	GIT_OBJ_OFS_DELTA = 6, /**< A delta, base is given by an offset. */
	GIT_OBJ_REF_DELTA = 7, /**< A delta, base is given by object id. */
} git_otype;

/** An open object database handle. */
typedef struct git_odb git_odb;

/** A custom backend in an ODB */
typedef struct git_odb_backend git_odb_backend;

/** An object read from the ODB */
typedef struct git_odb_object git_odb_object;

/** A stream to read/write from the ODB */
typedef struct git_odb_stream git_odb_stream;

/** A stream to write a packfile to the ODB */
typedef struct git_odb_writepack git_odb_writepack;

/** An open refs database handle. */
typedef struct git_refdb git_refdb;

/** A custom backend for refs */
typedef struct git_refdb_backend git_refdb_backend;

/**
 * Representation of an existing git repository,
 * including all its object contents
 */
typedef struct git_repository git_repository;

/** Representation of a generic object in a repository */
typedef struct git_object git_object;

/** Representation of an in-progress walk through the commits in a repo */
typedef struct git_revwalk git_revwalk;

/** Parsed representation of a tag object. */
typedef struct git_tag git_tag;

/** In-memory representation of a blob object. */
typedef struct git_blob git_blob;

/** Parsed representation of a commit object. */
typedef struct git_commit git_commit;

/** Representation of each one of the entries in a tree object. */
typedef struct git_tree_entry git_tree_entry;

/** Representation of a tree object. */
typedef struct git_tree git_tree;

/** Constructor for in-memory trees */
typedef struct git_treebuilder git_treebuilder;

/** Memory representation of an index file. */
typedef struct git_index git_index;

/** An interator for conflicts in the index. */
typedef struct git_index_conflict_iterator git_index_conflict_iterator;

/** Memory representation of a set of config files */
typedef struct git_config git_config;

/** Interface to access a configuration file */
typedef struct git_config_backend git_config_backend;

/** Representation of a reference log entry */
typedef struct git_reflog_entry git_reflog_entry;

/** Representation of a reference log */
typedef struct git_reflog git_reflog;

/** Representation of a git note */
typedef struct git_note git_note;

/** Representation of a git packbuilder */
typedef struct git_packbuilder git_packbuilder;

/** Time in a signature */
typedef struct git_time {
	git_time_t time; /** time in seconds from epoch */
	int offset; /** timezone offset, in minutes */
} git_time;

/** An action signature (e.g. for committers, taggers, etc) */
typedef struct git_signature {
	char *name; /** full name of the author */
	char *email; /** email of the author */
	git_time when; /** time when the action happened */
} git_signature;

/** In-memory representation of a reference. */
typedef struct git_reference git_reference;

/** Iterator for references */
typedef struct git_reference_iterator  git_reference_iterator;

/** Merge heads, the input to merge */
typedef struct git_merge_head git_merge_head;

/** Representation of a status collection */
typedef struct git_status_list git_status_list;


/** Basic type of any Git reference. */
typedef enum {
	GIT_REF_INVALID = 0, /** Invalid reference */
	GIT_REF_OID = 1, /** A reference which points at an object id */
	GIT_REF_SYMBOLIC = 2, /** A reference which points at another reference */
	GIT_REF_LISTALL = GIT_REF_OID|GIT_REF_SYMBOLIC,
} git_ref_t;

/** Basic type of any Git branch. */
typedef enum {
	GIT_BRANCH_LOCAL = 1,
	GIT_BRANCH_REMOTE = 2,
} git_branch_t;

/** Valid modes for index and tree entries. */
typedef enum {
	GIT_FILEMODE_NEW					= 0000000,
	GIT_FILEMODE_TREE					= 0040000,
	GIT_FILEMODE_BLOB					= 0100644,
	GIT_FILEMODE_BLOB_EXECUTABLE		= 0100755,
	GIT_FILEMODE_LINK					= 0120000,
	GIT_FILEMODE_COMMIT					= 0160000,
} git_filemode_t;

typedef struct git_refspec git_refspec;
typedef struct git_remote git_remote;
typedef struct git_push git_push;

typedef struct git_remote_head git_remote_head;
typedef struct git_remote_callbacks git_remote_callbacks;

/**
 * This is passed as the first argument to the callback to allow the
 * user to see the progress.
 */
typedef struct git_transfer_progress {
	unsigned int total_objects;
	unsigned int indexed_objects;
	unsigned int received_objects;
	size_t received_bytes;
} git_transfer_progress;

/**
 * Type for progress callbacks during indexing.  Return a value less than zero
 * to cancel the transfer.
 *
 * @param stats Structure containing information about the state of the transfer
 * @param payload Payload provided by caller
 */
typedef int (*git_transfer_progress_callback)(const git_transfer_progress *stats, void *payload);

/**
 * Opaque structure representing a submodule.
 */
typedef struct git_submodule git_submodule;

/**
 * Values that could be specified for the update rule of a submodule.
 *
 * Use the RESET value if you have altered the in-memory update value via
 * `git_submodule_set_update()` and wish to reset to the original default.
 */
typedef enum {
	GIT_SUBMODULE_UPDATE_RESET = -1,
	GIT_SUBMODULE_UPDATE_CHECKOUT = 1,
	GIT_SUBMODULE_UPDATE_REBASE = 2,
	GIT_SUBMODULE_UPDATE_MERGE = 3,
	GIT_SUBMODULE_UPDATE_NONE = 4
} git_submodule_update_t;

/**
 * Values that could be specified for how closely to examine the
 * working directory when getting submodule status.
 *
 * Use the RESET value if you have altered the in-memory ignore value via
 * `git_submodule_set_ignore()` and wish to reset to the original value.
 */
typedef enum {
	GIT_SUBMODULE_IGNORE_RESET = -1,    /* reset to on-disk value */
	GIT_SUBMODULE_IGNORE_NONE = 1,      /* any change or untracked == dirty */
	GIT_SUBMODULE_IGNORE_UNTRACKED = 2, /* dirty if tracked files change */
	GIT_SUBMODULE_IGNORE_DIRTY = 3,     /* only dirty if HEAD moved */
	GIT_SUBMODULE_IGNORE_ALL = 4        /* never dirty */
} git_submodule_ignore_t;

/** @} */
GIT_END_DECL

#endif
