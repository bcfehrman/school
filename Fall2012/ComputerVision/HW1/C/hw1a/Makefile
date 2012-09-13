INCPATH = -I /usr/include/opencv
LIBPATH = -L /usr/lib
LIBRARIES = -lm -lopencv_highgui -lopencv_core
EXEC = hw1a
COMPILER = g++

$(EXEC): $(EXEC).cpp
	$(COMPILER) $(INCPATH) $(LIBPATH) $(LIBRARIES) $(OPTIONS) $^ -o $@

clean:
	rm -rf $(EXEC) *o 
