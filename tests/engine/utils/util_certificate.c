// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright 2023 NXP
 */

#include <stdlib.h>

#include "util.h"
#include "util_certificate.h"

/**
 * struct certificate_data - Data of certificate linked list node.
 * @certificate: Buffer containing the certificate generated by SMW.
 * @certificate_length: Length of the certificate.
 */
struct certificate_data {
	unsigned char *certificate;
	unsigned int certificate_length;
};

static void certificate_free_data(void *data)
{
	struct certificate_data *certificate_data = data;

	if (certificate_data) {
		if (certificate_data->certificate)
			free(certificate_data->certificate);

		free(certificate_data);
	}
}

int util_certificate_init(struct llist **list)
{
	if (!list)
		return ERR_CODE(BAD_ARGS);

	return util_list_init(list, &certificate_free_data, LIST_ID_TYPE_UINT);
}

int util_certificate_add_node(struct llist *list, unsigned int id,
			      unsigned char *certificate,
			      unsigned int certificate_length)
{
	int res = ERR_CODE(BAD_ARGS);
	struct certificate_data *data = NULL;

	if (!list)
		return res;

	data = malloc(sizeof(*data));
	if (!data) {
		DBG_PRINT_ALLOC_FAILURE();
		return ERR_CODE(INTERNAL_OUT_OF_MEMORY);
	}

	data->certificate = certificate;
	data->certificate_length = certificate_length;

	res = util_list_add_node(list, id, data);

	if (res != ERR_CODE(PASSED) && data)
		free(data);

	return res;
}

int util_certificate_find_node(struct llist *list, unsigned int id,
			       unsigned char **certificate,
			       unsigned int *certificate_length)
{
	int res = ERR_CODE(BAD_ARGS);
	struct certificate_data *data = NULL;

	if (!list || !certificate || !certificate_length)
		return res;

	res = util_list_find_node(list, id, (void **)&data);
	if (res == ERR_CODE(PASSED) && !data)
		return ERR_CODE(FAILED);

	if (res == ERR_CODE(PASSED)) {
		*certificate = data->certificate;
		*certificate_length = data->certificate_length;
	}

	return res;
}
