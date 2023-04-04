/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright 2022-2023 NXP
 */

#ifndef __COMMON_H__
#define __COMMON_H__

#define PSA_COMPLIANT
#include <hsm_api.h>

#include "keymgr_derive.h"

/**
 * struct hdl - ELE handles
 * @session: Session handle
 * @key_store: Key store service flow handle
 *
 * This structure stores the ELE handles managed by the SMW library.
 */
struct hdl {
	hsm_hdl_t session;
	hsm_hdl_t key_store;
};

struct ele_hash_algo {
	enum smw_config_hash_algo_id algo_id;
	hsm_hash_algo_t ele_algo;
	uint32_t length;
};

/**
 * get_hash_algo() - Get the ELE hash algorithm information
 * @algo_id: SMW Hash algorithm id.
 *
 * Return:
 * NULL if algorithm not found, otherwise reference to the hash algorithm
 * information.
 */
const struct ele_hash_algo *
ele_get_hash_algo(enum smw_config_hash_algo_id algo_id);

/**
 * ele_key_handle() - Handle the Key operations.
 * @hdl: Pointer to the ELE handles structure.
 * @operation_id: Security Operation ID.
 * @args: Pointer to a structure of arguments defined by the internal API.
 * @status: Error code set only if the Security Operation is handled.
 *
 * This function handles the Key operations.
 * @status is set only if the function returns true.
 *
 * Return:
 * * true:	- the Security Operation has been handled.
 * * false:	- the Security Operation has not been handled.
 */
bool ele_key_handle(struct hdl *hdl, enum operation_id operation_id, void *args,
		    int *status);

/**
 * ele_hash_handle() - Handle the Hash operation.
 * @hdl: Pointer to the ELE handles structure.
 * @operation_id: Security Operation ID.
 * @args: Pointer to a structure of arguments defined by the internal API.
 * @status: Error code set only if the Security Operation is handled.
 *
 * This function handles the Hash operation.
 * @status is set only if the function returns true.
 *
 * Return:
 * * true:	- the Security Operation has been handled.
 * * false:	- the Security Operation has not been handled.
 */
bool ele_hash_handle(struct hdl *hdl, enum operation_id operation_id,
		     void *args, int *status);

/**
 * ele_mac_handle() - Handle the MAC operation.
 * @hdl: Pointer to the ELE handles structure.
 * @operation_id: Security Operation ID.
 * @args: Pointer to a structure of arguments defined by the internal API.
 * @status: Error code set only if the Security Operation is handled.
 *
 * This function handles the MAC operation.
 * @status is set only if the function returns true.
 *
 * Return:
 * * true:	- the Security Operation has been handled.
 * * false:	- the Security Operation has not been handled.
 */
bool ele_mac_handle(struct hdl *hdl, enum operation_id operation_id, void *args,
		    int *status);

/**
 * ele_sign_verify_handle() - Handle the Sign and Verify operation.
 * @hdl: Pointer to the ELE handles structure.
 * @operation_id: Security Operation ID.
 * @args: Pointer to a structure of arguments defined by the internal API.
 * @status: Error code set only if the Security Operation is handled.
 *
 * This function handles the signarture generation and verification operation.
 * @status is set only if the function returns true.
 *
 * Return:
 * * true:	- the Security Operation has been handled.
 * * false:	- the Security Operation has not been handled.
 */
bool ele_sign_verify_handle(struct hdl *hdl, enum operation_id operation_id,
			    void *args, int *status);

/**
 * ele_rng_handle() - Handle the random generation operation.
 * @hdl: Pointer to the ELE handles structure.
 * @operation_id: Security Operation ID.
 * @args: Pointer to a structure of arguments defined by the internal API.
 * @status: Error code set only if the Security Operation is handled.
 *
 * This function handles the random number generation operation.
 * @status is set only if the function returns true.
 *
 * Return:
 * * true:	- the Security Operation has been handled.
 * * false:	- the Security Operation has not been handled.
 */
bool ele_rng_handle(struct hdl *hdl, enum operation_id operation_id, void *args,
		    int *status);

/**
 * ele_cipher_handle() - Handle the cipher encryption/decryption operation.
 * @hdl: Pointer to the ELE handles structure.
 * @operation_id: Security Operation ID.
 * @args: Pointer to a structure of arguments defined by the internal API.
 * @status: Error code set only if the Security Operation is handled.
 *
 * This function handles the cipher encryption/decrytion operation.
 * @status is set only if the function returns true.
 *
 * Return:
 * * true:	- the Security Operation has been handled.
 * * false:	- the Security Operation has not been handled.
 */
