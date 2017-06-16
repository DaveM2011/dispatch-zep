
all:
	zephir install

key:
	ssh-keygen -q -f app/priv.key -t rsa -b 1024 -N ""
	rm -f app/priv.key.pub
	openssl rsa -in app/priv.key -pubout > app/public.key

run:
	mkdir -p app/data
	cd app && caddy
