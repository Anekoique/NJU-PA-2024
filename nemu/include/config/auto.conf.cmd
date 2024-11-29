deps_config := \
	src/device/Kconfig \
	src/memory/Kconfig \
	src/isa/riscv32/Kconfig \
	/home/stoair/NJU-PA-2024/nemu/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
