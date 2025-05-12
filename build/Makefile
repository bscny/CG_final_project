CC = g++
CFLAGS = -Wall -g -I. -lm

S_DIR = ..
O_DIR = obj_dir

OBJS = $(O_DIR)/main.o $(O_DIR)/ray.o $(O_DIR)/vec3.o $(O_DIR)/object.o $(O_DIR)/sphere.o $(O_DIR)/triangle.o $(O_DIR)/mesh.o

EXEC = out

all: $(EXEC)

$(EXEC): $(O_DIR) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(O_DIR):
	mkdir $@

clean:
	rm -rf $(EXEC) $(O_DIR) 

$(O_DIR)/%.o: $(S_DIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<
