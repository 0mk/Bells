all: TheClient TheServer

TheClient: Client
	$(MAKE) -C Client all

TheServer: Server/Makefile
	$(MAKE) -C Server all

install:
	$(MAKE) -C Client install
	$(MAKE) -C Server install

clean:
	$(MAKE) -C Client clean
	$(MAKE) -C Server clean

wipe:
	$(MAKE) -C Client clean
	$(MAKE) -C Server clean
	rm -rf Client/build
	rm -rf Server/build
