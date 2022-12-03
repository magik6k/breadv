package main

import (
	"fmt"
	"io"
	"io/ioutil"
	"math/bits"
	"os"
)

func main() {
	const size = 0x2000
	var out [size]byte

	prog, err := io.ReadAll(os.Stdin)
	if err != nil {
		panic(err)
	}
	if len(prog) > size-2 {
		panic("program too long")
	}

	out[0] = bits.Reverse8(uint8((len(prog) & 0xff) >> 0))
	out[1] = bits.Reverse8(uint8((len(prog) & 0xff00) >> 8))

	for i := range out[2:] {
		out[i+2] = 0
		if i < len(prog) {
			out[i+2] = prog[i]
		} else if i%4 == 0 {
			out[i+2] = 0x33 // nop
		}

		out[i+2] = bits.Reverse8(out[i+2]) // ehh..
	}

	fmt.Fprintf(os.Stderr, "%d/%d %.2f%%\n", len(prog), size, float64(len(prog))/size*100)

	if err := ioutil.WriteFile("rom.bin", out[:], 0664); err != nil {
		panic(err)
	}
}
