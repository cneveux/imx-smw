set(TEE_CONFIG_FILE tee_config.txt)
set(TEE_COMMAND ${TEST_CMD} ${TEE_CONFIG_FILE})

set(U_GENERATE_001_NAME U_TEE_Generate_001)
set(U_GENERATE_001_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_GENERATE_001_NAME}.json)
set(U_GENERATE_001_CMD ${TEE_COMMAND} ${U_GENERATE_001_DEF})
add_test(NAME ${U_GENERATE_001_NAME} COMMAND ${U_GENERATE_001_CMD})
set_tests_properties(${U_GENERATE_001_NAME} PROPERTIES LABELS
		     "${UNITARY_LABEL};${TEE_LABEL};${GENERATE_LABEL}")

set(U_GENERATE_003_NAME U_TEE_Generate_003)
set(U_GENERATE_003_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_GENERATE_003_NAME}.json)
set(U_GENERATE_003_CMD ${TEE_COMMAND} ${U_GENERATE_003_DEF})
add_test(NAME ${U_GENERATE_003_NAME} COMMAND ${U_GENERATE_003_CMD})
set_tests_properties(${U_GENERATE_003_NAME} PROPERTIES LABELS
		     "${UNITARY_LABEL};${TEE_LABEL};${GENERATE_LABEL}")

set(U_IMPORT_001_NAME U_TEE_Import_001)
set(U_IMPORT_001_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_IMPORT_001_NAME}.json)
set(U_IMPORT_001_CMD ${TEE_COMMAND} ${U_IMPORT_001_DEF})
add_test(NAME ${U_IMPORT_001_NAME} COMMAND ${U_IMPORT_001_CMD})
set_tests_properties(${U_IMPORT_001_NAME} PROPERTIES LABELS
		     "${UNITARY_LABEL};${TEE_LABEL};${IMPORT_LABEL}")

set(U_IMPORT_002_NAME U_TEE_Import_002)
set(U_IMPORT_002_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_IMPORT_002_NAME}.json)
set(U_IMPORT_002_CMD ${TEE_COMMAND} ${U_IMPORT_002_DEF})
add_test(NAME ${U_IMPORT_002_NAME} COMMAND ${U_IMPORT_002_CMD})
set_tests_properties(${U_IMPORT_002_NAME} PROPERTIES LABELS
		     "${UNITARY_LABEL};${TEE_LABEL};${IMPORT_LABEL}")

set(U_IMPORT_003_NAME U_TEE_Import_003)
set(U_IMPORT_003_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_IMPORT_003_NAME}.json)
set(U_IMPORT_003_CMD ${TEE_COMMAND} ${U_IMPORT_003_DEF})
add_test(NAME ${U_IMPORT_003_NAME} COMMAND ${U_IMPORT_003_CMD})
set_tests_properties(${U_IMPORT_003_NAME} PROPERTIES LABELS
			"${UNITARY_LABEL};${TEE_LABEL};${IMPORT_LABEL}")

set(U_IMPORT_004_NAME U_TEE_Import_004)
set(U_IMPORT_004_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_IMPORT_004_NAME}.json)
set(U_IMPORT_004_CMD ${TEE_COMMAND} ${U_IMPORT_004_DEF})
add_test(NAME ${U_IMPORT_004_NAME} COMMAND ${U_IMPORT_004_CMD})
set_tests_properties(${U_IMPORT_004_NAME} PROPERTIES LABELS
		     "${UNITARY_LABEL};${TEE_LABEL};${IMPORT_LABEL}")

set(U_EXPORT_001_NAME U_TEE_Export_001)
set(U_EXPORT_001_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_EXPORT_001_NAME}.json)
set(U_EXPORT_001_CMD ${TEE_COMMAND} ${U_EXPORT_001_DEF})
add_test(NAME ${U_EXPORT_001_NAME} COMMAND ${U_EXPORT_001_CMD})
set_tests_properties(${U_EXPORT_001_NAME} PROPERTIES LABELS
		     "${UNITARY_LABEL};${TEE_LABEL};${EXPORT_LABEL}")

