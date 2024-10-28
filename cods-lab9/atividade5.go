package main

import (
	"fmt"
	"math"
	"os"
	"strconv"
)

func main() {
	if len(os.Args) < 3 {
		fmt.Println("Uso: go run atividade5.go <tamanho da lista> <nº de goroutines>")
		return
	}
	N, _  := strconv.Atoi(os.Args[1])
	M, _  := strconv.Atoi(os.Args[2])

	filaNum := make(chan int, N)
	resultados := make(chan int, M)
	done := make(chan bool)

	for i:=0; i < M; i++ {
		go tarefa(filaNum, resultados, done)
	}

	for i := 1; i <= N; i++ {
		filaNum <- i
	}
	close(filaNum)

	for i := 0; i < M; i++ {
		<- done
	}
	close(resultados)

	primos := 0
	for primo := range resultados {
		primos += primo
	}
	
	fmt.Printf("Total de primos: %d", primos)
}

func ehPrimo(n int) bool {
	if n <= 1 {
		return false
	}
	if n == 2 {
		return true
	}
	if n % 2 == 0 {
		return false
	}
	for i := 3; i < int(math.Sqrt(float64(n))); i = i + 2 {

		if n % i == 0 {
			return false
		}
	}
	return true
}

func tarefa(filaNum chan int, resultados chan int, done chan bool) {
	i := 0
	for {
		num, more := <- filaNum
		if more {
			if ehPrimo(num) {
				i++
			}
		} else	{
			resultados <- i
			done <- true
			break
		}
	}
}