# 编译器
CC := gcc
# 编译选项
CCFLAGS := -Wall -O2
# 头文件路径
INCLUDES := -I/usr/include/x86_64-linux-gnu/ -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
# 库文件路径
LIBS := -L/usr/lib/x86_64-linux-gnu/
# 需要链接的库
LDLIBS := -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0 

TARGET = basic-tutorial

SRCS = main.c

# 生成目标文件列表
OBJS := $(SRCS:.c=.o)

# 默认目标
all: $(TARGET)

# 生成可执行文件
$(TARGET): $(OBJS)
	$(CC) $(CXXFLAGS) $(INCLUDES) $(LIBS) -o $@ $^ $(LDLIBS)

# 生成目标文件
%.o: %.c
	$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

# 清理生成的文件
clean:
	$(RM) $(TARGET) $(OBJS)

