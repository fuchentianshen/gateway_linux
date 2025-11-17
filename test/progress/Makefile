fork:01_fork.c
	-gcc $^ -o $@
	-./$@
	-rm $@

other:02_other.c
	-gcc $^ -o $@
	
execve:02_execve.c other
	-gcc $< -o $@
	-./$@
	-rm $@ other

waitpid:03_waitpid.c
	-gcc $^ -o $@
	-./$@
	-rm $@

signal:04_signal.c
	-gcc $^ -o $@
	-./$@
	-rm $@