bool ele_cipher_handle(struct hdl *hdl, enum operation_id operation_id,
		       void *args, int *status);

/**
 * ele_derive_key() - ELE key derivation operation.
 * @hdl: Pointer to the ELE handles structure.
 * @args: Pointer to the derive key arguments.
 *
 * Return:
 * SMW status
 */
int ele_derive_key(struct hdl *hdl, struct smw_keymgr_derive_key_args *args);

/**
 * ele_convert_err() - Convert ELE error into SMW status.
 * @err: ELE error code.
 *
 * Return:
 * SMW status
 */
int ele_convert_err(hsm_err_t err);

/**
 * ele_set_pubkey_type() - Set the ELE public key type
 * @key_type_id: SMW Key type id
 * @ele_type: ELE key type corresponding
 *
 * Return:
 * SMW_STATUS_OK                       - Success
 * SMW_STATUS_OPERATION_NOT_SUPPORTED  - Key type not supported
 */
int ele_set_pubkey_type(enum smw_config_key_type_id key_type_id,
			hsm_pubkey_type_t *ele_type);

/**
 * ele_set_key_policy() - Convert the user key policy to ELE key policy
 * @policy: Pointer to the key policy
 * @policy_len: Length of @policy
 * @ele_usage: ELE key usage(s) bit mask
 * @ele_algo: ELE key permitted algorithm (first algorithm defined)
 * @actual_policy: Key attributes policy used
 * @actual_policy_len: Length of key attributes policy used
 *
 * Return:
 * SMW_STATUS_OK                         - Success
 * SMW_STATUS_KEY_POLICY_ERROR           - User key policy definition error
 * SMW_STATUS_KEY_POLICY_WARNING_IGNORED - One of the user key policy is ignored
 * Other SMW status error.
 */
int ele_set_key_policy(const unsigned char *policy, unsigned int policy_len,
		       hsm_key_usage_t *ele_usage,
		       hsm_permitted_algo_t *ele_algo,
		       unsigned char **actual_policy,
		       unsigned int *actual_policy_len);

/**
 * ele_get_key_policy() - Convert the ELE key policy to user key policy
 * @policy: Pointer to the key policy
 * @policy_len: Length of @policy
 * @ele_usage: ELE key usage(s) bit mask
 * @ele_algo: ELE key permitted algorithm
 *
 * On success, the function allocates the @policy buffer and returns its length
 * in the @policy_len.
 *
 * Return:
 * SMW_STATUS_OK                         - Success
 * SMW_STATUS_INVALID_PARAM              - Invalid parameters
 * SMW_STATUS_ALLOC_FAILURE              - Memory allocation failure
 * SMW_STATUS_OPERATION_FAILURE          - Unexpected operation failure
 */
int ele_get_key_policy(unsigned char **policy, unsigned int *policy_len,
		       hsm_key_usage_t ele_usage,
		       hsm_permitted_algo_t ele_algo);

/**
 * ele_export_public_key() - Export the ELE public key
 * @hdl: Pointer to the ELE handles structure.
 * @key_desc: Key descriptor
 *
 * The function exports the public key of the given @key_desc->identifier.id.
 * The following fields of @key_desc parameters are output:
 *  - identifier.type_id
 *  - identifier.security_size
 *  - format_id
 *  - pub (if operation success)
 *  - ops (if operation success)
 *
 * Return:
 * SMW_STATUS_OK                       - Success
 * SMW_STATUS_OPERATION_NOT_SUPPORTED  - Key type not supported
 * Other SMW status error.
 */
int ele_export_public_key(struct hdl *hdl,
			  struct smw_keymgr_descriptor *key_desc);

/**
 * ele_get_lifecycle() - Convert the ELE lifecycle to user lifecycle
 * @lifecycle: Pointer to the lifecycle string
 * @lifecycle_len: Length of @lifecycle
 * @ele_lifecycle: ELE lifecycle(s) bit mask
 *
 * On success, the function allocates the @lifecycle buffer and returns its
 * length in the @lifecycle_len.
 *
 * Return:
 * SMW_STATUS_OK                         - Success
 * SMW_STATUS_INVALID_PARAM              - Invalid parameters
 * SMW_STATUS_ALLOC_FAILURE              - Memory allocation failure
 * SMW_STATUS_OPERATION_FAILURE          - Unexpected operation failure
 */
int ele_get_lifecycle(unsigned char **lifecycle, unsigned int *lifecycle_len,
		      hsm_key_lifecycle_t ele_lifecycle);

#endif /* __COMMON_H__ */
