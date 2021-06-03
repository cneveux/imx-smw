set(API_CONFIG_FILE api_config.txt)
set(API_COMMAND ${TEST_CMD} ${API_CONFIG_FILE})

set(U_GENERATE_001_NAME U_API_Generate_001)
set(U_GENERATE_001_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_GENERATE_001_NAME}.json)
set(U_GENERATE_001_CMD ${API_COMMAND} ${U_GENERATE_001_DEF})
add_test(NAME ${U_GENERATE_001_NAME} COMMAND ${U_GENERATE_001_CMD})
set_tests_properties(${U_GENERATE_001_NAME} PROPERTIES LABELS
		     "${UNITARY_LABEL};${API_LABEL};${GENERATE_LABEL}")

set(U_DELETE_001_NAME U_API_Delete_001)
set(U_DELETE_001_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_DELETE_001_NAME}.json)
set(U_DELETE_001_CMD ${API_COMMAND} ${U_DELETE_001_DEF})
add_test(NAME ${U_DELETE_001_NAME} COMMAND ${U_DELETE_001_CMD})
set_tests_properties(${U_DELETE_001_NAME} PROPERTIES LABELS
		     "${UNITARY_LABEL};${API_LABEL};${DELETE_LABEL}")

set(U_IMPORT_001_NAME U_API_Import_001)
set(U_IMPORT_001_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_IMPORT_001_NAME}.json)
set(U_IMPORT_001_CMD ${API_COMMAND} ${U_IMPORT_001_DEF})
add_test(NAME ${U_IMPORT_001_NAME} COMMAND ${U_IMPORT_001_CMD})
set_tests_properties(${U_IMPORT_001_NAME} PROPERTIES LABELS
		     "${UNITARY_LABEL};${API_LABEL};${IMPORT_LABEL}")

set(U_EXPORT_001_NAME U_API_Export_001)
set(U_EXPORT_001_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_EXPORT_001_NAME}.json)
set(U_EXPORT_001_CMD ${API_COMMAND} ${U_EXPORT_001_DEF})
add_test(NAME ${U_EXPORT_001_NAME} COMMAND ${U_EXPORT_001_CMD})
set_tests_properties(${U_EXPORT_001_NAME} PROPERTIES LABELS
		     "${UNITARY_LABEL};${API_LABEL};${EXPORT_LABEL}")

set(U_HASH_001_NAME U_API_Hash_001)
set(U_HASH_001_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_HASH_001_NAME}.json)
set(U_HASH_001_CMD ${API_COMMAND} ${U_HASH_001_DEF})
add_test(NAME ${U_HASH_001_NAME} COMMAND ${U_HASH_001_CMD})
set_tests_properties(${U_HASH_001_NAME} PROPERTIES LABELS
		     "${UNITARY_LABEL};${API_LABEL};${HASH_LABEL}")

set(U_HMAC_001_NAME U_API_Hmac_001)
set(U_HMAC_001_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_HMAC_001_NAME}.json)
set(U_HMAC_001_CMD ${API_COMMAND} ${U_HMAC_001_DEF})
add_test(NAME ${U_HMAC_001_NAME} COMMAND ${U_HMAC_001_CMD})
set_tests_properties(${U_HMAC_001_NAME} PROPERTIES LABELS
		     "${UNITARY_LABEL};${API_LABEL};${HMAC_LABEL}")

set(U_RNG_001_NAME U_API_Rng_001)
set(U_RNG_001_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_RNG_001_NAME}.json)
set(U_RNG_001_CMD ${API_COMMAND} ${U_RNG_001_DEF})
add_test(NAME ${U_RNG_001_NAME} COMMAND ${U_RNG_001_CMD})
set_tests_properties(${U_RNG_001_NAME} PROPERTIES LABELS
		     "${UNITARY_LABEL};${API_LABEL};${RNG_LABEL}")

set(U_SIGN_001_NAME U_API_Sign_001)
set(U_SIGN_001_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_SIGN_001_NAME}.json)
set(U_SIGN_001_CMD ${API_COMMAND} ${U_SIGN_001_DEF})
add_test(NAME ${U_SIGN_001_NAME} COMMAND ${U_SIGN_001_CMD})
set_tests_properties(${U_SIGN_001_NAME} PROPERTIES LABELS
		     "${UNITARY_LABEL};${API_LABEL};${SIGN_LABEL}")

