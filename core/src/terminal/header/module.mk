local_dir  := src/terminal/header
local_out  := obj/terminal_test.o
local_src  := $(addprefix $(local_dir)/, terminal_test.cpp)
local_objs := $(subst .cpp,.o,$(local_src))

$(local_out): $(local_src)
	$(COMP)

extra_clean += $(local_out) $(local_obj)