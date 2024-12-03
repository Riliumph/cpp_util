.PHONY: all clean human greet

all: human greet

human:
	echo "build human"
	$(MAKE) -C human

greet: human
	$(MAKE) -C greet

clean:
	$(MAKE) -C human clean
	$(MAKE) -C greet clean
