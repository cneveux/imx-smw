// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright 2021 NXP
 */

#include <stdlib.h>

#include "attributes.h"
#include "key_rsa.h"

#include "lib_device.h"
#include "libobj_types.h"
#include "util.h"

#include "trace.h"

#define RSA_KEY_PUBLIC	BIT(0)
#define RSA_KEY_PRIVATE BIT(1)
#define RSA_KEY_PAIR	(RSA_KEY_PUBLIC | RSA_KEY_PRIVATE)

enum attr_key_rsa_public_list {
	PUB_MODULUS = 0,
	PUB_MODULUS_LEN,
	PUB_PUB_EXP,
};

const struct template_attr attr_key_rsa_public[] = {
	[PUB_MODULUS] =
		TATTR(key_rsa_pair, modulus, MODULUS, 0, MUST, bignumber),
	[PUB_MODULUS_LEN] = TATTR(key_rsa_pair, modulus_length, MODULUS_BITS, 0,
				  MUST_NOT, ulong),
	[PUB_PUB_EXP] = TATTR(key_rsa_pair, pub_exp, PUBLIC_EXPONENT, 0, MUST,
			      bignumber),
};

enum attr_key_rsa_private_list {
	PRIV_MODULUS = 0,
	PRIV_PUB_EXP,
	PRIV_PRIV_EXP,
	PRIV_PRIME_P,
	PRIV_PRIME_Q,
	PRIV_EXP_DP,
	PRIV_EXP_DQ,
	PRIV_COEFF,
};

const struct template_attr attr_key_rsa_private[] = {
	[PRIV_MODULUS] =
		TATTR(key_rsa_pair, modulus, MODULUS, 0, MUST, bignumber),
	[PRIV_PUB_EXP] = TATTR(key_rsa_pair, pub_exp, PUBLIC_EXPONENT, 0,
			       OPTIONAL, bignumber),
	[PRIV_PRIV_EXP] = TATTR_P(key_rsa_pair, priv_exp, PRIVATE_EXPONENT, 0,
				  MUST, bignumber),
	[PRIV_PRIME_P] =
		TATTR_P(key_rsa_pair, prime_p, PRIME_1, 0, OPTIONAL, bignumber),
	[PRIV_PRIME_Q] =
		TATTR_P(key_rsa_pair, prime_q, PRIME_2, 0, OPTIONAL, bignumber),
	[PRIV_EXP_DP] = TATTR_P(key_rsa_pair, exp_dp, EXPONENT_1, 0, OPTIONAL,
				bignumber),
	[PRIV_EXP_DQ] = TATTR_P(key_rsa_pair, exp_dq, EXPONENT_2, 0, OPTIONAL,
				bignumber),
	[PRIV_COEFF] = TATTR_P(key_rsa_pair, coeff, COEFFICIENT, 0, OPTIONAL,
			       bignumber),
};

/**
 * key_rsa_allocate() - Allocate and initialize RSA keypair
 * @pub_obj: RSA Public key object
 * @priv_obj: RSA Private key object
 * @type: Type of Key to allocate
 *
 * Allocation and set the @type of key to allocate which is:
 *   RSA_KEY_PUBLIC
 *   RSA_KEY_PRIVATE
 *   RSA_KEY_PAIR
 *
 * return:
 * Key allocated if success
 * NULL otherwise
 */
static struct libobj_key_rsa_pair *key_rsa_allocate(struct libobj_obj *pub_obj,
						    struct libobj_obj *priv_obj,
						    unsigned int type)
{
	struct libobj_key_rsa_pair *key = NULL;

	key = calloc(1, sizeof(*key));
	if (key) {
		key->type = type;

		if (type & RSA_KEY_PUBLIC)
			set_subkey_to(pub_obj, key);

		if (type & RSA_KEY_PRIVATE)
			set_subkey_to(priv_obj, key);
	}

	DBG_TRACE("Allocated a new RSA key (%p) of type %d", key, type);

	return key;
}

