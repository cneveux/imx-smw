// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright 2020-2022 NXP
 */

#include <string.h>

#include "smw_osal.h"

#include "util_sem.h"
#include "util_thread.h"
#include "keymgr.h"
#include "hash.h"
#include "sign_verify.h"
#include "hmac.h"
#include "rng.h"
#include "cipher.h"
#include "operation_context.h"
#include "config.h"
#include "info.h"

#define SUBTEST_STATUS_PASSED_MAX_LEN 30

/**
 * execute_delete_key_cmd() - Execute delete key command.
 * @cmd: Command name.
 * @params: Command parameters.
 * @cmn_params: Some parameters common to commands.
 * @status: Pointer to SMW command status.
 *
 * Return:
 * PASSED		- Passed.
 * Error code from delete_key().
 */
static int execute_delete_key_cmd(char *cmd, struct json_object *params,
				  struct cmn_params *cmn_params,
				  enum smw_status_code *status)
{
	(void)cmd;

	return delete_key(params, cmn_params, status);
}

/**
 * execute_generate_cmd() - Execute generate key command.
 * @cmd: Command name.
 * @params: Command parameters.
 * @cmn_params: Some parameters common to commands.
 * @status: Pointer to SMW command status.
 *
 * Return:
 * PASSED          - Passed.
 * -MISSING_PARAMS - Subsystem missing
 * Error code from generate_key().
 */
static int execute_generate_cmd(char *cmd, struct json_object *params,
				struct cmn_params *cmn_params,
				enum smw_status_code *status)
{
	(void)cmd;

	/* Check mandatory params */
	if (!cmn_params->subsystem) {
		DBG_PRINT_MISS_PARAM("subsystem");
		return ERR_CODE(MISSING_PARAMS);
	}

	return generate_key(params, cmn_params, status);
}

/**
 * execute_hash_cmd() - Execute hash command.
 * @cmd: Command name.
 * @params: Command parameters.
 * @cmn_params: Some parameters common to commands.
 * @status: Pointer to SMW command status.
 *
 * Return:
 * PASSED		- Passed.
 * -UNDEFINED_CMD	- Command is undefined.
 * Error code from hash().
 */
static int execute_hash_cmd(char *cmd, struct json_object *params,
			    struct cmn_params *cmn_params,
			    enum smw_status_code *status)
{
	(void)cmd;

	/* Check mandatory params */
	if (!cmn_params->subsystem) {
		DBG_PRINT_MISS_PARAM("subsystem");
		return ERR_CODE(MISSING_PARAMS);
	}

	return hash(params, cmn_params, status);
}

/**
 * execute_hmac_cmd() - Execute hmac command.
 * @cmd: Command name.
 * @params: Command parameters.
 * @cmn_params: Some parameters common to commands.
 * @status: Pointer to SMW command status.
 *
 * Return:
 * PASSED		- Passed.
 * -UNDEFINED_CMD	- Command is undefined.
 * Error code from hmac().
 */
static int execute_hmac_cmd(char *cmd, struct json_object *params,
			    struct cmn_params *cmn_params,
			    enum smw_status_code *status)
{
	(void)cmd;

	/* Check mandatory params */
	if (!cmn_params->subsystem) {
		DBG_PRINT_MISS_PARAM("subsystem");
		return ERR_CODE(MISSING_PARAMS);
	}

	return hmac(params, cmn_params, status);
}

/**
 * execute_import_cmd() - Execute import key command.
 * @cmd: Command name.
 * @params: Command parameters.
 * @cmn_params: Some parameters common to commands.
 * @status: Pointer to SMW command status.
 *
 * Return:
 * PASSED		- Passed.
 * -UNDEFINED_CMD	- Command is undefined.
 * Error code from import_key().
 */
static int execute_import_cmd(char *cmd, struct json_object *params,
			      struct cmn_params *cmn_params,
			      enum smw_status_code *status)
{
	(void)cmd;

	/* Check mandatory params */
	if (!cmn_params->subsystem) {
		DBG_PRINT_MISS_PARAM("subsystem");
		return ERR_CODE(MISSING_PARAMS);
	}

	return import_key(params, cmn_params, status);
}

/**
 * execute_export_cmd() - Execute export command.
 * @cmd: Command name.
 * @params: Command parameters.
 * @cmn_params: Some parameters common to commands.
 * @status: Pointer to SMW command status.
 *
 * Return:
 * PASSED		- Passed.
 * -UNDEFINED_CMD	- Command is undefined.
 * Error code from export_key().
 */