set(U_HASH_001_NAME U_TEE_Hash_001)
set(U_HASH_001_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_HASH_001_NAME}.json)
set(U_HASH_001_CMD ${TEE_COMMAND} ${U_HASH_001_DEF})
add_test(NAME ${U_HASH_001_NAME} COMMAND ${U_HASH_001_CMD})
set_tests_properties(${U_HASH_001_NAME} PROPERTIES LABELS
		     "${UNITARY_LABEL};${TEE_LABEL};${HASH_LABEL}")

set(U_HMAC_001_NAME U_TEE_Hmac_001)
set(U_HMAC_001_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_HMAC_001_NAME}.json)
set(U_HMAC_001_CMD ${TEE_COMMAND} ${U_HMAC_001_DEF})
add_test(NAME ${U_HMAC_001_NAME} COMMAND ${U_HMAC_001_CMD})
set_tests_properties(${U_HMAC_001_NAME} PROPERTIES LABELS
		     "${UNITARY_LABEL};${TEE_LABEL};${HMAC_LABEL}")

set(U_RNG_001_NAME U_TEE_Rng_001)
set(U_RNG_001_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_RNG_001_NAME}.json)
set(U_RNG_001_CMD ${TEE_COMMAND} ${U_RNG_001_DEF})
add_test(NAME ${U_RNG_001_NAME} COMMAND ${U_RNG_001_CMD})
set_tests_properties(${U_RNG_001_NAME} PROPERTIES LABELS
		     "${UNITARY_LABEL};${TEE_LABEL};${RNG_LABEL}")

set(U_SIGN_VERIFY_001_NAME U_TEE_Sign_Verify_001)
set(U_SIGN_VERIFY_001_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_SIGN_VERIFY_001_NAME}.json)
set(U_SIGN_VERIFY_001_CMD ${TEE_COMMAND} ${U_SIGN_VERIFY_001_DEF})
add_test(NAME ${U_SIGN_VERIFY_001_NAME} COMMAND ${U_SIGN_VERIFY_001_CMD})
set_tests_properties(${U_SIGN_VERIFY_001_NAME} PROPERTIES LABELS
		     "${UNITARY_LABEL};${TEE_LABEL};${SIGN_LABEL};${VERIFY_LABEL}")

set(U_SIGN_VERIFY_002_NAME U_TEE_Sign_Verify_002)
set(U_SIGN_VERIFY_002_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_SIGN_VERIFY_002_NAME}.json)
set(U_SIGN_VERIFY_002_CMD ${TEE_COMMAND} ${U_SIGN_VERIFY_002_DEF})
add_test(NAME ${U_SIGN_VERIFY_002_NAME} COMMAND ${U_SIGN_VERIFY_002_CMD})
set_tests_properties(${U_SIGN_VERIFY_002_NAME} PROPERTIES LABELS
		     "${UNITARY_LABEL};${TEE_LABEL};${SIGN_LABEL};${VERIFY_LABEL}")

set(U_SIGN_VERIFY_003_NAME U_TEE_Sign_Verify_003)
set(U_SIGN_VERIFY_003_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_SIGN_VERIFY_003_NAME}.json)
set(U_SIGN_VERIFY_003_CMD ${TEE_COMMAND} ${U_SIGN_VERIFY_003_DEF})
add_test(NAME ${U_SIGN_VERIFY_003_NAME} COMMAND ${U_SIGN_VERIFY_003_CMD})
set_tests_properties(${U_SIGN_VERIFY_003_NAME} PROPERTIES LABELS
			"${UNITARY_LABEL};${TEE_LABEL};${SIGN_LABEL};${VERIFY_LABEL}")

set(U_SIGN_VERIFY_004_NAME U_TEE_Sign_Verify_004)
set(U_SIGN_VERIFY_004_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_SIGN_VERIFY_004_NAME}.json)
set(U_SIGN_VERIFY_004_CMD ${TEE_COMMAND} ${U_SIGN_VERIFY_004_DEF})
add_test(NAME ${U_SIGN_VERIFY_004_NAME} COMMAND ${U_SIGN_VERIFY_004_CMD})
set_tests_properties(${U_SIGN_VERIFY_004_NAME} PROPERTIES LABELS
			"${UNITARY_LABEL};${TEE_LABEL};${SIGN_LABEL};${VERIFY_LABEL}")

