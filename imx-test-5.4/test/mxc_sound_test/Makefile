#
# Makefile for OSS sound driver test app.
#
CFLAGS += -Wall -D__LINUX -I$(LINUXPATH)/sound/oss -g $(CCOPTS) -lpthread


OBJS = \
	$(OBJDIR)/check_ioctls.out \
	$(OBJDIR)/check_recording.out \
	$(OBJDIR)/check_audio.out \
	$(OBJDIR)/check_minor_open.out \
	$(OBJDIR)/check_mixer.out \
	$(OBJDIR)/check_volume2.out \
	$(OBJDIR)/check_synchronization.out \
	$(OBJDIR)/check_2w1r.out \
	$(OBJDIR)/autorun-sound.sh

all : $(OBJS)

.PHONY: clean
clean :
	rm -f $(OBJS)

#
# include the Rules
#
include ../make.rules
