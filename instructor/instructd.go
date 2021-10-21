package main

import (
	"errors"
	"fmt"
	"io"
	"log"
	"net/http"
	"os"
	"strconv"
	"sync"
	"time"

	"go.bug.st/serial"
)

const maxlen = 7000

func main() {
	mode := &serial.Mode{
		BaudRate: 115200,
		DataBits: 8,
		Parity: serial.NoParity,
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

	http.HandleFunc("/", func(resp http.ResponseWriter, req *http.Request) {
		hndlk.Lock()
		defer hndlk.Unlock()

		if req.ContentLength < 1 {
			http.Error(resp, "no body len", http.StatusUnprocessableEntity)
			return
		}
		if req.ContentLength > maxlen {
			http.Error(resp, "body too chubby", http.StatusRequestEntityTooLarge)
			return
		}

		lis := req.Header.Get("X-LastI")
		if lis == "" {
			http.Error(resp, "No X-LastI", http.StatusTeapot)
			return
		}

		li, err := strconv.ParseInt(lis, 16, 16)
		if err != nil {
			http.Error(resp, "parsing last instr idx fail", http.StatusTeapot)
			return
		}
		li = li - 0x200 + 4

		var buf [maxlen]byte
		n, err := req.Body.Read(buf[:])
		if err != nil && err != io.EOF {
			http.Error(resp, fmt.Sprintf("read body err: %s", err), http.StatusUnprocessableEntity)
			return
		}

		if int64(n) != req.ContentLength {
			http.Error(resp, "body len didn't match content len", http.StatusTeapot)
			return
		}

		if err := ping(); err != nil {
			http.Error(resp, fmt.Errorf("ping error: %w", err).Error(), http.StatusBadGateway)
			return
		}

		_, err = port.Write([]byte{'T'})
		if err != nil {
			http.Error(resp, fmt.Errorf("start transfer: %w", err).Error(), http.StatusInternalServerError)
			return
		}

		var readBuf [1]byte
		_, err = port.Read(readBuf[:])
		if err != nil {
			http.Error(resp, fmt.Errorf("start transfer ack: %w", err).Error(), http.StatusInternalServerError)
			return
		}

		if readBuf[0] != 'B' {
			http.Error(resp, fmt.Errorf("bad start transfer resp: %x", readBuf[0]).Error(), http.StatusInternalServerError)
			return
		}

		_, err = port.Write([]byte{byte(req.ContentLength & 0xff), byte((req.ContentLength >> 8) & 0xff)})
		if err != nil {
			http.Error(resp, fmt.Errorf("start transfer: %w", err).Error(), http.StatusInternalServerError)
			return
		}
		_, err = port.Write([]byte{byte(li & 0xff), byte((li >> 8) & 0xff)})
		if err != nil {
			http.Error(resp, fmt.Errorf("start transfer: %w", err).Error(), http.StatusInternalServerError)
			return
		}

		_, err = port.Read(readBuf[:])
		if err != nil {
			http.Error(resp, fmt.Errorf("start transfer ack: %w", err).Error(), http.StatusInternalServerError)
			return
		}

		if readBuf[0] != 'A' {
			http.Error(resp, fmt.Errorf("bad len ack: %x", readBuf[0]).Error(), http.StatusInternalServerError)
			return
		}

		for i := 0; i < n; i++ {
			fmt.Printf("\rWrite %d (%02x)", i, buf[i])
			_, err = port.Write([]byte{buf[i]})
			if err != nil {
				http.Error(resp, fmt.Errorf("transfer: %w", err).Error(), http.StatusInternalServerError)
				return
			}
			_, err = port.Read(readBuf[:])
			if err != nil {
				http.Error(resp, fmt.Errorf("start transfer ack: %w", err).Error(), http.StatusInternalServerError)
				return
			}

			if readBuf[0] != 'D' {
				http.Error(resp, fmt.Errorf("bad data ack: %x", readBuf[0]).Error(), http.StatusInternalServerError)
				return
			}
		}

		fmt.Printf("\rWrite ok                     \n")

		if err := ping(); err != nil {
			http.Error(resp, fmt.Errorf("ping error: %w", err).Error(), http.StatusBadGateway)
			return
		}

		resp.WriteHeader(http.StatusOK)
	})
	if err := http.ListenAndServe("127.0.0.1:13333", http.DefaultServeMux); err != nil {
		log.Fatal()
	}
}