static int execute_export_cmd(char *cmd, struct json_object *params,
			      struct cmn_params *cmn_params,
			      enum smw_status_code *status)
{
	if (!strcmp(cmd, EXPORT_KEYPAIR))
		return export_key(params, cmn_params, EXP_KEYPAIR, status);
	else if (!strcmp(cmd, EXPORT_PRIVATE))
		return export_key(params, cmn_params, EXP_PRIV, status);
	else if (!strcmp(cmd, EXPORT_PUBLIC))
		return export_key(params, cmn_params, EXP_PUB, status);

	DBG_PRINT("Undefined command");
	return ERR_CODE(UNDEFINED_CMD);
}

/**
 * execute_derive_cmd() - Execute derive command.
 * @cmd: Command name.
 * @params: Command parameters.
 * @cmn_params: Some parameters common to commands.
 * @status: Pointer to SMW command status.
 *
 * Return:
 * PASSED		- Passed.
 * -UNDEFINED_CMD	- Command is undefined.
 * Error code from derive_key().
 */
static int execute_derive_cmd(char *cmd, struct json_object *params,
			      struct cmn_params *cmn_params,
			      enum smw_status_code *status)
{
	(void)cmd;

	return derive_key(params, cmn_params, status);
}

/**
 * execute_sign_cmd() - Execute sign command.
 * @cmd: Command name.
 * @params: Command parameters.
 * @cmn_params: Some parameters common to commands.
 * @status: Pointer to SMW command status.
 *
 * Return:
 * PASSED		- Passed.
 * -UNDEFINED_CMD	- Command is undefined.
 * Error code from sign_verify().
 */
static int execute_sign_cmd(char *cmd, struct json_object *params,
			    struct cmn_params *cmn_params,
			    enum smw_status_code *status)
{
	(void)cmd;

	/* Check mandatory params */
	if (!cmn_params->subsystem) {
		DBG_PRINT_MISS_PARAM("subsystem");
		return ERR_CODE(MISSING_PARAMS);
	}

	return sign_verify(SIGN_OPERATION, params, cmn_params, status);
}

/**
 * execute_sign_verify_cmd() - Execute sign or verify command.
 * @cmd: Command name.
 * @params: Command parameters.
 * @cmn_params: Some parameters common to commands.
 * @status: Pointer to SMW command status.
 *
 * Return:
 * PASSED		- Passed.
 * -UNDEFINED_CMD	- Command is undefined.
 * Error code from sign_verify().
 */
static int execute_verify_cmd(char *cmd, struct json_object *params,
			      struct cmn_params *cmn_params,
			      enum smw_status_code *status)
{
	(void)cmd;

	/* Check mandatory params */
	if (!cmn_params->subsystem) {
		DBG_PRINT_MISS_PARAM("subsystem");
		return ERR_CODE(MISSING_PARAMS);
	}

	return sign_verify(VERIFY_OPERATION, params, cmn_params, status);
}

/**
 * execute_rng_cmd() - Execute RNG command.
 * @cmd: Command name.
 * @params: Command parameters.
 * @cmn_params: Some parameters common to commands.
 * @status: Pointer to SMW command status.
 *
 * Return:
 * PASSED		- Passed.
 * -UNDEFINED_CMD	- Command is undefined.
 * Error code from hash().
 */
static int execute_rng_cmd(char *cmd, struct json_object *params,
			   struct cmn_params *cmn_params,
			   enum smw_status_code *status)
{
	(void)cmd;

	/* Check mandatory params */
	if (!cmn_params->subsystem) {
		DBG_PRINT_MISS_PARAM("subsystem");
		return ERR_CODE(MISSING_PARAMS);
	}

	return rng(params, cmn_params, status);
}

/**
 * execute_cipher_cmd() - Execute cipher command
 * @cmd: Command name.
 * @params: Command parameters.
 * @cmn_params: Some parameters common to commands.
 * @status: Pointer to SMW command status.
 *
 * PASSED		- Passed.
 * -UNDEFINED_CMD	- Command is undefined.
 * Error code from cipher().
 * Error code from cipher_init().
 * Error code from cipher_update().
 * Error code from cipher_final().
 */