/**
 * key_rsa_free() - Free private or public key
 * @obj: RSA Keypair object
 * @type: Type of key private/public to free
 *
 * Free the key's field related to the request @type.
 *
 * Then, if the requested key @type to free is the same of the @key type:
 *    - Delete the key from SMW subsystem if key'id set
 *    - Free the keypair common fields
 *    - Free the keypair object itself
 *
 * Else key is a keypair, hence switch the key type to the remaining
 * key type part not freed.
 */
static void key_rsa_free(struct libobj_obj *obj, unsigned int type)
{
	struct libobj_key_rsa_pair *key = get_subkey_from(obj);

	if (!key)
		return;

	switch (type) {
	case RSA_KEY_PUBLIC:
		break;

	case RSA_KEY_PRIVATE:
		if (key->priv_exp.value) {
			free(key->priv_exp.value);
			key->priv_exp.value = NULL;
		}
		if (key->prime_p.value) {
			free(key->prime_p.value);
			key->prime_p.value = NULL;
		}
		if (key->prime_q.value) {
			free(key->prime_q.value);
			key->prime_q.value = NULL;
		}
		if (key->exp_dp.value) {
			free(key->exp_dp.value);
			key->exp_dp.value = NULL;
		}
		if (key->exp_dq.value) {
			free(key->exp_dq.value);
			key->exp_dq.value = NULL;
		}
		if (key->coeff.value) {
			free(key->coeff.value);
			key->coeff.value = NULL;
		}
		break;

	default:
		return;
	}

	if (key->type == type) {
		if (key->modulus.value)
			free(key->modulus.value);
		if (key->pub_exp.value)
			free(key->pub_exp.value);

		(void)libdev_delete_key(key->key_id);

		free(key);
	} else {
		key->type &= ~type;
	}

	set_subkey_to(obj, NULL);
}

void key_rsa_public_free(struct libobj_obj *obj)
{
	key_rsa_free(obj, RSA_KEY_PUBLIC);
}

void key_rsa_private_free(struct libobj_obj *obj)
{
	key_rsa_free(obj, RSA_KEY_PRIVATE);
}

CK_RV key_rsa_public_create(CK_SESSION_HANDLE hsession, struct libobj_obj *obj,
			    struct libattr_list *attrs)
{
	CK_RV ret;
	struct libobj_key_rsa_pair *new_key;

	new_key = key_rsa_allocate(obj, NULL, RSA_KEY_PUBLIC);
	if (!new_key)
		return CKR_HOST_MEMORY;

	DBG_TRACE("Create a new RSA public key (%p)", new_key);

	ret = attr_get_value(new_key, &attr_key_rsa_public[PUB_MODULUS], attrs,
			     NO_OVERWRITE);
	if (ret != CKR_OK)
		goto end;

	ret = attr_get_value(new_key, &attr_key_rsa_public[PUB_MODULUS_LEN],
			     attrs, NO_OVERWRITE);
	if (ret != CKR_OK)
		goto end;

	ret = attr_get_value(new_key, &attr_key_rsa_public[PUB_PUB_EXP], attrs,
			     NO_OVERWRITE);
	if (ret != CKR_OK)
		goto end;

	ret = libdev_import_key(hsession, obj);
	DBG_TRACE("Public Key ID 0x%llX", new_key->key_id);

end:
	if (ret != CKR_OK)
		key_rsa_public_free(obj);

	return ret;
}

CK_RV key_rsa_public_get_attribute(CK_ATTRIBUTE_PTR attr,
				   const struct libobj_obj *obj)
{
	CK_RV ret;

	DBG_TRACE("Get attribute type=%#lx", attr->type);

	ret = attr_get_obj_value(attr, attr_key_rsa_public,
				 ARRAY_SIZE(attr_key_rsa_public),
				 get_subkey_from(obj));
	if (ret == CKR_ATTRIBUTE_TYPE_INVALID)
		attr->ulValueLen = CK_UNAVAILABLE_INFORMATION;

	DBG_TRACE("Get attribute type=%#lx ret %ld", attr->type, ret);
	return ret;
}

CK_RV key_rsa_public_modify_attribute(CK_ATTRIBUTE_PTR attr,
				      struct libobj_obj *obj)
{
	CK_RV ret;

