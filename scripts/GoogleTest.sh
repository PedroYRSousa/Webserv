set -euox pipefail
readonly LINUX_LATEST_CONTAINER="gcr.io/google.com/absl-177019/linux_gcc-latest:latest"
GTEST_ROOT="$(realpath $(dirname ${0})/..)"
DIR=$(pwd)
WORKDIR="/src"
COMMAND="git clone https://github.com/google/googletest.git && cd googletest && mkdir -p build && cd build && cmake .. && make && make install && cd .. && cd .. && rm -rf googletest && make googletest"
docker run \
  --volume="${GTEST_ROOT}:${GTEST_ROOT}" \
  --workdir="${DIR}" \
  ${LINUX_LATEST_CONTAINER} \
  bash -c "${COMMAND}"