static int execute_cipher_cmd(char *cmd, struct json_object *params,
			      struct cmn_params *cmn_params,
			      enum smw_status_code *status)
{
	if (!strcmp(cmd, CIPHER))
		return cipher(params, cmn_params, status);
	else if (!strcmp(cmd, CIPHER_INIT))
		return cipher_init(params, cmn_params, status);
	else if (!strcmp(cmd, CIPHER_UPDATE))
		return cipher_update(params, cmn_params, status);
	else if (!strcmp(cmd, CIPHER_FINAL))
		return cipher_final(params, cmn_params, status);

	DBG_PRINT("Undefined command");
	return ERR_CODE(UNDEFINED_CMD);
}

/**
 * execute_operation_context() - Execute an operation context operation
 * @cmd: Command name.
 * @params: Command parameters.
 * @cmn_params: Some parameters common to commands.
 * @status: Pointer to SMW command status.
 *
 * Return:
 * PASSED		- Passed.
 * -UNDEFINED_CMD	- Command is undefined.
 * Error code from cancel_operation().
 * Error code from copy_context().
 */
static int execute_op_context_cmd(char *cmd, struct json_object *params,
				  struct cmn_params *cmn_params,
				  enum smw_status_code *status)
{
	if (!strcmp(cmd, OP_CTX_CANCEL))
		return cancel_operation(params, cmn_params, status);
	else if (!strcmp(cmd, OP_CTX_COPY))
		return copy_context(params, cmn_params, status);

	DBG_PRINT("Undefined command");
	return ERR_CODE(UNDEFINED_CMD);
}

/**
 * execute_config_cmd() - Execute configuration load or unload command.
 * @cmd: Command name.
 * @params: Command parameters.
 * @cmn_params: Some parameters common to commands.
 * @status: Pointer to SMW command status.
 *
 * Return:
 * PASSED		- Passed.
 * -UNDEFINED_CMD	- Command is undefined.
 * Error code from smw_config_load() and smw_config_unload().
 */
static int execute_config_cmd(char *cmd, struct json_object *params,
			      struct cmn_params *cmn_params,
			      enum smw_status_code *status)
{
	if (!strcmp(cmd, CONFIG_LOAD))
		return config_load(params, cmn_params, status);
	if (!strcmp(cmd, CONFIG_UNLOAD))
		return config_unload(params, cmn_params, status);

	DBG_PRINT("Undefined command");
	return ERR_CODE(UNDEFINED_CMD);
}

/**
 * execute_save_keys_cmd() - Execute backup keys ids in a file
 * @cmd: Command name.
 * @params: Command parameters.
 * @cmn_params: Some parameters common to commands.
 * @status: Pointer to SMW command status.
 *
 * Return:
 * PASSED		- Passed.
 * -MISSING_PARAMS	- Missing mandatory parameters in @params.
 * -BAD_RESULT		- SMW API status differs from expected one.
 * -BAD_ARGS		- One of the arguments is bad.
 * -UNDEFINED_CMD	- Command is undefined.
 */
static int execute_save_keys_cmd(char *cmd, struct json_object *params,
				 struct cmn_params *cmn_params,
				 enum smw_status_code *status)
{
	(void)cmd;

	return save_key_ids_to_file(params, cmn_params, status);
}

/**
 * execute_restore_keys_cmd() - Execute restore keys ids from a file
 * @cmd: Command name.
 * @params: Command parameters.
 * @cmn_params: Some parameters common to commands.
 * @status: Pointer to SMW command status.
 *
 * Return:
 * PASSED                  - Success.
 * -MISSING_PARAMS         - Missing mandatory parameters in @params.
 * -BAD_RESULT             - SMW API status differs from expected one.
 * -BAD_ARGS               - One of the arguments is bad.
 * -INTERNAL_OUT_OF_MEMORY - Memory allocation failed.
 */
static int execute_restore_keys_cmd(char *cmd, struct json_object *params,
				    struct cmn_params *cmn_params,
				    enum smw_status_code *status)
{
	(void)cmd;

	return restore_key_ids_from_file(params, cmn_params, status);
}

/**
 * execute_get_version_cmd() - Execute get version commands
 * @cmd: Command name.
 * @params: Command parameters.
 * @cmn_params: Some parameters common to commands.
 * @status: Pointer to SMW command status.
 *
 * Return:
 * PASSED		- Success.
 * -BAD_RESULT		- SMW API status differs from expected one.
 * -BAD_ARGS		- One of the arguments is bad.
 * -BAD_PARAM_TYPE	- A parameter value is undefined.
 * -VALUE_NOTFOUND	- Test definition Value not found.
 * -FAILED		- Test failed
 */
