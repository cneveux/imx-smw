/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright 2021-2022 NXP
 */

#ifndef __LIBOBJ_TYPES_H__
#define __LIBOBJ_TYPES_H__

#include <assert.h>

#include "types.h"

/**
 * struct libobj_obj - Definition of an object element of a object list
 * @lock: Mutex to lock object when used
 * @class: Object class
 * @object: Pointer to the object (type depend on the class)
 * @prev: Previous element of the list
 * @next: Next element of the list
 */
struct libobj_obj {
	CK_VOID_PTR lock;
	CK_OBJECT_CLASS class;
	void *object;
	struct libobj_obj *prev;
	struct libobj_obj *next;
};

struct libobj_storage {
	bool token;
	bool private;
	bool modifiable;
	bool copyable;
	bool destroyable;
	struct librfc2279 label;
	struct librfc2279 unique_id;
	void *subobject;
};

#define get_object_from(obj) ((obj)->object)

#define get_subobj_from(obj, type)                                             \
	({                                                                     \
		struct libobj_##type *_obj_type = get_object_from(obj);        \
		assert(_obj_type);                                             \
		_obj_type->subobject;                                          \
	})

#define set_subobj_to(obj, type, subobj)                                       \
	({                                                                     \
		struct libobj_##type *_obj_type = get_object_from(obj);        \
		assert(_obj_type);                                             \
		_obj_type->subobject = subobj;                                 \
	})

#define is_token_obj(obj, type)                                                \
	({                                                                     \
		struct libobj_##type *_obj_type = get_object_from(obj);        \
		assert(_obj_type);                                             \
		_obj_type->token;                                              \
	})

#define set_token_obj(obj, type)                                               \
	({                                                                     \
		struct libobj_##type *_obj_type = get_object_from(obj);        \
		assert(_obj_type);                                             \
		_obj_type->token = true;                                       \
	})

#define is_destroyable_obj(obj, type)                                          \
	({                                                                     \
		struct libobj_##type *_obj_type = get_object_from(obj);        \
		assert(_obj_type);                                             \
		_obj_type->destroyable;                                        \
	})

#define is_modifiable_obj(obj, type)                                           \
	({                                                                     \
		struct libobj_##type *_obj_type = get_object_from(obj);        \
		assert(_obj_type);                                             \
		_obj_type->modifiable;                                         \
	})

#define is_private_obj(obj, type)                                              \
	({                                                                     \
		struct libobj_##type *_obj_type = get_object_from(obj);        \
		assert(_obj_type);                                             \
		_obj_type->private;                                            \
	})

#define get_unique_id_obj(obj, type)                                           \
	({                                                                     \
		struct libobj_##type *_obj_type = get_object_from(obj);        \
		assert(_obj_type);                                             \
		&_obj_type->unique_id;                                         \
	})

struct libobj_key {
	CK_KEY_TYPE type;
	struct libbytes id;
	CK_DATE start_date;
	CK_DATE end_date;
	bool derive;
	bool local;
	CK_MECHANISM_TYPE gen_mech;
	struct libmech_list mech;
	void *key;
	void *subkey;
};

#define set_key_to(obj, ptr)                                                   \
	({                                                                     \
		struct libobj_key *_key = get_subobj_from(obj, storage);       \
		assert(_key);                                                  \
		_key->key = ptr;                                               \
	})

#define set_subkey_to(obj, ptr)                                                \
	({                                                                     \
		struct libobj_key *_key = get_subobj_from(obj, storage);       \
		assert(_key);                                                  \
		_key->subkey = ptr;                                            \
	})

#define get_key_type(obj)                                                      \
	({                                                                     \
		struct libobj_key *_key = get_subobj_from(obj, storage);       \
		assert(_key);                                                  \
		_key->type;                                                    \
	})

#define get_key_from(obj)                                                      \
	({                                                                     \
		struct libobj_key *_key = get_subobj_from(obj, storage);       \
		assert(_key);                                                  \
		_key->key;                                                     \
	})

#define get_subkey_from(obj)                                                   \
	({                                                                     \
		struct libobj_key *_key = get_subobj_from(obj, storage);       \
		assert(_key);                                                  \
		_key->subkey;                                                  \
	})

struct libobj_key_ec_pair {
	unsigned long long key_id;
	unsigned int type;
	struct libbytes params;
	struct libbytes point_q;     // Public Key point
	struct libbignumber value_d; // Secure Key scalar
};

struct libobj_key_rsa_pair {
	unsigned long long key_id;
	unsigned int type;
	struct libbignumber modulus;  // Modulus n
	CK_ULONG modulus_length;      // Modulus length in bits
	struct libbignumber pub_exp;  // Public Exponent e
	struct libbignumber priv_exp; // Private Exponent d
	struct libbignumber prime_p;  // Private Prime p
	struct libbignumber prime_q;  // Private Prime q
	struct libbignumber exp_dp;   // Private exponent d modulo p-1
	struct libbignumber exp_dq;   // Private exponent d modulo q-1
	struct libbignumber coeff;    // Private CRT coefficient q^-1 mod p
};

struct libobj_key_cipher {
	unsigned long long key_id;
	struct libbytes value;
	size_t value_len;
};

#define get_key_id_from(obj, type)                                             \
	({                                                                     \
		struct libobj_key_##type *_key = get_subkey_from(obj);         \
		assert(_key);                                                  \
		_key->key_id;                                                  \
	})

struct libobj_data {
	struct librfc2279 application; // Application managing object
	struct libbytes id;	       // Object identifier
	struct libbytes value;	       // Value of the object
};

#endif /* __LIBOBJ_TYPES_H__ */
