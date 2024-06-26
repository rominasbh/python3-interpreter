

# Compile the mypython interpreter executable.
mypython:
	g++ -std=c++11 *.cpp -o mypython

# Clean up the compiled binary.
clean:
	rm -f mypython

# Remove the trace.log file
cleanlog:
	rm -f trace.log
	