set(U_SIGN_VERIFY_005_NAME U_TEE_Sign_Verify_005)
set(U_SIGN_VERIFY_005_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_SIGN_VERIFY_005_NAME}.json)
set(U_SIGN_VERIFY_005_CMD ${TEE_COMMAND} ${U_SIGN_VERIFY_005_DEF})
add_test(NAME ${U_SIGN_VERIFY_005_NAME} COMMAND ${U_SIGN_VERIFY_005_CMD})
set_tests_properties(${U_SIGN_VERIFY_005_NAME} PROPERTIES LABELS
			"${UNITARY_LABEL};${TEE_LABEL};${SIGN_LABEL};${VERIFY_LABEL}")


set(U_SIGN_VERIFY_006_NAME U_TEE_Sign_Verify_006)
set(U_SIGN_VERIFY_006_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${U_SIGN_VERIFY_006_NAME}.json)
set(U_SIGN_VERIFY_006_CMD ${TEE_COMMAND} ${U_SIGN_VERIFY_006_DEF})
add_test(NAME ${U_SIGN_VERIFY_006_NAME} COMMAND ${U_SIGN_VERIFY_006_CMD})
set_tests_properties(${U_SIGN_VERIFY_006_NAME} PROPERTIES LABELS
			"${UNITARY_LABEL};${TEE_LABEL};${SIGN_LABEL};${VERIFY_LABEL}")

set(F_KEYMGR_001_NAME F_TEE_Keymgr_001)
set(F_KEYMGR_001_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${F_KEYMGR_001_NAME}.json)
set(F_KEYMGR_001_CMD ${TEE_COMMAND} ${F_KEYMGR_001_DEF})
add_test(NAME ${F_KEYMGR_001_NAME} COMMAND ${F_KEYMGR_001_CMD})
set_tests_properties(${F_KEYMGR_001_NAME} PROPERTIES LABELS
		     "${FUNCTIONAL_LABEL};${TEE_LABEL};${KEYMGR_LABEL}")

set(F_KEYMGR_002_NAME F_TEE_Keymgr_002)
set(F_KEYMGR_002_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${F_KEYMGR_002_NAME}.json)
set(F_KEYMGR_002_CMD ${TEE_COMMAND} ${F_KEYMGR_002_DEF})
add_test(NAME ${F_KEYMGR_002_NAME} COMMAND ${F_KEYMGR_002_CMD})
set_tests_properties(${F_KEYMGR_002_NAME} PROPERTIES LABELS
		     "${FUNCTIONAL_LABEL};${TEE_LABEL};${KEYMGR_LABEL}")

set(F_KEYMGR_003_NAME F_TEE_Keymgr_003)
set(F_KEYMGR_003_DEF ${SMW_TESTS_TARGET_DEF_DIR_FULL_PATH}/${F_KEYMGR_003_NAME}.json)
set(F_KEYMGR_003_CMD ${TEE_COMMAND} ${F_KEYMGR_003_DEF})
add_test(NAME ${F_KEYMGR_003_NAME} COMMAND ${F_KEYMGR_003_CMD})
set_tests_properties(${F_KEYMGR_003_NAME} PROPERTIES LABELS
			"${FUNCTIONAL_LABEL};${TEE_LABEL};${KEYMGR_LABEL}")

install(FILES ${SMW_CONFIG_SRC_DIR}/${TEE_CONFIG_FILE}
        DESTINATION ${SMW_TESTS_TARGET_CONFIG_DIR}
        EXCLUDE_FROM_ALL
        COMPONENT ${PROJECT_NAME})

FILE(GLOB U_TEE_TEST ${TEST_DEF_SRC_DIR}/U_TEE*.json)
FILE(GLOB F_TEE_TEST ${TEST_DEF_SRC_DIR}/F_TEE*.json)

foreach(file IN LISTS U_TEE_TEST)
    install(FILES ${file}
            DESTINATION ${SMW_TESTS_TARGET_DEF_DIR}
            EXCLUDE_FROM_ALL
            COMPONENT ${PROJECT_NAME})
endforeach()

foreach(file IN LISTS F_TEE_TEST)
    install(FILES ${file}
            DESTINATION ${SMW_TESTS_TARGET_DEF_DIR}
            EXCLUDE_FROM_ALL
            COMPONENT ${PROJECT_NAME})
endforeach()