set(U_VERIFY_001_NAME U_API_Verify_001)
set(U_VERIFY_001_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_VERIFY_001_NAME}.json)
set(U_VERIFY_001_CMD ${API_COMMAND} ${U_VERIFY_001_DEF})
add_test(NAME ${U_VERIFY_001_NAME} COMMAND ${U_VERIFY_001_CMD})
set_tests_properties(${U_VERIFY_001_NAME} PROPERTIES LABELS
		     "${UNITARY_LABEL};${API_LABEL};${VERIFY_LABEL}")

set(U_CIPHER_001_NAME U_API_Cipher_001)
set(U_CIPHER_001_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_CIPHER_001_NAME}.json)
set(U_CIPHER_001_CMD ${API_COMMAND} ${U_CIPHER_001_DEF})
add_test(NAME ${U_CIPHER_001_NAME} COMMAND ${U_CIPHER_001_CMD})
set_tests_properties(${U_CIPHER_001_NAME} PROPERTIES LABELS
			"${UNITARY_LABEL};${API_LABEL};${CIPHER_LABEL}")

set(U_CIPHER_MULTIPART_001_NAME U_API_Cipher_Multipart_001)
set(U_CIPHER_MULTIPART_001_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_CIPHER_MULTIPART_001_NAME}.json)
set(U_CIPHER_MULTIPART_001_CMD ${API_COMMAND} ${U_CIPHER_MULTIPART_001_DEF})
add_test(NAME ${U_CIPHER_MULTIPART_001_NAME} COMMAND ${U_CIPHER_MULTIPART_001_CMD})
set_tests_properties(${U_CIPHER_MULTIPART_001_NAME} PROPERTIES LABELS
			"${UNITARY_LABEL};${API_LABEL};${CIPHER_LABEL};${MULTIPART_LABEL}")

set(U_CIPHER_MULTIPART_002_NAME U_API_Cipher_Multipart_002)
set(U_CIPHER_MULTIPART_002_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_CIPHER_MULTIPART_002_NAME}.json)
set(U_CIPHER_MULTIPART_002_CMD ${API_COMMAND} ${U_CIPHER_MULTIPART_002_DEF})
add_test(NAME ${U_CIPHER_MULTIPART_002_NAME} COMMAND ${U_CIPHER_MULTIPART_002_CMD})
set_tests_properties(${U_CIPHER_MULTIPART_002_NAME} PROPERTIES LABELS
			"${UNITARY_LABEL};${API_LABEL};${CIPHER_LABEL};${MULTIPART_LABEL}")

set(U_CIPHER_MULTIPART_003_NAME U_API_Cipher_Multipart_003)
set(U_CIPHER_MULTIPART_003_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_CIPHER_MULTIPART_003_NAME}.json)
set(U_CIPHER_MULTIPART_003_CMD ${API_COMMAND} ${U_CIPHER_MULTIPART_003_DEF})
add_test(NAME ${U_CIPHER_MULTIPART_003_NAME} COMMAND ${U_CIPHER_MULTIPART_003_CMD})
set_tests_properties(${U_CIPHER_MULTIPART_003_NAME} PROPERTIES LABELS
			"${UNITARY_LABEL};${API_LABEL};${CIPHER_LABEL};${MULTIPART_LABEL}")

set(U_CONTEXT_001_NAME U_API_Context_001)
set(U_CONTEXT_001_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_CONTEXT_001_NAME}.json)
set(U_CONTEXT_001_CMD ${API_COMMAND} ${U_CONTEXT_001_DEF})
add_test(NAME ${U_CONTEXT_001_NAME} COMMAND ${U_CONTEXT_001_CMD})
set_tests_properties(${U_CONTEXT_001_NAME} PROPERTIES LABELS
			"${UNITARY_LABEL};${API_LABEL};${CONTEXT_LABEL}")

install(FILES ${SMW_CONFIG_SRC_DIR}/${API_CONFIG_FILE}
        DESTINATION ${SMW_TESTS_TARGET_CONFIG_DIR}
        EXCLUDE_FROM_ALL
        COMPONENT ${PROJECT_NAME})

FILE(GLOB U_API_TEST ${TEST_DEF_SRC_DIR}/U_API*.json)

foreach(file IN LISTS U_API_TEST)
    install(FILES ${file}
            DESTINATION ${SMW_TESTS_TARGET_DEF_DIR}
            EXCLUDE_FROM_ALL
            COMPONENT ${PROJECT_NAME})
endforeach()
