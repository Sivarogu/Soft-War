.PHONY: sub-client

sub-client:
	rm -rf dist && mkdir -p dist && gcc sub_client.c -o dist/sub-client -lczmq -lzmq && ./dist/sub-client
