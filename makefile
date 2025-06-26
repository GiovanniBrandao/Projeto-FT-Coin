# Compiler variables
# C++ compiler to use
CXX = g++

# Compiler flags:
# -std=c++17: Uses the C++17 standard
# -Wall: Enables most common warnings (good practice)
# -Wextra: Enables extra warnings (even more strict)
# -g: Includes debugging information (useful for GDB)
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# Linker flags:
# -lmariadbcpp: Links with the MariaDB Connector/C++ library
# -lstdc++fs: Links with the C++17 filesystem library (required for std::filesystem)
#
# NOTE: If you encounter "file not found" or "undefined reference" errors
# after moving the executable or in a different environment, you might need
# to add -L/path/to/mariadb/lib here, or adjust -I if includes are not in standard paths.
LDFLAGS = -lmariadbcpp -lstdc++fs

# Source files (all .cpp files in the project)
SRCS = main.cpp carteira.cpp movimentacao.cpp relatorio.cpp interface.cpp ajuda.cpp

# Object files (generated from .cpp files, .o extension)
# This line transforms the list of .cpp files into a list of .o files
OBJS = $(SRCS:.cpp=.o)

# Name of the final executable
TARGET = FTCoin

# Default rule: "all" is the primary goal, which builds the TARGET
all: $(TARGET)

# Rule to build the TARGET executable from the object files (OBJS)
# Make automatically executes rules to create the OBJS first if they don't exist or are outdated
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Generic rule to compile any .cpp file into a .o file
# $<: Name of the first prerequisite (the .cpp file)
# $@: Name of the target (the .o file)
# The -c flag tells the compiler to compile only (don't link yet)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule "clean": removes all object files and the final executable
# Useful for performing a fresh, clean recompilation from scratch
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean