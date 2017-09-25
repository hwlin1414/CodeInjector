CC=clang
CFLAGS=-O3 -Weverything -g
LIBS=#-lreadline -lpthread -lncursesw -ldl -lm
SHAREDOBJS=#test.o
SHAREDLIB=#libtest.so
SHAREDFLAGS=-shared -Wl,-soname,$(SHAREDLIB).1
OBJS=main.o process.o
TARGET=a.out
RM=rm -f

all: $(SHAREDLIB) $(TARGET)

$(SHAREDLIB): $(SHAREDOBJS)
	$(CC) $(SHAREDFLAGS) -o $(.TARGET) $(SHAREDOBJS)

$(SHAREDOBJS): $(.PREFIX).c
	$(CC) $(CFLAGS) -fPIC -o $(.TARGET) -c $(.PREFIX).c

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(.TARGET) $(OBJS)

$(OBJS): $(.PREFIX).c
	$(CC) $(CFLAGS) -o $(.TARGET) -c $(.PREFIX).c

clean:
	$(RM) $(OBJS) $(SHAREDOBJS)
	$(RM) $(TARGET) $(SHAREDLIB)

.PHONY: clean