static int execute_get_version_cmd(char *cmd, struct json_object *params,
				   struct cmn_params *cmn_params,
				   enum smw_status_code *status)
{
	(void)cmd;

	return get_info(params, cmn_params, status);
}

/**
 * execute_command() - Execute a subtest command.
 * @cmd: Command name.
 * @params: Command parameters.
 * @cmn_params: Some parameters common to commands.
 * @status: Pointer to SMW command status.
 *
 * Return:
 * PASSED		- Passed.
 * -UNDEFINED_CMD	- Command is undefined.
 * Other error code otherwise.
 */
static int execute_command(char *cmd, struct json_object *params,
			   struct cmn_params *cmn_params,
			   enum smw_status_code *status)
{
	static struct cmd_op {
		const char *cmd_prefix;
		int (*op)(char *cmd, struct json_object *params,
			  struct cmn_params *cmn_params,
			  enum smw_status_code *status);
	} cmd_list[] = {
		{ DELETE, &execute_delete_key_cmd },
		{ GENERATE, &execute_generate_cmd },
		{ IMPORT, &execute_import_cmd },
		{ EXPORT, &execute_export_cmd },
		{ DERIVE, &execute_derive_cmd },
		{ HASH, &execute_hash_cmd },
		{ HMAC, &execute_hmac_cmd },
		{ SIGN, &execute_sign_cmd },
		{ VERIFY, &execute_verify_cmd },
		{ RNG, &execute_rng_cmd },
		{ CIPHER, &execute_cipher_cmd },
		{ OP_CTX, &execute_op_context_cmd },
		{ CONFIG, &execute_config_cmd },
		{ SAVE_KEY_IDS, &execute_save_keys_cmd },
		{ RESTORE_KEY_IDS, &execute_restore_keys_cmd },
		{ GET_VERSION, &execute_get_version_cmd },
	};

	for (size_t idx = 0; idx < ARRAY_SIZE(cmd_list); idx++) {
		if (!strncmp(cmd, cmd_list[idx].cmd_prefix,
			     strlen(cmd_list[idx].cmd_prefix))) {
			return cmd_list[idx].op(cmd, params, cmn_params,
						status);
		}
	}

	DBG_PRINT("Undefined command");
	return ERR_CODE(UNDEFINED_CMD);
}

/**
 * log_subtest_status() - Log the subtest status
 * @thr: Thread data
 * @obj: Subtest operation object
 * @res: Result of the subtest.
 * @status: SMW Library status
 *
 * Log the subtest name and the reason of the failure if any.
 * Function returns if overall test passed or failed.
 *
 * Return:
 * PASSED - subtest passed
 * -FAILED - subtest failed
 */
static int log_subtest_status(struct thread_data *thr,
			      struct json_object_iter *obj, int res,
			      enum smw_status_code status)
{
	int ret = FAILED;
	unsigned int idx = 0;
	const char *error = NULL;

	/* Find the error entry in the array of error string */
	for (; idx < list_err_size && res != ERR_CODE(idx); idx++)
		;

	switch (idx) {
	case PASSED:
		ret = PASSED;
		break;

	case BAD_RESULT:
		error = get_smw_string_status(status);
		break;

	default:
		if (idx < list_err_size)
			error = ERR_STATUS(idx);
		else
			error = ERR_STATUS(INTERNAL);

		break;
	}

	FPRINT_SUBTEST_STATUS(thr->app->log, obj->key, ERR_STATUS(ret), error);
	FPRINT_SUBTEST_STATUS(stdout, obj->key, ERR_STATUS(ret), error);

	return ERR_CODE(ret);
}

/**
 * get_depends_status() - Handle 'depends' test definition parameter.
 * @params: Test definition parameters.
 * @status_file: Pointer to current status file.
 *
 * If 'depends' parameter is set in the test definition file check that
 * associated subtest(s) succeed.
 * 'depends' parameter can be an integer or an array of integer in case there
 * are multiple dependencies.
 * If at least one dependent subtest failed, current subtest is skipped.
 *
 * Return:
 * PASSED		- Success.
 * -BAD_PARAM_TYPE	- Parameter is not correctly set.
 * -INTERNAL		- Internal error.
 * -NOT_RUN		- Subtest is skipped.
 */
