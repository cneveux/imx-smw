#!/bin/bash
set -eE
trap 'error ${LINENO}' ERR

script_name=$0

tmp_jobids="logs/jobids.txt"
smw_tmp_yaml="smw_tmp.yaml"
smw_setup_yaml="smw_setup.yaml"
smw_package_yaml="smw_package.yaml"
smw_ctest_yaml="smw_ctest.yaml"

devops_script="nexus-find-latest-db.sh"
lavacli_tool="lavacli"
squad_token="b786b334a22cd4734d4f180813f75bb7d725d491"

export PATH=/home/bamboo/.local/bin:$PATH

lava_url="${bamboo_lava_url:-https://lava.sw.nxp.com}"
lava_backend="${bamboo_lava_backend:-lava-https}"
lava_token="${bamboo_lava_token_secret:-${squad_token}}"
lava_user="${bamboo_lava_user:-squad-mougins}"
DAILY_BUILD_VERSION="${bamboo_daily_build_version:-"-2"}"

function usage_lavacli()
{
    printf "\n"
    printf " To install and register lavacli user\n"
    printf "  %s install token=[token] user=[user]\n" "${script_name}"
    printf "    token = [optional] Lava token key\n"
    printf "    user  = [optional] Lava user name of the token key\n"
    printf "Note: optional parameters in bamboo environment require the variables\n"
    printf " \$bamboo_lava_token_secret for the token\n"
    printf " \$bamboo_lava_user for the user\n"
    printf "\n"
}

function usage_submit()
{
    printf "\n"
    printf "To submit a squad job\n"
    printf "  %s submit platform script_dir yaml_dir squad_group package_url=[url] ctest_label=[label]\n" "${script_name}"
    printf "    platform    = Platform name\n"
    printf "    script_dir  = Directory where %s is located\n" "${devops_script}"
    printf "    yaml_dir    = Directory where Lava jobs descriptions for Security Middleware are located\n"
    printf "    squad_id    = Suffix of squad tests, can contains more than one job\n"
    printf "    package_url = [optional] URL to custom Security Middleware package\n"
    printf "    ctest_label = [optional] CTest label\n"
    printf "\n"
}

function usage_result()
{
    printf "\n"
    printf " To wait and fetch squad result\n"
    printf "  %s result token=[token]\n" "${script_name}"
    printf "    token = [optional] Lava token key\n"
    printf "Note: optional parameter in bamboo environment require the variable\n"
    printf " \$bamboo_lava_token_secret for the token\n"
    printf "\n"
}

function usage()
{
    printf "\n"
    printf "*******************************************\n"
    printf " Usage of Security Middleware squad script \n"
    printf "*******************************************\n"
    usage_lavacli
    usage_submit
    usage_result
}

function parse_parameters
{
    for arg in "$@"
    do
      case $arg in
        token=*)
          lava_token="${arg#*=}"
          ;;

        user=*)
          lava_user="${arg#*=}"
          ;;

        package_url=*)
          opt_package_url="${arg#*=}"
          ;;

        ctest_label=*)
          opt_ctest_label="${arg#*=}"
          ;;

        *)
          usage
          exit 1
          ;;
      esac

      shift
    done
}

function check_directory
{
    if [[ ! -d "$1" ]]; then
      printf "%s is not a directory\n" "$1"
      exit 1
    fi
}

function check_file
{
    if [[ ! -e "$1"/"$2" ]]; then
      printf "Cannot find %s in %s\n" "$2" "$1"
      exit 1
    fi
}

function install_lavacli
{
    #
    # Get optional script command parameters
    #
    parse_parameters "$@"

    if [[ ! -x "$(command -v ${lavacli_tool})" ]]; then
      # Install lavacli & Create identity default
      pip3 install --user lavacli

      if [[ ! -x "$(command -v ${lavacli_tool})" ]]; then
        printf "lavacli installation error\n"
        exit 1
      fi
    fi

    if [[ -z ${lava_token} || -z ${lava_user} ]]; then
      printf "No User and/or Token defined to use lavacli\n"
      exit 1
    fi

    ${lavacli_tool} identities add \
    --token "${lava_token}" \
    --uri ${lava_url}/RPC2 \
    --username "${lava_user}" default

    exit 0
}

