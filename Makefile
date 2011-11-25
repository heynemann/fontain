help:
	@echo "To build run 'make build'. To run 'make run'. It will serve the /tmp folder."

build:
	@g++ -Wall libfontain.cpp -levent -o libfontain

run: build
	@./libfontain -p 8000 -d /tmp
