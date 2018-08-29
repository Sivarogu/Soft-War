mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(dir $(patsubst %/,%,$(dir $(mkfile_path))))
root_path := $(abspath $(current_dir))

.PHONY: sub-client

#sub-client:
#	rm -rf dist && mkdir -p dist && gcc sub_client.c -o dist/sub-client -lczmq -lzmq -D_GNU_SOURCE && ./dist/sub-client

build-env:
	docker build -f env.Dockerfile -t softwar-dev-env $(root_path)

run-server:
	docker run -it \
		--name softwar-server \
		-p 4242:4242 -p 4243:4243 \
		--mount type=bind,source="$(root_path)",target=/app \
		softwar-dev-env /bin/bash \
		-c "cd server && make watch"

run-easy-event:
	docker run -it \
		--mount type=bind,source="$(root_path)",target=/app \
		softwar-dev-env /bin/bash \
		-c "cd external/easy-event && npm i && tsc --watch"

run-lib-client:
	docker run -it \
		--mount type=bind,source="$(root_path)",target=/app \
		softwar-dev-env /bin/bash \
		-c "cd lib-client && npm i && tsc --watch"

run-bridge-build:
	docker run -it \
		--mount type=bind,source="$(root_path)",target=/app \
		softwar-dev-env \
		/bin/bash \
		-c "cd zmq-websocket-bridge && npm i && tsc --watch"

run-bridge-start:
	docker run -it \
		--link softwar-server:softwar-server \
		--mount type=bind,source="$(root_path)",target=/app \
		-p 9127:9127 \
		softwar-dev-env /bin/bash \
		-c "nodemon zmq-socket-bridge"

run-visualizer:
	docker run -it \
		--mount type=bind,source="$(root_path)",target=/app \
		softwar-dev-env \
		/bin/bash \
		-c "webpack --watch"
