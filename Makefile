mh600e_gearbox.so: \
		mh600e_common.h \
		mh600e_gearbox.comp \
		mh600e_gears.h \
		mh600e_gears.c \
		mh600e_twitch.h \
		mh600e_twitch.c \
		mh600e_util.h \
		mh600e_util.c
	@halcompile --compile mh600e_gearbox.comp

mh600e_gearbox_sim.so: \
		mh600e_gearbox_sim.comp \
		mh600e_common.h \
		mh600e_gears.h \
		mh600e_gears.c \
		mh600e_util.h \
		mh600e_util.c
	@halcompile --compile mh600e_gearbox_sim.comp

gearbox: mh600e_gearbox.so

sim: mh600e_gearbox_sim.so

all: gearbox sim

install: all
	@halcompile --install mh600e_gearbox.comp

install-sim: install sim
	@halcompile --install mh600e_gearbox_sim.comp

run: install-sim
	@halrun -f mh600e_gearbox_sim.hal &
	@echo Launched halrun.

clean:
	@rm -f mh600e_gearbox.so
	@rm -f mh600e_gearbox_sim.so
