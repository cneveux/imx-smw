/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright 2021 NXP
 */
#ifndef __UTIL_CIPHER_H__
#define __UTIL_CIPHER_H__

#include "util_list.h"

/**
 * struct cipher_output_data - Cipher output data
 * @output: Pointer to output data.
 * @output_len: @output length in bytes.
 */
struct cipher_output_data {
	unsigned char *output;
	unsigned int output_len;
};

/**
 * util_cipher_add_out_data() - Add data in a cipher output linked list
 * @list: Pointer to cipher output data linked list.
 * @ctx_id: Local context ID.
 * @out_data: Data to add.
 * @data_len: @out_data length in bytes.
 *
 * If parameter @list is NULL it's allocated in this function.
 * If it's the first call for parameter @ctx_id, the node is allocated.
 * Else, parameter @out_data is added to existing node data.
 * All the memory allocated by this function is freed when
 * util_list_clear() is called.
 *
 * Return:
 * PASSED			- Success.
 * -INTERNAL_OUT_OF_MEMORY	- Memory allocation failed.
 */
int util_cipher_add_out_data(struct llist **list, unsigned int ctx_id,
			     unsigned char *out_data, unsigned int data_len);

/**
 * compare_output_data() - Compare cipher output data
 * @list: Pointer to cipher output data linked list.
 * @ctx_id: Local context ID.
 * @data: Data to compare.
 * @data_len: @data length in bytes.
 *
 * Return:
 * PASSED	- Success.
 * -INTERNAL	- @ctx_id node is not found.
 * -SUBSYSTEM	- Comparison failed.
 */
int compare_output_data(struct llist *list, unsigned int ctx_id,
			unsigned char *data, unsigned int data_len);

/**
 * util_cipher_copy_node() - Copy a cipher output data node
 * @list: Pointer to cipher output data linked list.
 * @dst_ctx_id: Context ID associated to the new node
 * @src_ctx_id: Context ID associated to the source node
 *
 * A new node is created in parameter @list linked list, associated to parameter
 * @dst_ctx_id. Data present in parameter @src_ctx_id node are copied in the new
 * node.
 *
 * Return:
 * PASSED			- Success
 * -INTERNAL			- Source node not found
 * -INTERNAL_OUT_OF_MEMORY	- Memory allocation failed
 */
int util_cipher_copy_node(struct llist **list, unsigned int dst_ctx_id,
			  unsigned int src_ctx_id);

#endif /* __UTIL_CIPHER_H__ */
