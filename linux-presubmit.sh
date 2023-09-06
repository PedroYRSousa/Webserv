set -euox pipefail

readonly LINUX_LATEST_CONTAINER="gcr.io/google.com/absl-177019/linux_hybrid-latest:20230816"

GTEST_ROOT="$(realpath $(dirname ${0})/..)"

COMMAND="make test && ./webserv_unit_test"

docker run \
  --volume="${GTEST_ROOT}:${GTEST_ROOT}" \
  --workdir="${GTEST_ROOT}" \
  ${LINUX_LATEST_CONTAINER} \
  bash -c "${COMMAND}"
