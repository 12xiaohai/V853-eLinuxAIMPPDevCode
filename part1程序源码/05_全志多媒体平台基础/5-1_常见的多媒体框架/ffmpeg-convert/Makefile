# 编译器
CXX := g++
# 编译选项
CXXFLAGS := -Wall -O2
# 头文件路径
INCLUDES := -I/usr/include/x86_64-linux-gnu/
# 库文件路径
LIBS := -L/usr/lib/x86_64-linux-gnu/
# 需要链接的库
LDLIBS := -lavformat -lavcodec -lavutil

# 目标文件
TARGET := convert

# 源文件列表
SRCS := main.cpp

# 生成目标文件列表
OBJS := $(SRCS:.cpp=.o)

# 默认目标
all: $(TARGET)

# 生成可执行文件
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -o $@ $^ $(LDLIBS)

# 生成目标文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# 清理生成的文件
clean:
	$(RM) $(TARGET) $(OBJS)
