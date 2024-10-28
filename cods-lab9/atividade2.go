package main

import (
	"fmt"
)

func goroutine(str chan string) {
	
	msg := <-str
	fmt.Printf(msg)

	str <- "Oi Main, bom dia, tudo bem?\n"

	msg = <-str
	fmt.Printf(msg)

	str <- "Certo, entendido.\n"

	fmt.Printf("finalizando\n")
}

func main() {
	
	str := make(chan string)
	
	go goroutine(str)
	
	str <- "Ola, Goroutine, bom dia!\n"

	msg := <-str
	fmt.Printf(msg)

	str <- "Tudo bem! Vou terminar tá?\n"

	msg = <-str
	fmt.Printf(msg)

	fmt.Printf("Finalizando\n")
	
}