static int get_depends_status(struct json_object *params, FILE *status_file)
{
	int i;
	int depends;
	int nb_members = 1;
	long fsave_pos;
	char depends_status[SUBTEST_STATUS_PASSED_MAX_LEN] = { 0 };
	char file_line[SUBTEST_STATUS_PASSED_MAX_LEN] = { 0 };
	char *read;
	json_object *depends_obj = NULL;
	json_object *array_member = NULL;

	if (!json_object_object_get_ex(params, DEPENDS_OBJ, &depends_obj))
		return ERR_CODE(PASSED);

	if (json_object_get_type(depends_obj) == json_type_array) {
		nb_members = json_object_array_length(depends_obj);

		/*
		 * 'depends' parameter must be an array only for multiple
		 * entries. Otherwise it must be an integer
		 */
		if (nb_members <= 1) {
			DBG_PRINT_BAD_PARAM(DEPENDS_OBJ);
			return ERR_CODE(BAD_PARAM_TYPE);
		}
	}

	for (i = 0; i < nb_members; i++) {
		if (nb_members > 1)
			array_member =
				json_object_array_get_idx(depends_obj, i);
		else
			array_member = depends_obj;

		depends = json_object_get_int(array_member);
		if (depends <= 0) {
			DBG_PRINT_BAD_PARAM(DEPENDS_OBJ);
			return ERR_CODE(BAD_PARAM_TYPE);
		}

		/* Expected dependent subtest status */
		if (sprintf(depends_status, "%s%d: PASSED\n", SUBTEST_OBJ,
			    depends) < 0)
			return ERR_CODE(INTERNAL);

		/* Backup current status file position */
		fsave_pos = ftell(status_file);
		if (fsave_pos == -1)
			return ERR_CODE(INTERNAL);

		/* Set status file position to beginning */
		if (fseek(status_file, 0, SEEK_SET))
			return ERR_CODE(INTERNAL);

		depends = 0;

		while (fgets(file_line, sizeof(file_line), status_file)) {
			read = strchr(file_line, '\n');
			if (read)
				*read = '\0';

			if (!strncmp(file_line, depends_status,
				     strlen(file_line))) {
				depends = 1;
				break;
			}
		};

		/* Restore status file position */
		if (fseek(status_file, fsave_pos, SEEK_SET))
			return ERR_CODE(INTERNAL);

		if (!depends)
			return ERR_CODE(NOT_RUN);
	}

	return ERR_CODE(PASSED);
}

/**
 * run_subtest() - Run a subtest.
 * @thr: Thread data
 * @obj: Operation object data
 *
 * Return:
 * PASSED - subtest passed
 * -FAILED - subtest failed
 */
