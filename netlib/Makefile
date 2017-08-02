SUBDIR = common net
all-recursive clean-recursive depend-recursive install-recursive:
	@target=`echo $@ | sed s/-recursive//`; \
    list='$(SUBDIR)'; \
    for subdir in $$list; do \
        echo "===> Making $$target in $$subdir..."; \
        (cd $$subdir && $(MAKE) $$target) \
    done;

all:    all-recursive

clean:  clean-recursive
install:  install-recursive