	DBG_TRACE("Modify attribute type=%#lx", attr->type);

	ret = attr_modify_obj_value(attr, attr_key_rsa_public,
				    ARRAY_SIZE(attr_key_rsa_public),
				    get_subkey_from(obj));

	DBG_TRACE("Modify attribute type=%#lx ret %ld", attr->type, ret);
	return ret;
}

CK_RV key_rsa_private_create(CK_SESSION_HANDLE hsession, struct libobj_obj *obj,
			     struct libattr_list *attrs)
{
	CK_RV ret;
	struct libobj_key_rsa_pair *new_key;

	new_key = key_rsa_allocate(NULL, obj, RSA_KEY_PRIVATE);
	if (!new_key)
		return CKR_HOST_MEMORY;

	DBG_TRACE("Create a new RSA private key (%p)", new_key);

	ret = attr_get_value(new_key, &attr_key_rsa_private[PRIV_MODULUS],
			     attrs, NO_OVERWRITE);
	if (ret != CKR_OK)
		goto end;

	ret = attr_get_value(new_key, &attr_key_rsa_private[PRIV_MODULUS],
			     attrs, NO_OVERWRITE);
	if (ret != CKR_OK)
		goto end;

	/*
	 * Private key public exponent can be omitted, in this case
	 * the token must use the default value 65537. otherwise it
	 * must return an error.
	 */
	ret = attr_get_value(new_key, &attr_key_rsa_private[PRIV_PUB_EXP],
			     attrs, NO_OVERWRITE);
	if (ret != CKR_OK)
		goto end;

	ret = attr_get_value(new_key, &attr_key_rsa_private[PRIV_PRIV_EXP],
			     attrs, NO_OVERWRITE);
	if (ret != CKR_OK)
		goto end;

	ret = attr_get_value(new_key, &attr_key_rsa_private[PRIV_PRIME_P],
			     attrs, NO_OVERWRITE);
	if (ret != CKR_OK)
		goto end;

	ret = attr_get_value(new_key, &attr_key_rsa_private[PRIV_PRIME_Q],
			     attrs, NO_OVERWRITE);
	if (ret != CKR_OK)
		goto end;

	ret = attr_get_value(new_key, &attr_key_rsa_private[PRIV_EXP_DP], attrs,
			     NO_OVERWRITE);
	if (ret != CKR_OK)
		goto end;

	ret = attr_get_value(new_key, &attr_key_rsa_private[PRIV_EXP_DQ], attrs,
			     NO_OVERWRITE);
	if (ret != CKR_OK)
		goto end;

	ret = attr_get_value(new_key, &attr_key_rsa_private[PRIV_COEFF], attrs,
			     NO_OVERWRITE);
	if (ret != CKR_OK)
		goto end;

	ret = libdev_import_key(hsession, obj);
	DBG_TRACE("Private Key ID 0x%llX", new_key->key_id);

end:
	if (ret != CKR_OK)
		key_rsa_private_free(obj);

	return ret;
}

CK_RV key_rsa_private_get_attribute(CK_ATTRIBUTE_PTR attr,
				    const struct libobj_obj *obj, bool protect)
{
	CK_RV ret;

	DBG_TRACE("Get attribute type=%#lx protected=%s", attr->type,
		  protect ? "YES" : "NO");

	ret = attr_get_obj_prot_value(attr, attr_key_rsa_private,
				      ARRAY_SIZE(attr_key_rsa_private),
				      get_subkey_from(obj), protect);
	if (ret == CKR_ATTRIBUTE_TYPE_INVALID)
		attr->ulValueLen = CK_UNAVAILABLE_INFORMATION;

	DBG_TRACE("Get attribute type=%#lx ret %ld", attr->type, ret);
	return ret;
}

CK_RV key_rsa_private_modify_attribute(CK_ATTRIBUTE_PTR attr,
				       struct libobj_obj *obj)
{
	CK_RV ret;

	DBG_TRACE("Modify attribute type=%#lx", attr->type);

	ret = attr_modify_obj_value(attr, attr_key_rsa_private,
				    ARRAY_SIZE(attr_key_rsa_private),
				    get_subkey_from(obj));