function squad_submit
{
    #
    # Get the mandatory parameters
    #
    if [[ $# -lt 4 ]]; then
      usage
      exit 0
    fi

    platform=$1
    script_dir=$2
    yaml_dir=$3
    squad_id=$4

    shift 4

    parse_parameters "$@"

    #
    # Define the Yaml replacement variable
    # depending on the platform
    #
    case $platform in
      imx8qxpc0mek)
        FILENAME_BOOTIMAGE=imx-boot-imx8qxpc0mek-sd.bin-flash
        FILENAME_DTB=imx8qxp-mek.dtb
        FILENAME_KERNEL=Image-imx8qxpc0mek.bin
        IMAGE_NAME=imx-image-core-imx8qxpc0mek
        BOARD_ID=imx8qxpc0mek
        LAVA_DEVICE_TYPE=fsl-imx8qxp-c0-mek-linux
        UBOOT_MMC_BLK=0x40
        UBOOT_MMC_CNT=0x1000
        ;;

      *)
        printf "Platform %s not supported\n" "${platform}"
        printf "Supported platforms: %s " "imx8qxpc0mek"
        printf "\n"
        usage
        exit 1
        ;;
    esac

    check_directory "${script_dir}"
    check_directory "${yaml_dir}"

    check_file "${script_dir}" "${devops_script}"

    PACKAGE_URL=${opt_package_url}

    if [[ ! -z ${opt_ctest_label} ]]; then
      CTEST_LABEL="-L ${opt_ctest_label}"
    fi

    cat "${yaml_dir}"/"${smw_setup_yaml}" > "${yaml_dir}"/"${smw_tmp_yaml}"
    if [[ ! -z ${opt_package_url} ]]; then
      cat "${yaml_dir}"/"${smw_package_yaml}" >> "${yaml_dir}"/"${smw_tmp_yaml}"
    fi

    cat "${yaml_dir}"/"${smw_ctest_yaml}" >> "${yaml_dir}"/"${smw_tmp_yaml}"
    JOB_FILE="${yaml_dir}"/"${smw_tmp_yaml}"
    check_file "${yaml_dir}" "${smw_tmp_yaml}"

    sed -i "s|REPLACE_UBOOT_MMC_BLK|$UBOOT_MMC_BLK|" "$JOB_FILE"
    sed -i "s|REPLACE_UBOOT_MMC_CNT|$UBOOT_MMC_CNT|" "$JOB_FILE"

    if [[ ! -z ${opt_package_url} ]]; then
      sed -i "s|REPLACE_PACKAGE_URL|$PACKAGE_URL|" "$JOB_FILE"
    fi
    sed -i "s|REPLACE_CTEST_LABEL|$CTEST_LABEL|" "$JOB_FILE"

    NEXUS_REPO=IMX-raw_Linux_Internal_Daily_Build
    IMAGE_TYPE=Linux_IMX_Core
    ROOTFS_FOLDER_NAME=fsl-imx-internal-wayland
    JOB_TAG=daas_mougins
    SQUAD_GROUP="mougins-devops"
    SQUAD_SLUG=SMW
    LAVA_JOB_NAME="SMW CI on $BOARD_ID"

    "${script_dir}"/"${devops_script}" \
              -l nl \
              -r "$NEXUS_REPO" \
              -i "$IMAGE_TYPE" \
              -j "$ROOTFS_FOLDER_NAME" \
              -d "$FILENAME_DTB" \
              -k "$FILENAME_KERNEL" \
              -m "$FILENAME_BOOTIMAGE" \
              -b "$IMAGE_NAME" \
              -y "$JOB_FILE" \
              -o "$LAVA_JOB_NAME" \
              -v "$LAVA_DEVICE_TYPE" \
              -t "$JOB_TAG" \
              -n "$DAILY_BUILD_VERSION"

    if [[ ! -z ${opt_package_url} ]]; then
        printf "PACKAGE_URL = %s\n" "$PACKAGE_URL"
    fi

    if [[ ! -x "$(command -v ${lavacli_tool})" ]]; then
      printf "lavacli not installed\n"
      exit 1
    fi

    sleep 5

    # Return job id submitted to LAVA
    job_id="$(${lavacli_tool} jobs submit "$JOB_FILE")"
    printf "Job ID %s\n" "${job_id}"

    if [[ ${job_id} == "Unable to submit" ]]; then
      printf "Unable to submit job for %s\n" "${platform}"
      exit 1
    fi

    if [[ ${lava_backend} == "lava-https" ]]; then
	    squad_token=${lava_token}
    fi

    squad_url_suffix="${SQUAD_GROUP}"/"${SQUAD_SLUG}"/SMW_"${squad_id}"/"${BOARD_ID}"

    curl --noproxy "*" \
         --header "Auth-Token: ${squad_token}" \
         --form backend="${lava_backend}" \
         --form testjob_id="${job_id}" \
         "http://squad.sw.nxp.com/api/watchjob/${squad_url_suffix}"

    echo "${platform} ${job_id}" >> "${tmp_jobids}"
}

function squad_result
{
    if [[ ! -x "$(command -v ${lavacli_tool})" ]]; then
      printf "lavacli not installed"
      exit 1
    fi

    check_file "." "${tmp_jobids}"

    #
    # Get optional script command parameters
    #
    parse_parameters "$@"

    if [[ -z ${lava_token} ]]; then
       printf "No Token defined to use lavacli\n"
       exit 1
    fi

    while IFS= read -r line; do
      elem=($line)
      platform="${elem[0]}"
      job_id="${elem[1]}"

      printf "Check jobid %s on platform %s\n" "${job_id}" "${platform}"

      # Wait for test to finish
      lavacli jobs wait "${job_id}" --polling 60 --timeout 3600 || true

      # Fetch Juint report
      curl -X GET -H "Authorization: Token ${lava_token}" \
          --output logs/"${platform}"_"${job_id}".xml \
          ${lava_url}/api/v0.2/jobs/"${job_id}"/junit/

      # Fetch logs
      curl -X GET -H "Authorization: Token ${lava_token}" \
          --output logs/"${platform}"_"${job_id}"_log.txt \
          ${lava_url}/api/v0.2/jobs/"${job_id}"/logs/
    done < "${tmp_jobids}"

    if grep -q "RESULT=fail" logs/*.txt; then
      exit 1
    fi
}

if [[ $# -eq 0 ]]; then
  usage
  exit 0
fi

opt_action=$1
shift

case $opt_action in
  install)
    install_lavacli "$@"
    ;;

  submit)
    squad_submit "$@"
    ;;

  result)
    squad_result "$@"
    ;;

  *)
    usage
    ;;
esac
