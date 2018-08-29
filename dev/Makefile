mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(dir $(patsubst %/,%,$(dir $(mkfile_path))))
root_path := $(abspath $(current_dir))

.PHONY: sub-client

sub-client:
	rm -rf dist && mkdir -p dist && gcc sub_client.c -o dist/sub-client -lczmq -lzmq -D_GNU_SOURCE && ./dist/sub-client

build-env:
	docker build -f env.Dockerfile -t softwar-dev-env $(root_path)

run-server:
	docker run -it \
		-p 4242:4242 -p 4243:4243 \
		--mount type=bind,source="$(root_path)",target=/app \
		softwar-dev-env \
		make -C server watch
