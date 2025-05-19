# 编译器
CC = gcc

# 编译选项
CFLAGS = -Wall -g

# 目标文件
TARGET = test

# 源文件
SRC = find_optimize.c

# 默认目标
all: $(TARGET)

# 编译规则
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# 清理目标
clean:
	rm -f $(TARGET)