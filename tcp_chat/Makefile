

.PHONY:all
all:
	cp server/data_pool client -R
	cp server/util client -R
	make -C ./server
	make -C ./client

.PHONY:clean
clean:
	(cd server && make clean)
	(cd client && make clean)
