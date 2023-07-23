# Comum
SRC_DIR					:= ./src
LOG_DIR					:= ./logs
INC_DIR					:= ./includes

CC						:= c++
INCLUDES				:= -I${INC_DIR}

SRC						:= $(wildcard $(SRC_DIR)/**/*.cpp $(SRC_DIR)/*.cpp)

# Main
OBJ_DIR					:= ./obj

NAME					:= webserv

OBJ						:= $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))
CFLAGS					:= -Wall -Werror -Wextra -std=c++98 ${INCLUDES}
LDFLAGS					:= -Wall -Werror -Wextra -std=c++98 -g -fsanitize=address ${INCLUDES}

# Testes
TEST_OBJ_DIR			:= ./obj_test

TEST_NAME				:= webserv_unit_test

TEST_OBJ				:= $(patsubst $(SRC_DIR)/%.cpp,${TEST_OBJ_DIR}/%.o,$(SRC))
TEST_CFLAGS				:= -Wall -Werror -Wextra -pthread ${INCLUDES}
TEST_LDFLAGS			:= -Wall -Werror -Wextra -lgtest -lgtest_main -pthread -g -fsanitize=address ${INCLUDES}

########## COMMANDS ##########

# Main
all: ${NAME}

${NAME}: ${OBJ}
	@mkdir -p ${LOG_DIR}
	${CC} -o ${NAME} $^ ${LDFLAGS}

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@mkdir -p $(dir $@)
	${CC} -c ${CFLAGS} $< -o $@

run: fclean all
	rm -rf ${LOG_DIR}/*.txt
	./${NAME}

clean:
	rm -rf ${OBJ_DIR}

fclean: clean
	rm -rf ${NAME}
	rm -rf ${LOG_DIR}

re: fclean all

# Testes
test: ${TEST_NAME}

${TEST_NAME}: ${TEST_OBJ}
	@mkdir -p ${LOG_DIR}
	${CC} -o ${TEST_NAME} $^ ${TEST_LDFLAGS}

${TEST_OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@mkdir -p $(dir $@)
	${CC} -c ${TEST_CFLAGS} -DTEST_MODE=1 $< -o $@

test_run: test_fclean test
	rm -rf ${LOG_DIR}/*.txt
	./${TEST_NAME}

test_clean:
	rm -rf ${TEST_OBJ_DIR}

test_fclean: test_clean
	rm -rf ${TEST_NAME}
	rm -rf ${LOG_DIR}

test_re: test_fclean test

# Comum
install:
	rm -rf googletest;
	sudo apt-get update -y;
	sudo apt-get upgrade -y;
	sudo apt-get install cppcheck -y;
	git clone https://github.com/google/googletest.git;
	cd googletest && mkdir -p build && cd build && cmake .. && sudo make && sudo make install;
	rm -rf googletest;

cppcheck:
	cppcheck ${SRC_DIR} -i./src/test --suppress=missingInclude --enable=all -I./includes > result_cppcheck.txt 2>&1

.PHONY: all run clean fclean re test test_run test_clean test_fclean install cppcheck
