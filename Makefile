SUBDIRS = \
	dv-kernel-driver \
	dv-user-driver/src \
	application

.PHONY: all clean

all clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir -f Makefile $@; \
	done
