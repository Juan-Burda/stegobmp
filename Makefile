include Makefile.inc

all: $(BIN_FILE)

$(BIN_FILE): $(OBJS)
	@mkdir -p $(BIN_DIR)
	@$(GCC) $(OBJS) -o $(BIN_FILE)

%.o: %.c
	$(GCC) $(GCCFLAGS) -c $< -o $@

clean:
	@rm -rf $(OUTPUT_DIR) $(OBJS)

.PHONY: clean all