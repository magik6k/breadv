package main

import (
	"encoding/hex"
	"errors"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"net/http"
	"os"
	"path"
	"strconv"
	"sync"
	"time"

	"go.bug.st/serial"
)

const maxlen = 7000

const (
	ARITHI = 0b00100_11
	LUI    = 0b01101_11
	STORE  = 0b01000_11

	F_SW = 0b010

	SW = (F_SW << 12) | STORE

	F_ADD = 0b000

	ADDI = (F_ADD << 12) | ARITHI

	RS1_X0 = 0b00000_000_00000_00000_00
	RS1_X1 = 0b00001_000_00000_00000_00
	RS1_X2 = 0b00010_000_00000_00000_00

	RS2_X1 = 0b00001_00000_000_00000_00000_00

	RD_X1 = 0b00001_00000_00
	RD_X2 = 0b00010_00000_00
)

func main() {
	mode := &serial.Mode{
		BaudRate: 115200,
		DataBits: 8,
		Parity:   serial.OddParity,
		StopBits: serial.OneStopBit,
	}

	log.Println("Open port")

	port, err := serial.Open(os.Args[1], mode)
	if err != nil {
		log.Fatal(err)
	}
	defer port.Close()

	time.Sleep(time.Second)

	ping := func() error {
		log.Println("Ping")

		_, err = port.Write([]byte{'P'})
		if err != nil {
			return err
		}

		var readBuf [1]byte
		n, err := port.Read(readBuf[:])
		if err != nil {
			return err
		}
		if n != 1 {
			return errors.New("wut")
		}

		if readBuf[0] != 'P' {
			return fmt.Errorf("bad ping response :%x", readBuf[0])
		}
		log.Println("Ping OK")
		return nil
	}

	if err := ping(); err != nil {
		log.Fatal(err)
	}
	if err := ping(); err != nil {
		log.Fatal(err)
	}

	var hndlk sync.Mutex

	const (
		ExecOne     = 'X'
		ExecHandoff = 'Y'
	)

	storeDiscard := 0

	var exec func(resp http.ResponseWriter, req *http.Request, pb string, mode byte) bool
	exec = func(resp http.ResponseWriter, req *http.Request, pb string, mode byte) bool {
		storeDiscard <<= 1

		if storeDiscard&0b1000 > 0 {
			// will be discarded, send nop
			exec(resp, req, "00000013", ExecOne)
		}

		instr, err := hex.DecodeString(pb)
		if err != nil {
			http.Error(resp, fmt.Errorf("decode hex instr: %w", err).Error(), http.StatusInternalServerError)
			return true
		}

		if instr[3]&0x7f == STORE {
			storeDiscard |= 1
		}

		_, err = port.Write([]byte{mode})
		if err != nil {
			http.Error(resp, fmt.Errorf("start exec: %w", err).Error(), http.StatusInternalServerError)
			return true
		}

		var readBuf [1]byte
		_, err = port.Read(readBuf[:])
		if err != nil {
			http.Error(resp, fmt.Errorf("start transfer ack: %w", err).Error(), http.StatusInternalServerError)
			return true
		}

		if readBuf[0] != 'R' {
			http.Error(resp, fmt.Errorf("bad ready resp: %x", readBuf[0]).Error(), http.StatusInternalServerError)
			return true
		}

		fmt.Printf("Ready > ")

		_, err = port.Write(instr)
		if err != nil {
			http.Error(resp, fmt.Errorf("transfer: %w", err).Error(), http.StatusInternalServerError)
			return true
		}

		fmt.Printf("Execute %s > ", pb)

		_, err = port.Read(readBuf[:])
		if err != nil {
			http.Error(resp, fmt.Errorf("start transfer ack: %w", err).Error(), http.StatusInternalServerError)
			return true
		}

		if readBuf[0] != 'A' {
			http.Error(resp, fmt.Errorf("bad transfer resp: %x", readBuf[0]).Error(), http.StatusInternalServerError)
			return true
		}

		fmt.Printf("Sent > ")

		_, err = port.Read(readBuf[:])
		if err != nil {
			http.Error(resp, fmt.Errorf("start transfer ack: %w", err).Error(), http.StatusInternalServerError)
			return true
		}
		if readBuf[0] != 'D' {
			http.Error(resp, fmt.Errorf("bad transfer(2) resp: %x", readBuf[0]).Error(), http.StatusInternalServerError)
			return true
		}

		fmt.Printf("Executed\n")

		return false
	}

	http.HandleFunc("/execute/", func(resp http.ResponseWriter, req *http.Request) {
		hndlk.Lock()
		defer hndlk.Unlock()

		if err := ping(); err != nil {
			http.Error(resp, fmt.Errorf("ping error: %w", err).Error(), http.StatusBadGateway)
			return
		}

		pb := path.Base(req.URL.Path)
		if len(pb) != 8 {
			http.Error(resp, fmt.Errorf("path base len bust be 8").Error(), http.StatusBadRequest)
			return
		}

		if exec(resp, req, pb, ExecOne) {
			return
		}

		if err := ping(); err != nil {
			http.Error(resp, fmt.Errorf("ping error: %w", err).Error(), http.StatusBadGateway)
			return
		}

		resp.WriteHeader(http.StatusOK)
	})

	http.HandleFunc("/handoff/", func(resp http.ResponseWriter, req *http.Request) {
		hndlk.Lock()
		defer hndlk.Unlock()

		if err := ping(); err != nil {
			http.Error(resp, fmt.Errorf("ping error: %w", err).Error(), http.StatusBadGateway)
			return
		}

		pb := path.Base(req.URL.Path)
		if len(pb) != 8 {
			http.Error(resp, fmt.Errorf("path base len bust be 8").Error(), http.StatusBadRequest)
			return
		}

		log.Println("Handoff:")
		if exec(resp, req, pb, ExecHandoff) {
			return
		}

		resp.WriteHeader(http.StatusOK)
	})

	http.HandleFunc("/load/", func(resp http.ResponseWriter, req *http.Request) {
		hndlk.Lock()
		defer hndlk.Unlock()

		if err := ping(); err != nil {
			http.Error(resp, fmt.Errorf("ping error: %w", err).Error(), http.StatusBadGateway)
			return
		}

		if req.ContentLength < 1 {
			http.Error(resp, "no body len", http.StatusUnprocessableEntity)
			return
		}
		if req.ContentLength%4 != 0 {
			http.Error(resp, "body len not multiple of 4", http.StatusUnprocessableEntity)
			return
		}

		toSend, err := ioutil.ReadAll(req.Body)
		if err != nil {
			http.Error(resp, fmt.Errorf("read body: %w", err).Error(), http.StatusInternalServerError)
			return
		}

		var base int64
		bstr := req.FormValue("base")
		if bstr != "" {
			var err error
			base, err = strconv.ParseInt(bstr, 0, 32)
			if err != nil {
				http.Error(resp, "parsing base", http.StatusUnprocessableEntity)
				return
			}
		}

		// data -> []addr
		instrs := map[int32][]int32{}
		for i := 0; i < len(toSend); i += 4 {

			n := int32(toSend[i+3])
			n <<= 8
			n |= int32(toSend[i+2])
			n <<= 8
			n |= int32(toSend[i+1])
			n <<= 8
			n |= int32(toSend[i+0])

			instrs[n] = append(instrs[n], int32(i)+int32(base))
		}

		// set addr to 0
		at := int32(0)
		instr := uint32(0) | RS1_X0 | RD_X2 | ADDI
		if exec(resp, req, fmt.Sprintf("%08x", instr), ExecOne) {
			return
		}

		for n, addrs := range instrs {
			// set x1 (data)
			m := (n << 20) >> 20
			k := (n - m) >> 12

			fmt.Printf("LOAD > %08x\n", uint32(n))

			var instr = uint32(k<<12) | RD_X1 | LUI
			if exec(resp, req, fmt.Sprintf("%08x", instr), ExecOne) {
				return
			}

			instr = uint32(m<<20) | RS1_X1 | RD_X1 | ADDI
			if exec(resp, req, fmt.Sprintf("%08x", instr), ExecOne) {
				return
			}

			for _, addr := range addrs {
				n := addr
				m := (n << 20) >> 20
				k := (n - m) >> 12

				if k != at {
					instr = uint32(k<<12) | RD_X2 | LUI
					if exec(resp, req, fmt.Sprintf("%08x", instr), ExecOne) {
						return
					}
					at = k
				}

				instr = uint32((m>>5)<<25) | RS2_X1 | RS1_X2 | uint32((m&0b11111)<<7) | SW
				if exec(resp, req, fmt.Sprintf("%08x", instr), ExecOne) {
					return
				}
				/*
					exec(resp, req, "00000013", ExecOne)
					exec(resp, req, "00000013", ExecOne)
					exec(resp, req, "00000013", ExecOne)
				*/
			}
		}

		if err := ping(); err != nil {
			http.Error(resp, fmt.Errorf("ping error: %w", err).Error(), http.StatusBadGateway)
			return
		}

		resp.WriteHeader(http.StatusOK)
	})

	http.HandleFunc("/dump/", func(resp http.ResponseWriter, req *http.Request) {
		hndlk.Lock()
		defer hndlk.Unlock()

		if err := ping(); err != nil {
			http.Error(resp, fmt.Errorf("ping error: %w", err).Error(), http.StatusBadGateway)
			return
		}

		lstr := path.Base(req.URL.Path)
		l, err := strconv.ParseInt(lstr, 0, 32)
		if err != nil {
			http.Error(resp, fmt.Errorf("parse len: %w", err).Error(), http.StatusBadGateway)
			return
		}
		if l%4 != 0 {
			http.Error(resp, "len must be divisible by 4", http.StatusBadGateway)
			return
		}

		_, err = port.Write([]byte{'D', byte((l >> 24) & 0xff), byte((l >> 16) & 0xff), byte((l >> 8) & 0xff), byte((l >> 0) & 0xff)})
		if err != nil {
			http.Error(resp, fmt.Errorf("start exec: %w", err).Error(), http.StatusInternalServerError)
			return
		}

		resp.WriteHeader(http.StatusOK)

		_, err = io.CopyBuffer(resp, io.LimitReader(port, l), make([]byte, 1))
		if err != nil {
			fmt.Println("DUMP ERR: ", err)
			return
		}

		if err := ping(); err != nil {
			fmt.Println("DUMP ERR: ", err)
			return
		}
	})

	if err := http.ListenAndServe("127.0.0.1:13333", http.DefaultServeMux); err != nil {
		log.Fatal()
	}
}
