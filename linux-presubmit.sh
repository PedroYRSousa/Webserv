set -euox pipefail

readonly LINUX_LATEST_CONTAINER="gcr.io/google.com/absl-177019/linux_hybrid-latest:20230816"

GTEST_ROOT="$(realpath $(dirname ${0})/..)"
# Defina a pasta de trabalho dentro do container
DIR=$(pwd)
WORKDIR="/src"

# Comando para compilar e executar o projeto
COMMAND="make test && ./webserv_unit_test"

# Execute o comando dentro do container
docker run \
  --volume="${GTEST_ROOT}:${GTEST_ROOT}" \
  --workdir="${DIR}" \
  ${LINUX_LATEST_CONTAINER} \
  bash -c "${COMMAND}"
