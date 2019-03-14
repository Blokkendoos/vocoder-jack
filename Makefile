all:
	$(MAKE) -C util/ play_loop
	$(MAKE) -C src/ vocoder
	
clean:
	rm -f *~
	for subdir in util/ src/ ; do \
		$(MAKE) -C "$$subdir" clean ;\
	done
