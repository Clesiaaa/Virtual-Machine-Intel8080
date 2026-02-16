# Variables
CC = gcc
CFLAGS = -Wall -I./include
SRCDIR = src
OBJDIR = obj
INCDIR = include

# Object files
OBJS = $(OBJDIR)/main.o \
       $(OBJDIR)/cpu.o \
       $(OBJDIR)/ram.o \
       $(OBJDIR)/alu.o \
       $(OBJDIR)/instructions.o \
       $(OBJDIR)/compile.o \
       $(OBJDIR)/filesystem.o

# Target executable
TARGET = emulator

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Compile source files to object files
$(OBJDIR)/main.o: main.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c main.c -o $(OBJDIR)/main.o

$(OBJDIR)/cpu.o: $(SRCDIR)/core/cpu.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRCDIR)/core/cpu.c -o $(OBJDIR)/cpu.o

$(OBJDIR)/ram.o: $(SRCDIR)/core/ram.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRCDIR)/core/ram.c -o $(OBJDIR)/ram.o

$(OBJDIR)/alu.o: $(SRCDIR)/core/alu.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRCDIR)/core/alu.c -o $(OBJDIR)/alu.o

$(OBJDIR)/instructions.o: $(SRCDIR)/execution_engine/instructions.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRCDIR)/execution_engine/instructions.c -o $(OBJDIR)/instructions.o

$(OBJDIR)/compile.o: $(SRCDIR)/compiler/compile.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRCDIR)/compiler/compile.c -o $(OBJDIR)/compile.o

$(OBJDIR)/filesystem.o: $(SRCDIR)/fs/filesystem.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRCDIR)/fs/filesystem.c -o $(OBJDIR)/filesystem.o

# Create obj directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Clean up generated files
clean:
	rm -f $(OBJDIR)/*.o $(TARGET)

# Remove obj directory and executable
distclean: clean
	rmdir $(OBJDIR) 2>/dev/null || true

# Rebuild everything
rebuild: clean all

# Phony targets
.PHONY: all clean distclean rebuild