static int run_subtest(struct thread_data *thr, struct json_object_iter *obj)
{
	int res = ERR_CODE(FAILED);
	enum smw_status_code status = SMW_STATUS_OPERATION_FAILURE;
	char *cmd_name = NULL;
	char *expected_status = NULL;
	const char *sub_used = NULL;
	const char *sub_exp = NULL;
	struct cmn_params cmn_params = { 0 };

	cmn_params.app = thr->app;

	/* Verify the type of the subtest tag/value is json object */
	if (json_object_get_type(obj->val) != json_type_object) {
		FPRINT_MESSAGE(thr->app->log, "Error in test definiton file: ");
		FPRINT_MESSAGE(thr->app->log,
			       "\"subtest\" is not a json-c object\n");
		DBG_PRINT("\"subtest\" is not a json-c object");

		res = ERR_CODE(BAD_PARAM_TYPE);
		goto exit;
	}

	/* 'command' is a mandatory parameter */
	res = util_read_json_type(&cmd_name, CMD_OBJ, t_string, obj->val);
	if (res != ERR_CODE(PASSED)) {
		if (res == ERR_CODE(VALUE_NOTFOUND)) {
			DBG_PRINT_MISS_PARAM(CMD_OBJ);
			res = ERR_CODE(MISSING_PARAMS);
		}

		goto exit;
	}

	res = util_read_json_type(&cmn_params.subsystem, SUBSYSTEM_OBJ,
				  t_string, obj->val);
	if (res != ERR_CODE(PASSED) && res != ERR_CODE(VALUE_NOTFOUND))
		goto exit;

	/* Check dependent subtest(s) status */
	res = get_depends_status(obj->val, thr->app->log);
	if (res != ERR_CODE(PASSED))
		goto exit;

	/*
	 * Get expected result parameter 'result' set in test definition file.
	 * If not defined, the default value is SMW_STATUS_OK.
	 */
	res = util_read_json_type(&expected_status, RES_OBJ, t_string,
				  obj->val);
	if (res != ERR_CODE(PASSED) && res != ERR_CODE(VALUE_NOTFOUND))
		goto exit;

	if (expected_status) {
		res = get_smw_int_status(&cmn_params.expected_res,
					 expected_status);

		if (res != ERR_CODE(PASSED))
			goto exit;

	} else {
		cmn_params.expected_res = SMW_STATUS_OK;
	}

	/*
	 * Get SMW API version.
	 * If not set in test definition file, use default value.
	 */
	cmn_params.version = SMW_API_DEFAULT_VERSION;
	res = util_read_json_type(&cmn_params.version, VERSION_OBJ, t_int,
				  obj->val);
	if (res != ERR_CODE(PASSED) && res != ERR_CODE(VALUE_NOTFOUND))
		goto exit;

	/*
	 * Get expected subsystem to be used.
	 * If not set in test definition file don't verify it.
	 */
	res = util_read_json_type(&sub_exp, SUBSYSTEM_EXP_OBJ, t_string,
				  obj->val);
	if (res != ERR_CODE(PASSED) && res != ERR_CODE(VALUE_NOTFOUND))
		goto exit;

	/* First wait and post semaphore */
	res = util_sem_wait_before(thr, obj->val);
	if (res != ERR_CODE(PASSED))
		goto exit;

	res = util_sem_post_before(thr, obj->val);
	if (res != ERR_CODE(PASSED))
		goto exit;

	/* Execute subtest command */
	res = execute_command(cmd_name, obj->val, &cmn_params, &status);
	if (res != ERR_CODE(PASSED))
		goto exit;

	/* Last wait and post semaphore */
	res = util_sem_post_after(thr, obj->val);
	if (res != ERR_CODE(PASSED))
		goto exit;

	res = util_sem_wait_after(thr, obj->val);
	if (res != ERR_CODE(PASSED))
		goto exit;

	if (sub_exp) {
		sub_used = smw_osal_latest_subsystem_name();
		if (sub_used) {
			DBG_PRINT("Selected subsystem: %s", sub_used);
			if (strcmp(sub_used, sub_exp)) {
				DBG_PRINT("Expected subsystem: %s", sub_exp);
				res = ERR_CODE(BAD_RESULT);
			}
		} else {
			DBG_PRINT("WARNING - subsystem cannot be verified!");
		}
	}

exit:
	return log_subtest_status(thr, obj, res, status);
}

void *process_thread(void *arg)
{
	int status = ERR_CODE(PASSED);
	int err = ERR_CODE(BAD_ARGS);
	int i;
	bool ran_subtest = false;
	struct thread_data *thr = arg;
	struct json_object_iter obj;

	if (!thr || !thr->def) {
		DBG_PRINT_BAD_ARGS();
		exit(ERR_CODE(BAD_ARGS));
	}

	if (!json_object_get_object(thr->def)) {
		DBG_PRINT("Thread definition json_object_get_object error");
		err = ERR_CODE(INTERNAL);
		goto exit;
	}

	/* First wait and post semaphore if multi-thread test */
	err = util_sem_wait_before(thr, thr->parent_def);
	if (err != ERR_CODE(PASSED)) {
		err = ERR_CODE(FAILED);
		goto exit;
	}

	err = util_sem_post_before(thr, thr->parent_def);
	if (err != ERR_CODE(PASSED)) {
		err = ERR_CODE(FAILED);
		goto exit;
	}

	thr->state = RUNNING;

	for (i = 0; i < thr->loop + 1; i++) {
		json_object_object_foreachC(thr->def, obj)
		{
			/* Run the JSON-C "subtest" object, other tag is ignored */
			if (strncmp(obj.key, SUBTEST_OBJ, strlen(SUBTEST_OBJ)))
				continue;

			err = run_subtest(thr, &obj);
			status = (status == ERR_CODE(PASSED)) ? err : status;
			ran_subtest = true;
		}
	}

	if (!ran_subtest)
		status = ERR_CODE(FAILED);

	/* Last wait and post semaphore if multi-thread test */
	err = util_sem_post_after(thr, thr->parent_def);
	if (err != ERR_CODE(PASSED)) {
		err = ERR_CODE(FAILED);
		goto exit;
	}

	err = util_sem_wait_after(thr, thr->parent_def);
	if (err != ERR_CODE(PASSED))
		err = ERR_CODE(FAILED);

exit:
	thr->state = EXITED;
	thr->status = (status == ERR_CODE(PASSED)) ? err : status;

	/* Decrement (free) the thread JSON-C definition */
	if (thr->def)
		json_object_put(thr->def);

	return &thr->status;
}
