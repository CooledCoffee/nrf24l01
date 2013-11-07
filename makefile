all:
	sdcc -mmcs51 --iram-size 256 --xram-size 0 --code-size 8192 --nooverlay --noinduction --verbose --debug -V --std-sdcc89 --model-small examples/master.c
	makebin -p master.ihx master.bin
	sdcc -mmcs51 --iram-size 256 --xram-size 0 --code-size 8192 --nooverlay --noinduction --verbose --debug -V --std-sdcc89 --model-small examples/slave.c
	makebin -p slave.ihx slave.bin
	echo "success"

clean:
	rm master.*
	rm slave.*