	DBG_TRACE("Modify attribute type=%#lx ret %ld", attr->type, ret);
	return ret;
}

CK_RV key_rsa_keypair_generate(CK_SESSION_HANDLE hsession,
			       CK_MECHANISM_PTR mech,
			       struct libobj_obj *pub_obj,
			       struct libattr_list *pub_attrs,
			       struct libobj_obj *priv_obj,
			       struct libattr_list *priv_attrs)
{
	(void)hsession;
	(void)mech;
	CK_RV ret;
	struct libobj_key_rsa_pair *keypair;

	keypair = key_rsa_allocate(pub_obj, priv_obj, RSA_KEY_PAIR);
	if (!keypair)
		return CKR_HOST_MEMORY;

	DBG_TRACE("Generate a RSA keypair (%p)", keypair);

	/* Verify the public key attributes */
	ret = attr_get_value(keypair, &attr_key_rsa_public[PUB_MODULUS],
			     pub_attrs, MUST_NOT);
	if (ret != CKR_OK)
		goto end;

	ret = attr_get_value(keypair, &attr_key_rsa_public[PUB_MODULUS_LEN],
			     pub_attrs, MUST);
	if (ret != CKR_OK)
		goto end;

	ret = attr_get_value(keypair, &attr_key_rsa_public[PUB_PUB_EXP],
			     pub_attrs, OPTIONAL);
	if (ret != CKR_OK)
		goto end;

	/* Verify the private key attributes */
	ret = attr_get_value(keypair, &attr_key_rsa_private[PRIV_MODULUS],
			     priv_attrs, MUST_NOT);
	if (ret != CKR_OK)
		goto end;

	ret = attr_get_value(keypair, &attr_key_rsa_private[PRIV_PUB_EXP],
			     priv_attrs, MUST_NOT);
	if (ret != CKR_OK)
		goto end;

	ret = attr_get_value(keypair, &attr_key_rsa_private[PRIV_PRIV_EXP],
			     priv_attrs, MUST_NOT);
	if (ret != CKR_OK)
		goto end;

	ret = attr_get_value(keypair, &attr_key_rsa_private[PRIV_PRIME_P],
			     priv_attrs, MUST_NOT);
	if (ret != CKR_OK)
		goto end;

	ret = attr_get_value(keypair, &attr_key_rsa_private[PRIV_PRIME_Q],
			     priv_attrs, MUST_NOT);
	if (ret != CKR_OK)
		goto end;

	ret = attr_get_value(keypair, &attr_key_rsa_private[PRIV_EXP_DP],
			     priv_attrs, MUST_NOT);
	if (ret != CKR_OK)
		goto end;

	ret = attr_get_value(keypair, &attr_key_rsa_private[PRIV_EXP_DQ],
			     priv_attrs, MUST_NOT);
	if (ret != CKR_OK)
		goto end;

	ret = attr_get_value(keypair, &attr_key_rsa_private[PRIV_COEFF],
			     priv_attrs, MUST_NOT);
	if (ret != CKR_OK)
		goto end;

	ret = libdev_operate_mechanism(hsession, mech, priv_obj);
	DBG_TRACE("Key Pair ID 0x%llX", keypair->key_id);

end:
	if (ret != CKR_OK) {
		key_rsa_public_free(pub_obj);
		key_rsa_private_free(priv_obj);
	}

	return ret;
}

CK_RV key_rsa_get_id(struct libbytes *id, struct libobj_obj *obj,
		     size_t prefix_len)
{
	struct libobj_key_rsa_pair *keypair;

	if (!obj || !id)
		return CKR_GENERAL_ERROR;

	keypair = get_subkey_from(obj);

	id->number = prefix_len + sizeof(keypair->key_id);
	id->array = malloc(id->number);
	if (!id->array)
		return CKR_HOST_MEMORY;

	DBG_TRACE("RSA Key ID 0x%llX", keypair->key_id);

	TO_CK_BYTES(&id->array[prefix_len], keypair->key_id);

	return CKR_OK;
}
