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
	if len(prog) > size {
		panic("program too long")
	}

	for i := range out {
		out[i] = 0
		if i < len(prog) {
			out[i] = prog[i]
		} else if i%4 == 0 {
			out[i] = 0x33 // nop
		}

		out[i] = bits.Reverse8(out[i]) // ehh..
	}

	fmt.Fprintf(os.Stderr, "%d/%d %.2f%%\n", len(prog), size, float64(len(prog))/size*100)

	if err := ioutil.WriteFile("rom.bin", out[:], 0664); err != nil {
		panic(err)
	}
}
