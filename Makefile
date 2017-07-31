include conf.mk
include common.mk

$(shell $(call pinfo,"Making dependencies..."))

$(shell make -f deps.mk deps >/dev/null)
$(shell $(call pinfo,"Done"))

include $(DEP_FILES)

lib.so: $(OBJ_FILES)
	@$(call pinfo,"Linking...")
	$(CC) -g -shared $(W_FLAGS) $(I_FLAGS) $(L_FLAGS) -O3 $^ -o lib.so
	@$(call psucc,"Linked into lib.so")
	@$(call pinfo,"Copying lib.so to python...")
	cp $@ python/lib.so
	@$(call psucc,"Copied lib.so to python")

OBJFLAGS= $(D_FLAGS) $(W_FLAGS) $(I_FLAGS) $(F_FLAGS) -fPIC

obj/%.o: src/%.c
	@$(call pinfo,"Creating object file $@...")
	mkdir -p $(@D)
	$(CC) -c -g $(OBJFLAGS) --std=$(STD) $< -o $@
	@$(call psucc,"Created object file $@")

clean_dep:
	@$(call pinfo,"Removing dep/*")
	rm -rf dep/*

clean_obj:
	@$(call pinfo,"Removing obj/*")
	rm -rf obj/*

clean: clean_dep clean_obj

obj: $(OBJ_FILES)

lib: lib.so

.PHONY: clean clean_dep clean_obj obj lib
.DEFAULT_GOAL := lib
